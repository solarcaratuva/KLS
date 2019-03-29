// Uses pawelsky library found at
// https://github.com/pawelsky/FlexCAN_Library
#include <FlexCAN.h>
#include <Metro.h>
#include <kinetis_flexcan.h>
#include <cstdlib>
#include <string>

#define TOKEN_SIZE 64
#define BUFFER_SIZE 10
FlexCAN Can1(250000, 1);
static CAN_message_t msg_rx, msg_tx;  // memory allocated for CAN packets
// static CAN_stats_t stat;
static void print_CAN_msg(CAN_message_t &msg);
void parse_serial();

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
    /*stat = Can1.getStats();
    Serial.print("RX Mailbox Entries: ");
    Serial.println(stat.ringRxMax);
    Serial.print("RX Frames Lost: ");
    Serial.println(stat.ringRxFramesLost);
    if (Can1.available()) {
        int st = Can1.read(msg_rx);  // write data into msg
        Serial.println(st);
        print_CAN_msg(msg_rx);
    }*/
    if (Can1.available()) {
        int st = Can1.read(msg_rx);  // write data into msg
        Serial.println(st);
        // static prefix makes variable persistent with each call of this function.
        static bool print_header = true;
        print_CAN_msg(msg_rx, print_header);
    }
    // parse_serial();  // not blocking. (!important)
    // int st = Can1.write(msg_tx);
    // print_CAN_msg(msg_tx);
    // Serial.println(st);
    delay(100);
}

static void print_msg_binary(CAN_message_t &msg, bool print_header) {
    if (print_header) {
        Serial.println(F("id,\tlen,\text,\t[buf]"));
    }
    Serial.print(msg.id, HEX);
    Serial.write(',');
    Serial.write('\t');
    Serial.print(msg.len);
    Serial.write(',');
    Serial.write('\t');
    Serial.print(msg.ext);
    Serial.write(',');
    Serial.write('\t');
    Serial.write('"');
    char buf[4];
    int num_bits = 4 * msg.len;
    int msb = 1 << (BITS - 1);
    for (int i = 0; i < num_bits; i++) {
        if (msg.buf[i] & msb) {
            buf[i % 4] = '1';
        } else {
            buf[i % 4] = '0';
        }
        if ((i + 1) % 4 == 0) {
            for (int j = 0; j < 3; j++) {
                Serial.write(buf[j]);
            }
        }
        temp <<= 1;
    }
}

static void print_msg_decode(CAN_message_t &msg, bool print_header) {
    if (print_header) {
        Serial.println(F("id,\tlen,\text,\t[buf]"));
    }
    Serial.print(msg.id, HEX);
    Serial.write(',');
    Serial.write('\t');
    Serial.print(msg.len);
    Serial.write(',');
    Serial.write('\t');
    Serial.print(msg.ext);
    Serial.write(',');
    Serial.write('\t');
    Serial.write('"');
    for (int j = 0; j < 8; j++) {
        Serial.print(msg.buf[j]);
    }
    Serial.println('"');
}
