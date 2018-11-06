  /*
  Example for different sending methods
  
  https://github.com/sui77/rc-switch/
  
*/

#include <RCSwitch.h>
#include "LowPower.h"

#define LDR_PIN 5
#define NUM_SAMPLES 5000
#define SLEEP_TIME 8

int counter = 0;
RCSwitch mySwitch = RCSwitch();

void sleepSeconds(int seconds) {
  for (int i = 0; i < seconds; i++) { 
     LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF); 
  }
}

void setup() {
  pinMode(9, OUTPUT);
  Serial.begin(9600);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);

  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  
  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(15);
  
}

float getCurrentStatus() {
  float status = 0.0;
  for (int i = 0; i < NUM_SAMPLES; i++) 
    status += analogRead(LDR_PIN);

  return status / NUM_SAMPLES;
}

void loop() {
  // Led on
  digitalWrite(9, HIGH);

  // Transmits data
  mySwitch.send(counter, 24);
  
  // Enter power down state for 8 s with ADC and BOD module disabled
  // LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  sleepSeconds(1);

  // Next packet
  counter++;
  
  // Led off
  digitalWrite(9, LOW);

  sleepSeconds(5);
}
