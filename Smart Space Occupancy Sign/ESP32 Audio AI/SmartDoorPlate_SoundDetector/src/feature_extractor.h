#ifndef FEATURE_EXTRACTOR_H
#define FEATURE_EXTRACTOR_H

#include "arduinoFFT.h" // 需要安装这个库

static double vReal[1024];
static double vImag[1024];

class FeatureExtractor {
public:
    // 特征顺序：[音量RMS, 爆发次数, 爆发强度, 人声占比]
    static void extractFeatures(int16_t* samples, int count, float* features) {

        
        long sum_sq = 0;
        int burst_count = 0;
        double total_burst_energy = 0;
        int threshold = 650; // 你之前调优后的阈值

        // 1. 计算 RMS 音量 (模拟 Python 的 np.sqrt(np.mean(y**2)))
        for (int i = 0; i < count; i++) {
            sum_sq += (long)samples[i] * samples[i];
        }
        features[0] = (sqrt((double)sum_sq / count) / 32768.0) * 3.5;

        // 2 & 3. 爆发次数与强度 (Burst Count & Strength)
        int chunk_size = 160; // 10ms 窗口 (16000Hz * 0.01s)
        for (int i = 0; i < count; i += chunk_size) {
            double chunk_sum = 0;
            int actual_chunk = (i + chunk_size <= count) ? chunk_size : (count - i);
            
            for (int j = 0; j < actual_chunk; j++) {
                chunk_sum += (double)abs(samples[i + j]) * 3;// 【核心补偿】在这里乘以 4.0，强行拉高传感器捕捉到的微弱波动
            }
            
            // 计算当前 10ms 块的平均能量
            double chunk_avg = chunk_sum / actual_chunk;

            // 如果放大后的能量超过了我们同步好的 300 阈值，计入 Burst
            if (chunk_avg > threshold) {
                burst_count++;
                total_burst_energy += chunk_avg;
            }
        }
        features[1] = (float)burst_count;
        features[2] = burst_count > 0 ? (float)(total_burst_energy / burst_count) : 0;

        // 3. 计算人声占比 (FFT 300Hz - 3000Hz)
        // 取前 1024 个点做一次 FFT 示例
        for (int i = 0; i < 1024; i++) {
            vReal[i] = (i < count) ? (double)samples[i] : 0;
            vImag[i] = 0;
        }

        static ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, 1024, 16000);
        FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
        FFT.compute(FFT_FORWARD);
        FFT.complexToMagnitude();

        double voice_sum = 0;
        double total_sum = 0;
        for (int i = 2; i < 512; i++) { // 跳过接近 0 的极低频
            double freq = i * 16000.0 / 1024.0;
            if (freq >= 300 && freq <= 3000) voice_sum += vReal[i];
            if (freq >= 20) total_sum += vReal[i];
        }
        features[3] = total_sum > 0 ? (float)(voice_sum / total_sum) : 0;
    }
};

#endif