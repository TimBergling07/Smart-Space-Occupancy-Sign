#include <SoftwareSerial.h>

// 1. 硬件参数定义（严格匹配官方手册）
SoftwareSerial radarSerial(2, 3);  // D2=RX（接雷达TX），D3=TX（接雷达RX）
const int RED_LED = 4;
const int YELLOW_LED = 5;
const int GREEN_LED = 6;
const int RADAR_IO_PIN = 7;  // 雷达GPIO状态引脚（J2Pin2）

// 2. 状态变量
bool isHumanPresent = false;       
bool lastHumanState = false;       
unsigned long noHumanStartTime = 0;
unsigned long lastRadarDataTime = 0;
String lastRadarData = "";         // 保存上一次的雷达原始数据（解决错位）
float lastDistanceM = 0.0;         // 保存上一次的解析距离（避免归零）
const unsigned long RADAR_TIMEOUT = 3000;  
const float DISTANCE_THRESHOLD = 100.0;  // 1米=100cm


void setup() {
  Serial.begin(9600);  
  radarSerial.begin(115200);  
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RADAR_IO_PIN, INPUT);  

  setSingleLight(GREEN_LED);
  Serial.println("=== HLK-LD2402 人体检测系统（调试版）初始化完成 ===");
  Serial.println("提示：1. 确保雷达供电3.3V  2. 先执行「自动门限生成」");
  Serial.println("      3. 无人判定：\"OFF\" 或 距离>1米");
  Serial.println("      4. 调试日志将显示空格处理前后的原始数据");
  Serial.println("----------------------------------------");

  // 初始化：先读取一次雷达数据，避免首次打印空数据
  readRadarState();
  delay(165);  // 匹配雷达数据刷新周期
}


// 修复核心：调整“打印-读取”顺序，先打印上一次数据，再读新数据
void loop() {
  radarMonitor();    // 步骤1：打印【上一次】的雷达数据（解决错位）
  readRadarState();  // 步骤2：读取【新的】雷达数据（更新状态）
  controlLights();   
  delay(165);        
}


// 3. 读取雷达状态（优化解析容错性+调试空格处理）
void readRadarState() {
  String radarData = "";
  unsigned long readStartTime = millis();
  
  // 读取串口数据（增加超时，避免卡滞）
  while (radarSerial.available() > 0 && (millis() - readStartTime) < 100) {
    char c = radarSerial.read();
    if (c == '\n') break;  // 换行符结束
    radarData += c;
    delay(1);
  }

  // ========== 调试打印1：原始数据（未处理空格） ==========
  Serial.print("[调试1] 原始雷达数据（未去空格）：");
  Serial.println("[" + radarData + "]"); // 用[]包裹，直观看到空格/空白符

  // 第一步：清理所有空白字符（首尾+中间空格、回车、制表符）
  radarData = removeAllWhitespace(radarData);

  // ========== 调试打印2：原始数据（处理所有空格后） ==========
  Serial.print("[调试2] 雷达数据（处理所有空格后）：");
  Serial.println("[" + radarData + "]");

  lastRadarData = radarData;  // 保存新读取的原始数据（供下一次打印）
  lastRadarDataTime = millis();

  // 严格校验数据格式，避免解析错误
  if (radarData.startsWith("distance:")) {
    // 提取距离子串（从"distance:"后开始）
    String distStrRaw = radarData.substring(9);
    
    // ========== 调试打印3：距离子串（未处理空格） ==========
    Serial.print("[调试3] 距离子串（未去空格）：");
    Serial.println("[" + distStrRaw + "]");

    // 第二步：清理距离子串的所有空白字符
    String distStr = removeAllWhitespace(distStrRaw);

    // ========== 调试打印4：距离子串（处理所有空格后） ==========
    Serial.print("[调试4] 距离子串（处理所有空格后）：");
    Serial.println("[" + distStr + "]");

    // 容错：判断是否为有效数字
    if (isNumeric(distStr)) {
      float distanceCm = distStr.toFloat();
      lastDistanceM = distanceCm / 100.0;  // 保存有效距离
      // 判定有人/无人
      isHumanPresent = (distanceCm <= DISTANCE_THRESHOLD);
      // ========== 调试打印5：解析成功日志 ==========
      Serial.print("[调试5] 距离解析成功：");
      Serial.print(distanceCm);
      Serial.print("cm → ");
      Serial.print(lastDistanceM);
      Serial.println("m | 有人判定：" + String(isHumanPresent ? "是" : "否"));
    } else {
      // 非数字时，沿用上次距离，避免归零
      Serial.println("[解析异常] 距离格式错误（含非数字字符），沿用上次距离：" + String(lastDistanceM) + "m");
    }
  } else if (radarData == "OFF") {
    isHumanPresent = false;
    lastDistanceM = 0.0;  // OFF时距离置0
    Serial.println("[调试5] 检测到OFF状态，距离置0 | 有人判定：否");
  } else {
    // 非预期数据，用GPIO备用检测，沿用上次距离
    isHumanPresent = (digitalRead(RADAR_IO_PIN) == HIGH);
    Serial.println("[解析异常] 非预期数据（非distance/OFF），用GPIO判断 | 有人判定：" + String(isHumanPresent ? "是" : "否"));
  }

  // 状态变化时重置计时
  if (isHumanPresent != lastHumanState) {
    if (!isHumanPresent) noHumanStartTime = millis();
    lastHumanState = isHumanPresent;
  }
}


// 4. 雷达异常监测（打印上一次的有效数据，解决错位）
void radarMonitor() {
  unsigned long currentTime = millis();
  bool radarTimeout = (currentTime - lastRadarDataTime) > RADAR_TIMEOUT;

  Serial.print("\n【雷达监测】");
  if (radarTimeout) {
    Serial.println("状态：❌ 雷达超时（无串口数据）");
    Serial.println("  排查：接线/TX→D2/RX→D3/波特率115200");
  } else {
    Serial.println("状态：✅ 串口数据正常");
    Serial.print("  原始数据：");
    Serial.println(lastRadarData);  // 打印上一次的原始数据
  }

  // 打印核心状态（使用保存的距离，避免归零）
  Serial.print("【当前状态】有人：");
  Serial.print(isHumanPresent ? "是" : "否");
  Serial.print(" | 探测距离：");
  Serial.print(lastDistanceM);
  Serial.print("m | 无人持续时间：");
  Serial.print(isHumanPresent ? 0 : (currentTime - noHumanStartTime) / 1000.0);
  Serial.println("s");
  Serial.println("----------------------------------------");
}


// 5. 灯光控制（逻辑不变）
void controlLights() {
  unsigned long currentTime = millis();
  bool radarTimeout = (currentTime - lastRadarDataTime) > RADAR_TIMEOUT;

  if (radarTimeout) {
    digitalWrite(YELLOW_LED, (currentTime % 500) < 250 ? HIGH : LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    return;
  }

  if (isHumanPresent) {
    setSingleLight(RED_LED);
  } else {
    unsigned long noHumanDuration = currentTime - noHumanStartTime;
    if (noHumanDuration < 25000) setSingleLight(RED_LED);
    else if (noHumanDuration <= 30000) setSingleLight(YELLOW_LED);
    else setSingleLight(GREEN_LED);
  }
}


// 6. 工具函数：移除字符串中所有空白字符（空格、回车、制表符、换行）
String removeAllWhitespace(String str) {
  String result = "";
  for (int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    // 排除空格(32)、回车(13)、制表符(9)、换行(10)
    if (c != ' ' && c != '\r' && c != '\t' && c != '\n') {
      result += c;
    }
  }
  return result;
}


// 7. 工具函数：判断字符串是否为有效数字（含小数）
bool isNumeric(String str) {
  if (str.length() == 0) return false; // 空字符串直接返回false
  bool hasDecimal = false;
  for (int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    // 允许数字和单个小数点
    if (!isDigit(c)) {
      if (c == '.' && !hasDecimal) {
        hasDecimal = true;
      } else {
        return false; // 含非数字/多个小数点，返回false
      }
    }
  }
  return true;
}


// 8. 工具函数：单独点亮指定LED，熄灭其他
void setSingleLight(int targetLed) {
  digitalWrite(RED_LED, targetLed == RED_LED ? HIGH : LOW);
  digitalWrite(YELLOW_LED, targetLed == YELLOW_LED ? HIGH : LOW);
  digitalWrite(GREEN_LED, targetLed == GREEN_LED ? HIGH : LOW);
}