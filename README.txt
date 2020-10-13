How to configure wanted instruction frequency (MIPS/FCY)?
-Open system.h file and uncomment the definition for wanted oscillator mode configuration.
How to secure non-atomic instructions from all interrupts?
-You can use INTERRUPT_PROTECT(x) function, defined in system.h file. 