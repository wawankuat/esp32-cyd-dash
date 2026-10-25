name: Build ESP32 Firmware

on: [push, workflow_dispatch]

jobs:
  build:

      - name: Setup Arduino CLI
        uses: arduino/setup-arduino-cli@v1

      - name: Install ESP32 Platform
        run: |
          arduino-cli core update-index --additional-urls https://githubusercontent.com
          arduino-cli core install esp32:esp32 --additional-urls https://githubusercontent.com

      - name: Install Libraries
        run: |
          arduino-cli lib install "TFT_eSPI"

      - name: Compile Sketch
        run: |
          arduino-cli compile --fqbn esp32:esp32:esp32s3 esp32-cyd-dash.ino --output-dir ./build

      - name: Upload Binary Artifact
        uses: actions/upload-artifact@v3
        with:
          name: esp32-firmware
          path: ./build/*.bin
