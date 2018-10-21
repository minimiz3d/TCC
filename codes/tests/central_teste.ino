#include <VirtualWire.h>
#include <SPI.h>
#include "src/lmic.h"
#include "src/hal/hal.h"

// Estado das vagas
bool estado[2] = {0};
uint8_t msg_enviada = 1;

// LoRaWAN Network Session Key
static const PROGMEM u1_t NWKSKEY[16] = {0x15, 0xFF, 0x21, 0xBF, 0x19, 0x6A, 0x2F, 0xC4, 0xE3, 0x23, 0x06, 0xB6, 0x97, 0xFD, 0xEE, 0x53};

// LoRaWAN App Session Key
static const PROGMEM u1_t APPSKEY[16] = {0x97, 0xA1, 0x65, 0xA2, 0x7A, 0x73, 0x3D, 0x71, 0xFE, 0xCD, 0xAD, 0x2F, 0xA0, 0xAD, 0xC6, 0x23};

// LoRaWAN Device Address (sink node)
static const u4_t DEVADDR = 0x26031A32;

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 6,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 5,
    .dio = {2, 3, 4},
};

// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).
void os_getArtEui(u1_t *buf) {}
void os_getDevEui(u1_t *buf) {}
void os_getDevKey(u1_t *buf) {}

static osjob_t send_current_status_job;

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            break;
        case EV_RFU1:
            Serial.println(F("EV_RFU1"));
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
              Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.println(F("Received "));
              Serial.println(LMIC.dataLen);
              Serial.println(F(" bytes of payload"));
            }
            
            // Mensagem foi enviada ao gateway com sucesso, então novas mensagens podem ser enviadas
            msg_enviada = 1;

            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
         default:
            Serial.println(F("Unknown event"));
            break;
    }
}

// Verifica se a mensagem recebida foi enviado por algum sensor node
void checkMsg(*msgBuffer) {
    // TODO: Implementar esta função
    if 
        return true;
    
    else 
        return false;
}

void send_current_status(osjob_t *job) {
    // Monta o JSON a ser enviado
    String estado_vagas = "";

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

void setup() {
    Serial.begin(9600);
    Serial.println("Iniciando setup");

    // LMIC init
    os_init();
    
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // If not running an AVR with PROGMEM, just use the arrays directly
    LMIC_serxloratSession(0x1, DEVADDR, NWKSKEY, APPSKEY);

    // Set static session parameters. Instead of dynamically establishing a session
    // by joining the network, precomputed session parameters are be provided.
    #ifdef PROGMEM
    // On AVR, these values are stored in flash and only copied to RAM
    // once. Copy them to a temporary buffer here, LMIC_setSession will
    // copy them into a buffer of its own again.
    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
    #else
    // If not running an AVR with PROGMEM, just use the arrays directly
    LMIC_serxloratSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
    #endif

    #if defined(CFG_us915)
    // NA-US channels 0-71 are configured automatically
    // but only one group of 8 should (a subband) should be active
    // TTN recommends the second sub band, 1 in a zero based count.
    // https://github.com/TheThingsNetwork/gateway-conf/blob/master/US-global_conf.json
    LMIC_selectSubBand(1);
    #endif

    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    /*
    DR_SF12 : DR0
    DR_SF11 : DR1
    DR_SF10 : DR2
    DR_SF9  : DR3
    DR_SF8  : DR4
    DR_SF7  : DR5
    DR_SF8C : DR6
    */
    // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(DR_SF12, 14);
}

void loop() {
    if (msg_enviada) {
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
                }
            }
        }

        send_current_status(&send_current_status_job);
        msg_enviada = 0;
    }

    os_runloop_once();
}