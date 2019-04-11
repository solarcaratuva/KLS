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

void loop() {
    if (Can1.available()) {
        Can1.read(msg_rx);
        kls_l.parse(msg_rx);
        kls_l.print();

        kls_r.parse(msg_rx);
        kls_r.print();
    }
    delay(60);
    // Commands to clear on a VT100 serial terminal
    /*Serial.write(27);     // ESC command
    Serial.print("[2J");  // clear screen command
    Serial.write(27);
    Serial.print("[H");  // cursor to home command*/
}
