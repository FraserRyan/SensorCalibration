#include <Arduino.h>
#include "rtd_surveyor.h"
#include <headers.h>

#ifdef USE_PULSE_OUT
  #include "ph_iso_surveyor.h"       
  Surveyor_pH_Isolated pH = Surveyor_pH_Isolated(A0);         
#else
  #include "ph_surveyor.h"             
  Surveyor_pH pH = Surveyor_pH(pH_Pin);   
#endif

#ifdef TEMP_CAL
  Surveyor_RTD RTD = Surveyor_RTD(A1_temp_Pin);
  uint8_t user_bytes_received = 0;
  const uint8_t bufferlen = 32;
  char user_data[bufferlen];
  void parse_cmd(char* string);
#endif

#ifdef PH_CAL
  uint8_t user_bytes_received = 0;                
  const uint8_t bufferlen = 32;                   
  char user_data[bufferlen];    
#endif

void setup() {
  Serial.begin(115200);

#ifdef PH_CAL
  Serial.println(F("Use commands \"CAL,7\", \"CAL,4\", and \"CAL,10\" to calibrate the circuit to those respective values"));
  Serial.println(F("Use command \"CAL,CLEAR\" to clear the calibration"));
  if (pH.begin()) {                                     
    Serial.println("Loaded EEPROM");
  } 
#endif

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
    //uncomment for readings in F
    // Serial.println(RTD.read_RTD_temp_F());
    delay(500);
  #endif

  #ifdef PH_CAL
    if (Serial.available() > 0) {                                                      
      user_bytes_received = Serial.readBytesUntil(13, user_data, sizeof(user_data));   
    }

    if (user_bytes_received) {                                                      
      parse_cmd(user_data);                                                          
      user_bytes_received = 0;                                                        
      memset(user_data, 0, sizeof(user_data));                                         
    }
    
    Serial.println(pH.read_ph());                                                      
    delay(1000);
  #endif


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

#ifdef PH_CAL
  void parse_cmd(char* string) {                   
    strupr(string);                                
    if (strcmp(string, "CAL,7") == 0) {       
      pH.cal_mid();                                
      Serial.println("MID CALIBRATED");
    }
    else if (strcmp(string, "CAL,4") == 0) {            
      pH.cal_low();                                
      Serial.println("LOW CALIBRATED");
    }
    else if (strcmp(string, "CAL,10") == 0) {      
      pH.cal_high();                               
      Serial.println("HIGH CALIBRATED");
    }
    else if (strcmp(string, "CAL,CLEAR") == 0) { 
      pH.cal_clear();                              
      Serial.println("CALIBRATION CLEARED");
    }
  }
#endif