#ifndef DCM_USER
#define DCM_USER

#include "system.h"
#include "user_cfg.h"

//Macro function for setting wanted bit in register.
#define MASK(x)                ((uint16_t)(1<<x))

//PWM frequency
#define FPWM                   (20000u)                  //PWM frequency 20KHz

//PWM_PERIOD = FCY/(2*FPWM) - 1
#define PWM_PERIOD             ((FCY/FPWM)*0.5-1)        //49-199-799, depends on configured FCY

#define NUMBER_OF_COUNTS       (20000u)

//DC machine informations.
typedef struct
{
  float    f_Ra;          //Armature resistance.
  float    f_La;          //Armature inductance.
  float    f_Ta;          //Time constant of the electrical module.
  float    f_F;           //Flux.
  float    f_J;           //Moment of inertia.
  uint16_t u_wn;          //Nominal angular frequency in rad/s.
  uint16_t u_nn;          //Nominal angular speed in rpm.
  uint16_t u_Udc;         //Nominal DC bus voltage.
  uint16_t u_Uan;         //Nominal armature voltage.
  uint8_t  u_Ian;         //Nominal armature current.
  uint8_t  u_Reserved[3]; //Reserved bytes for proper memory alignment.
}t_DCMInfo;

//Sensor values.
typedef struct 
{
  float  Current;    //Measured armature current.
  float  VoltageDC;  //Measured DC voltage.
  float  Speed;      //Measured speed. (electromotive force in our case)
}t_MeasuredValues;

//Current PI regulator parameters.
typedef struct
{
  float       ReferentCurrent;
  float       MeasuredCurrent;
  float       Error;
  float       Output;
  float       Kpi;         //Proportional amplification of current loop.
  float       Kii;         //Integral amplification of current loop.
  int         MaxOutput;
  int         MinOutput;
}t_CurrentReg;

//Speed PI regulator parameters.
typedef struct
{
  float       ReferentSpeed;
  float       MeasuredSpeed;
  float       Error;
  float       Output;
  float       Kpw;        //Proportional amplification of speed loop.
  float       Kiw;        //Integral amplification of speed loop.
  int         MaxOutput;
  int         MinOutput;
}t_SpeedReg;

//PI regulators.
typedef struct
{
  t_CurrentReg s_CurrentReg;
  t_SpeedReg   s_SpeedReg;
}t_PIRegulatorData;

//Informations about direct current machine.
extern t_DCMInfo          DCMInfo;

//Measurement and reference values.
extern t_MeasuredValues   Measured;

//PI regulators parameters.
extern t_PIRegulatorData  PIReg;

//extern uint16_t a_ADCBuffers[NUMBER_OF_USED_BUFFERS];

//Initialization of all dsPIC30F4011 peripherals..
void dsPIC30F4011_v_Init(void); 

//Initialization of DC machine parameters.
void DCM_v_Init(void);

//Initialization of sensor parameters.
void Sensor_v_Init(void);

//Init of PI regulators.
void PIReg_v_Init(void);

//Calculation of output for wanted PI regulator.
void v_CalculatePIRegOutput(e_RegulatorTypes RegulatorType);

//Set referent current value in Amps.
float f_SetReferentCurrent(float f_ReferentCurrent);

//Set referent speed value in round per minutes.
float f_SetReferentSpeed(float ReferentSpeed);

//Interface for getting referent value.
uint16_t u_GetReferentValue(void);

//Interrupts definitions.
void _ISRFAST _T1Interrupt(void); 
void _ISRFAST _T2Interrupt(void); 
void _ISRFAST _PWMInterrupt(void);
void _ISRFAST _U1TXInterrupt(void);

#endif //DCM_USER



