// Copyright 2021 Tosee Intelligence (hangzhoou) co.,ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SENTRY2_H_
#define SENTRY2_H_

#include "SentryFactory.h"

#define SENTRY2_DEVICE_ID 0x04

class Sentry2
{
public:
    sentry_method_base_t method_base;
    Sentry2(uint8_t address = 0x60)
    {
        method_base.address = address;
        SentryFactory_Init(&factory, address, SENTRY2_DEVICE_ID, product_vision_state_,
                           int(kVisionMaxType), int(kVisionQrCode), method_base);
    }
    virtual ~Sentry2() {}
    Sentry2(const Sentry2 &) = delete;
    Sentry2 &operator=(const Sentry2 &) = delete;

    enum sentry_vision_e
    {
        //% block="Color"
        kVisionColor = 1,
        //% block="Blob"
        kVisionBlob = 2,
        //% block="AprilTag"
        kVisionAprilTag = 3,
        //% block="Line"
        kVisionLine = 4,
        //% block="Learning"
        kVisionLearning = 5,
        //% block="Card"
        kVisionCard = 6,
        //% block="Face"
        kVisionFace = 7,
        //% block="20Class"
        kVision20Classes = 8,
        //% block="QrCode"
        kVisionQrCode = 9,
        //% block="Custom"
        kVisionCustom = 10,
        //% block="Motion"
        kVisionMotionDetect = 11,
        //% blockHidden=true
        kVisionMaxType,
    };

    /* SentryFactory label */
    enum
    {
        kUnknownLabel = 0
    };
    /* Sentry card label */
    enum card_label_e
    {
        //% block="Forward"
        kCardForward = 1,
        //% block="Left"
        kCardLeft = 2,
        //% block="Right"
        kCardRight = 3,
        //% block="Turn Around"
        kCardTurnAround = 4,
        //% block="Park"
        kCardPark = 5,
        //% block="Green Light"
        kCardGreenLight = 6,
        //% block="Red Light"
        kCardRedLight = 7,
        //% block="Speed 40"
        kCardSpeed40 = 8,
        //% block="Speed 60"
        kCardSpeed60 = 9,
        //% block="Speed 80"
        kCardSpeed80 = 10,
        //% block="✔"
        kCardCheck = 11,
        //% block="☓"
        kCardCross = 12,
        //% block="◯"
        kCardCircle = 13,
        //% block="☐"
        kCardSquare = 14,
        //% block="△"
        kCardTriangle = 15,
        //% block="+"
        kCardPlus = 16,
        //% block="-"
        kCardMinus = 17,
        //% block="÷"
        kCardDivide = 18,
        //% block="="
        kCardEqual = 19,
        //% block="0"
        kCardZero = 20,
        //% block="1"
        kCardOne = 21,
        //% block="2"
        kCardTwo = 22,
        //% block="3"
        kCardThree = 23,
        //% block="4"
        kCardFour = 24,
        //% block="5"
        kCardFive = 25,
        //% block="6"
        kCardSix = 26,
        //% block="7"
        kCardSeven = 27,
        //% block="8"
        kCardEight = 28,
        //% block="9"
        kCardNine = 29,
    };
    /* SentryFactory 20 classes label */
    enum class20_label_e
    {
        //% block="Airplane"
        kAirplane = 1,
        //% block="Bicycle"
        kBicycle = 2,
        //% block="Bird"
        kBird = 3,
        //% block="Boat"
        kBoat = 4,
        //% block="Bottle"
        kBottle = 5,
        //% block="Bus"
        kBus = 6,
        //% block="Car"
        kCar = 7,
        //% block="Cat"
        kCat = 8,
        //% block="Chair"
        kChair = 9,
        //% block="Cow"
        kCow = 10,
        //% block="DiningTable"
        kDiningTable = 11,
        //% block="Dog"
        kDog = 12,
        //% block="Horse"
        kHorse = 13,
        //% block="MotorBike"
        kMotorBike = 14,
        //% block="Person"
        kPerson = 15,
        //% block="PottedPlant"
        kPottedPlant = 16,
        //% block="Sheep"
        kSheep = 17,
        //% block="Sofa"
        kSofa = 18,
        //% block="Train"
        kTrain = 19,
        //% block="TvMonitor"
        kTvMonitor = 20
    };
    /* SentryFactory color label */
    enum color_label_e
    {
        //% block="Black"
        kColorBlack = 1,
        //% block="White"
        kColorWhite = 2,
        //% block="Red"
        kColorRed = 3,
        //% block="Green"
        kColorGreen = 4,
        //% block="Blue"
        kColorBlue = 5,
        //% block="Yellow"
        kColorYellow = 6
    };

    /**
     * @brief  Sentry begin with Serial.
     * @param  mode: uart or i2c.
     * @retval SENTRY_OK: begin success.
     *         other: protocol assert fail.
     */
    uint8_t begin(sentry_mode_e mode = kI2CMode,
                  int set_default = 1)
    {
        return SentryFactory_Begin(&factory, mode, set_default);
    }
    /**
     * @brief  begin vision.
     * @param  vision_type: vision type.
     * @retval SENTRY_OK:  success
     *         other:  error
     */
    uint8_t VisionBegin(sentry_vision_e vision_type)
    {
        return SentryFactory_VisionBegin(&factory, (int)vision_type);
    }
    /**
     * @brief  end vision.
     * @param  vision_type: vision type.
     * @retval SENTRY_OK:  success
     *         other:  error
     */
    uint8_t VisionEnd(sentry_vision_e vision_type)
    {
        return SentryFactory_VisionEnd(&factory, (int)vision_type);
    }
    /**
     * @brief  Set max detect object number.
     * @param  vision_type: vision type.
     * @param  param:  vision parameter
     * @param  max_num:  max number
     * @retval SENTRY_OK:  success
     *         other:  error
     */
    uint8_t SetParamNum(sentry_vision_e vision_type, int max_num)
    {
        return SentryFactory_SetParamNum(&factory, (int)vision_type, max_num);
    }
    /**
     * @brief  write vision parameter.
     * @param  vision_type: vision type.
     * @param  param:  vision parameter
     * @param  param_id:  parameter id
     * @retval SENTRY_OK:  success
     *         other:  error
     */
    uint8_t SetParam(sentry_vision_e vision_type, sentry_object_t *param,
                     int param_id = 1)
    {
        return Sentry_Stream_SetParam(factory.stream, (int)vision_type, param, param_id);
    }

    uint8_t VisionSetMode(int vision_type, int mode)
    {
        return SentryFactory_VisionSetMode(&factory, vision_type, mode);
    }

    /**
     * @brief  get vision result data, this function will update vision
     *         result automatically.
     * @param  vision_type: vision type.
     * @param  obj_info:  object information
     * @retval information value
     */
    int GetValue(sentry_vision_e vision_type, sentry_obj_info_e obj_info,
                 int obj_id = 1)
    {
        return SentryFactory_GetValue(&factory, (int)vision_type, obj_info, obj_id);
    }

    char *GetQrCodeValue()
    {
        if (factory.qrcode_state)
        {
            return factory.qrcode_state->qrcode_result[0].str;
        }
        return NULL;
    }

    uint8_t VisionSetDefault(sentry_vision_e vision_type)
    {
        return SentryFactory_VisionSetDefault(&factory, (int)vision_type);
    }

    uint8_t LedSetColor(sentry_led_color_e detected_color,
                        sentry_led_color_e undetected_color,
                        uint8_t level = 1)
    {
        return SentryFactory_LedSetColor(&factory, detected_color, undetected_color, level);
    }

    uint8_t CameraSetAwb(sentry_camera_white_balance_e awb)
    {
        return SentryFactory_CameraSetAwb(&factory, awb);
    }

private:
    SentryFactory factory;
    sentry_vision_state_t *product_vision_state_[kVisionMaxType - 1] = {nullptr};
};

#endif /* SENTRY2_H_ */
