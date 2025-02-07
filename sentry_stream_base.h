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

#ifndef SENTRY_STREAM_H
#define SENTRY_STREAM_H

#define SENTRY_MICRO_BIT 1

#include "sentry_type.h"
#include "debug/debug_tool.h"

typedef struct sentry_method_base_t
{
    void* user;
    uint8_t address;
    int (*serial_read)(struct sentry_method_base_t *base, uint8_t *pkg_b, int len);
    void (*serial_write)(struct sentry_method_base_t *base, const uint8_t *pkg_b, int len);
    uint32_t (*i2c_read)(struct sentry_method_base_t * base, uint8_t reg_address, uint8_t *temp);
    uint32_t (*i2c_write)(struct sentry_method_base_t * base, uint8_t reg_address, uint8_t value);
}sentry_method_base_t;

typedef struct
{
    sentry_err_t (*Get)(struct sentry_method_base_t * base, const uint8_t reg_address, uint8_t *value);
    sentry_err_t (*Set)(struct sentry_method_base_t * base, const uint8_t reg_address, const uint8_t value);
    sentry_err_t (*SetParam)(struct sentry_method_base_t * base, int vision_type, sentry_object_t *param, int param_id);
    sentry_err_t (*Read)(struct sentry_method_base_t * base, int vision_type, sentry_vision_state_t *vision_state);
    sentry_err_t (*ReadQrCode)(struct sentry_method_base_t * base, int vision_type, sentry_qrcode_state_t *qrcode);
    sentry_err_t (*Write)(struct sentry_method_base_t * base, int vision_type, const sentry_vision_state_t *vision_state);
}sentry_stream_method_t;

typedef struct
{
    sentry_method_base_t base;
    sentry_stream_method_t method;
}sentry_stream_t;


#define Sentry_Stream_Get(stream_, reg_address, value) stream_.method.Get(&(stream_.base), reg_address, value)
#define Sentry_Stream_Set(stream_, reg_address, value) stream_.method.Set(&(stream_.base), reg_address, value)
#define Sentry_Stream_SetParam(stream_, vision_type, param, param_id) stream_.method.SetParam(&(stream_.base), vision_type, param, param_id)
#define Sentry_Stream_Read(stream_, vision_type, vision_state) stream_.method.Read(&(stream_.base), vision_type, vision_state)
#define Sentry_Stream_ReadQrCode(stream_, vision_type, qrcode) stream_.method.ReadQrCode(&(stream_.base), vision_type, qrcode)
#define Sentry_Stream_Write(stream_, vision_type, vision_state) stream_.method.Write(&(stream_.base), vision_type, vision_state)

#endif

