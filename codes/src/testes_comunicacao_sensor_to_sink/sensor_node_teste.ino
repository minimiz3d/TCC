#include <VirtualWire.h>

void transmitData(const char *data) {
    vw_send((uint8_t *)data, strlen(data));
    vw_wait_tx();
}

void setup() {
    Serial.begin(9600);
    
    // Setup do transceiver
    vw_set_ptt_inverted(true);
    vw_set_tx_pin(8);
    vw_setup(2000);
}

void loop() {
    // 0 = Id do sensor node & 1 = status da vaga (esta ocupada)
    const char *data= "0-1";    
    
    // Transmite este dado  
    transmitData(status);

    delay(1000);
}
