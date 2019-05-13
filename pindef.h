#ifndef __PINDEF_H__
#define __PINDEF_H__

/******************************************/
/*  pin definitions for driver interface  */
/******************************************/
#define PIN_BRAKE_CTRL 20      // analog input
#define PIN_REGEN_CTRL 17      // analog input
#define PIN_GEARSHIFT_CTRL 14  // analog input
#define PIN_TURNSIG_CTRL 15    // analog input
#define PIN_THROTTLE_CTRL 16   // analog input
#define PIN_STEER_CTRL 21      // analog input
#define PIN_HAZARD_CTRL 16     // binary input (pushbutton switch)
#define PIN_DOWN_CTRL 26       // binary input (pushbutton switch)
#define PIN_UP_CTRL 27         // binary input (pushbutton switch)
#define PIN_SEL_CTRL 28        // analog input (pushbutton switch)
#define PIN_KILL_SENSE 2       // binary input
#define PIN_HAZARD_LED 25
#define PIN_UART_1_RX 0  // used for nextion LCD
#define PIN_UART_1_TX 1  // used for nextion LCD

/******************************************/
/* pin definitions for motor controllers  */
/******************************************/
#define PIN_MOTOR_L_REGEN_EN 50  // binary output
#define PIN_MOTOR_L_REGEN 36     // pwm output (0% - 100%)
#define PIN_MOTOR_L_THROTTLE 35  // pwm output (0% - 100%)
#define PIN_MOTOR_L_ECO_EN 49    // binary output
#define PIN_MOTOR_L_METER A11    // analog input
#define PIN_MOTOR_L_FWD_EN 55    // binary output
#define PIN_MOTOR_L_REV_EN 54    // binary output

#define PIN_MOTOR_R_REGEN_EN 53  // binary output
#define PIN_MOTOR_R_REGEN 37     // pwm output
#define PIN_MOTOR_R_THROTTLE 38  // pwm output
#define PIN_MOTOR_R_ECO_EN 39    // binary output
#define PIN_MOTOR_R_METER A10    // analog input
#define PIN_MOTOR_R_FWD_EN 52    // binary output
#define PIN_MOTOR_R_REV_EN 51    // binary output

/******************************************/
/* pin definitions for high power devices */
/******************************************/
#define PIN_HORN 57            // binary output
#define PIN_HEADLIGHTS 42      // binary output
#define PIN_BRAKELIGHTS 56     // binary output
#define PIN_STROBE_CTRL 6      // output
#define PIN_RIGHT_BLINKERS 41  // binary output
#define PIN_LEFT_BLINKERS 40   // binary output
#define PIN_HEADLIGHT_CTRL 23  // binary input
#define PIN_HORN_CTRL 22       // binary input
#define PIN_BRAKE_CTRL 20      // binary input

#endif  // __PINDEF_H__
