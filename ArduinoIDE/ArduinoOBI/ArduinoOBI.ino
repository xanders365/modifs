#include <Arduino.h>
#include <OneWire2.h>

// Keep these in sync with ArduinoOBI/platformio.ini's esp32cam environment.
#if defined(ARDUINO_ARCH_ESP32)
#define ONEWIRE_PIN 14
#define ENABLE_PIN 13
#else
#define ONEWIRE_PIN 6
#define ENABLE_PIN 8
#endif

#ifndef ARDUINO_OBI_VERSION_MAJOR
#define ARDUINO_OBI_VERSION_MAJOR 0
#endif
#ifndef ARDUINO_OBI_VERSION_MINOR
#define ARDUINO_OBI_VERSION_MINOR 0
#endif
#ifndef ARDUINO_OBI_VERSION_PATCH
#define ARDUINO_OBI_VERSION_PATCH 0
#endif

OneWire makita(ONEWIRE_PIN);

void cmd_and_read_33(byte *cmd, uint8_t cmd_len, byte *rsp, uint8_t rsp_len) {
  makita.reset();
  delayMicroseconds(400);
  makita.write(0x33, 0);

  for (int i = 0; i < 8; i++) {
    delayMicroseconds(90);
    rsp[i] = makita.read();
  }
  for (int i = 0; i < cmd_len; i++) {
    delayMicroseconds(90);
    makita.write(cmd[i], 0);
  }
  for (int i = 8; i < rsp_len + 8; i++) {
    delayMicroseconds(90);
    rsp[i] = makita.read();
  }
}

void cmd_and_read_cc(byte *cmd, uint8_t cmd_len, byte *rsp, uint8_t rsp_len) {
  makita.reset();
  delayMicroseconds(400);
  makita.write(0xcc, 0);

  for (int i = 0; i < cmd_len; i++) {
    delayMicroseconds(90);
    makita.write(cmd[i], 0);
  }
  for (int i = 0; i < rsp_len; i++) {
    delayMicroseconds(90);
    rsp[i] = makita.read();
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(ENABLE_PIN, OUTPUT);
}

void send_usb(byte *rsp, byte rsp_len) {
  for (int i = 0; i < rsp_len; i++) {
    Serial.write(rsp[i]);
  }
}

void read_usb() {
  if (Serial.available() >= 4) {
    byte start = Serial.read();
    byte cmd;
    byte len;
    byte data[255];
    byte rsp[255];
    byte rsp_len;

    if (start != 0x01) {
      return;
    }

    len = Serial.read();
    rsp_len = Serial.read();
    cmd = Serial.read();
    if (len > 0) {
      for (int i = 0; i < len; i++) {
        while (Serial.available() < 1) {}
        data[i] = Serial.read();
      }
    }

    digitalWrite(ENABLE_PIN, HIGH);
    delay(400);

    switch (cmd) {
      case 0x01:
        rsp[0] = 0x01;
        rsp[2] = ARDUINO_OBI_VERSION_MAJOR;
        rsp[3] = ARDUINO_OBI_VERSION_MINOR;
        rsp[4] = ARDUINO_OBI_VERSION_PATCH;
        break;
      case 0x31:
      case 0x32:
        makita.reset();
        delayMicroseconds(400);
        makita.write(0xcc, 0);
        delayMicroseconds(90);
        makita.write(0x99, 0);
        delay(400);
        makita.reset();
        delayMicroseconds(400);
        makita.write(cmd, 0);
        delayMicroseconds(90);
        rsp[3] = makita.read();
        delayMicroseconds(90);
        rsp[2] = makita.read();
        delayMicroseconds(90);
        break;
      case 0x33:
        cmd_and_read_33(data, len, &rsp[2], rsp_len);
        break;
      case 0xCC:
        cmd_and_read_cc(data, len, &rsp[2], rsp_len);
        break;
      default:
        rsp_len = 0;
        break;
    }

    rsp[0] = cmd;
    rsp[1] = rsp_len;
    send_usb(rsp, rsp_len + 2);
    digitalWrite(ENABLE_PIN, LOW);
  }
}

void loop() {
  read_usb();
}
