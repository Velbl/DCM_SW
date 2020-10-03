/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* TODO Define system operating frequency */

/** The system clock source (SYS_FREQ)
 * The Internal Fast RC (FRC) oscillator provides a nominal 7.37 MHZ clock.
 */
#define SYS_FREQ        7370000L     // 7.37MHz 

/** Microcontroller MIPs (FCY)
 * MIPS (FCY) and system clock source relationship:
 * FCY = (SYS_FREQ * PLL MULTIPLIER)/(PROGRAMMABLE POSTSCALER * 4) 
 */
#define FCY             SYS_FREQ/4   //1.8425MHz

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */

