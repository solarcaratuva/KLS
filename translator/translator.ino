// William Zhang
// Written to translate ID of second
// KLS 12201-8080I motor controller

#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include <cstdlib>
#include <string>

FlexCAN Can0(250000, 0);
FlexCAN Can1(250000, 1);
static CAN_message_t msg_rx, msg_tx;  // memory allocated for CAN packets

void setup() {
    Can0.begin();
    Can1.begin();
    Serial.begin(115200);
    Serial.println(F("Initialized CAN!"));
}

void loop() {
    if (Can0.available()) {
        Can0.read(msg_rx);
        // message 1
        if (msg_rx.id == 0x0CF11E05) {
            msg_tx.id = 0x0CF11E06;
            msg_tx.ext = msg_rx.ext;
            msg_tx.len = msg_rx.len;
            msg_tx.timeout = msg_rx.timeout;
            memcpy(msg_tx.buf, msg_rx.buf, 8);
            Can1.write(msg_tx);
        }
        // message 2
        if (msg_rx.id == 0x0CF11F05) {
            msg_tx.id = 0x0CF11F06;
            msg_tx.ext = msg_rx.ext;
            msg_tx.len = msg_rx.len;
            msg_tx.timeout = msg_rx.timeout;
            memcpy(msg_tx.buf, msg_rx.buf, 8);
            Can1.write(msg_tx);
        }
    }
}
