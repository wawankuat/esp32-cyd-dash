# 🏎️ ESP32-S3 CYD Dashboard & OBD2 Reader

Projek ini membina sistem paparan data pemanduan (Live Data) dan pengimbas kod kesalahan (DTC) untuk kereta Perodua Bezza 1.0 (2019) menggunakan skrin **ESP32-S3 CYD 2.8 inci**.

## 🔌 Sambungan Pinout
* **Skrin CYD TX (IO43)** -> CAN Transceiver RX
* **Skrin CYD RX (IO44)** -> CAN Transceiver TX

## 🛠️ Keperluan Perisian
1. Arduino IDE / VS Code (PlatformIO)
2. Pustaka `TFT_eSPI` (Sila pastikan fail `User_Setup.h` dilaraskan untuk skrin ILI9341).
