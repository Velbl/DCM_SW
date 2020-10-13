/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */

#include "user.h"            /* variables/params used by user.c               */
#include "system.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */
/**
 * This function updates the duty cycle register and updated disable bit.
 */
static void DCMC_v_SetDutyCycle(uint16_t u_PDCx, uint16_t u_DutyCycle, uint8_t u_UpdateDisable)
{
        PWMCON2bits.UDIS = u_UpdateDisable & 0x01;
        *(&PDC1+ u_PDCx - 1) = u_DutyCycle; //Write wanted duty cycle in wanted duty cycle register (PDCx).
}
static void DCMC_v_InterruptCfg(uint8_t u_Config)
{   
    /*Clear the interrupt flags*/
    _PWMIF  = 0;
    _FLTAIF = 0;
    /*Set priority for the period match*/
    _PWMIP = (0x0007 & u_Set);
    /*Set priority for the period match*/
    _FLTAIP = (0x0070 & u_Set)>>4;
    /*Enable/disable of interrupt period match*/
    _PWMIE  = (0x0008 & u_Set)>>3;
    /*Enable/disable of interrupt Fault A*/
    _FLTAIE = (0x0080 & u_Set)>>7;
}
/*I/O PORTS configuration.*/
void Ports_v_Init()
{
    ADPCFG = 0x0008;          //Set RB0,RB1,RB2 as analog pins.
    TRISB  = 0xffff;          //Set all B pins as inputs. 
    TRISE  = 0xffff;          //Set all E pins as inputs.
    TRISF  = 0xfff0;          //Set F0,F1,F2,F3 as outputs.
}
/*TIMERS configuration*/
void Timers_v_Init()
{
    /*Timer 1*/
    T1CONbits.TCS   = 0;      //internal clock FCY
    T1CONbits.TCKPS = 3;      //prescaler 1:256
    PR1   = 782;              //100ms
    _T1IP = 1;                //Timer 1 interrupt priority level = 1
    _T1IF = 0;                //Timer 1 interrupt flag status cleared.
    __write_to_IEC(IEC0bits.T1IE = 1);// proper way to enable/disable wanted interrupt
    T1CONbits.TON   = 1;      //timer 1 on
    /*Timer 2*/
    T2CONbits.TCS   = 0;      //internal clock FCY
    T2CONbits.TCKPS = 3;      //prescaler 1:256
    PR2 = 3906;               //0.5s
    _T2IP = 2;                //Timer 2 interrupt priority level = 2
    _T2IF = 0;                //Timer 1 interrupt flag status cleared.
    __write_to_IEC(IEC0bits.T2IE = 1);// proper way to enable/disable wanted interrupt
    T2CONbits.TON   = 1;      //timer 2 on
    /*Timer 3*/
    T3CONbits.TCS   = 0;      //internal clock FCY
    T3CONbits.TCKPS = 3;      //prescaler 1:256
    PR3 = 7812;               //1s
    _T3IP = 3;                //Timer 3 interrupt priority level = 3
    _T3IF = 0;                //Timer 3 interrupt flag status cleared.
    __write_to_IEC(IEC0bits.T3IE = 1);// proper way to enable/disable wanted interrupt
    T3CONbits.TON   = 1;      //timer 3 on   
}
/*PWM configuration*/
void PWM_v_Init()
{

}
/*ADC configuration*/
void ADC_v_Init()
{
    
}

void dsPIC30F4011_v_Init()
{
    Ports_v_Init();
    Timers_v_Init();
    PWM_v_Init();
    ADC_v_Init();
}

//PR = FCY / (Fwanted * Prescaler) - 1