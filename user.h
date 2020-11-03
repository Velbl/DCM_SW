#ifndef DCM_USER
#define DCM_USER

#include "system.h"

//Macro function for setting wanted bit in register.
#define MASK(x)   ((uint16_t)(1<<x))
//PWM frequency
#define FPWM           (20000u)                  //PWM frequency 20KHz
//PWM_PERIOD = FCY/(2*FPWM) - 1
#define PWM_PERIOD          ((FCY/FPWM)*0.5-1)   //49-199-799, depends on configured FCY
//Max possible number of current measurement values.
#define CURRENT_MEASUREMENT_RESOLUTION
//Period after wich we are sending data on UART.
#define UART_SEND_TIME   ()                 //1s
//Period for setting the reference speed.
#define SPEED_START_TIME ()                 //2s
//Period for setting reference speed to zero.
#define SPEED_STOP_TIME  ()                 //10s
//Reference speed.
//1.15 format ->3277 = 10%  
#define SPEED_REF        (6553u) 




typedef struct 
{
  uint16_t  u_Current;
  uint16_t  u_VoltageDC;
  uint16_t  u_Speed;
  uint16_t  u_DutyCycle1;
  uint16_t  u_DutyCycle2;
}t_SensorValues;

extern t_SensorValues SensorValues;

/*Global functions definitions*/
void dsPIC30F4011_v_Init   (void); 

/*Interrupts definitions*/
void _ISRFAST _T1Interrupt(void); 
void _ISRFAST _T2Interrupt(void); 
void _ISRFAST _PWMInterrupt(void);
void _ISRFAST _ADCInterrupt(void);
void _ISRFAST _U1TXInterrupt(void);

#endif //DCM_USER



