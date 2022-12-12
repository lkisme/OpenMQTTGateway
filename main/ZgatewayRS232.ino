/*  
  OpenMQTTGateway  - ESP8266 or Arduino program for home automation 

   Act as a wifi or ethernet gateway between your RS232 device and a MQTT broker 
   Send and receiving command by MQTT
 
  This gateway enables to:
 - receive MQTT data from a topic and send RS232 signal corresponding to the received MQTT data
 - publish MQTT data to a different topic related to received RS232 signal

    Copyright: (c)Florian ROBERT
  
    This file is part of OpenMQTTGateway.
    
    OpenMQTTGateway is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenMQTTGateway is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "User_config.h"

#ifdef ZgatewayRS232

#  include <SoftwareSerial.h>
#include "su03t/jx_uart_send.h"

SoftwareSerial RS232Serial(RS232_RX_GPIO, RS232_TX_GPIO); // RX, TX

void setupRS232() {
  // define pin modes for RX, TX:
  pinMode(RS232_RX_GPIO, INPUT);
  pinMode(RS232_TX_GPIO, OUTPUT);
  // set the data rate for the SoftwareSerial port
  RS232Serial.begin(RS232Baud);

  Log.notice(F("RS232_RX_GPIO: %d " CR), RS232_RX_GPIO);
  Log.notice(F("RS232_TX_GPIO: %d " CR), RS232_TX_GPIO);
  Log.notice(F("RS232Baud: %d " CR), RS232Baud);
  Log.trace(F("ZgatewayRS232 setup done " CR));
}

void RS232toMQTT() {
  //This function is Blocking, but there should only ever be a few bytes, usually an ACK or a NACK.
  if (RS232Serial.available()) {
    Log.trace(F("RS232toMQTT" CR));
    static char RS232data[MAX_INPUT];
    static unsigned int input_pos = 0;
    static char inChar;
    do {
      if (RS232Serial.available()) {
        inChar = RS232Serial.read();
        RS232data[input_pos] = inChar;
        input_pos++;
        Log.trace(F("Received %c" CR), inChar);
      }
    } while (inChar != RS232InPost && input_pos < MAX_INPUT);
    RS232data[input_pos] = 0;
    input_pos = 0;
    Log.trace(F("Publish %s" CR), RS232data);
    char* output = RS232data + sizeof(RS232Pre) - 1;
    pub(subjectRS232toMQTT, output);
  }
}
void serialPrint(unsigned char* buff) {
  int len = 9;
  byte bytes[len];
  int i = 0;
  unsigned char c;
  Log.trace(F("MQTTtoRS232 json" CR));
  for (i = 0; i < len; i++) {
    bytes[i] = (byte)buff[i];
    Log.trace(F("%2x" CR),buff[i]);
  }
  RS232Serial.write(bytes, len);
}

bool compare(const char* src, const char* dest) {
  return 0 == strcmp(src, dest);
}

void MQTTtoRS232(char* topicOri, JsonObject& RS232data) {
  Log.trace(F("json" CR));
  if (cmpToMainTopic(topicOri, subjectMQTTtoRS232)) {
    if (RS232data.containsKey("cmd") && RS232data.containsKey("para")) {
      Serial.println("Json content legal !");
      const char* command = RS232data["cmd"];
      int parameter = RS232data["para"];
      Log.trace(F("command: %s" CR), command);
      Log.trace(F("parameter: %s" CR), parameter);
      unsigned char buff[9] = {0};
      if (compare(command, "alert")) {
        _uart_alert(parameter,buff);//由于第一次设置变量，不会触发播放语音
        serialPrint(buff);
        _uart_read_cmd(0, buff);//所以进行第二次设置变量，且设置为不播放语音的变量
        delay(50);
      }else if (compare(command, "status")) {
        _uart_report_status(parameter,buff);
        serialPrint(buff);
        _uart_read_cmd(-1, buff);
        delay(50);
      }else if (compare(command, "cmd")) {
        _uart_read_cmd(parameter, buff);
        serialPrint(buff);
        _uart_read_cmd(0, buff);
        delay(50);
      }
      serialPrint(buff);
    }
  }
}
#endif