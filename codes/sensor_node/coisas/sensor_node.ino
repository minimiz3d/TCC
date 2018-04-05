#include <VirtualWire.h>
#include "LowPower.h"

// Tempo em segundos em modo de baixo consumo (>= 8s)
#define SLEEP_TIME  8
#define NUM_SAMPLES 10   
#define DEBUG       true

// ~DEBUG
void teste(String status) {
  if (status.toFloat() > 700) {
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
  }
}

void transmitData(const char *data) {
  vw_send((uint8_t *)data, strlen(data));
  vw_wait_tx();
}

void sendPacket(String status) {
  const char *start = "T1START";
  transmitData(start);
  delay(50);

  char payloadBuffer[10];
  status.toCharArray(payloadBuffer, 10);
  transmitData(payloadBuffer);
  delay(50);

  const char *end = "T1END";
  transmitData(end);

  // ~DEBUG
  if (DEBUG)
    teste(status);
}

float getCurrentStatus() {
  float status = 0.0;
  for (int i = 0; i < 10; i++) 
    status += analogRead(A0);

  return status / NUM_SAMPLES;
}

void sleep() {
  for (int i = 0; i < SLEEP_TIME/8; i++) 
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}

void setup() {
  // Setup do transceiver
  vw_set_ptt_inverted(true);
  vw_set_tx_pin(8);
  vw_setup(2000);

  // LED interno ~DEBUG
  pinMode(13, OUTPUT);
}

void loop() {
  // Obtém o estado atual da vaga
  String status = String(getCurrentStatus());

  // Transmite este dado  
  sendPacket(status);

  // Entra em modo de baixo consumo
  sleep();
}
