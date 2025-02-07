#include "sentry_stream.h"

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


#define SENTRY_PROTOC_OK 0xE0
#define SENTRY_PROTOC_FAIL 0xE1
#define SENTRY_PROTOC_UNKNOWN 0xE2
#define SENTRY_PROTOC_TIMEOUT 0xE3
#define SENTRY_PROTOC_CHECK_ERROR 0xE4
#define SENTRY_PROTOC_LENGTH_ERROR 0xE5
#define SENTRY_PROTOC_UNSUPPORT_COMMAND 0xE6
#define SENTRY_PROTOC_UNSUPPORT_REG_ADDRESS 0xE7
#define SENTRY_PROTOC_UNSUPPORT_REG_VALUE 0xE8
#define SENTRY_PROTOC_READ_ONLY 0xE9
#define SENTRY_PROTOC_RESTART_ERROR 0xEA
#define SENTRY_PROTOC_RESULT_NOT_END 0xEC

// Protocol
#define SENTRY_PROTOC_START 0xFF
#define SENTRY_PROTOC_END 0xED
#define SENTRY_PROTOC_COMMADN_SET 0x01
#define SENTRY_PROTOC_COMMADN_GET 0x02
#define SENTRY_PROTOC_SET_PARAM 0x21
#define SENTRY_PROTOC_GET_RESULT 0x23
#define SENTRY_PROTOC_SET_RESULT 0x25
#define SENTRY_PROTOC_MESSAGE 0x11

#define PROTOCOL_SINGLE_BUFFER_SIZE 255

typedef struct _pkg_t
{
  int len;
  uint8_t buf[PROTOCOL_SINGLE_BUFFER_SIZE];
} pkg_t;

static int readpkg(struct sentry_method_base_t *base, pkg_t *pkg, int timeout)
{
  int start_receive = 0;
  int timeout_t = timeout;
  int index = 0, read_len = 1;

  memset(pkg, 0, sizeof(pkg_t));

  while (1)
  {
#ifdef SENTRY_MICRO_BIT
    timeout_t -= 60;
#else
    timeout_t -= 900;
#endif
    if (timeout_t < 0)
    {
      break;
    }

    if (!start_receive)
    {
      index = 0;
      if (!base->serial_read(base, &pkg->buf[index], 1))
      {
        continue;
      }
      else
      {
        timeout_t = timeout;
        index += 1;
      }
    }

    switch (pkg->buf[index - 1])
    {
    case SENTRY_PROTOC_START:
      start_receive = 1;
      read_len = 1;
      break;
    case SENTRY_PROTOC_END:
      if (start_receive && (pkg->buf[1] == index))
      {
#if SENTRY_DEBUG_ENABLE && LOG_OUTPUT
        DOPRINTF("pkg_r[%d]:", index);
        for (int i = 0; i < index; ++i)
        {
          DOPRINTF("%x ", pkg->buf[i]);
        }
        DOPRINTF("\n");
#endif
        int checksum = 0;

        for (int i = 0; i < index - 2; ++i)
        {
          checksum += pkg->buf[i];
        }

        checksum &= 0xff;

        if (pkg->buf[index - 2] != checksum)
        {
          pkg->buf[0] = SENTRY_PROTOC_CHECK_ERROR;
#if SENTRY_DEBUG_ENABLE && LOG_OUTPUT
          DOPRINTF("Check error!\n");
#endif
        }
        else
        {
          pkg->len = pkg->buf[1] - 5;
          memmove(pkg->buf, pkg->buf + 3, pkg->len);
        }
        return 1;
      }
      break;
    default:
      if (index == 2)
      {
        read_len = pkg->buf[1] - 2;
      }
      break;
    }

    if (start_receive)
    {
      if (base->serial_read(base, &pkg->buf[index], read_len))
      {
        timeout_t = timeout;
        index += read_len;
      }
    }
  }

  return 0;
}

static int writepkg(struct sentry_method_base_t *base, pkg_t *pkg)
{
  if (pkg->len <= 0 || pkg->len >= PROTOCOL_SINGLE_BUFFER_SIZE - 4)
  {
    return 0;
  }

  int checksum = SENTRY_PROTOC_START + 4 + pkg->len;

  for (int i = pkg->len - 1; i >= 0; i--)
  {
    checksum += pkg->buf[i];
    pkg->buf[i + 2] = pkg->buf[i];
  }

  pkg->buf[0] = SENTRY_PROTOC_START;
  pkg->buf[1] = 4 + pkg->len;
  pkg->buf[pkg->len + 2] = checksum & 0xff;
  pkg->buf[pkg->len + 3] = SENTRY_PROTOC_END;
  pkg->len += 4;

  base->serial_write(base, pkg->buf, pkg->len);

  return 1;
}

// 实现函数
static sentry_err_t sentry_uart_get(struct sentry_method_base_t *base, const uint8_t reg_address, uint8_t *value)
{
  uint8_t try_time = 0;
  for (;;)
  {
    pkg_t pkg = {3, {base->address, SENTRY_PROTOC_COMMADN_GET, reg_address}};

    if (!writepkg(base, &pkg))
    {
      return SENTRY_FAIL;
    }

    if (!readpkg(base, &pkg, 1000))
    {
      return SENTRY_READ_TIMEOUT;
    }
    if (pkg.len > 0)
    {
      if (pkg.buf[0] == SENTRY_PROTOC_OK && pkg.buf[1] == SENTRY_PROTOC_COMMADN_GET)
      {
        *value = pkg.buf[2];
        return SENTRY_OK;
      }
      else
      {
        return pkg.buf[0];
      }
    }
    try_time++;
    if (try_time > 3)
    {
      return SENTRY_READ_TIMEOUT;
    }
  }
  return SENTRY_OK;
}

static sentry_err_t sentry_uart_set(struct sentry_method_base_t *base, const uint8_t reg_address, const uint8_t value)
{
  uint8_t try_time = 0;
  for (;;)
  {
    pkg_t pkg = {4, {base->address, SENTRY_PROTOC_COMMADN_SET, reg_address, value}};

    if (!writepkg(base, &pkg))
    {
      return SENTRY_FAIL;
    }

    try_time++;
    if (try_time > 3)
    {
      return SENTRY_READ_TIMEOUT;
    }
    if (!readpkg(base, &pkg, 1000))
    {
      return SENTRY_READ_TIMEOUT;
    }
    if (pkg.len > 0)
    {
      if (pkg.buf[1] != SENTRY_PROTOC_COMMADN_SET || pkg.buf[2] != reg_address)
      {
        continue;
      }
      if (SENTRY_PROTOC_OK == pkg.buf[0])
      {
        return SENTRY_OK;
      }
      return pkg.buf[0];
    }
  }
  return SENTRY_OK;
}

static sentry_err_t sentry_uart_set_param(struct sentry_method_base_t *base, int vision_type, sentry_object_t *param, int param_id)
{
  sentry_err_t err = SENTRY_OK;
  int try_time = 0;

  pkg_t pkg;
  for (;;)
  {
    pkg.len = 15;
    pkg.buf[0] = base->address;
    pkg.buf[1] = SENTRY_PROTOC_SET_PARAM;
    pkg.buf[2] = vision_type;
    pkg.buf[3] = (uint8_t)param_id;
    pkg.buf[4] = (uint8_t)param_id;
    pkg.buf[5] = (param->result_data1 >> 8) & 0xFF;
    pkg.buf[6] = param->result_data1 & 0xFF;
    pkg.buf[7] = (param->result_data2 >> 8) & 0xFF;
    pkg.buf[8] = param->result_data2 & 0xFF;
    pkg.buf[9] = (param->result_data3 >> 8) & 0xFF;
    pkg.buf[10] = param->result_data3 & 0xFF;
    pkg.buf[11] = (param->result_data4 >> 8) & 0xFF;
    pkg.buf[12] = param->result_data4 & 0xFF;
    pkg.buf[13] = (param->result_data5 >> 8) & 0xFF;
    pkg.buf[14] = param->result_data5 & 0xFF;

    if (!writepkg(base, &pkg))
    {
      return SENTRY_FAIL;
    }

    if (!readpkg(base, &pkg, 1000))
    {
      return SENTRY_READ_TIMEOUT;
    }
    if (pkg.len > 0)
    {
      if (pkg.buf[0] == SENTRY_PROTOC_OK)
      {
        if (pkg.buf[1] == SENTRY_PROTOC_SET_PARAM)
        {
          return SENTRY_OK;
        }
        else
        {
          return SENTRY_UNSUPPORT_PARAM;
        }
      }
      return pkg.buf[0];
    }
    try_time++;
    if (try_time > 3)
    {
      return SENTRY_READ_TIMEOUT;
    }
  }

  return err;
}

static sentry_err_t sentry_uart_read(struct sentry_method_base_t *base, int vision_type, sentry_vision_state_t *vision_state)
{
  int try_time = 0;
  sentry_err_t err = SENTRY_OK;
  for (;;)
  {
    pkg_t pkg = {5, {base->address, SENTRY_PROTOC_GET_RESULT, (uint8_t)vision_type, 1, SENTRY_MAX_RESULT}};
    vision_state->detect = 0;
    if (!writepkg(base, &pkg))
    {
      return SENTRY_FAIL;
    }

    if (!readpkg(base, &pkg, 1000))
    {
      return SENTRY_READ_TIMEOUT;
    }
    if (pkg.len > 0)
    {
      if (pkg.buf[0] == SENTRY_PROTOC_OK || pkg.buf[0] == SENTRY_PROTOC_RESULT_NOT_END || pkg.buf[3] == vision_type)
      {
        if (pkg.buf[1] == SENTRY_PROTOC_GET_RESULT)
        {
          vision_state->frame = pkg.buf[2];
          uint8_t start_id = pkg.buf[4];
          uint8_t stop_id = pkg.buf[5];
          uint8_t *presult = &pkg.buf[6];
          if (stop_id == 0)
            return SENTRY_OK;
          for (uint8_t i = start_id - 1, j = 0; i < stop_id; i++, j++)
          {
            vision_state->vision_result[i].x_value = presult[10 * j + 0] << 8 | presult[10 * j + 1];
            vision_state->vision_result[i].y_value = presult[10 * j + 2] << 8 | presult[10 * j + 3];
            vision_state->vision_result[i].width = presult[10 * j + 4] << 8 | presult[10 * j + 5];
            vision_state->vision_result[i].height = presult[10 * j + 6] << 8 | presult[10 * j + 7];
            vision_state->vision_result[i].label = presult[10 * j + 8] << 8 | presult[10 * j + 9];
            vision_state->detect++;
          }
          if (pkg.buf[0] == SENTRY_PROTOC_OK)
          {
            return SENTRY_OK;
          }
          else
          {
            continue;
          }
        }
        else
        {
          EPRINTF("Error command: 0x%x\n", pkg.buf[1]);
          return SENTRY_UNSUPPORT_PARAM;
        }
      }
      else
      {
        return SENTRY_UNSUPPORT_PARAM;
      }
    }
    try_time++;
    if (try_time > 3)
    {
      return SENTRY_READ_TIMEOUT;
    }
  }
  return SENTRY_OK;
}

static sentry_err_t sentry_uart_read_qrcode(struct sentry_method_base_t *base, int vision_type, sentry_qrcode_state_t *qrcode)
{
  int try_time = 0;
  sentry_err_t err = SENTRY_OK;
  for (;;)
  {
    pkg_t pkg = {5, {base->address, SENTRY_PROTOC_GET_RESULT, (uint8_t)vision_type, 0, 0}};
    qrcode->detect = 0;
    if (!writepkg(base, &pkg))
    {
      return SENTRY_FAIL;
    }

    if (!readpkg(base, &pkg, 1000))
    {
      return SENTRY_READ_TIMEOUT;
    }
    if (pkg.len > 0)
    {
      if (pkg.buf[0] == SENTRY_PROTOC_OK || pkg.buf[3] == vision_type)
      {
        if (pkg.buf[1] == SENTRY_PROTOC_GET_RESULT)
        {
          qrcode->frame = pkg.buf[2];
          qrcode->detect = 0;
          if (pkg.buf[5] == 0)
            return SENTRY_OK;
          qrcode->detect = (pkg.buf[5] - pkg.buf[4] + 1) > 0;
          if (!qrcode->detect)
            return SENTRY_OK;
          uint8_t *presult = &pkg.buf[6];
          qrcode->qrcode_result[0].x_value = presult[0] << 8 | presult[1];
          qrcode->qrcode_result[0].y_value = presult[2] << 8 | presult[3];
          qrcode->qrcode_result[0].width = presult[4] << 8 | presult[5];
          qrcode->qrcode_result[0].height = presult[6] << 8 | presult[7];
          qrcode->qrcode_result[0].length = presult[8] << 8 | presult[9];
          for (uint16_t i = 0; i < qrcode->qrcode_result[0].length; ++i)
          {
            qrcode->qrcode_result[0].str[i] = presult[11 + 2 * i];
          }
          qrcode->qrcode_result[0].str[qrcode->qrcode_result[0].length] = 0;
          return SENTRY_OK;
        }
        else
        {
          EPRINTF("Error command: 0x%x\n", pkg.buf[1]);
          return SENTRY_UNSUPPORT_PARAM;
        }
      }
      else
      {
        return SENTRY_UNSUPPORT_PARAM;
      }
    }
    try_time++;
    if (try_time > 3)
    {
      return SENTRY_READ_TIMEOUT;
    }
  }
  return err;
}

static sentry_err_t sentry_uart_write(struct sentry_method_base_t *base, int vision_type, const sentry_vision_state_t *vision_state)
{
  sentry_err_t err = SENTRY_OK;
  pkg_t pkg;
  for (;;)
  {
    for (size_t i = 0; i < vision_state->detect; i++)
    {
      pkg.buf[0] = base->address;
      pkg.buf[1] = SENTRY_PROTOC_SET_RESULT;
      pkg.buf[2] = vision_type;
      pkg.buf[3] = i + 1;
      pkg.buf[4] = i + 1;
      pkg.buf[5] = (vision_state->vision_result[i].result_data1 >> 8) & 0xFF;
      pkg.buf[6] = vision_state->vision_result[i].result_data1 & 0xFF;
      pkg.buf[7] = (vision_state->vision_result[i].result_data2 >> 8) & 0xFF;
      pkg.buf[8] = vision_state->vision_result[i].result_data2 & 0xFF;
      pkg.buf[9] = (vision_state->vision_result[i].result_data3 >> 8) & 0xFF;
      pkg.buf[10] = vision_state->vision_result[i].result_data3 & 0xFF;
      pkg.buf[11] = (vision_state->vision_result[i].result_data4 >> 8) & 0xFF;
      pkg.buf[12] = vision_state->vision_result[i].result_data4 & 0xFF;
      pkg.buf[13] = (vision_state->vision_result[i].result_data5 >> 8) & 0xFF;
      pkg.buf[14] = vision_state->vision_result[i].result_data5 & 0xFF;
      pkg.len = 15;
      if (!writepkg(base, &pkg))
      {
        return SENTRY_FAIL;
      }

      if (!readpkg(base, &pkg, 1000))
      {
        return SENTRY_READ_TIMEOUT;
      }
      if (pkg.len > 0)
      {
        if (6 == pkg.len && SENTRY_PROTOC_OK == pkg.buf[3] && SENTRY_PROTOC_SET_RESULT == pkg.buf[4] && vision_type == pkg.buf[5])
        {
          return err;
        }
        else
        {
          return SENTRY_FAIL;
        }
      }
    }
  }

  return err;
}

// 初始化函数
void sentry_uart_init(sentry_stream_method_t *method)
{
  method->Get = &sentry_uart_get;
  method->Set = &sentry_uart_set;
  method->SetParam = &sentry_uart_set_param;
  method->Read = &sentry_uart_read;
  method->ReadQrCode = &sentry_uart_read_qrcode;
  method->Write = &sentry_uart_write;
}




