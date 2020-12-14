/* 
 * File:   user_cfg.h
 * Author: Stefan Velbl
 *
 * Created on 17 November 2020, 20:58
 */

#ifndef USER_CFG_H
#define	USER_CFG_H

/// Informations about different regulator types.
typedef enum
{
  /// Current PI regulator.
  CURRENT_REGULATOR = 0x00,
  /// Speed PI regulator.
  SPEED_REGULATOR   = 0x01
}e_RegulatorTypes;

/// Informations about different fixed point format types.
typedef enum
{
  /// 1.15 fixed point format.
  FORMAT_1_15 = 0x00,
  /// 3.13 fixed point format.
  FORMAT_3_13 = 0x01
}e_FormatTypes;

/*****************************PROGRAM MODES************************************/

// Scenario in which output of the speed regulator will be reference for the 
// current regulator.
// Speed and current PI regulator values will be printed on terminal.
//#define REAL_SCENARIO

// Test environment for the current PI regulator.
// Current PI regulator values will be printed on terminal.
#define CURRENT_REG

// Test environment for the speed PI regulator.
// Speed PI regulator values will be printed on terminal.
//#define SPEED_REG

/******************************************************************************/

// Defined PWM frequency.
#define FPWM                      (20000u)                  // PWM frequency is 20KHz.

// Defined PWM_PERIOD.
#define PWM_PERIOD                (0x02E2)                  // ((FCY/FPWM)*0.5-1) = 49-199-799, depends on configured FCY

// Macro used as parameter in delay function.
#define ONE_SECOND                (1000)

// Macros defined for setting wanted number of bits for shifting.
#define SHIFT_AMOUNT_1_15         (15)                      // 2^15 = 32,768 -> Base value.
#define SHIFT_AMOUNT_3_13         (13)                      // 2^13 = 8,192 -> Base value.

// Maximal and minimal ADC value.
#define OUTPUT_MAX_VALUE          (1023)
#define OUTPUT_MIN_VALUE          (-1023)

// Maximal and minimal current value.
#define MINIMAL_CURRENT           (-18)
#define MAXIMAL_CURRENT           (18)

// Maximal and minimal speed value.
#define MINIMAL_SPEED             (-3370)
#define MAXIMAL_SPEED             (3370)

// Maximal and minimal base value for 1.15 fixed point format.
#define PII_REG_MAX_OUTPUT        (32768)
#define PII_REG_MIN_OUTPUT        (-32768)

// Maximal and minimal base value for 3.13 fixed point format.
#define PIW_REG_MAX_OUTPUT        (8192)
#define PIW_REG_MIN_OUTPUT        (-8192)

//Current PI regulator parameters.
#define REFERENT_CURRENT            (16384) //1.15 format       (50%)                //RefCurrent
#define CURRENT_PROPORTIONAL_GAIN   (67)    //1.15 format       (0.0021f)            //Kpi
#define CUREENT_INTEGRAL_GAIN       (8)     //1.15 format       (0.00024592f)        //Kii

//Speed PI regulator parameters.
#define REFERENT_SPEED              (3277)  //3.13 format       (40%)                //RefSpeed
#define SPEED_PROPORTIONAL_GAIN     (10813) //3.13 format       (2.32f)              //Kpw
#define SPEED_INTEGRAL_GAIN         (3277)  //3.13 format       (0.4f)               //Kiw

#endif	/* USER_CFG_H */

