/* 
 * File:   user_cfg.h
 * Author: Pc
 *
 * Created on 17 November 2020, 20:58
 */

#ifndef USER_CFG_H
#define	USER_CFG_H

//#define CURRENT_REGULATOR_TEST
#define SPEED_REGULATOR_TEST

typedef enum
{
    CURRENT_REGULATOR = 0x00,
    SPEED_REGULATOR   = 0x01
}e_RegulatorTypes;

#define OUTPUT_MAX_VALUE      (1023)
#define OUTPUT_MIN_VALUE      (-1023)

//DC machine parameters.
#define NOMINAL_VOLTAGE           (260u)
#define NOMINAL_CURRENT           (17.6f)
#define NOMINAL_ANGULAR_FREQUENCY (353)
#define NOMINAL_SPEED             (3370.0f)
#define NOMINAL_FLUX              (0.7f)
#define ARMATURE_RESISTANCE       (0.75f)
#define ARMATURE_INDUCTANCE       (0.00332f)
#define MOMENT_OF_INERTIA         (0.02f)
#define ELECTRICAL_TIME_CONSTANT  (0.00443f)
#define NOMINAL_DC_BUS_VOLTAGE    (600)


#define INVERTOR_AMPLIFICATION      (2*NOMINAL_DC_BUS_VOLTAGE)                       //Kch
#define DIRECT_BRANCH_AMPLIFICATION ((INVERTOR_AMPLIFICATION*1)/ARMATURE_RESISTANCE) //K
#define RESPONSE_SPEED              (1000u)                                          //lambda

//Current PI regulator.
#define CURRENT_LOOP_TIME_CONSTANT  (0.000005f)                                      //Ti
#define CURRENT_PROPORTIONAL_GAIN   (0.002344f)  //(77)    //1.15 format       (0.002344f)          //Kpi
#define CUREENT_INTEGRAL_GAIN       (0.0002465f) //(8)     //1.15 format       (0.0002465f)         //Kii

//Speed PI regulator.
#define SPEED_LOOP_TIME_CONSTANT    (0.005f)                                          //Tw
#define SPEED_PROPORTIONAL_GAIN     (2.32f)//(8191)  //3.13 format       (2.32f)               //Kpw
#define SPEED_INTEGRAL_GAIN         (0.4f) //(13106) //1.15 format       (0.4f)                //Kiw

#endif	/* USER_CFG_H */

