/* ************************************************************** 
 * Arduino sketch with low power
 * Author: Martijn Quaedvlieg / Jan de Laet (january 2017)
 * Generated with Generate script by Jan de Laet
 * 
 * *************************************************************/
#include <SPI.h>

// define the activation method ABP or OTAA
#define ACT_METHOD_ABP

/* **************************************************************
* keys for device
* *************************************************************/
static const uint8_t PROGMEM NWKSKEY[16] = { 0x63, 0x16, 0x0D, 0x65, 0xEC, 0x16, 0x7B, 0x2D, 0x79, 0xFF, 0x16, 0xE5, 0xC0, 0x00, 0x00, 0x00 };
static const uint8_t PROGMEM APPSKEY[16] = { 0xF6, 0xE3, 0xA2, 0xB5, 0x22, 0x8C, 0xF4, 0x6F, 0xDA, 0x57, 0x41, 0xC8, 0x77, 0x00, 0x00, 0x00 };
static const uint32_t DEVADDR = 0x12345678;

/* **************************************************************
 * radio
 * *************************************************************/
// Uses LMIC libary by Thomas Telkamp and Matthijs Kooijman (https://github.com/matthijskooijman/arduino-lmic)
#include <lmic.h>
#include <hal/hal.h>

// Declare the job control structures
static osjob_t sendjob;

// These callbacks are only used in over-the-air activation, so they are
// left empty when ABP (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).
#ifdef ACT_METHOD_ABP
  void os_getArtEui (u1_t* buf) { }
  void os_getDevEui (u1_t* buf) { }
  void os_getDevKey (u1_t* buf) { }
#else
  void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
  void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}
  void os_getDevKey (u1_t* buf) { memcpy_P(buf, APPKEY, 16);}
#endif

/* ************************************************************** 
 * Pin mapping
 * *************************************************************/
const lmic_pinmap lmic_pins = {
    .nss = 8,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {2, 5, LMIC_UNUSED_PIN},
};

/* **************************************************************
 * low power
 * *************************************************************/
// low power library does not work for Sodaq processor (ARM)
#ifndef _Sodaq_RN2483_h
  #include "LowPower.h"
#endif

// show debug statements; comment next line to disable debug statements
#define DEBUG

/* ************************************************************** 
 *  sensor settings
 * *************************************************************/
// TTNmapper settings

// data to send
static uint8_t dataTX[1];

/* **************************************************************
 * user settings
 * *************************************************************/
unsigned long cycle_length = 5 * 1 * 1000UL; // cycle * mins_or_secs * 1000;
unsigned long sense_every = 1;
unsigned long send_every = 1;

unsigned long cycle = -1;  //  init at -1, so first cycle starts as cycle 0 for 1st sense/send
unsigned long prevSleep = 0; 

/* **************************************************************
 * setup
 * *************************************************************/
void setup() {
  // Wait (max 10 seconds) for the Serial Monitor
  while ((!Serial) && (millis() < 10000)){ }

  //Set baud rate
  Serial.begin(9600);
  Serial.println(F("Sink node do projeto (template version: 13Jan2017 generated: 14Nov2018)"));

  init_node();
  init_sensor();
}


/* **************************************************************
 * loop
 * *************************************************************/
void loop() {
  // next cycle
  cycle += 1;
  
  // check if we need to sense
  if ( (cycle % sense_every) == 0 ) { do_sense(); }

  // check if need to send
  if ( (cycle % send_every) == 0 ) { build_data(); do_send(); }
  
  // go to sleep
  unsigned long current = millis();
  do_sleep(cycle_length - (current - prevSleep));  // sleep minus elapsed time
  prevSleep = current;
  // Sodaq has no sleep, adjust prevSleep
  #ifdef _Sodaq_RN2483_h
    prevSleep += cycle_length;
  #endif
}


/* **************************************************************
 * sensor code, typical would be init_sensor(), do_sense(), build_data()
 * *************************************************************/
/* **************************************************************
 * init the sensor
 * *************************************************************/
void init_sensor() {
}

/* **************************************************************
 * do the reading
 * *************************************************************/
void do_sense() {
}

/* **************************************************************
 * build data to transmit in dataTX
 *
 * Suggested payload function for this data
 *
 * if (bytes[0] >= 0x20) {
 *   str = '';
 *   for (var i = 0; i < bytes.length; i += 1) str += String.fromCharCode(bytes[i]);
 *   return { payload: str };
 * }
 *
 * *************************************************************/
void build_data() {
  dataTX[0] = 'T';     //first byte 
}


/* **************************************************************
* sleep
* *************************************************************/
void do_sleep(float sleepTime) {

  #ifdef DEBUG
    Serial.print(F("Sleep for "));
    Serial.print(sleepTime/1000, 3);
    Serial.println(F(" seconds"));
  #endif

  Serial.flush();

  #ifndef _Sodaq_RN2483_h
    // sleep logic using LowPower library
    int delays[] = {8000, 4000, 2000, 1000, 500, 250, 120, 60, 30, 15};
    period_t sleep[] = {SLEEP_8S, SLEEP_4S, SLEEP_2S, SLEEP_1S, SLEEP_500MS,  SLEEP_250MS, SLEEP_120MS, SLEEP_60MS, SLEEP_30MS, SLEEP_15MS};

    // correction for overhead in this routine
    sleepTime = sleepTime * 0.93;

    float x;
    unsigned int i;
    for (i=0; i<=9; i++) {
      for (x=sleepTime; x>=delays[i]; x-=delays[i]) {
        LowPower.powerDown(sleep[i], ADC_OFF, BOD_OFF);
        sleepTime -= delays[i];
      } 
    }
  #else
    // no LowPower implemented yet, so just a delay
    delay(sleepTime);
  #endif

}

  
/* **************************************************************
 * radio code, typical would be init_node(), do_send(), etc
 * *************************************************************/
/* **************************************************************
 * init the Node
 * *************************************************************/
void init_node() {
  #ifdef VCC_ENABLE
     // For Pinoccio Scout boards
     pinMode(VCC_ENABLE, OUTPUT);
     digitalWrite(VCC_ENABLE, HIGH);
     delay(1000);
  #endif

  // LMIC init
  os_init();
  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();

  #ifdef ACT_METHOD_ABP
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
      LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
    #endif

    #if defined(CFG_eu868)
      // Set up the channels used by the Things Network, which corresponds
      // to the defaults of most gateways. Without this, only three base
      // channels from the LoRaWAN specification are used, which certainly
      // works, so it is good for debugging, but can overload those
      // frequencies, so be sure to configure the full frequency range of
      // your network here (unless your network autoconfigures them).
      // Setting up channels should happen after LMIC_setSession, as that
      // configures the minimal channel set.
      // NA-US channels 0-71 are configured automatically
      LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
      LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
      LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
      LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
      LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
      LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
      LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
      LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
      LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
      // TTN defines an additional channel at 869.525Mhz using SF9 for class B
      // devices' ping slots. LMIC does not have an easy way to define set this
      // frequency and support for class B is spotty and untested, so this
      // frequency is not configured here.
    #elif defined(CFG_us915)
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

    // Set data rate and transmit power (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(DR_SF7,14);
  #endif

  #ifdef ACT_METHOD_OTAA
    // got this fix from forum: https://www.thethingsnetwork.org/forum/t/over-the-air-activation-otaa-with-lmic/1921/36
    LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);
  #endif

}

/* **************************************************************
 * send the message
 * *************************************************************/
void do_send() {

  Serial.print(millis());
  Serial.print(F(" Sending.. "));  

  send_message(&sendjob);

  // wait for send to complete
  Serial.print(millis());
  Serial.print(F(" Waiting.. "));  
 
  while ( (LMIC.opmode & OP_JOINING) or (LMIC.opmode & OP_TXRXPEND) ) { os_runloop_once();  }
  Serial.print(millis());
  Serial.println(F(" TX_COMPLETE"));
}
 
/* *****************************************************************************
* send_message
* ****************************************************************************/
void send_message(osjob_t* j) {
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND) {
    Serial.println(F("OP_TXRXPEND, not sending"));
  } else {
    // Prepare upstream data transmission at the next possible time.
    LMIC_setTxData2(1, dataTX, sizeof(dataTX), 0);
    Serial.println(F("Packet queued"));
  }
}

/*******************************************************************************/
void onEvent (ev_t ev) {
  switch (ev) {
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
      // Disable link check validation (automatically enabled
      // during join, but not supported by TTN at this time).
      LMIC_setLinkCheckMode(0);
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
      if (LMIC.dataLen) {
        // data received in rx slot after tx
        Serial.print(F("Data Received: "));
        Serial.write(LMIC.frame+LMIC.dataBeg, LMIC.dataLen);
        Serial.println();
      }
      // schedule next transmission
      // os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), send_message);
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
