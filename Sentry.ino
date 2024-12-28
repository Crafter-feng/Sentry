#include <Arduino.h>
#include <Wire.h>
#include "Sentry2.h"

// Serial
#define PIN_SERIAL_TX (0ul)
#define PIN_SERIAL_RX (1ul)

// Wire
#define PIN_WIRE_SDA        (4u)
#define PIN_WIRE_SCL        (5u)

uint32_t I2CRead(uint8_t address, uint8_t reg_address, uint8_t* temp) {
  uint8_t ret = SENTRY_OK;
  Wire.beginTransmission((uint8_t)address);
  ret = Wire.write(reg_address);
  //Debug Output
  DOPRINTF("[R:%02x,", reg_address);

  if (!ret) return SENTRY_READ_TIMEOUT;
  Wire.endTransmission();
  if (Wire.requestFrom(address, 1) != 1)
    return SENTRY_READ_TIMEOUT;

  *temp = Wire.read();
  //Debug Output
  DOPRINTF("%02x],", *temp);

  return SENTRY_OK;
}

uint32_t I2CWrite(uint8_t address, uint8_t reg_address, uint8_t value) {
  uint8_t ret = SENTRY_OK;
  Wire.beginTransmission((uint8_t)address);
  ret = Wire.write(reg_address);
  if (!ret) return SENTRY_READ_TIMEOUT;
  ret = Wire.write(value);
  if (!ret) return SENTRY_READ_TIMEOUT;
  Wire.endTransmission();
  //Debug Output
  DOPRINTF("[W:%02x,%02x],",reg_address,value);

  return SENTRY_OK;
}

Sentry2 sentry(0x60);
int cnt = 0;
void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop()
{
  DOPRINTF("Sentry init test");
  // sentry.begin(kSerialMode);
  // delay(1000);
  // sentry.VisionBegin(Sentry2::kVisionLine);
  // delay(5000);
  // sentry.VisionEnd(Sentry2::kVisionLine);
}
