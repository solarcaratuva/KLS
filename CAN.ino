#include "KLS.h"

FlexCAN Can1(250000, 1);
// CAN struct to store received data
static CAN_message_t msg_rx;
// KLS motor controller class initialized with ID 0x05
KLS kls(0x05);

void setup() {
    Can1.begin();
    Serial.begin(115200);
    Serial.println(F("Initialized CAN!"));
}

void loop() {
    if (Can1.available()) {
        Can1.read(msg_rx);  // write data into msg
        kls.parse(msg_rx);
        kls.print();
    }
    delay(60);
    // Commands to clear on a VT100 serial terminal
    /*Serial.write(27);     // ESC command
    Serial.print("[2J");  // clear screen command
    Serial.write(27);
    Serial.print("[H");  // cursor to home command*/
}
