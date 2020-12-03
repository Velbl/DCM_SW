/* 
 * File:   user_cfg.h
 * Author: Pc
 *
 * Created on 17 November 2020, 20:58
 */

#ifndef USER_CFG_H
#define	USER_CFG_H

// TEST SCENARIOS
//#define CURRENT_REGULATOR_TEST
//#define SPEED_REGULATOR_TEST

//PWM frequency
#define FPWM                      (20000u)                  //PWM frequency 20KHz

//PWM_PERIOD = FCY/(2*FPWM) - 1
#define PWM_PERIOD                (0x02E2)                     //((FCY/FPWM)*0.5-1)        //49-199-799, depends on configured FCY
#define SIX_SECONDS               (6000)
#define ONE_SECOND                (1000)
#define NUMBER_OF_MEASUREMENTS    (400)

#define SHIFT_AMOUNT_1_15         (15)                      // 2^15 = 32768 -> Base value.
#define SHIFT_AMOUNT_1_13         (13)                      // 2^13 = 8,192 -> Base value.

#define OUTPUT_MAX_VALUE          (1023)
#define OUTPUT_MIN_VALUE          (-1023)

#define MINIMAL_CURRENT           (-18)
#define MAXIMAL_CURRENT           (18)

#define MINIMAL_SPEED             (-3370)
#define MAXIMAL_SPEED             (3370)

#define PII_REG_MAX_OUTPUT        (32768)
#define PII_REG_MIN_OUTPUT        (-32768)

#define PIW_REG_MAX_OUTPUT        (8192)
#define PIW_REG_MIN_OUTPUT        (-8192)

//DC machine parameters.
#define NOMINAL_VOLTAGE           (260u)
#define NOMINAL_CURRENT           (17.6f)
#define NOMINAL_ANGULAR_FREQUENCY (353u)
#define NOMINAL_SPEED             (3370)
#define NOMINAL_FLUX              (0.7f)
#define ARMATURE_RESISTANCE       (0.75f)
#define ARMATURE_INDUCTANCE       (0.00332f)
#define MOMENT_OF_INERTIA         (0.02f)
#define ELECTRICAL_TIME_CONSTANT  (0.00443f)
#define NOMINAL_DC_BUS_VOLTAGE    (600u)


//#define INVERTOR_AMPLIFICATION      (2*NOMINAL_DC_BUS_VOLTAGE)                       //Kch
//#define DIRECT_BRANCH_AMPLIFICATION ((INVERTOR_AMPLIFICATION*1)/ARMATURE_RESISTANCE) //K
//#define RESPONSE_SPEED              (1000u)                                          //lambda
//#define CURRENT_LOOP_TIME_CONSTANT  (0.000005f)                                      //Ti 
//#define SPEED_LOOP_TIME_CONSTANT    (0.005f)                                         //Tw

//Current PI regulator.
#define REFERENT_CURRENT            (16384) //1.15 format       (50%)                  //RefCurrent
#define CURRENT_PROPORTIONAL_GAIN   (67)    //1.15 format       (0.0021f)            //Kpi
#define CUREENT_INTEGRAL_GAIN       (8)     //1.15 format       (0.00024592f)            //Kii

//Speed PI regulator.
#define REFERENT_SPEED              (3277)  //3.13 format       (40%)                  //RefSpeed
#define SPEED_PROPORTIONAL_GAIN     (10813) //3.13 format       (2.32f)                //Kpw
#define SPEED_INTEGRAL_GAIN         (3277)  //3.13 format       (0.4f)                 //Kiw

typedef enum
{
  CURRENT_REGULATOR = 0x00,
  SPEED_REGULATOR   = 0x01
}e_RegulatorTypes;

typedef enum
{
  FORMAT_1_15 = 0x00,
  FORMAT_3_13 = 0x01
}e_FormatTypes;

#endif	/* USER_CFG_H */

