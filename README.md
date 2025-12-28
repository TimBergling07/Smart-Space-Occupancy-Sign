# **Smart Space Occupancy Sign**

### **An Intelligent Multi-modal Sensing System for Real-time Space Management**

## **1. Project Overview**

This project addresses the conflict between "Displayed Status" and "Actual Usage" in shared spaces (classrooms, meeting rooms, labs). We developed an intelligent occupancy sign that uses **Multi-modal Heterogeneous Fusion** to detect human presence with high reliability, even in "static" or "silent" scenarios.

## **2. System Architecture**

The system adopts a distributed control architecture:

* **Audio AI Module (ESP32)**: Captures acoustic features and runs a Random Forest classifier to identify human activities (talking, typing, moving).
* **Microwave Radar Module (Arduino Uno)**: Detects micro-movements (breathing, slight shifting) via 24GHz microwave Doppler effect.
* **Indicator System**: A tri-color LED system (Red: Occupied, Yellow: Leaving soon, Green: Vacant).

## **3. Key Technical Highlights**

### **3.1 AI Audio Feature Engineering**

To overcome the limitations of far-field sound pickup, we implemented custom non-linear gain compensation:

* **RMS Factor (*5.0)**: Amplifies the root-mean-square energy of the signal to capture faint distant sounds.
* **Burst Factor (*4.0)**: Enhances transient pulse detection (e.g., pen clicking, page turning).
* **Classifier**: A Random Forest model optimized for low-power edge deployment on ESP32.

### **3.2 Heterogeneous Logic Fusion**

We designed an **Asynchronous Trigger-and-Hold** logic to handle the "Blind Zone" and "Static Presence" issues:

1. **Fast Wake-up**: The system triggers "Occupied" if *either* the Radar or AI detects a signal within a 6-second window.
2. **Robust Maintenance**: A **30-minute cooldown timer** prevents frequent flickering. The status only reverts to "Vacant" if no signals are detected by *both* sensors for the entire duration.

## **4. Repository Structure**

```text
├── ESP32-Audio-AI/          # Source code for AI inference & sound processing
├── Arduino-Uno-Radar/       # Radar signal processing & LED control logic
└── Hardware/            # 3D modeling files (.STL & .STEP) for the enclosure

```

## **5. Hardware Requirements**

* **Microcontrollers**: ESP32-DevKitC, Arduino Uno R3
* **Sensors**: INMP441 (I2S Digital Microphone), LD2402 (24GHz Microwave Radar)
* **Outputs**: RGB LED Strip / Matrix
* **Enclosure**: Custom 3D-printed PLA housing

## **6. Reflection & Future Work**

* **Current Limitation**: In extreme silence (e.g., deep sleep without movement), detection remains a challenge.
* **Optimization**: Future iterations will migrate to a single-chip solution (ESP32-S3) to reduce power consumption and latency.

## **7. Authors & Acknowledgments**

Developed by a team of students from **Shanghai Jiao Tong University (SJTU)**:

* **Wang Yanzhe (Project Lead & AI Developer)**
* Xiong Jinyi(AI Developer), Li Zeyu(3D-Model Designer), Pan Jingyuan(Electronic Control), Dong Yunhe(Report and PPT Making).

Special thanks to **SJTU teachers:Yang Li and Wang Zechen** for providing instructions, testing environment and equipment.
