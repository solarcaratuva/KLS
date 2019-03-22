// Uses collin80 library found at
// https://github.com/collin80/FlexCAN_Library
#include <kinetis_flexcan.h>
#include <FlexCAN.h>
#include <Metro.h>
#include <cstdlib>
#include <string>

static CAN_message_t msg_rx, msg_tx;  // memory allocated for CAN packets
static void print_CAN_msg(CAN_message_t &msg);
void parse_serial();

void setup() {
    Can1.begin();
    msg_rx.flags.extended = 1; 
    msg_tx.id = 0x100;  // ID field
    msg_tx.flags.extended = 1;     // extended identifier
    msg_tx.len = 8;     // number of bytes to expect in data field
    // data field (strnpcy used to prevent copy of null terminator)
    strncpy((char *)msg_tx.buf, "hello", 8); 
    while (!Serial) continue;
    Serial.println(F("CAN Blaster 9000!"));
}

void loop() {
    if (Can1.available()) {
        Can1.read(msg_rx);  // write data into msg
        print_CAN_msg(msg_rx);
    }
    parse_serial();  // not blocking. (!important)
    delay(1);
}

static void print_CAN_msg(CAN_message_t &msg) {
    // static prefix makes variable persistent with each call of this function.
    static bool print_header = true;  
    if (print_header) {
        Serial.println(F("id,\timestamp,\tlen,\text,\t[buf]"));
        print_header = false;
    }
    Serial.print(msg.id, HEX);
    Serial.write(',');
    Serial.write('\t');
    Serial.print(msg.timestamp);
    Serial.write(',');
    Serial.write('\t');
    Serial.print(msg.len);
    Serial.write(',');
    Serial.write('\t');
    Serial.print(msg.flags.extended);
    Serial.write(',');
    Serial.write('\t');
    Serial.write('"');
    for (int i = 0; i < msg.len; i++) {
        Serial.write(msg.buf[i]);
    }
    Serial.println('"');
}

// really shitty serial interpreter.
// works for the momenet but uses
// naive branching logic structures.
// should be implemented more
// abstractly with data structures
// like classes or nested structures
// and key-value pairs
void parse_serial() {
    if (Serial.available()) {
        char c = Serial.read();
        static char tokens[10][16] = {"", "", "", "", "", "", "", "", "", ""};
        static uint8_t tokenIndex = 0;
        static uint8_t i = 0;  // represents index of character in a given token (defined by tokenIndex)
        uint8_t parse_i = 0;  // once the '\n' character is received, the tokens are decoded and
                              // branching logic is used to read/write system variables.
        // buffer overflow prevention
        if ((i < sizeof(tokens[0])) && (tokenIndex < sizeof(tokens) / sizeof(tokens[0]))) {
            switch (c) {
                case ' ':
                    tokens[tokenIndex][i++] = 0;  // make sure to add the null terminator
                    tokenIndex++;  // increment token index as the space indicates end of token.
                    i = 0;         // reset index of charater
                    break;
                case ',':
                    tokens[tokenIndex][i++] = 0;  // make sure to add the null terminator
                    tokenIndex++;  // increment token index as the space indicates end of token.
                    i = 0;         // reset index of charater
                    break;
                case '[':
                    tokens[tokenIndex][i++] = 0;  // make sure to add the null terminator
                    tokenIndex++;  // increment token index as the space indicates end of token.
                    i = 0;         // reset index of charater
                    break;
                case ']':
                    tokens[tokenIndex][i++] = 0;  // make sure to add the null terminator
                    tokenIndex++;  // increment token index as the space indicates end of token.
                    i = 0;         // reset index of charater
                    break;
                case '\n':
                    tokens[tokenIndex][i++] = 0;  // make sure to add the null terminator
                    if (strcmp(tokens[parse_i], "set") == 0) {
                        parse_i++;
                        if (tokenIndex < parse_i) {
                            Serial.println("error: missing argument");
                        } else if (strcmp(tokens[parse_i], "id") == 0) {
                            parse_i++;
                            if (tokenIndex < parse_i) {
                                Serial.println("error: missing argument");
                            } else {
                                msg_tx.id = atoi(tokens[parse_i]);
                            }
                        } else if (strcmp(tokens[parse_i], "ext") == 0) {
                            parse_i++;
                            if (tokenIndex < parse_i) {
                                Serial.println("error: missing argument");
                            } else {
                                msg_tx.ext = atoi(tokens[parse_i]);
                            }
                        } else if (strcmp(tokens[parse_i], "len") == 0) {
                            parse_i++;
                            if (tokenIndex < parse_i) {
                                Serial.println("error: missing argument");
                            } else {
                                msg_tx.len = (uint8_t)atoi(tokens[parse_i]);
                            }
                        } else if (strcmp(tokens[parse_i], "buf") == 0) {
                            parse_i++;
                            const uint8_t index_offset = parse_i;
                            while ((parse_i <= tokenIndex) &&
                                   ((parse_i - index_offset) < msg_tx.len)) {
                                msg_tx.buf[parse_i - index_offset] = atoi(tokens[parse_i]);
                                parse_i++;
                            }
                        } else {
                            Serial.println("error: invalid variable");
                        }
                    } else if (strcmp(tokens[parse_i], "get") == 0) {
                        parse_i++;
                        if (tokenIndex < parse_i) {
                            Serial.println("error: missing argument");
                        } else if (strcmp(tokens[parse_i], "id") == 0) {
                            Serial.println(msg_tx.id);
                        } else if (strcmp(tokens[parse_i], "ext") == 0) {
                            Serial.println(msg_tx.ext);
                        } else if (strcmp(tokens[parse_i], "len") == 0) {
                            Serial.println(msg_tx.len);
                        } else if (strcmp(tokens[parse_i], "buf") == 0) {
                            int expectedLen = msg_tx.len;
                            int array_index = 0;
                            Serial.write('[');
                            while ((++array_index) < expectedLen) {
                                Serial.write('0');
                                Serial.write('x');
                                Serial.print(msg_tx.buf[array_index - 1], HEX);
                                Serial.write(',');
                            }
                            Serial.print(msg_tx.buf[array_index], DEC);
                            Serial.write(']');
                        } else {
                            Serial.println("invalid variable");
                        }
                    } else if (strcmp(tokens[parse_i], "send") == 0) {
                        Can1.write(msg_tx);
                    } else {
                        Serial.println("error: unknown opcode");
                    }
                    tokenIndex = 0;  // reset token index
                    i = 0;
                    break;
                case '\r':
                    i = 0;
                    tokenIndex = 0;  // reset token index
                    break;
                default:
                    tokens[tokenIndex][i++] = c;
                    break;
            }
        }
    }
}
