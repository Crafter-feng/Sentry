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


namespace tosee_sentry
{
    Sentry2 sentry2;

    //%
    int sentry2_Begin(int mode, int addr)
    {
        return sentry2.begin((sentry_mode_e)mode, addr);
    }

    //%
    int sentry2_LedSetColor(int detected_color, int undetected_color, int leval)
    {
        return sentry2.LedSetColor((sentry_led_color_e)detected_color, (sentry_led_color_e)undetected_color, leval);
    }

    //%
    int sentry2_CameraSetAwb(int awb)
    {
        return sentry2.CameraSetAwb((sentry_camera_white_balance_e)awb);
    }

    //%
    int sentry2_SetParamNum(int vision_type, int max_num)
    {
        return sentry2.SetParamNum((Sentry2::sentry_vision_e)vision_type, max_num);
    }

    // //%
    // int sentry2_SetParam(int vision_type, Buffer buff, int param_id)
    // {
    //     if (buff->length == 10)
    //     {
    //         sentry_object_t param; 

    //         param.result_data1 = buff->data[0]<<8|buff->data[1];
    //         param.result_data2 = buff->data[2]<<8|buff->data[3];
    //         param.result_data3 = buff->data[4]<<8|buff->data[5];
    //         param.result_data4 = buff->data[6]<<8|buff->data[7];
    //         param.result_data5 = buff->data[8]<<8|buff->data[9];

    //         return sentry2.SetParam((Sentry2::sentry_vision_e)vision_type, &param, param_id);
    //     }
        
    //     return SENTRY_FAIL;
    // }

    //%
    int sentry2_VisionSetStatus(int status, int vision_type)
    {
        if (status)
        {
            return sentry2.VisionBegin((Sentry2::sentry_vision_e)vision_type);
        }
        else
        {
            return sentry2.VisionEnd((Sentry2::sentry_vision_e)vision_type);
        }
    }

    //%
    int sentry2_GetValue(int vision_type, int obj, int obj_id)
    {
        return sentry2.GetValue((Sentry2::sentry_vision_e)vision_type, (sentry_obj_info_e)obj, obj_id);
    }

    //%
    // String sentry2_GetQrCodeValue()
    // {
    //     return PSTR(sentry2.GetQrCodeValue());
    // }
}

Sentry2 sentry(0x60);
int cnt = 0;
void setup()
{
  Wire.begin();
  delay(100);
  Serial.begin(115200);
  sentry.begin(kI2CMode);
  Serial.println("Sentry init test");
  tosee_sentry::sentry2_Begin(kI2CMode, 0x60);
}

void loop()
{
  sentry.begin(kI2CMode);
  delay(1000);
  sentry.VisionBegin(Sentry2::kVisionLine);
  delay(5000);
  sentry.VisionEnd(Sentry2::kVisionLine);
}
