#ifndef DCM_USER
#define DCM_USER

#include "system.h"
/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/
#define FPWM           20000                  //PWM frequency 20KHz
//PWM_PERIOD = FCY/(2*FPWM) - 1
#define PWM_PERIOD          ((FCY/FPWM)*0.5-1)  //49-199-799, depends on configured FCY
#define SIZE           100                    //number of buffer dots
/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

/* I/O and Peripheral Initialization */
void dsPIC30F4011_v_Init   (void); 
void Ports_v_Init (void);
void Timers_v_Init(void);
void PWM_v_Init   (void);
void ADC_v_Init   (void);

/* Timer interrupts */
void _ISRFAST _T1Interrupt(void); 
void _ISRFAST _T2Interrupt(void); 
void _ISRFAST _T3Interrupt(void); 

#endif //DCM_USER



