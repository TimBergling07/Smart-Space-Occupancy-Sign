#ifndef FEATURE_EXTRACTOR_H
#define FEATURE_EXTRACTOR_H

#include "arduinoFFT.h" // 需要安装这个库

class FeatureExtractor {
public:
    // 特征顺序：[音量RMS, 爆发次数, 爆发强度, 人声占比]
    static void extractFeatures(int16_t* samples, int count, float* features) {
        double vReal[1024];
        double vImag[1024];
        
        long sum_sq = 0;
        int burst_count = 0;
        double total_burst_energy = 0;
        int threshold = 1500; // 你之前调优后的阈值

        // 1. 计算 RMS 音量 (模拟 Python 的 np.sqrt(np.mean(y**2)))
        for (int i = 0; i < count; i++) {
            sum_sq += (long)samples[i] * samples[i];
        }
        features[0] = sqrt((double)sum_sq / count) / 32768.0;

        // 2. 计算爆发次数和强度 (10ms 窗口 = 160个采样点)
        int chunk_size = 160; 
        for (int i = 0; i < count; i += chunk_size) {
            double chunk_energy = 0;
            for (int j = 0; j < chunk_size && (i + j) < count; j++) {
                chunk_energy += abs(samples[i + j]);
            }
            chunk_energy /= chunk_size;

            if (chunk_energy > threshold) {
                burst_count++;
                total_burst_energy += chunk_energy;
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

        ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, 1024, 16000);
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