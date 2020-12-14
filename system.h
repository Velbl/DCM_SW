// Phase Locked Loop macros.
// Usage of this macros need to follow Oscillator mode configuration.

/*************************OSCILATOR CONFIGURATION******************************/ 
//#define   PLL1   (1)                    //2MIPS (milion instructions per second)
  #define   PLL4     (4)                  //8MIPS (milion instructions per second)
//#define   PLL16    (16)                 //32MIPS(milion instructions per second)

//FOSC = Frequency of one instruction cycle.
#define FOSC      8000000L                //8MHz crystal clock.

/// FCY = Frequency of one instruction which take more than one instruction cycle.
/// In dsPIC30F4011 one instruction takes four instruction cycles.
/// FCY = (SYS_FREQ * PLL MULTIPLIER)/(PROGRAMMABLE POSTSCALER * 4) 

#ifdef PLL1
  #define FCY       ((FOSC * PLL1)  / 4 )   //2MIPS (milion instructions per second)
#elif  PLL4
  #define FCY       ((FOSC * PLL4)  / 4 )   //8MIPS (milion instructions per second)
#elif  PLL16
  #define FCY       ((FOSC * PLL16) / 4)    //32MIPS (milion instructions per second)
#endif
#define   TCY       (1/FCY)                 //Instruction clock
/******************************************************************************/

/*******************Sharing Memory problems solution.**************************/

 /**Work flow:
  * Save last SRbits.IPL, set current SRbits.IPL to level 7 which means that
  * CPU will not be interrupted by any interrupt (source) with any programmed 
  * priority level, even with priority 7 interrupts.
  * Then, execute instruction and return previous SRbits.IPL.
 */  
#define INTERRUPT_PROTECT(x)                                                 \
{                                                                            \
  uint8_t saved_ipl;                                                         \
  SET_AND_SAVE_CPU_IPL(saved_ipl,7);                                         \
  x;                                                                         \
  RESTORE_CPU_IPL(saved_ipl);\                                               \
}(void) 0;                                                                     
/******************************************************************************/