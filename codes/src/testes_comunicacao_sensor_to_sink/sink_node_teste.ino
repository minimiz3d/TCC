#include <VirtualWire.h>

uint8_t estado[2] = {"0"};

void setup() {
    Serial.begin(9600); // Debugging only
    Serial.println("Iniciando setup");

    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);            // Bits per sec

    vw_rx_start(); // Start the receiver PLL running
}

void loop() {
    uint8_t msgBuffer[VW_MAX_MESSAGE_LEN];
    uint8_t msgLen = VW_MAX_MESSAGE_LEN;

    // Mensagem foi recebida 
    if (vw_get_message(msgBuffer, &msgLen)) {
        if (checkMsg(&msgBuffer)) {
            // Obtém qual dos sensor nodes enviou a mensagem
            int sensor_node_id = msgBuffer[0];
            
            // Se o estado da vaga for atualizado, então transmite para o gateway
            if msgBuffer[2] != estado[sensor_node_id] {
                // Atualiza estado da vaga
                Serial.print("Estado da vaga atualizado");
                estado[sensor_node_id] = msgBuffer[1];                    
            }
        }
    }
    
}