#ifndef DCM_USER
#define DCM_USER

#include "system.h"

#define FPWM           20000                  //PWM frequency 20KHz
/*PWM_PERIOD = FCY/(2*FPWM) - 1*/
#define PWM_PERIOD          ((FCY/FPWM)*0.5-1)  //49-199-799, depends on configured FCY
/*Mask for register configuratiion*/
#define MASK(x)   ((uint16_t)(1<<x))
/*Global functions definitions*/
void dsPIC30F4011_v_Init   (void); 

/*Interrupts definitions*/
void _ISRFAST _T1Interrupt(void); 
void _ISRFAST _T2Interrupt(void); 
void _ISRFAST _PWMInterrupt(void);
#endif //DCM_USER



