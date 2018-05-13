#include <VirtualWire.h>
#include <SPI.h>
#include "src/lmic.h"
#include "src/hal/hal.h"

// Estado das vagas
bool estado[2] = {0};

// Varia


void setup() {
    Serial.begin(9600);
    
    vw_set_rx_pin(); // A definir pino de recepção
    vw_set_ptt_inverted(true);
    vw_setup(2000);
    vw_rx_start();
}

void send_to_gateway() {
    // Monta o JSON a ser enviado
    String estado_vagas = ""

    // Converte a String em um array uint8_t
    uint8_t estado_atual[estado_vagas.length()+1];
    estado_vagas.getBytes(estado_atual, estado_vagas.length()+1);

    // Verifica se há alguma operação de transmissão ocorrendo
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, estado_atual, sizeof(estado_atual)-1, 0);
        Serial.println(F("Packet queued"));
    }
}

// Verifica se a mensagem recebida foi enviado por algum sensor node
void checkMsg(*msgBuffer) {
    if 
        return true;
    
    else 
        return false;
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
            if msgBuffer[1] != estado[sensor_node_id] {
                // Atualiza estado da vaga
                estado[sensor_node_id] = msgBuffer[1];
                
                send_to_gateway();
            }
        }
    }
}