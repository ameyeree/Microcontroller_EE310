/*
 * ---------------------
 * Title: Simple LED Blinking 
 * ---------------------
 * Program Details:
 *  The purpose of this program is to simply blink an LED 
 * Inputs: myDelay 
 * Outputs: RD0 (output)
 * Setup: C- Simulator
 * Date: Feb 24, 2023
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File 
 * Compiler: xc8, 2.4
 * Author: Farid Farahmand
 * Versions:
 *      V1.0: Original
 * Useful links:  
 *      Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
 *      PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
 *      List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 
 */

#include <xc.h> // must have this
//#include "../../../../../Program Files/Microchip/xc8/v2.40/pic/include/proc/pic18f46k42.h"
#include "C:/Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"
//#include "C:\Program Files\Microchip\xc8\v2.46\pic\include\proc\pic18f46k42.h"
#define _XTAL_FREQ 4000000
#define FCY _XTAL_FREQ/4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initializePORTB();
void initializePORTD();

unsigned char sevenSegValues[] = {0x3F,0x06};

void main(void) {
    initializePORTB();
    initializePORTD();
    while(1){
        PORTB = 0b00000010;
        PORTD = sevenSegValues[0];
        __delay_ms(1);
        PORTB = 0b00000001;
        PORTD = sevenSegValues[1];
        __delay_ms(1);
    }
    return;
}
void initializePORTB(){
    TRISB = 0;
    PORTB = 0;
    LATB = 0;
    ANSELB = 0;
}

void initializePORTD(){
    TRISD = 0;
    PORTD = 0;
    LATD = 0;
    ANSELD = 0;
}