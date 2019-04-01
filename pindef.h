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
#define PIN_STEER_CTRL 48      // analog input
#define PIN_HAZARD_CTRL 24     // binary input (pushbutton switch)
#define PIN_DOWN_CTRL 26       // binary input (pushbutton switch)
#define PIN_UP_CTRL 27         // binary input (pushbutton switch)
#define PIN_SEL_CTRL 28        // analog input (pushbutton switch)
#define PIN_KILL_SENSE 4       // binary input
#define PIN_HAZARD_LED 25
#define PIN_UART_1_RX 2  // used for nextion LCD
#define PIN_UART_1_TX 3  // used for nextion LCD

/******************************************/
/* pin definitions for motor controllers  */
/******************************************/
#define PIN_MOTOR_L_REGEN_EN 50  // binary output
#define PIN_MOTOR_L_REGEN 36     // pwm output (0% - 100%)
#define PIN_MOTOR_L_THROTTLE 35  // pwm output (0% - 100%)
#define PIN_MOTOR_L_ECO_EN 49    // binary output
#define PIN_MOTOR_L_METER A11    // analog input
#define PIN_MOTOR_L_FWD_EN 52    // binary output
#define PIN_MOTOR_L_REV_EN 51    // binary output

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
#define PIN_HORN 55         // binary output
#define PIN_HEADLIGHTS 56   // binary output
#define PIN_BRAKELIGHTS 41  // binary output
#define PIN_BLINKERS 40     // binary output

#endif  // __PINDEF_H__
