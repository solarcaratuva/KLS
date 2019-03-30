// Uses pawelsky library found at
// https://github.com/pawelsky/FlexCAN_Library
#ifndef __KLS_H__
#define __KLS_H__

#include <FlexCAN.h>
#include <Metro.h>
#include <kinetis_flexcan.h>
#include <cstdlib>
#include <string>

typedef struct KLS_switches {
    bool hall_a;
    bool hall_b;
    bool hall_c;
    bool brake;
    bool backward;
    bool forward;
    bool foot;
    bool boost;
} KLS_switches;

typedef struct KLS_status {
    unsigned int rpm;
    float current;
    float voltage;
    float throttle;
    unsigned int controller_temp;
    unsigned int motor_temp;
    bool command_status;
    bool feedback_status;
    KLS_switches switches;
    bool errors[16] = {0};
} KLS_status;

class KLS {
   public:
    KLS_status status;
    KLS();
    uint8_t parse(const CAN_message_t &msg) {
        uint8_t parsed = 0;
        // message 1
        if (msg.id == 0x0CF11E05) {
            parsed = 1;
            status.rpm = (msg.buf[1] << 8) + msg.buf[0];
            status.current = (msg.buf[3] << 8) + msg.buf[2];
            status.voltage = ((msg.buf[5] << 8) + msg.buf[4]) / 10.0;

            uint8_t err_count = parse_errors(msg.buf[6], msg.buf[7]);
            if (err_count != 0) {
                Serial.print("Errors detected: ");
                for (int i = 0; i < 16; i++) {
                    if (status.errors[i]) {
                        Serial.print("ERR");
                        Serial.print(i);
                        Serial.print(" ");
                    }
                }
                Serial.println("");
            }
        }
        // message 2
        if (msg.id == 0x0CF11F05) {
            parsed = 2;
            // throttle will only go from 0.8-4.2V
            status.throttle = (msg.buf[0] * 5) / 256.0;
            status.controller_temp = msg.buf[1] - 40;
            status.motor_temp = msg.buf[2] - 30;
            uint8_t controller_status = msg.buf[4];

            // two least significant bits
            status.command_status = controller_status & 0x03;
            status.feedback_status = (controller_status & 0x0C) >> 2;

            uint8_t switch_status = msg.buf[5];

            status.switches.hall_a = switch_status & 0x01;
            status.switches.hall_b = switch_status & 0x02;
            status.switches.hall_c = switch_status & 0x04;

            status.switches.brake = switch_status & 0x08;
            status.switches.backward = switch_status & 0x10;
            status.switches.forward = switch_status & 0x20;
            status.switches.foot = switch_status & 0x40;
            status.switches.boost = switch_status & 0x80;
        }
        return parsed;
    }

    uint8_t parse_errors(uint8_t lsb, uint8_t msb) {
        uint8_t err_count = 0;
        for (int i = 0; i < 8; i++) {
            if (lsb & 0x01) {
                Serial.print("Error code ");
                Serial.println(i, DEC);
                status.errors[i] = 1;
                err_count++;
            }
            lsb >>= 1;
        }
        for (int i = 0; i < 8; i++) {
            if (msb & 0x01) {
                Serial.print("Error code ");
                Serial.println(i + 8, DEC);
                status.errors[i + 8] = 1;
                err_count++;
            }
            msb >>= 1;
        }
        return err_count;
    }

    void update(const KLS_status &new_status) {
        status.rpm = new_status.rpm;
        status.current = new_status.current;
        status.voltage = new_status.voltage;
        status.throttle = new_status.throttle;
        status.controller_temp = new_status.controller_temp;
        status.motor_temp = new_status.motor_temp;
        status.command_status = new_status.command_status;
        status.feedback_status = new_status.feedback_status;

        status.switches.hall_a = new_status.switches.hall_a;
        status.switches.hall_b = new_status.switches.hall_b;
        status.switches.hall_c = new_status.switches.hall_c;
        status.switches.brake = new_status.switches.brake;
        status.switches.backward = new_status.switches.backward;
        status.switches.forward = new_status.switches.forward;
        status.switches.foot = new_status.switches.foot;
        status.switches.boost = new_status.switches.boost;
    }

    void update(unsigned int rpm, float current, float voltage, float throttle) {
        status.rpm = rpm;
        status.current = current;
        status.voltage = voltage;
        status.throttle = throttle;
    }
};

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
        uint8_t temp = msg.buf[i];
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
        float curr = (curr_msb + curr_lsb);
        Serial.print("Current: ");
        Serial.print(curr, 3);
        Serial.println(" A");

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
        // throttle will only go from 0.8-4.2V
        float throttle = (msg.buf[0] * 5) / 256.0;
        Serial.print("Throttle: ");
        Serial.print(throttle, 3);
        Serial.println(" V");

        unsigned int controller_temp = msg.buf[1] - 40;
        Serial.print("Controller Temp: ");
        Serial.print(controller_temp);
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
