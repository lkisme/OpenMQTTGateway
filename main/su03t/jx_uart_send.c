#include "jx_uart_send.h"

// 串口通信消息头
const unsigned char g_uart_send_head[] = {
  0xaa, 0x55
};

// 串口通信消息尾
const unsigned char g_uart_send_foot[] = {
  0x55, 0xaa
};

// 串口发送函数实现
void _uart_send_impl(unsigned char* buff, int len) {
  // TODO: 调用项目实际的串口发送函数
  //使用真实串口
  /*
  int i = 0;
  unsigned char c;
  for (i = 0; i < len; i++) {
    c = buff[i];
    Serial.printf("%02X ", c);
  }
  Serial.printf("\n");
  */
}
// 十六位整数转32位整数
void _int16_to_int32(uart_param_t* param) {
  if (sizeof(int) >= 4)
    return;
  unsigned long value = param->d_long;
  unsigned long sign = (value >> 15) & 1;
  unsigned long v = value;
  if (sign)
    v = 0xFFFF0000 | value;
  uart_param_t p;
  p.d_long = v;
  param->d_ucs[0] = p.d_ucs[0];
  param->d_ucs[1] = p.d_ucs[1];
  param->d_ucs[2] = p.d_ucs[2];
  param->d_ucs[3] = p.d_ucs[3];
}

// action: report_weather
void _uart_report_weather(int p_temp_up, int p_temp_down, char p_wind, int p_wind_grade, int p_rain, int p_snow, int p_month, int p_date, char p_weather) {
  uart_param_t param;
  int i = 0;
  unsigned char buff[UART_SEND_MAX] = {0};
  for (i = 0; i < UART_MSG_HEAD_LEN; i++) {
      buff[i + 0] = g_uart_send_head[i];
  }
  buff[2] = U_MSG_report_weather;
  param.d_int = p_temp_up;
  _int16_to_int32(&param);
  buff[3] = param.d_ucs[0];
  buff[4] = param.d_ucs[1];
  buff[5] = param.d_ucs[2];
  buff[6] = param.d_ucs[3];
  param.d_int = p_temp_down;
  _int16_to_int32(&param);
  buff[7] = param.d_ucs[0];
  buff[8] = param.d_ucs[1];
  buff[9] = param.d_ucs[2];
  buff[10] = param.d_ucs[3];
  param.d_char = p_wind;
  buff[11] = param.d_uchar;
  param.d_int = p_wind_grade;
  _int16_to_int32(&param);
  buff[12] = param.d_ucs[0];
  buff[13] = param.d_ucs[1];
  buff[14] = param.d_ucs[2];
  buff[15] = param.d_ucs[3];
  param.d_int = p_rain;
  _int16_to_int32(&param);
  buff[16] = param.d_ucs[0];
  buff[17] = param.d_ucs[1];
  buff[18] = param.d_ucs[2];
  buff[19] = param.d_ucs[3];
  param.d_int = p_snow;
  _int16_to_int32(&param);
  buff[20] = param.d_ucs[0];
  buff[21] = param.d_ucs[1];
  buff[22] = param.d_ucs[2];
  buff[23] = param.d_ucs[3];
  param.d_int = p_month;
  _int16_to_int32(&param);
  buff[24] = param.d_ucs[0];
  buff[25] = param.d_ucs[1];
  buff[26] = param.d_ucs[2];
  buff[27] = param.d_ucs[3];
  param.d_int = p_date;
  _int16_to_int32(&param);
  buff[28] = param.d_ucs[0];
  buff[29] = param.d_ucs[1];
  buff[30] = param.d_ucs[2];
  buff[31] = param.d_ucs[3];
  param.d_char = p_weather;
  buff[32] = param.d_uchar;
  for (i = 0; i < UART_MSG_FOOT_LEN; i++) {
      buff[i + 33] = g_uart_send_foot[i];
  }
  _uart_send_impl(buff, 35);
}

// action: report_status
void _uart_report_status(int status_para, unsigned char * buff) {
  uart_param_t param;
  int i = 0;
//  unsigned char buff[UART_SEND_MAX] = {0};
  for (i = 0; i < UART_MSG_HEAD_LEN; i++) {
      buff[i + 0] = g_uart_send_head[i];
  }
  buff[2] = U_MSG_report_status;
  param.d_int = status_para;
  _int16_to_int32(&param);
  buff[3] = param.d_ucs[0];
  buff[4] = param.d_ucs[1];
  buff[5] = param.d_ucs[2];
  buff[6] = param.d_ucs[3];
  for (i = 0; i < UART_MSG_FOOT_LEN; i++) {
      buff[i + 7] = g_uart_send_foot[i];
  }
//  _uart_send_impl(buff, 9);
}

// action: alert
void _uart_alert(int p_type, unsigned char * buff) {
  uart_param_t param;
  int i = 0;
//  unsigned char buff[UART_SEND_MAX] = {0};
  for (i = 0; i < UART_MSG_HEAD_LEN; i++) {
      buff[i + 0] = g_uart_send_head[i];
  }
  buff[2] = U_MSG_alert;
  param.d_int = p_type;
  _int16_to_int32(&param);
  buff[3] = param.d_ucs[0];
  buff[4] = param.d_ucs[1];
  buff[5] = param.d_ucs[2];
  buff[6] = param.d_ucs[3];
  for (i = 0; i < UART_MSG_FOOT_LEN; i++) {
      buff[i + 7] = g_uart_send_foot[i];
  }
//  callback(buff, 9);
}

// action: read_cmd
void _uart_read_cmd_content(int type_p, unsigned char * buff) {
  uart_param_t param;
  int i = 0;
//  unsigned char buff[UART_SEND_MAX] = {0};
  for (i = 0; i < UART_MSG_HEAD_LEN; i++) {
      buff[i + 0] = g_uart_send_head[i];
  }
  buff[2] = U_MSG_read_cmd;
  param.d_int = type_p;
  _int16_to_int32(&param);
  buff[3] = param.d_ucs[0];
  buff[4] = param.d_ucs[1];
  buff[5] = param.d_ucs[2];
  buff[6] = param.d_ucs[3];
  for (i = 0; i < UART_MSG_FOOT_LEN; i++) {
      buff[i + 7] = g_uart_send_foot[i];
  }
//  return buff;
}
void _uart_read_cmd(int type_p, unsigned char * buff) {
  uart_param_t param;
  int i = 0;
//  unsigned char buff[UART_SEND_MAX] = {0};
  for (i = 0; i < UART_MSG_HEAD_LEN; i++) {
      buff[i + 0] = g_uart_send_head[i];
  }
  buff[2] = U_MSG_read_cmd;
  param.d_int = type_p;
  _int16_to_int32(&param);
  buff[3] = param.d_ucs[0];
  buff[4] = param.d_ucs[1];
  buff[5] = param.d_ucs[2];
  buff[6] = param.d_ucs[3];
  for (i = 0; i < UART_MSG_FOOT_LEN; i++) {
      buff[i + 7] = g_uart_send_foot[i];
  }
//  callback(buff, 7 +UART_MSG_FOOT_LEN);
}

// action: test_in
void _uart_test_in(int ppp, unsigned char * buff) {
  uart_param_t param;
  int i = 0;
//  unsigned char buff[UART_SEND_MAX] = {0};
  for (i = 0; i < UART_MSG_HEAD_LEN; i++) {
      buff[i + 0] = g_uart_send_head[i];
  }
  buff[2] = U_MSG_test_in;
  param.d_int = ppp;
  _int16_to_int32(&param);
  buff[3] = param.d_ucs[0];
  buff[4] = param.d_ucs[1];
  buff[5] = param.d_ucs[2];
  buff[6] = param.d_ucs[3];
  for (i = 0; i < UART_MSG_FOOT_LEN; i++) {
      buff[i + 7] = g_uart_send_foot[i];
  }
//  callback(buff, 9);
}
