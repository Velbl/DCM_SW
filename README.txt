How can I download and open this project?
-Project is created in MPLAB X IDE v5.40 so you need to download it from: https://www.microchip.com/mplab/mplab-x-ide
-Also you will need XC-16 compiler, download it from: https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers
-In order to create local workspace on your computer you need to:
  -Download Git from: https://git-scm.com/downloads
  -Position in wanted folder
  -Right click inside the wanted folder and then choose Git bash option from drop menu
  -After that you should be able to see comand prompt
  -Write instuction: "git clone https://github.com/Velbl/DCM_SW.git" and press enter
-After this you should be able to start the project on your computer.

How files are organized?
-Each peripheral configuration is created in a separate file followed by its name.
-In the user file are created all functions needed by the user.
-Main program is placed in <main.c> where are called all the user functions and peripheral functions that are visible to the user (global functions).
-Minimal use of global variables and functions.

How to configure wanted instruction frequency (MIPS/FCY)?
-Open system.h file and uncomment the definition for wanted oscillator mode configuration.

How to secure non-atomic instructions from all interrupts?
-You can use INTERRUPT_PROTECT(x) function, defined in the system.h file. 