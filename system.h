/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* TODO Define system operating frequency */

/* Microcontroller MIPs (FCY) */
/* The Internal Fast RC (FRC) oscillator provides a nominal 7.37 MHZ clock.
 * = Speed of the processor.
 */
#define SYS_FREQ        7370000L
/*Processor speed*/
#define FCY             SYS_FREQ/4 

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */

