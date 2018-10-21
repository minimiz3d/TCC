#include <RCSwitch.h>
#include <LowPower.h>
#include <VirtualWire.h>

#define NUM_SAMPLES 500
#define SLEEP_TIME  8
unsigned long time;
RCSwitch mySwitch = RCSwitch();

float getCurrentStatus() {
  float status = 0.0;
  for (int i = 0; i < NUM_SAMPLES; i++) 
    status += analogRead(A0);

  return status / NUM_SAMPLES;
}


void getCurrentTimeAndPrint() {
    time = millis();
    Serial.println(time);
}

void setup() {
    Serial.println("Setup...");
    // MY SWITCH INICIO
    pinMode(8, OUTPUT);
    // mySwitch.enableTransmit(6);
    // mySwitch.setRepeatTransmit(10);
    // MY SWITCH FIM

    // VIRTUAL WIRE INICIO
    // Initialise the IO and ISR
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_set_tx_pin(6);
    vw_setup(2000);	 // Bits per sec
    // VIRTUAL WIRE FIM 

    Serial.begin(9600);
}

void checkLowPowerStateConsumption() {
    while (true)
        // Fica 8 segundos neste modo
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}

float checkLDRreadingConsumption() {
    /*  Verifica o consumo em cada leitura 
        Com 500 valores + média destes valores =~ 100ms
    */
    float currentStatus = getCurrentStatus();
    
    return currentStatus;
}

void sendData() {
    String x = "teste";
    mySwitch.send(x.toInt(), 24);
}

void checkTXConsumption_RCSWITCH() {
    // Verifica consumo na transmissÃƒÆ’Ã†â€™Ãƒâ€šÃ‚Â£o
    // TODO: Testar com a 'VirtualWire' e com a 'RC Switch'
    sendData();
}

void transmitData(const char *data) {
  vw_send((uint8_t *)data, strlen(data));
  vw_wait_tx();
}

void sendPacket(String status) {
    /*
        Tempo total de transmissão =~ 222ms
    */
    const char *start = "T1START";
    transmitData(start);
    //   delay(50);

    char payloadBuffer[10];
    status.toCharArray(payloadBuffer, 10);
    transmitData(payloadBuffer);
    //   delay(50);

    const char *end = "T1END";
    transmitData(end);
}

void sleep() {
    // Mantém os pinos de alimentação do LDR e RF com V = 0V
    // digitalWrite(RF_VCC, LOW);
    // digitalWrite(LDR_VCC, LOW);

    for (int i = 0; i < SLEEP_TIME * 8; i++)
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}

void loop() {
    getCurrentTimeAndPrint();
    
    float current = checkLDRreadingConsumption();
    sendPacket(String(650));

    getCurrentTimeAndPrint();
    
    // delay(1000);
    // Serial.print("LDR: "); Serial.println(current);
    delay(10000);
}




