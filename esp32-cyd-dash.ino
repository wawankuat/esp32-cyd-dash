#include <SPI.h>
#include <TFT_eSPI.h> 
// Paksa library menggunakan pinout dari fail tempatan kita, bukannya fail dalam folder library asal
#define PATH_TO_USER_SETUP_HDR "User_Setup.h" 

TFT_eSPI tft = TFT_eSPI();

// Pembolehubah Data Terimaan OBD2 Bezza 1.0
int rpm = 0;
int speed_kmh = 0;
int coolant_temp = 88;
int current_gear = 3; // Menggunakan logik nisbah kelajuan/RPM untuk kotak gear automatik Bezza
float voltage = 12.8;

unsigned long lastUiUpdate = 0;
bool showingDtcPage = false;

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi Sistem Komunikasi OBD2 
  // Jika guna ELM327 Bluetooth: Sediakan fungsi SerialBT.begin("ESP32_Dash"); di sini
  setupObdConnection();

  // Inisialisasi Paparan Skrin CYD
  tft.init();
  tft.setRotation(1); // Mod Melintang (Landscape)
  tft.fillScreen(TFT_BLACK);
  
  drawHaltechLayout();
}

void loop() {
  if (!showingDtcPage) {
    // 1. Ambil data secara berkala dari OBD2 Bezza
    requestObdData();

    // 2. Kemaskini paparan skrin setiap 100ms (10 FPS) supaya lancar
    if (millis() - lastUiUpdate > 100) { 
      updateDashValues();
      lastUiUpdate = millis();
    }
    
    // Contoh Logik: Jika ada trigger / butang ditekan pada CYD, tukar ke skrin semakan DTC
    // if(digitalRead(BUTTON_PIN) == LOW) { checkBezzaDTC(); }
  }
}

// Rekabentuk Grafik Statik seiras gambar inspirasi anda
void drawHaltechLayout() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  
  // Sempadan Bingkai Widget Tepi
  tft.drawRoundRect(10, 10, 65, 140, 4, TFT_DARKGREY);  // Kotak Bar Minyak/Gear
  tft.drawRoundRect(245, 10, 65, 80, 4, TFT_DARKGREY);  // Kotak Voltan
  tft.drawRoundRect(245, 100, 65, 130, 4, TFT_DARKGREY); // Kotak Suhu

  // Label Statik Grafik (Tachometer Bahagian Tengah)
  tft.drawCircle(160, 120, 85, TFT_SILVER);
  tft.drawString("Haltech", 135, 95, 2);
  tft.drawString("ESP32", 145, 120, 1);
  tft.drawString("SUPER DASH", 130, 130, 1);
  
  // Teks Rujukan Unit
  tft.drawString("GEAR", 25, 130, 1);
  tft.drawString("KM/H", 185, 175, 1);
  tft.drawString("TEMP", 263, 215, 1);
}

// Kemaskini Parameter Dinamik mengikut bacaan semasa kenderaan
void updateDashValues() {
  // --- SIMULASI DATA UNTUK DEMO (Gantikan dengan variabel dari fungsi requestObdData) ---
  speed_kmh = random(54, 58);
  rpm = random(2500, 3000);
  
  // 1. Paparan Kelajuan Besar (Tengah-Bawah)
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextFont(4); // Font saiz besar
  tft.drawNumber(speed_kmh, 140, 160);
  
  // 2. Paparan Kedudukan Gear Automatik (Kiri-Bawah)
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextFont(4);
  tft.drawNumber(current_gear, 33, 95);

  // 3. Paparan Voltan Bateri (Kanan-Atas)
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextFont(2);
  tft.setCursor(250, 20);
  tft.print(voltage, 1); tft.print("v");

  // 4. Paparan Suhu Enjin Coolant (Kanan-Bawah)
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextFont(4);
  tft.drawNumber(coolant_temp, 250, 175);
  tft.drawString("C", 295, 175, 2);
}

// Struktur Penghantaran Request PID OBD2 Standard untuk Perodua Bezza
void requestObdData() {
  // Contoh Request PID standard ISO 15765-4 (CAN 11bit 500kbps)
  // Engine RPM PID: 01 0C  |  Vehicle Speed PID: 01 0D  | Coolant Temp PID: 01 05
  
  // Logik komunikasi ELM327 / CAN Transceiver diletakkan di sini untuk mengemas kini:
  // rpm, speed_kmh, coolant_temp, dan voltage.
}

void setupObdConnection() {
  // Sediakan tetapan sambungan ke ELM327 atau chip CAN transceiver
  // Contoh jika guna ELM327: Kirim arahan ATZ, AT SP 0 (Auto Protocol)
}

// Fungsi Membaca & Memaparkan DTC (Diagnostic Trouble Codes) Bezza 1.0
void checkBezzaDTC() {
  showingDtcPage = true;
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED);
  tft.drawString("OBD2 DIAGNOSTIC SCAN...", 20, 20, 4);
  
  // Kirim Kod OBD2 Mode 03 (Request Emission-Related Diagnostic Trouble Codes)
  // Contoh maklum balas simulasi:
  delay(2000); 
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.drawString("SISTEM ENGINE BEZZA OK", 20, 50, 4);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Tiada Kod Kesalahan (DTC) Dijumpai.", 20, 100, 2);
  
  delay(4000); // Tunggu 4 saat sebelum masuk semula ke paparan dashboard utama
  showingDtcPage = false;
  drawHaltechLayout();
}
