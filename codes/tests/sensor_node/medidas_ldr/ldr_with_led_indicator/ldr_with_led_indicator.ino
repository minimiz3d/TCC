#include "LowPower.h"
#include <RCSwitch.h>

#define NUM_SAMPLES 5000
#define SLEEP_TIME  1
#define MAX_DEGREE 60
#define LDR_THRESHOLD 500
#define A_THRESHOLD 400
#define B_THRESHOLD 450
#define C_THRESHOLD 500
#define D_THRESHOLD 550
#define E_THRESHOLD 600
#define F_THRESHOLD 650
#define FADE_TIME 25
#define LED_PIN 9
#define INDICATOR_PIN 7
#define LDR_PIN 5
#define TX_PIN 6
#define TX_POWER_PIN 8

float sin_value;
float ldr_value = 0;
int led_value = 0;

RCSwitch mySwitch = RCSwitch();

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(INDICATOR_PIN, OUTPUT);
  pinMode(TX_POWER_PIN, INPUT);
}

void sleep() {
  for (int i = 0; i < SLEEP_TIME; i++) 
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}

float getCurrentStatus() {
  float status = 0.0;
  for (int i = 0; i < NUM_SAMPLES; i++) 
    status += analogRead(LDR_PIN);

  return status / NUM_SAMPLES;
}

void blink(int number_of_times) {
  // Indica inÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â­cio
  // digitalWrite(INDICATOR_PIN, HIGH);
  // delay(1000);
  // digitalWrite(INDICATOR_PIN, LOW);

  for(uint8_t i = 0; i < number_of_times; i++) {
    analogWrite(LED_PIN, 255);
    delay(300);

    analogWrite(LED_PIN, 0);
    delay(300);
  }

  // Indica fim 
  // digitalWrite(INDICATOR_PIN, HIGH);
  // delay(1000);
  // digitalWrite(INDICATOR_PIN, LOW);
}

void blink_debug(int number_of_times, int pin) {
  for(uint8_t i = 0; i < number_of_times; i++) {
    analogWrite(pin, 255);
    delay(250);

    analogWrite(pin, 0);
    delay(250);
  }
}

void fade_in_fade_out() {
  for (int x=0; x<MAX_DEGREE; x++) {
    // converte graus para radianos e entÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â£o obtÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â©m o valor do seno
    sin_value = (sin(x*(3.1412/MAX_DEGREE)));
    led_value = int(sin_value*255);
    analogWrite(LED_PIN, led_value);
    delay(FADE_TIME);
  }
}

float get_ldr_value() {
  ldr_value = analogRead(LDR_PIN);
  return ldr_value;
}

void sendData() {
  mySwitch.send(atol("OLA"));
  blink(1);
}

void test_rf() {
  // Configura RF
  pinMode(TX_POWER_PIN, OUTPUT);
  digitalWrite(TX_POWER_PIN, HIGH);
  mySwitch.enableTransmit(TX_PIN);
  mySwitch.setRepeatTransmit(10);

  // Transmite dados
  sendData();

  // Desabilita o RF
  mySwitch.disableTransmit();
  pinMode(TX_POWER_PIN, INPUT);

  sleep();
//  delay(3000);
}

int is_car_parked() {
  ldr_value = getCurrentStatus();

  // NÃƒÂ£o hÃƒÂ¡ veÃƒÂ­culo
  if (ldr_value >= F_THRESHOLD) 
    return -1;
  
  // HÃƒÂ¡ veÃƒÂ­culo
  return 0;
}

void test_threshold() {
  // ldr_value = get_ldr_value();
  ldr_value = getCurrentStatus();
  
  if (ldr_value >= A_THRESHOLD && ldr_value < B_THRESHOLD) {blink(1);} 
  else if (ldr_value >= B_THRESHOLD && ldr_value < C_THRESHOLD) {blink(3);}
  else if (ldr_value >= C_THRESHOLD && ldr_value < D_THRESHOLD) {blink(5);}
  else if (ldr_value >= D_THRESHOLD && ldr_value < E_THRESHOLD) {blink(7);}
  else if (ldr_value >= E_THRESHOLD && ldr_value < F_THRESHOLD) {blink(10);}
  else if (ldr_value >= F_THRESHOLD) {blink(12);}
  else {fade_in_fade_out();}
}

void test_parked_or_not() {
  if (is_car_parked() == -1) {
    digitalWrite(INDICATOR_PIN, HIGH);
    delay(250);
    digitalWrite(INDICATOR_PIN, LOW);
  }
  else 
    blink_debug(3, INDICATOR_PIN);


  delay(5000);
}

void loop() {
//  test_rf();
  test_threshold();
  test_parked_or_not();
}




