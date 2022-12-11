/*  
  OpenMQTTGateway Addon  - ESP8266 or Arduino program for home automation 

   Act as a wifi or ethernet gateway between your 433mhz/infrared IR signal and a MQTT broker 
   Send and receiving command by MQTT
 
    Output GPIO defined to High or Low
  
    Copyright: (c)Florian ROBERT
    
    Contributors:
    - 1technophile
  
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

#ifdef ZactuatorONOFF

void setupONOFF() {
  pinMode(ACTUATOR_ONOFF_GPIO, OUTPUT);
#  ifdef ACTUATOR_ONOFF_DEFAULT
  digitalWrite(ACTUATOR_ONOFF_GPIO, ACTUATOR_ONOFF_DEFAULT);
#  endif
}

#  if jsonReceiving
void MQTTtoONOFF(char* topicOri, JsonObject& ONOFFdata) {
  if (cmpToMainTopic(topicOri, subjectMQTTtoONOFF)) {
    Log.trace(F("MQTTtoONOFF json data analysis" CR));
    int boolSWITCHTYPE = ONOFFdata["cmd"] | 99;
    int gpio = ONOFFdata["gpio"] | ACTUATOR_ONOFF_GPIO;
    if (boolSWITCHTYPE != 99) {
      Log.notice(F("MQTTtoONOFF boolSWITCHTYPE ok: %d" CR), boolSWITCHTYPE);
      Log.notice(F("GPIO number: %d" CR), gpio);
      pinMode(gpio, OUTPUT);
      digitalWrite(gpio, boolSWITCHTYPE);
      // we acknowledge the sending by publishing the value to an acknowledgement topic
      pub(subjectGTWONOFFtoMQTT, ONOFFdata);
    } else {
      if (ONOFFdata["cmd"] == "high_pulse") {
        Log.notice(F("MQTTtoONOFF high_pulse ok" CR));
        Log.notice(F("GPIO number: %d" CR), gpio);
        int pulselength = ONOFFdata["pulse_length"] | 500;
        Log.notice(F("Pulse length: %d ms" CR), pulselength);
        pinMode(gpio, OUTPUT);
        digitalWrite(gpio, HIGH);
        delay(pulselength);
        digitalWrite(gpio, LOW);
      } else if (ONOFFdata["cmd"] == "low_pulse") {
        Log.notice(F("MQTTtoONOFF low_pulse ok" CR));
        Log.notice(F("GPIO number: %d" CR), gpio);
        int pulselength = ONOFFdata["pulse_length"] | 500;
        Log.notice(F("Pulse length: %d ms" CR), pulselength);
        pinMode(gpio, OUTPUT);
        digitalWrite(gpio, LOW);
        delay(pulselength);
        digitalWrite(gpio, HIGH);
      } else {
        Log.error(F("MQTTtoONOFF failed json read" CR));
      }
    }
  }
}
#  endif

#  if simpleReceiving
void MQTTtoONOFF(char* topicOri, char* datacallback) {
  if ((cmpToMainTopic(topicOri, subjectMQTTtoONOFF))) {
    Log.trace(F("MQTTtoONOFF" CR));
    char* endptr = NULL;
    long track = strtol(datacallback, &endptr, 10);
    if (datacallback == endptr)
      return;

    Log.notice(F("GPIO number: %d" CR), track);
    speak((char)track);
  }
}
#  endif

void ActuatorButtonTrigger() {
  uint8_t level = !digitalRead(ACTUATOR_ONOFF_GPIO);
  char* level_string = "ON";
  if (level != ACTUATOR_ON) {
    level_string = "OFF";
  }
  Log.trace(F("Actuator triggered %s by button" CR), level_string);
  digitalWrite(ACTUATOR_ONOFF_GPIO, level);
  // Send the state of the switch to the broker so as to update the status
  StaticJsonDocument<JSON_MSG_BUFFER> jsonBuffer;
  JsonObject ONOFFdata = jsonBuffer.to<JsonObject>();
  ONOFFdata["cmd"] = (int)level;
  pub(subjectGTWONOFFtoMQTT, ONOFFdata);
}

void speak(char SB_DATA)
{
  pinMode(2, OUTPUT);  
  char S_DATA;
  char B_DATA;
  S_DATA =  SB_DATA;
  digitalWrite(2, LOW);
  delay(5);
  B_DATA = S_DATA & 0x01;
  for(int j=0;j<8;j++)
  {
     if((int)B_DATA == 1)
     {
         digitalWrite(2, HIGH);
         delayMicroseconds(600);
         digitalWrite(2, LOW);
         delayMicroseconds(200);
    } else {
      digitalWrite(2, HIGH);
      delayMicroseconds(200); //延时200us 
      digitalWrite(2, LOW);
      delayMicroseconds(600);
    }
    S_DATA = S_DATA >> 1;
    B_DATA = S_DATA & 0x01;
  }
      delayMicroseconds(200);
  digitalWrite(2, HIGH);
}

#endif
