# Build ArduinoOBI with the Arduino IDE

This folder provides a standard Arduino IDE sketch alongside the existing PlatformIO project. PlatformIO files and firmware builds are unchanged.

## Install the OneWire library used by this project

The firmware depends on the project's customized OneWire library. Copy the complete folder `ArduinoOBI/lib/OneWire` from the repository into the Arduino libraries folder, and rename the copied folder to `OneWire2` if necessary. The installed library should contain `library.properties`, `OneWire2.cpp`, `OneWire2.h`, and the `util` directory directly inside `OneWire2`.

Typical library locations:

- Windows: `Documents/Arduino/libraries/OneWire2`
- macOS: `~/Documents/Arduino/libraries/OneWire2`
- Linux: `~/Arduino/libraries/OneWire2`

Restart the Arduino IDE after installing the library.

## Open and build

1. Open `ArduinoIDE/ArduinoOBI/ArduinoOBI.ino` in Arduino IDE.
2. For an Arduino Uno, select **Arduino Uno**. The sketch uses OneWire pin 6 and ENABLE pin 8.
3. For an AI Thinker ESP32-CAM, install/select the ESP32 Arduino core and select **AI Thinker ESP32-CAM**. The sketch uses OneWire GPIO 14 and ENABLE GPIO 13. Connect pull-up resistors to **3.3 V**, not 5 V.
4. Select the connected board's port and choose **Verify** to compile, or **Upload** to flash.

The IDE sketch defaults firmware version fields to `0.0.0`; PlatformIO continues to derive version fields from Git tags using its existing script.
