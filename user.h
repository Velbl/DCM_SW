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

/* Timer interrupts */
void _ISRFAST _T1Interrupt(void); 
void _ISRFAST _T2Interrupt(void); 
void _ISRFAST _T3Interrupt(void); 

 



