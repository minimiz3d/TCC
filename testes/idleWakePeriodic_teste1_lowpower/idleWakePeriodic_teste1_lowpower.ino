// **** INCLUDES *****
#include "LowPower.h"

void setup()
{
  pinMode(13, OUTPUT);
  // No setup is required for this library
}

void loop() 
{
  // Enter idle state for 8 s with the rest of peripherals turned off
  // Each microcontroller comes with different number of peripherals
  // Comment off line of code where necessary

  // ATmega328P, ATmega168
  LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
                SPI_OFF, USART0_OFF, TWI_OFF);

  // Do something here
  // Example: Read sensor, data logging, data transmission.
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
}

