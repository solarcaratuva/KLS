#include "KLS.h"

FlexCAN Can1(250000, 1);
// CAN struct to store received data
static CAN_message_t msg_rx;

// Left motor initialized with ID 0x05
KLS kls_l(0x05);
// Right motor initialized with ID 0x06
KLS kls_r(0x06);

void setup() {
    Can1.begin();
    Serial.begin(115200);
    Serial.println(F("Initialized CAN!"));
}

void generate_sawtooth() {
    for (int i = 0; i < 13652; i++) {
        kls_l.set_throttle(i);
        kls_r.set_throttle(i);
    }
}

void loop() {
    if (Can1.available()) {
        Can1.read(msg_rx);
        kls_l.parse(msg_rx);
        kls_l.print();
        kls_r.parse(msg_rx);
        kls_r.print();
        // kls_l.set_throttle(32000);
    } else {
        // kls_l.set_throttle(0);
    }

    generate_sawtooth();

    // Commands to clear on a VT100 serial terminal
    /*Serial.write(27);     // ESC command
    Serial.print("[2J");  // clear screen command
    Serial.write(27);
    Serial.print("[H");  // cursor to home command*/
}
