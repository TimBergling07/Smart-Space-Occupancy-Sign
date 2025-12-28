import os
import numpy as np
import librosa
from sklearn.ensemble import RandomForestClassifier
from micromlgen import port
import joblib

# === 特征提取：确保这部分逻辑与未来 ESP32 的 C++ 代码一一对应 ===
def extract_esp32_features(y, sr=16000):
    # 1. 平均音量 (RMS)
    rms = np.sqrt(np.mean(y**2))
    
    # 2 & 3. 爆发次数与强度 (Burst Count & Strength)
    # 模拟 ESP32 的 16-bit 整数处理，我们将音频放缩到 0-32768
    y_int16 = np.abs(y * 32768)
    y_int16_boosted = y_int16 * 3
    chunk_size = int(0.01 * sr)  # 10ms 一个窗口
    num_chunks = len(y_int16_boosted) // chunk_size
    
    burst_count = 0
    energies = []
    threshold = 650 # 这个值你可以根据数据分布微调
    
    for i in range(num_chunks):
        chunk_energy = np.mean(y_int16[i*chunk_size : (i+1)*chunk_size])
        if chunk_energy > threshold:
            burst_count += 1
            energies.append(chunk_energy)
    
    burst_strength = np.mean(energies) if burst_count > 0 else 0
    
    # 4. 人声占比 (Voice Ratio: 300Hz - 3000Hz)
    # 使用 1024 点 FFT，这在 ESP32 上是常见的配置
    fft = np.abs(np.fft.rfft(y, n=1024))
    freqs = np.fft.rfftfreq(1024, 1/sr)
    
    voice_band = (freqs >= 300) & (freqs <= 3000)
    total_band = (freqs >= 20) # 过滤极低频噪声
    
    voice_ratio = np.sum(fft[voice_band]) / np.sum(fft[total_band]) if np.sum(fft[total_band]) > 0 else 0
    
    return [rms, burst_count, burst_strength, voice_ratio]

# === 加载数据并训练 ===
X, y_labels = [], []
data_root = r"E:\SJTU\Grade One\First Semester\Introduction to Engineering\The Big Project\code\official\random forest for sound classification(4d)\data_augmented" # 你的数据路径

for label_name, label_id in [('empty', 0), ('occupied', 1)]:
    folder = os.path.join(data_root, label_name)
    for file in os.listdir(folder):
        if file.endswith('.wav'):
            audio, _ = librosa.load(os.path.join(folder, file), sr=16000)
            X.append(extract_esp32_features(audio))
            y_labels.append(label_id)

X = np.array(X)
y_labels = np.array(y_labels)

# 训练随机森林
# n_estimators 不建议太大（如10-20），防止生成的 C 代码占用太多闪存
clf = RandomForestClassifier(n_estimators=15, max_depth=5, random_state=42)
clf.fit(X, y_labels)

# === 导出 model.h ===
c_code = port(clf, classmap={0: 'Empty', 1: 'Occupied'})
with open("model.h", "w") as f:
    f.write(c_code)

print("model.h 已生成！你可以打开看看里面的 if-else 逻辑。")
print("文件绝对路径是:", os.path.abspath("model.h"))
joblib.dump(clf, 'my_random_forest.pkl') # 把模型存成文件