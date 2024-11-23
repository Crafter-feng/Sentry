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

#ifndef SENTRY_FACTORY_H_
#define SENTRY_FACTORY_H_

#include "sentry_i2c.h"
#include "sentry_uart.h"

typedef struct {
    uint32_t address;
    uint8_t device_id;
    int vision_max_type;
    int vision_qrcode_type;
    sentry_vision_state_t **vision_state;
    sentry_stream_t stream;
    int mode;
    uint16_t img_w;
    uint16_t img_h;
    sentry_qrcode_state_t *qrcode_state;
} SentryFactory;

void SentryFactory_Init(SentryFactory *factory, uint32_t address, uint8_t device_id,
                        sentry_vision_state_t **vision_state, int vision_max_type,
                        int vision_qrcode_type);

void SentryFactory_Free(SentryFactory *factory);

uint8_t SentryFactory_SensorStartupCheck(SentryFactory *factory);

uint8_t SentryFactory_ProtocolVersionCheck(SentryFactory *factory);

uint8_t SentryFactory_GetImageShape(SentryFactory *factory);

uint8_t SentryFactory_SensorInit(SentryFactory *factory, int set_default);

uint8_t SentryFactory_Begin(SentryFactory *factory, sentry_mode_e mode, int set_default);

uint8_t SentryFactory_VisionBegin(SentryFactory *factory, int vision_type);

uint8_t SentryFactory_VisionEnd(SentryFactory *factory, int vision_type);

int SentryFactory_GetValue(SentryFactory *factory, int vision_type, sentry_obj_info_e obj_info, int obj_id);

int SentryFactory_GetParamNum(SentryFactory *factory, int vision_type);

uint8_t SentryFactory_SetParamNum(SentryFactory *factory, int vision_type, int max_num);

const sentry_vision_state_t *SentryFactory_GetVisionState(SentryFactory *factory, int vision_type);

uint8_t SentryFactory_SetVisionState(SentryFactory *factory, int vision_type, sentry_vision_state_t *state);

uint8_t SentryFactory_VisionSetStatus(SentryFactory *factory, int vision_type, int enable);

uint8_t SentryFactory_VisionSetDefault(SentryFactory *factory, int vision_type);

int SentryFactory_VisionGetStatus(SentryFactory *factory, int vision_type);

uint8_t SentryFactory_VisionSetMode(SentryFactory *factory, int vision_type, int mode);

uint8_t SentryFactory_VisionGetMode(SentryFactory *factory, int vision_type, int *mode);

uint8_t SentryFactory_VisionSetLevel(SentryFactory *factory, int vision_type, sentry_vision_level_e level);

uint8_t SentryFactory_VisionGetLevel(SentryFactory *factory, int vision_type, sentry_vision_level_e *level);

uint8_t SentryFactory_SensorSetRestart(SentryFactory *factory);

uint8_t SentryFactory_SensorSetDefault(SentryFactory *factory, int vision_default_only);

uint8_t SentryFactory_SeneorSetCoordinateType(SentryFactory *factory, sentry_coordinate_type_e type);

uint8_t SentryFactory_SensorLockReg(SentryFactory *factory, int lock);

uint8_t SentryFactory_LedSetColor(SentryFactory *factory, sentry_led_color_e detected_color,
                                  sentry_led_color_e undetected_color, uint8_t level);

uint8_t SentryFactory_CameraSetZoom(SentryFactory *factory, sentry_camera_zoom_e zoom);

uint8_t SentryFactory_CameraSetAwb(SentryFactory *factory, sentry_camera_white_balance_e awb);

sentry_camera_zoom_e SentryFactory_CameraGetZoom(SentryFactory *factory);

sentry_camera_white_balance_e SentryFactory_CameraGetAwb(SentryFactory *factory);

int SentryFactory_CameraGetRotate(SentryFactory *factory);

sentry_camera_fps_e SentryFactory_CameraGetFPS(SentryFactory *factory);

uint8_t SentryFactory_UartSetBaudrate(SentryFactory *factory, sentry_baudrate_e baud);

uint8_t SentryFactory_Snapshot(SentryFactory *factory, uint8_t image_dest,
                               sentry_snapshot_src_e image_src, sentry_snapshot_type_e image_type);

uint8_t SentryFactory_UserImageCoordinateConfig(SentryFactory *factory, uint8_t image_id,
                                                uint16_t x_value, uint16_t y_value,
                                                uint16_t width, uint16_t height);

uint8_t SentryFactory_ScreenConfig(SentryFactory *factory, int enable, int only_user_image);

uint8_t SentryFactory_ScreenShow(SentryFactory *factory, uint8_t image_id, uint8_t auto_reload);

uint8_t SentryFactory_ScreenShowFromFlash(SentryFactory *factory, uint8_t image_id, uint8_t auto_reload);

uint8_t SentryFactory_ScreenFill(SentryFactory *factory, uint8_t image_id, uint8_t r, uint8_t g, uint8_t b, uint8_t auto_reload);

#endif // SENTRY_FACTORY_H
