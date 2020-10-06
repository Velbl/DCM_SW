/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/
/** Phase Locked Loop macros.
 *  Usage of this macros need to follow Oscillator mode configuration.
 */
  /*Select wanted MIPS configuration.*/
  #define NO_PLL (1)                    //2MIPS (milion instructions per second)
  //#define PLL4   (4)                    //8MIPS (milion instructions per second)
  //#define PLL16  (16)                  //32MIPS (milion instructions per second)


/** Oscillator frequency 
 *FOSC = Frequency of one instruction cycle.
 */
#define FOSC      8000000L                //8MHz crystal clock


/** FCY = Frequency of one instruction which take more than one instruction cycle.
 *  In dsPIC30F4011 one instruction takes four instruction cycles.
 *  FCY = (SYS_FREQ * PLL MULTIPLIER)/(PROGRAMMABLE POSTSCALER * 4) 
 */
#ifdef NO_PLL
  #define FCY       ((FOSC * NO_PLL) / 4) //2MIPS (milion instructions per second)
#elif  PLL4
  #define FCY       ((FOSC * PLL4) / 4)   //8MIPS (milion instructions per second)
#elif  PLL16
  #define FCY       ((FOSC * PLL16) / 4)  //32MIPS (milion instructions per second)
#endif
  

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration functions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */
