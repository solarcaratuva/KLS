#include "KLS.h"

KLS::KLS(uint8_t addr = 0x05) {
    id = addr;

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

    // set analog resolution for all functions
    analogWriteResolution(14);

    // if the ID is odd, left motor
    // if the ID is even, right motor
    if (id & 0x01) {
        // set output PWM frequency for motor controller
        analogWriteFrequency(PIN_MOTOR_L_THROTTLE, 32000);
        analogWriteFrequency(PIN_MOTOR_L_REGEN, 32000);
        // initialize GPIO for motor controller
        pinMode(PIN_MOTOR_L_REGEN_EN, OUTPUT);
        pinMode(PIN_MOTOR_L_REGEN, OUTPUT);
        pinMode(PIN_MOTOR_L_THROTTLE, OUTPUT);
        pinMode(PIN_MOTOR_L_ECO_EN, OUTPUT);
        pinMode(PIN_MOTOR_L_METER, INPUT);
        pinMode(PIN_MOTOR_L_FWD_EN, OUTPUT);
        pinMode(PIN_MOTOR_L_REV_EN, OUTPUT);

        digitalWrite(PIN_MOTOR_L_REGEN_EN, HIGH);
        analogWrite(PIN_MOTOR_L_REGEN, 0);
        analogWrite(PIN_MOTOR_L_THROTTLE, 0);
        digitalWrite(PIN_MOTOR_L_ECO_EN, LOW);
        // ??? = analogRead(PIN_MOTOR_L_METER);
        pinMode(PIN_MOTOR_L_FWD_EN, HIGH);
        pinMode(PIN_MOTOR_L_REV_EN, LOW);
    } else {
        // set output PWM frequency for motor controller
        analogWriteFrequency(PIN_MOTOR_R_THROTTLE, 32000);
        analogWriteFrequency(PIN_MOTOR_R_REGEN, 32000);
        // initialize GPIO for motor controller
        pinMode(PIN_MOTOR_R_REGEN_EN, OUTPUT);
        pinMode(PIN_MOTOR_R_REGEN, OUTPUT);
        pinMode(PIN_MOTOR_R_THROTTLE, OUTPUT);
        pinMode(PIN_MOTOR_R_ECO_EN, OUTPUT);
        pinMode(PIN_MOTOR_R_METER, INPUT);
        pinMode(PIN_MOTOR_R_FWD_EN, OUTPUT);
        pinMode(PIN_MOTOR_R_REV_EN, OUTPUT);

        digitalWrite(PIN_MOTOR_R_REGEN_EN, HIGH);
        analogWrite(PIN_MOTOR_R_REGEN, 0);
        analogWrite(PIN_MOTOR_R_THROTTLE, 0);
        digitalWrite(PIN_MOTOR_R_ECO_EN, LOW);
        // ??? = analogRead(PIN_MOTOR_R_METER);
        digitalWrite(PIN_MOTOR_R_FWD_EN, HIGH);
        digitalWrite(PIN_MOTOR_R_REV_EN, LOW);
    }
};

uint8_t KLS::parse(const CAN_message_t &msg) {
    uint8_t parsed = 0;
    // message 1
    if (msg.id == (uint32_t)0x0CF11E00 + id) {
        parsed = 1;
        // rpm values range from 0-6000RPM
        status.rpm = (msg.buf[1] << 8) + msg.buf[0];
        // current values range from 0-400A
        status.current = ((msg.buf[3] << 8) + msg.buf[2]) / 10.0;
        // voltage values range from 0-180V
        status.voltage = ((msg.buf[5] << 8) + msg.buf[4]) / 10.0;
        status.errors = parse_errors(msg.buf[6], msg.buf[7]);
    }
    // message 2
    if (msg.id == (uint32_t)0x0CF11F00 + id) {
        parsed = 2;
        // throttle will only go from 0.8-4.2V
        // throttle values map from 0-255 to 0-5V
        status.throttle = (msg.buf[0] * 5.0) / 255.0;
        // temperature offset of 40C
        status.controller_temp = msg.buf[1] - 40;
        // temperature offset of 30C
        status.motor_temp = msg.buf[2] - 30;

        uint8_t controller_status = msg.buf[4];
        // two least significant bits
        status.command_status = controller_status & 0x03;
        status.feedback_status = (controller_status & 0x0C) >> 2;

        uint8_t switch_status = msg.buf[5];
        // mask each bit to extract each switch status
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

KLS_errors KLS::parse_errors(uint8_t lsb, uint8_t msb) {
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

void KLS::set_throttle(uint32_t value) {
    // if the ID is odd, left motor
    // if the ID is even, right motor
    if (value > MAX_PWM) {
        value = MAX_PWM;
    }
    if (id & 0x01) {
        analogWrite(PIN_MOTOR_L_THROTTLE, value);
    } else {
        analogWrite(PIN_MOTOR_R_THROTTLE, value);
    }
}

// untested
void KLS::regen_en(bool value) {
    const uint32_t regen_en_pin = (id & 0x01) ? PIN_MOTOR_L_REGEN_EN : PIN_MOTOR_R_REGEN_EN;
    digitalWrite(regen_en_pin, value);
}

// untested
void KLS::set_regen(uint32_t value) {
    // if the ID is odd, left motor
    // if the ID is even, right motor
    if (value > MAX_PWM) {
        value = MAX_PWM;
    }
    if (id & 0x01) {
        analogWrite(PIN_MOTOR_L_REGEN, value);
    } else {
        analogWrite(PIN_MOTOR_R_REGEN, value);
    }
}

// untested
void KLS::set_direction(uint8_t value) {
    const uint32_t fwd_en_pin = (id & 0x01) ? PIN_MOTOR_L_FWD_EN : PIN_MOTOR_R_FWD_EN;
    const uint32_t rev_en_pin = (id & 0x01) ? PIN_MOTOR_L_REV_EN : PIN_MOTOR_R_REV_EN;

    if (value > 0) {
        digitalWrite(fwd_en_pin, HIGH);
        digitalWrite(rev_en_pin, LOW);
    } else if (value < 0) {
        digitalWrite(fwd_en_pin, LOW);
        digitalWrite(rev_en_pin, HIGH);
    } else {
        digitalWrite(fwd_en_pin, LOW);
        digitalWrite(rev_en_pin, LOW);
    }
}

// untested
void KLS::eco_en(boolean value) {
    digitalWrite(PIN_MOTOR_R_ECO_EN, value);
    digitalWrite(PIN_MOTOR_L_ECO_EN, value);
}

void KLS::update(const KLS_status &new_status) {
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

void KLS::update(uint32_t rpm, float current, float voltage, float throttle) {
    status.rpm = rpm;
    status.current = current;
    status.voltage = voltage;
    status.throttle = throttle;
}

void KLS::print() {
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
