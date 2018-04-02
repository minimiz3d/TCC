#include <EEPROM.h>
 
#define SAMPLE_TIME 1000  //The time between each EEPROM write function call in ms
 
int LDR = A0;      //the ADC pin
int address = 0;      //EEPROM address counter
int offset = 30;
int baseAdress = 30;
int maxAddress = baseAdress + offset;

unsigned long timer;
 
float conv_coeff = 0.0;   //coefficient for converting from 0-1024 to 0-5 range

void writeData();

int getCurrentStatus() {
  int status = 0;
  for (int i = 0; i < 10; i++) 
    status += analogRead(A0);  
 
  return status/10;
}

void setup() {
  Serial.begin(9600);     //start the serial connection as always
  timer = millis();         //millis() returns the time since program start in ms
  delay(10000);
}
 
void loop() {
  if(millis()-timer > SAMPLE_TIME)  //check if it's time to do a temp sensor sample
  {
    writeData();
    timer = millis();
  }
   
  delay(1);
}

void stop() {
  while(1);
}

void writeData() {
//  int value = analogRead(LDR)/4;     //read sensor value
  int value = getCurrentStatus()/4;
   
  EEPROM.write(baseAdress, value);         //write value to current address counter address

  Serial.print("LDR: "); Serial.println(value);
  
  Serial.print("Sensor value stored at address ");
  Serial.println(baseAdress);

  if (baseAdress == maxAddress){
    Serial.print("Parando");
    stop();
  }
   
  baseAdress++;                      //increment address counter
  if(address == EEPROM.length())  //check if address counter has reached the end of EEPROM
  {
    address = 0;              //if yes: reset address counter
  }
}
