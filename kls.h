// Uses pawelsky library found at
// https://github.com/pawelsky/FlexCAN_Library
#ifndef __KLS_H__
#define __KLS_H__

#include <FlexCAN.h>
#include <Metro.h>
#include <kinetis_flexcan.h>
#include <cstdlib>
#include <string>

void print_msg_binary(CAN_message_t &msg, bool print_header) {
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
    int msb = 1 << (num_bits - 1);
    for (int i = 0; i < num_bits; i++) {
        int temp = msg.buf[i];
        if (temp & msb) {
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

void print_msg(CAN_message_t &msg, bool print_header) {
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

void print_msg_decode(CAN_message_t &msg) {
    // message 1
    if (msg.id == 0x0CF11E05) {
        unsigned int speed_lsb = msg.buf[0];
        unsigned int speed_msb = msg.buf[1] << 8;
        unsigned int speed = speed_msb + speed_lsb;
        Serial.print("Speed: ");
        Serial.print(speed, DEC);
        Serial.println(" RPM");

        unsigned int curr_lsb = msg.buf[2];
        unsigned int curr_msb = msg.buf[3] << 8;
        float curr = (curr_msb + curr_lsb) * 1000.0;
        Serial.print("Current: ");
        Serial.print(curr, 3);
        Serial.println(" mA");

        unsigned int volt_lsb = msg.buf[4];
        unsigned int volt_msb = msg.buf[5] << 8;
        float volt = (volt_msb + volt_lsb) / 10.0;
        Serial.print("Voltage: ");
        Serial.print(volt, 3);
        Serial.println(" V");

        unsigned int err_lsb = msg.buf[6];
        for (int i = 0; i < 8; i++) {
            if (err_lsb & 0x01) {
                Serial.print("Error code ");
                Serial.println(i, DEC);
            }
            err_lsb >>= 1;
        }
        unsigned int err_msb = msg.buf[7];
        for (int i = 0; i < 8; i++) {
            if (err_msb & 0x01) {
                Serial.print("Error code ");
                Serial.println(i + 8, DEC);
            }
            err_msb >>= 1;
        }
        Serial.println("---");
    }
    // message 2
    if (msg.id == 0x0CF11F05) {
        float throttle = (msg.buf[0] * 5) / 255.0;
        Serial.print("Throttle: ");
        Serial.print(throttle, 3);
        Serial.println(" V");

        unsigned int cont_temp = msg.buf[1] - 40;
        Serial.print("Controller Temp: ");
        Serial.print(cont_temp);
        Serial.println(" C");

        unsigned int motor_temp = msg.buf[2] - 30;
        Serial.print("Motor Temp: ");
        Serial.print(motor_temp);
        Serial.println(" C");

        unsigned int cont_status = msg.buf[4];

        // two least significant bits
        unsigned int comm_status = cont_status & 0x03;

        Serial.print("Command status: ");
        Serial.println(comm_status);

        unsigned int fdbk_status = (cont_status & 0x0C) >> 2;

        Serial.print("Feedback status: ");
        Serial.println(fdbk_status);

        unsigned int switch_status = msg.buf[5];
        
        bool hall_a = switch_status & 0x01;
        bool hall_b = switch_status & 0x02;
        bool hall_c = switch_status & 0x04;
        
        Serial.print("Hall sensors: ");
        Serial.print(hall_a);
        Serial.print(" ");
        Serial.print(hall_b);
        Serial.print(" ");
        Serial.println(hall_c);
        
        bool brake = switch_status & 0x08;
        bool backward = switch_status & 0x10;
        bool forward = switch_status & 0x20;
        bool foot = switch_status & 0x40;
        bool boost = switch_status & 0x80;

        Serial.print("Brake: ");
        Serial.println(brake);

        Serial.print("Backward: ");
        Serial.println(backward);

        Serial.print("Forward: ");
        Serial.println(forward);

        Serial.print("Foot: ");
        Serial.println(foot);

        Serial.print("Boost: ");
        Serial.println(boost);

        Serial.println("---");
    }
}

#endif  // __KLS_H__
