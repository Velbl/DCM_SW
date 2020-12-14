#ifndef DCM_USER
#define DCM_USER

#include "user_cfg.h"

/// Informations about system state.
typedef enum
{
  /// System is not initialized.
  NOT_INITIALIZED = 0x00,
  /// System is initialized, ready for work.
  INIT_STATE      = 0x01,
  /// System is in idle state.
  /// All wanted values from PI regulators (speed and current) are printed on terminal.
  IDLE_STATE      = 0x02
}tStates;

/// Informations about valuesSensor values.
typedef struct 
{
  int  Current;    //Measured armature current. (measured from solder probe)                       
  int  VoltageDC;  //Measured DC voltage.
  int  Speed;      //Measured speed. (electromotive force in our case) 
}t_MeasuredValues;

/// Informations about current PI regulator.
typedef struct
{
  int       ReferentCurrent; /// Referent current (speed PI regulator output).
  int       MeasuredCurrent; /// Measured current (measured value from solder probe).
  int       Error;           /// Deference between referent and measured current.
  int       Output;          /// Calculated current PI regulator output.
  int       Kpi;             /// Proportional amplification of current loop.
  int       Kii;             /// Integral amplification of current loop.
  int       MaxOutput;       /// Maximal current output limit.
  int       MinOutput;       /// Minimal current output limit.
}t_CurrentReg;

/// Informations about speed PI regulator.
typedef struct
{
  int       ReferentSpeed; /// Referent speed (user defined speed).
  int       MeasuredSpeed; /// Measured speed (measured electromotive force, no encoder available)
  int       Error;         /// Deference between referent and measured speed.
  int       Output;        /// Calculated speed PI regulator output.
  int       Kpw;           /// Proportional amplification of speed loop.
  int       Kiw;           /// Integral amplification of speed loop.
  int       MaxOutput;     /// Maximal speed output limit.
  int       MinOutput;     /// Minimal speed output limit.
}t_SpeedReg;

/// Structure holding informations about all PI regulators.
typedef struct
{
  /// Informations about current PI regulator.
  t_CurrentReg s_CurrentReg;
  /// Informations about speed PI regulator.
  t_SpeedReg   s_SpeedReg;
}t_PIRegulatorData;

/// Configuration of all dsPIC30F4011 peripherals.
void dsPIC30F4011_v_Config(void); 

/// Function which perform system init state actions.
/// Set state to INIT_STATE.
/// Call dsPIC30F4011_v_Init() function, for peripherals initialization.
void v_InitState(void);
  
/// Function which perform system idle state actions.
/// Set state to IDLE_STATE.
/// Write all PI regulators values on terminal.
void v_IdleState(void);

/// Function which perform calculations for wanted PI regulator.
/// Based on measured value and referent value,
/// error value is calculated.
/// Based on: error value, calculated proportional amplification parameter 
/// and integral amplification parameter, wanted PI regulator output is calculated.
void v_CalculatePIRegOutput(e_RegulatorTypes RegulatorType);

/// Set referent current value in amperes [A].
/// Insert value from -18A (- nominal current) to 18A (nominal current).
/// Real current referent value is calculated speed output from speed PI regulator.
void f_SetReferentCurrent(int SpeedOutput);                            

/// Set referent speed value in round per minutes [rpm].
/// Insert value from 0rpm to 3370rpm (nominal speed).
/// Reference speed value is set by potentiometer.
void f_SetReferentSpeed(int ReferentSpeed);

/// Function which perform transformation of measured value from floating point to proper fixed point format. 
/// For 1.15 fixed point format, measured input from 0 to 1023 is scaled from 0 to 32768.
/// For 3.13 fixed point format, measured input from 0 to 1023 is scaled from 0 to 8192.
int i_ConvertToFixedPoint(int MeasuredValue, e_FormatTypes FormatType);

/// Setter function which update active state.
void SetState(tStates NewState);

/// Getter function which return updated active state.
tStates GetState(void);

/// Timer 1 interrupt, function definition.
void _ISRFAST _T1Interrupt(void); 

/// Timer 2 interrupt, function definition.
void _ISRFAST _T2Interrupt(void); 

/// PWM interrupt, function definition.
void _ISRFAST _PWMInterrupt(void);

#endif //DCM_USER



