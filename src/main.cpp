#include <Arduino.h>
#include "rtd_surveyor.h"
#include <headers.h>

#ifdef USE_PULSE_OUT
  #include "ph_iso_surveyor.h"       
  Surveyor_pH_Isolated pH = Surveyor_pH_Isolated(A0);         
#else
  #include "ph_surveyor.h"             
  Surveyor_pH pH = Surveyor_pH(A0);   
#endif

#ifdef TEMP_CAL
  Surveyor_RTD RTD = Surveyor_RTD(A1_temp_Pin);
  uint8_t user_bytes_received = 0;
  const uint8_t bufferlen = 32;
  char user_data[bufferlen];
  void parse_cmd(char* string);
#endif

#ifdef PH_CAL
  
#endif

void setup() {
  Serial.begin(115200);
  #ifdef TEMP_CAL
    Serial.println(F("Use command \"CAL,nnn.n\" to calibrate the circuit to a specific temperature\n\"CAL,CLEAR\" clears the calibration"));
    if(RTD.begin()){
      Serial.println("Loaded EEPROM");
    }
  #endif
  #ifdef PH_CAL
  
  #endif
}

void loop() {


  #ifdef TEMP_CAL
    if (Serial.available() > 0) {
      user_bytes_received = Serial.readBytesUntil(13, user_data, sizeof(user_data));
    }

    if (user_bytes_received) {
      parse_cmd(user_data);
      user_bytes_received = 0;
      memset(user_data, 0, sizeof(user_data));
    }
    Serial.println(RTD.read_RTD_temp_C());
    delay(500);
  #endif

  //uncomment for readings in F
  // Serial.println(RTD.read_RTD_temp_F());
  // Serial.println("Voltage:"); 
  // Serial.print(analogRead(A1_temp_Pin));
  // float tempC = (((analogRead((A1_temp_Pin)))/1000)-1.058)/0.009;
  // Serial.print("Ryan's temp:");
  // Serial.print(tempC);
  // Serial.println("");
  

}

#ifdef TEMP_CAL
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
#endif