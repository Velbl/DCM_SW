/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/
/* Oscillator frequency */
//FOSC = Frequency of one instruction cycle.
#define FOSC      8000000L              //8MHz cristal clock
/**
 * FCY = Frequency of one instruction which take more than one instruction cycle.
 * In dsPIC30F4011 one instruction takes four instruction cycles.
 * FCY = (SYS_FREQ * PLL MULTIPLIER)/(PROGRAMMABLE POSTSCALER * 4) 
 */
#define FCY       ((SYS_FREQ * 1) / 4)  //2MIPS (milion instructions per second)
  
/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration functions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */
