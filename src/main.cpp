#include <Arduino.h>
#include "rtd_surveyor.h"
#include <headers.h>


const int A1_temp_Pin             = 6;    // Temperature sensor 
Surveyor_RTD RTD = Surveyor_RTD(A1_temp_Pin);


uint8_t user_bytes_received = 0;
const uint8_t bufferlen = 32;
char user_data[bufferlen];

void parse_cmd(char* string);

const int LED12                   = 12;   // On board LED Digital output 12
const int LED13                   = 13;   // On board LED Digital output 13
const int LED14                   = 14;   // On board LED Digital output 14
const int LED15                   = 15;   // On board LED Digital output 15
const int LED16                   = 16;   // On board LED Digital output 16
const int LED17                   = 17;   // On board LED Digital output 17

void setup() {
  Serial.begin(115200);
  pinMode(LED12,OUTPUT);
  pinMode(LED13,OUTPUT);
  pinMode(LED14,OUTPUT);
  pinMode(LED15,OUTPUT);
  pinMode(LED16,OUTPUT);
  pinMode(LED17,OUTPUT);

  Serial.println(F("Use command \"CAL,nnn.n\" to calibrate the circuit to a specific temperature\n\"CAL,CLEAR\" clears the calibration"));
  if(RTD.begin()){
    Serial.println("Loaded EEPROM");
  }
}

void loop() {
  //Turn all LEDS on

  digitalWrite(LED12,HIGH);
  digitalWrite(LED13,HIGH);
  digitalWrite(LED14,HIGH);
  digitalWrite(LED15,HIGH);
  digitalWrite(LED16,HIGH);
  digitalWrite(LED17,HIGH);


  if (Serial.available() > 0) {
    user_bytes_received = Serial.readBytesUntil(13, user_data, sizeof(user_data));
  }

  if (user_bytes_received) {
    parse_cmd(user_data);
    user_bytes_received = 0;
    memset(user_data, 0, sizeof(user_data));
  }
  
  Serial.println(RTD.read_RTD_temp_C());
  
  //uncomment for readings in F
  // Serial.println(RTD.read_RTD_temp_F());
  // Serial.println("Voltage:"); 
  // Serial.print(analogRead(A1_temp_Pin));
  // float tempC = (((analogRead((A1_temp_Pin)))/1000)-1.058)/0.009;
  // Serial.print("Ryan's temp:");
  // Serial.print(tempC);
  // Serial.println("");
  delay(500);

}

void parse_cmd(char* string) {
  strupr(string);
  String cmd = String(string);
  if(cmd.startsWith("CAL")){
    int index = cmd.indexOf(',');
    if(index != -1){
      String param = cmd.substring(index+1, cmd.length());
      if(param.equals("CLEAR")){
        RTD.cal_clear();
        Serial.println("CALIBRATION CLEARED");
      }else {
        RTD.cal(param.toFloat());
        Serial.println("RTD CALIBRATED");
      }
    }
  }
}