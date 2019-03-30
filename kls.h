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

typedef struct KLS_errors {
    uint8_t count;
    bool errors[16] = {0};
} KLS_errors;

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
    KLS_errors errors;
} KLS_status;

class KLS {
   public:
    KLS_status status;

    KLS() {
        status.rpm = 0;
        status.current = 0.0;
        status.voltage = 0.0;
        status.throttle = 0.0;
        status.controller_temp = 0;
        status.motor_temp = 0;
        status.command_status = 0;
        status.feedback_status = 0;

        status.switches.hall_a = 0;
        status.switches.hall_b = 0;
        status.switches.hall_c = 0;
        status.switches.brake = 0;
        status.switches.backward = 0;
        status.switches.forward = 0;
        status.switches.foot = 0;
        status.switches.boost = 0;

        status.errors.count = 0;
    };

    uint8_t parse(const CAN_message_t &msg) {
        uint8_t parsed = 0;
        // message 1
        if (msg.id == 0x0CF11E05) {
            parsed = 1;
            status.rpm = (msg.buf[1] << 8) + msg.buf[0];
            status.current = (msg.buf[3] << 8) + msg.buf[2];
            status.voltage = ((msg.buf[5] << 8) + msg.buf[4]) / 10.0;
            status.errors = parse_errors(msg.buf[6], msg.buf[7]);
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

    KLS_errors parse_errors(uint8_t lsb, uint8_t msb) {
        KLS_errors err;
        err.count = 0;
        for (int i = 0; i < 8; i++) {
            if (lsb & 0x01) {
                err.errors[i] = 1;
                err.count++;
            }
            lsb >>= 1;
        }
        for (int i = 0; i < 8; i++) {
            if (msb & 0x01) {
                err.errors[i + 8] = 1;
                err.count++;
            }
            msb >>= 1;
        }
        return err;
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

    void print() {
        Serial.print("Speed: ");
        Serial.print(status.rpm, DEC);
        Serial.println(" RPM");

        Serial.print("Current: ");
        Serial.print(status.current, 3);
        Serial.println(" A");

        Serial.print("Voltage: ");
        Serial.print(status.voltage, 3);
        Serial.println(" V");

        Serial.print("Throttle: ");
        Serial.print(status.throttle, 3);
        Serial.println(" V");

        Serial.print("Controller Temp: ");
        Serial.print(status.controller_temp);
        Serial.println(" C");

        Serial.print("Motor Temp: ");
        Serial.print(status.motor_temp);
        Serial.println(" C");

        Serial.print("Command status: ");
        Serial.println(status.command_status);

        Serial.print("Feedback status: ");
        Serial.println(status.feedback_status);

        Serial.print("Hall sensors: ");
        Serial.print(status.switches.hall_a);
        Serial.print(" ");
        Serial.print(status.switches.hall_b);
        Serial.print(" ");
        Serial.println(status.switches.hall_c);

        Serial.print("Brake: ");
        Serial.println(status.switches.brake);

        Serial.print("Backward: ");
        Serial.println(status.switches.backward);

        Serial.print("Forward: ");
        Serial.println(status.switches.forward);

        Serial.print("Foot: ");
        Serial.println(status.switches.foot);

        Serial.print("Boost: ");
        Serial.println(status.switches.boost);

        Serial.println("---");
    }
};

#endif  // __KLS_H__
