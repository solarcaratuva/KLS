/*
 * William Zhang
 * Written to interface with KLS 12201-8080I
 * motor controller over CAN
 * Uses pawelsky fork of FlexCAN library found at
 * https://github.com/pawelsky/FlexCAN_Library
 */

#ifndef __KLS_H__
#define __KLS_H__

#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include <cstdlib>
#include <string>
#include "pindef.h"

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
    uint32_t rpm;
    float current;
    float voltage;
    float throttle;
    uint32_t controller_temp;
    uint32_t motor_temp;
    bool command_status;
    bool feedback_status;
    KLS_switches switches;
    KLS_errors errors;
} KLS_status;

class KLS {
   private:
    uint8_t id;

   public:
    KLS_status status;
    KLS(uint8_t addr);

    uint8_t parse(const CAN_message_t &msg);

    KLS_errors parse_errors(uint8_t lsb, uint8_t msb);

    void set_throttle(uint32_t value);

    void update(const KLS_status &new_status);

    void update(uint32_t rpm, float current, float voltage, float throttle);

    void print();
};

#endif  // __KLS_H__
