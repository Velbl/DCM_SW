/** Device header file 
 * The MPLAB X IDE generates a Makefile which controls the build process.
 * The C compiler will generate a pre-defined macro __XC16__ and also
  __dsPIC30F4011__. 
 */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */

#include "system.h"          /* variables/params used by system.c             */
