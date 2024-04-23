/*******************************************************************************
 * Title: Assignment 8: Sensors
 *******************************************************************************
 * Program Details:
 *  The purpose of this program is to get familiar with integrating sensors
 * with the PIC. 
 * The program starts out on power up awaiting for a pass-code to be entered
 * from the user using the Photo-resistors. PR1 is the left hand side one, 
 * and PR2 is the right hand side one. Once the pass-code has been entered the
 * system is enabled. An incorrect guess will results in the buzzer activating,
 * while a correct guess will activate the relay which is attached to a DC 
 * motor. If the button is pressed at anytime, the interrupt will be triggered
 * and a melody will be played on the buzzer.
 * 
 * Inputs: 
 *      Two Photo-resistors: RB0, RB1
 *      One Button: RB2 (Interrupt)
 * Outputs: 
 *      Seven segment display: RD0-RD7
 *      Two LEDS: RA1 and RA2
 *      One Buzzer: RA0
 *      One Relay: RA3
 * Setup: C- Simulator
 * Date: April 22nd, 2024
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File as well as the calcFunctions header file
 * Compiler: xc8, 2.46
 * OS and Computer: HP, Windows 11 Home 64-bit
 * Author: Anthony Meyer Slechta
 * Versions:
 *      V1.0: Original
 * Useful links:  
 *      Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
 *      PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
 *******************************************************************************/

/**************************
 * Headers
 **************************/
#include <xc.h>
#include "cnfg.h"
#include "functions.h"
//#include "initialization.h"
#include "C:/Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"



 
void main(void) {

    // Initializes interrupts and all ports
    //When system is on SYS_LED should be on
    initializeALL();
    setSecretCode();
    
    while(1){
        getUserGuess();
        handleGuess();   
    }

}

