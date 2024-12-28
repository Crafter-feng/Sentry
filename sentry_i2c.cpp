#include "sentry_i2c.h"

static sentry_err_t sentry_i2c_get(struct sentry_method_base_t * base, const uint8_t reg_address, uint8_t *value)
{
  return base->i2c_read(base, reg_address, value);
}

static sentry_err_t sentry_i2c_set(struct sentry_method_base_t * base, const uint8_t reg_address, const uint8_t value)
{
  return base->i2c_write(base, reg_address, value);
}

static sentry_err_t sentry_i2c_read(struct sentry_method_base_t * base, int vision_type, sentry_vision_state_t *vision_state)
{
  sentry_err_t err;
  uint8_t result_data_tmp[2];
  err = sentry_i2c_set(base, kRegVisionId, vision_type);
  if (err)
    return err;
  err = sentry_i2c_get(base, kRegFrameCount, &vision_state->frame);
  if (err)
    return err;
  err = sentry_i2c_get(base, kRegResultNumber, &vision_state->detect);
  if (err)
    return err;
  if (!vision_state->detect)
    return SENTRY_OK;
  vision_state->detect = SENTRY_MAX_RESULT < vision_state->detect
                             ? SENTRY_MAX_RESULT
                             : vision_state->detect;
  for (uint8_t i = 0; i < vision_state->detect; ++i)
  {
    err = sentry_i2c_set(base, kRegResultId, i + 1);
    if (err)
      return err;
    sentry_i2c_get(base, kRegResultData1L, result_data_tmp);
    sentry_i2c_get(base, kRegResultData1H, result_data_tmp + 1);
    vision_state->vision_result[i].result_data1 =
        result_data_tmp[1] << 8 | result_data_tmp[0];
    sentry_i2c_get(base, kRegResultData2L, result_data_tmp);
    sentry_i2c_get(base, kRegResultData2H, result_data_tmp + 1);
    vision_state->vision_result[i].result_data2 =
        result_data_tmp[1] << 8 | result_data_tmp[0];
    sentry_i2c_get(base, kRegResultData3L, result_data_tmp);
    sentry_i2c_get(base, kRegResultData3H, result_data_tmp + 1);
    vision_state->vision_result[i].result_data3 =
        result_data_tmp[1] << 8 | result_data_tmp[0];
    sentry_i2c_get(base, kRegResultData4L, result_data_tmp);
    sentry_i2c_get(base, kRegResultData4H, result_data_tmp + 1);
    vision_state->vision_result[i].result_data4 =
        result_data_tmp[1] << 8 | result_data_tmp[0];
    sentry_i2c_get(base, kRegResultData5L, result_data_tmp);
    sentry_i2c_get(base, kRegResultData5H, result_data_tmp + 1);
    vision_state->vision_result[i].result_data5 =
        result_data_tmp[1] << 8 | result_data_tmp[0];
  }
  return SENTRY_OK;
}

static sentry_err_t sentry_i2c_set_param(struct sentry_method_base_t * base, int vision_type, sentry_object_t *param, int param_id)
{
  sentry_err_t err;
  uint8_t result_data_tmp[2];

  err = sentry_i2c_set(base, kRegVisionId, vision_type);
  if (err)
    return err;
  err = sentry_i2c_set(base, kRegParamId, param_id);
  if (err)
    return err;
  sentry_i2c_set(base, kRegParamValue1H, (param->result_data1 >> 8) & 0xFF);
  sentry_i2c_set(base, kRegParamValue1L, param->result_data1 & 0xFF);
  sentry_i2c_set(base, kRegParamValue2H, (param->result_data2 >> 8) & 0xFF);
  sentry_i2c_set(base, kRegParamValue2L, param->result_data2 & 0xFF);
  sentry_i2c_set(base, kRegParamValue3H, (param->result_data3 >> 8) & 0xFF);
  sentry_i2c_set(base, kRegParamValue3L, param->result_data3 & 0xFF);
  sentry_i2c_set(base, kRegParamValue4H, (param->result_data4 >> 8) & 0xFF);
  sentry_i2c_set(base, kRegParamValue4L, param->result_data4 & 0xFF);
  sentry_i2c_set(base, kRegParamValue5H, (param->result_data5 >> 8) & 0xFF);
  sentry_i2c_set(base, kRegParamValue5L, param->result_data5 & 0xFF);

  return SENTRY_OK;
}

static sentry_err_t sentry_i2c_read_qrcode(struct sentry_method_base_t * base, int vision_type, sentry_qrcode_state_t *qrcode)
{
  sentry_err_t err;
  uint8_t result_data_tmp[2];

  err = sentry_i2c_set(base, kRegVisionId, vision_type);
  if (err)
    return err;
  err = sentry_i2c_get(base, kRegFrameCount, &qrcode->frame);
  if (err)
    return err;
  err = sentry_i2c_get(base, kRegResultNumber, &qrcode->detect);
  if (err)
    return err;
  if (!qrcode->detect)
    return SENTRY_OK;
  err = sentry_i2c_set(base, kRegResultId, 1);
  if (err)
    return err;
  sentry_i2c_get(base, kRegResultData1L, result_data_tmp);
  sentry_i2c_get(base, kRegResultData1H, result_data_tmp + 1);
  qrcode->qrcode_result[0].x_value =
      result_data_tmp[1] << 8 | result_data_tmp[0];
  sentry_i2c_get(base, kRegResultData2L, result_data_tmp);
  sentry_i2c_get(base, kRegResultData2H, result_data_tmp + 1);
  qrcode->qrcode_result[0].y_value =
      result_data_tmp[1] << 8 | result_data_tmp[0];
  sentry_i2c_get(base, kRegResultData3L, result_data_tmp);
  sentry_i2c_get(base, kRegResultData3H, result_data_tmp + 1);
  qrcode->qrcode_result[0].width = result_data_tmp[1] << 8 | result_data_tmp[0];
  sentry_i2c_get(base, kRegResultData4L, result_data_tmp);
  sentry_i2c_get(base, kRegResultData4H, result_data_tmp + 1);
  qrcode->qrcode_result[0].height =
      result_data_tmp[1] << 8 | result_data_tmp[0];
  sentry_i2c_get(base, kRegResultData5L, result_data_tmp);
  sentry_i2c_get(base, kRegResultData5H, result_data_tmp + 1);
  qrcode->qrcode_result[0].length =
      result_data_tmp[1] << 8 | result_data_tmp[0];
  for (uint16_t i = 0; i < qrcode->qrcode_result[0].length; ++i)
  {
    uint8_t result_id = i / 5 + 2;
    uint8_t offset = i % 5;
    if (0 == i % 5)
    {
      err = sentry_i2c_set(base, kRegResultId, result_id);
    }
    if (err)
      return err;
    sentry_i2c_get(base, kRegResultData1L + 2 * offset, (uint8_t *)(qrcode->qrcode_result[0].str + i));
  }
  qrcode->qrcode_result[0].str[qrcode->qrcode_result[0].length] = 0;

  return SENTRY_OK;
}

static sentry_err_t sentry_i2c_write(struct sentry_method_base_t * base, int vision_type, const sentry_vision_state_t *vision_state)
{
  sentry_err_t err = SENTRY_OK;

  if (vision_state->detect >= SENTRY_MAX_RESULT)
  {
    return SENTRY_FAIL;
  }

  err = sentry_i2c_set(base, kRegVisionId, vision_type);
  if (err)
    return err;
  err = sentry_i2c_set(base, kRegResultNumber, vision_state->detect);
  if (err)
    return err;
  for (uint8_t i = 0; i < vision_state->detect; ++i)
  {
    sentry_i2c_set(base, kRegResultId, i + 1);
    sentry_i2c_set(base, kRegResultData1L, vision_state->vision_result[i].result_data1 & 0xFF);
    sentry_i2c_set(base, kRegResultData1H, (vision_state->vision_result[i].result_data1 >> 8) & 0xFF);
    sentry_i2c_set(base, kRegResultData2L, vision_state->vision_result[i].result_data2 & 0xFF);
    sentry_i2c_set(base, kRegResultData2H, (vision_state->vision_result[i].result_data2 >> 8) & 0xFF);
    sentry_i2c_set(base, kRegResultData3L, vision_state->vision_result[i].result_data3 & 0xFF);
    sentry_i2c_set(base, kRegResultData3H, (vision_state->vision_result[i].result_data3 >> 8) & 0xFF);
    sentry_i2c_set(base, kRegResultData4L, vision_state->vision_result[i].result_data4 & 0xFF);
    sentry_i2c_set(base, kRegResultData4H, (vision_state->vision_result[i].result_data4 >> 8) & 0xFF);
    sentry_i2c_set(base, kRegResultData5L, vision_state->vision_result[i].result_data5 & 0xFF);
    sentry_i2c_set(base, kRegResultData5H, (vision_state->vision_result[i].result_data5 >> 8) & 0xFF);
  }

  return err;
}

void sentry_i2c_init(sentry_stream_method_t *method)
{
  method->Get = sentry_i2c_get;
  method->Set = sentry_i2c_set;
  method->SetParam = sentry_i2c_set_param;
  method->Read = sentry_i2c_read;
  method->ReadQrCode = sentry_i2c_read_qrcode;
  method->Write = sentry_i2c_write;
}