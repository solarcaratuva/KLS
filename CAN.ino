#include "kls.h"

FlexCAN Can1(250000, 1);
static CAN_message_t msg_rx, msg_tx;  // memory allocated for CAN packets
KLS k;

void setup() {
    Can1.begin();
    // Can1.startStats();
    msg_tx.id = 256;  // ID field
    msg_tx.ext = 1;   // extended identifier
    msg_tx.timeout = 500;
    msg_tx.len = 8;  // number of bytes to expect in data field
    // data field (strnpcy used to prevent copy of null terminator)
    strncpy((char *)msg_tx.buf, "hello", 8);
    while (!Serial) continue;
    Serial.println(F("Initialized CAN!"));
}

void loop() {
    if (Can1.available()) {
        int st = Can1.read(msg_rx);  // write data into msg
        k.parse(msg_rx);
        k.print();
    }
    delay(60);
}
