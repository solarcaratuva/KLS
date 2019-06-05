#include "KLS.h"

FlexCAN Can0(250000, 0);
FlexCAN Can1(250000, 1);
// CAN struct to store received data
static CAN_message_t msg_rx_0;
static CAN_message_t msg_rx_1;

// Left motor initialized with ID 0x05
KLS kls_l(0x05);
// Right motor initialized with ID 0x06
KLS kls_r(0x06);

void setup() {
    Can0.begin();
    Can1.begin();
    Serial.begin(115200);
    while (!Serial)
        ;
    Serial.println(F("Initialized CAN!"));
}

void generate_sawtooth() {
    for (int i = 0; i < 12500; i++) {
        kls_l.set_throttle(i);
        kls_r.set_throttle(i);
    }
}

void loop() {
    if (Can0.available()) {
        Can0.read(msg_rx_0);
        Serial.printf("Can0: %08x\n", msg_rx_0.id);
        /*kls_l.parse(msg_rx);
        kls_l.print();
        kls_r.parse(msg_rx);
        kls_r.print();*/
        // kls_l.set_throttle(32000);
    } else {
        // kls_l.set_throttle(0);
    }
    if (Can1.available()) {
        Can1.read(msg_rx_1);
        Serial.printf("Can1: %08x\n", msg_rx_1.id);
    }
    delay(500);
    // Commands to clear on a VT100 serial terminal
    /*Serial.write(27);     // ESC command
    Serial.print("[2J");  // clear screen command
    Serial.write(27);
    Serial.print("[H");  // cursor to home command*/
}
