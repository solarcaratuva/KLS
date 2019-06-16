#include "KLS.h"

FlexCAN Can1(250000, 1);
// CAN struct to store received data
static CAN_message_t msg_rx;

// Left motor initialized with source address 0x05
KLS kls_l(0x05);
// Right motor initialized with source address 0x06
KLS kls_r(0x06);

void setup() {
    // Can0.begin();
    Can1.begin();
    Serial.begin(115200);
    while (!Serial)
        ;
    Serial.println(F("Initialized CAN!"));
}

void generate_sawtooth() {
    for (int i = 0; i < 12500; i++) {
        // kls_l.set_throttle(i);
        kls_r.set_throttle(i);
    }
}

void loop() {
    uint32_t throttle = map(analogRead(PIN_THROTTLE_CTRL), 0, 1023, 0, MAX_PWM);
    kls_l.set_throttle(throttle);
    kls_r.set_throttle(throttle);
    if (Can1.available()) {
        Can1.read(msg_rx);
        // Serial.printf("Can1: %08x\n", msg_rx_1.id);
        kls_l.parse(msg_rx);
        Serial.printf("Left Motor:\n");
        kls_l.print();
        Serial.printf("Right Motor:\n");
        kls_r.parse(msg_rx);
        kls_r.print();
    }
    delay(150);
    // Commands to clear on a VT100 serial terminal
    /*Serial.write(27);     // ESC command
    Serial.print("[2J");  // clear screen command
    Serial.write(27);
    Serial.print("[H");  // cursor to home command*/
}
