#include <Arduino.h>
#include <Wire.h>
#include "Sentry2.h"

uint32_t sentry_i2c_read(sentry_method_base_t *base, uint8_t reg_address, uint8_t *temp)
{
  uint8_t ret = SENTRY_OK;
  Wire.beginTransmission((uint8_t)base->address);
  ret = Wire.write(reg_address);
  // Debug Output
  DOPRINTF("[R:%02x,", reg_address);

  if (!ret)
    return SENTRY_READ_TIMEOUT;
  Wire.endTransmission();
  if (Wire.requestFrom(base->address, 1) != 1)
    return SENTRY_READ_TIMEOUT;

  *temp = Wire.read();
  // Debug Output
  DOPRINTF("%02x],", *temp);

  return SENTRY_OK;
}

uint32_t sentry_i2c_write(sentry_method_base_t *base, uint8_t reg_address, uint8_t value)
{
  uint8_t ret = SENTRY_OK;
  Wire.beginTransmission((uint8_t)base->address);
  ret = Wire.write(reg_address);
  if (!ret)
    return SENTRY_READ_TIMEOUT;
  ret = Wire.write(value);
  if (!ret)
    return SENTRY_READ_TIMEOUT;
  Wire.endTransmission();
  // Debug Output
  DOPRINTF("[W:%02x,%02x],", reg_address, value);

  return SENTRY_OK;
}

int sentry_serial_read(sentry_method_base_t *base, uint8_t *pkg_b, int len)
{
  int ret = 0;
  ret = Serial1.readBytes(pkg_b, len);
#if SENTRY_DEBUG_ENABLE && LOG_OUTPUT
  DOPRINTF("R%d %02x\n", ret, pkg_b[0]);
#endif
  return ret > 0 ? 1 : 0;
}

void sentry_serial_write(sentry_method_base_t *base, const uint8_t *pkg_b, int len)
{
#if SENTRY_DEBUG_ENABLE && LOG_OUTPUT
  DOPRINTF("pkg_b[%d]", len);
  for (unsigned int i = 0; i < len; ++i)
  {
    DOPRINTF("%02x ", pkg_b[i]);
  }
  DOPRINTF("\n");
#endif

  Serial1.write((unsigned char *)pkg_b, len);
}

void sentry_debug_send(uint8_t *buffer, int bufferLen)
{
  Serial.write(buffer, bufferLen);
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
