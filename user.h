#ifndef DCM_USER
#define DCM_USER

#include "system.h"

//Macro function for setting wanted bit in register.
#define MASK(x)   ((uint16_t)(1<<x))
//PWM frequency
#define FPWM           (20000u)                  //PWM frequency 20KHz
//PWM_PERIOD = FCY/(2*FPWM) - 1
#define PWM_PERIOD          ((FCY/FPWM)*0.5-1)   //49-199-799, depends on configured FCY

typedef struct 
{
  uint16_t  u_Current;    //Measured armature current.
  uint16_t  u_VoltageDC;  //Measured DC voltage.
  uint16_t  u_Speed;      //Measured speed. (electromotive force in our case)
  uint16_t  u_RefCurrent; //Armature current reference, given by the user.
  uint16_t  u_RefSpeed;   //Reference speed, given bu the user.
}t_SensorValues;

typedef struct
{
  float    f_Ra;       //Armature resistance.
  float    f_La;       //Armature inductance.
  float    f_Ta;       //Time constant of the electrical module.
  float    f_F;        //Flux.
  uint16_t u_wn;       //Nominal angular frequency in rad/s.
  uint16_t u_nn;       //Nominal angular speed in rpm.
  uint8_t  u_Uan;      //Nominal armature voltage.
  uint8_t  u_Ian;      //Nominal armature current.
  uint8_t  u_Udc;      //Nominal DC voltage.
  uint8_t  u_Reserved; //Reserved bit for proper alignment.
}t_DCMInfo;


typedef struct
{
  float    f_K;           //Amplification in the direct branch.
  float    f_Kpi;         //Integral amplification in current loop.
  float    f_Kii;         //Integral amplification in current loop.
  uint16_t u_Ti;          //The period in which the current loop is executed.
  uint16_t u_Reference;   //Current reference.
  uint16_t u_Feedback;    //Current mreasurement.
  uint16_t u_Lambda;      //Defined response speed.
  uint8_t  u_Kch;         //Choper amplification.
  uint8_t  u_Reserved;    //Reserved byte for proper alignment.
}t_CurrentReg;

typedef struct
{
  float f_Tw;           //The period in which the speed loop is executed.
  float f_Ks;           //Parameter needed for calculation of Kpw and Kiw.
  float f_Kpw;          //Proportional amplification in speed loop.
  float f_Kiw;          //Integral amplification in speed loop.
  uint16_t u_Reference; //Speed reference.
  uint16_t u_Feedback;  //Speed measurement.
}t_SpeedReg;

//Measurement and reference values.
extern t_SensorValues SensorValues;

//Informations about direct current machine.
extern t_DCMInfo      DCMInfo;

//Current regulator parameters.
extern t_CurrentReg   CurrentReg;

//Speed regulator parameters.
extern t_SpeedReg     SpeedReg;

//Initialization of all dsPIC30F4011 peripherals..
void dsPIC30F4011_v_Init   (void); 

//Interrupts definitions.
void _ISRFAST _T1Interrupt(void); 
void _ISRFAST _T2Interrupt(void); 
void _ISRFAST _PWMInterrupt(void);
void _ISRFAST _U1TXInterrupt(void);

#endif //DCM_USER



