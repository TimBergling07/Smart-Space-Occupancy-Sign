#include <Arduino.h>
#include <driver/i2s.h>
#include "feature_extractor.h"
#include "model.h"
#include "soc/rtc_wdt.h"

// --- 硬件引脚 (最稳配置) ---
#define I2S_WS 25
#define I2S_SD 27
#define I2S_SCK 26
#define I2S_PORT I2S_NUM_0

// --- 采样配置 (2秒窗口对齐训练集) ---
#define SAMPLE_RATE 16000
#define TOTAL_SAMPLES 32000 
int16_t audioBuffer[TOTAL_SAMPLES]; 
float features[4];

// --- 状态机配置 (让灯光更稳) ---
enum RoomStatus { EMPTY, OCCUPIED };
RoomStatus currentStatus = EMPTY;
int confidence = 0;
const int OCCUPIED_THRESHOLD = 3; // 连续3次(6s)判定有人 -> 亮灯
const int EMPTY_THRESHOLD = 6;    // 连续6次(12s)判定没人 -> 灭灯 (稍微调快一点点)

// 实例化你刚刚重新练好的随机森林
Eloquent::ML::Port::RandomForest classifier;

void setup() {
    // 强制关闭看门狗，防止 FFT 运算时重启
    disableCore0WDT(); 
    disableLoopWDT();
    rtc_wdt_protect_off();
    rtc_wdt_disable();
    
    Serial.begin(115200);
    pinMode(2, OUTPUT); // ESP32 板载蓝灯 (IO2)

    // I2S 驱动安装
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S_MSB | I2S_COMM_FORMAT_I2S),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 1024,
        .use_apll = false
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = -1,
        .data_in_num = I2S_SD
    };

    i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_PORT, &pin_config);
    
    Serial.println("\n======================================");
    Serial.println(">>>  AI 随机森林决策系统：正式启动 <<<");
    Serial.println(">>> 状态机、2s窗口、增益补偿已就绪 <<<");
    Serial.println("======================================");
}

void loop() {
    size_t bytes_read = 0;
    size_t total_read = 0;

    Serial.println("\n[监听中...] 正在采集 2 秒环境音频...");

    // 1. 采集 2 秒数据
    // 请确保这里的 while 括号是完整的
    while (total_read < TOTAL_SAMPLES) {
        rtc_wdt_feed(); // 喂狗，防止采集期间重启
        size_t to_read = (TOTAL_SAMPLES - total_read) > 1024 ? 1024 : (TOTAL_SAMPLES - total_read);
        i2s_read(I2S_PORT, &audioBuffer[total_read], to_read * sizeof(int16_t), &bytes_read, portMAX_DELAY);
        total_read += (bytes_read / sizeof(int16_t));
    }

    // 2. 提取四维特征 (注意：feature_extractor.h 里 threshold 必须也是 800)
    FeatureExtractor::extractFeatures(audioBuffer, TOTAL_SAMPLES, features);

    // 3. AI 推理
    int prediction = classifier.predict(features);

    // 4. 状态机逻辑
    if (prediction == 1) { // AI 判定有人 (Occupied)
        if (currentStatus == EMPTY) {
            confidence++;
            Serial.printf("检测到疑似信号... 置信度: %d/%d\n", confidence, OCCUPIED_THRESHOLD);
            if (confidence >= OCCUPIED_THRESHOLD) {
                currentStatus = OCCUPIED;
                confidence = 0;
                Serial.println(">>> 状态切换：【有人】 (Occupied)");
            }
        } else {
            confidence = 0; // 已经在有人状态，重置计数器
        }
    } else { // AI 判定没人 (Empty)
        if (currentStatus == OCCUPIED) {
            confidence++;
            Serial.printf("环境变安静了... 确认中: %d/%d\n", confidence, EMPTY_THRESHOLD);
            if (confidence >= EMPTY_THRESHOLD) {
                currentStatus = EMPTY;
                confidence = 0;
                Serial.println(">>> 状态切换：【无人】 (Empty)");
            }
        } else {
            confidence = 0;
        }
    }

    // 5. 串口报告 (方便演示时观察特征值)
    Serial.print("RMS: "); Serial.print(features[0], 4);
    Serial.print(" | Burst: "); Serial.print((int)features[1]);
    Serial.print(" | B_Str: "); Serial.print(features[2], 1);
    Serial.print(" | Voice: "); Serial.print(features[3] * 100, 2);
    Serial.printf("%% | 判定: %s\n", classifier.idxToLabel(prediction));

    // 6. 执行动作 (蓝灯控制)
    digitalWrite(2, (currentStatus == OCCUPIED ? HIGH : LOW));

    yield(); // 给系统留出时间处理后台任务
} // <--- 重点检查这个最后的大括号！