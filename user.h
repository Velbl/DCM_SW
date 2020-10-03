/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

/* I/O and Peripheral Initialization */
void InitApp(void); 
/* Timer 1 interrupt */
void _ISRFAST _T1Interrupt(void); 


/*Specification of function which will be using reserved interrupt vector 48.
 * Vector table found in dsPIC30F4011 data sheet - Interrupt Priority Section 5.1.
 */
void __attribute__((interrupt(auto_psv,irq(52)))) MyIRQ(void); //My interrupt request
 



