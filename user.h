#ifndef DCM_USER
#define DCM_USER

#include "system.h"
/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/
#define FPWM           20000                  //PWM frequency 20KHz
//PWM_PERIOD = FCY/(2*FPWM) - 1
#define PWM_PERIOD          ((FCY/FPWM)*0.5-1)  //49-199-799, depends on configured FCY

/* TODO Application specific user parameters used in user.c may go here */
/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

/* I/O and Peripheral Initialization */
void dsPIC30F4011_v_Init   (void); 

/* Timer interrupts */
void _ISRFAST _T1Interrupt(void); 
void _ISRFAST _T2Interrupt(void); 
void __attribute__((interrupt)) _PWMInterrupt(void);
#endif //DCM_USER



