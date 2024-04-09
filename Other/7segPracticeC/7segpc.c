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
#include "header.h"
#include "C:/Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"
//Below is for delay
#define _XTAL_FREQ 4000000
#define FCY _XTAL_FREQ/4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initializePORTA();
void initializePORTB();
void initializePORTC();
void initializePORTD();
unsigned char sevenSegValues[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x00};
void updatedSevenSeg();
void lhsSevenSeg(int index);
void rhsSevenSeg(int index);
int getKeypad();

void main(void) {
    initializePORTA();
    initializePORTB();
    initializePORTC();
    initializePORTD();
    int value1 = -1;
    int value2 = -1;
    int convertedFirstNumber = 0;
//    while(value1 < 0 && value1 > 9){
//        value1 = getKeypad();
//    }
    while(1){
        value1 = getKeypad();
        rhsSevenSeg(value1);
        __delay_ms(2000);
    }
    
    return;
}
void initializePORTA(){
    PORTA = 0;
    LATA = 0;
    ANSELA = 0;
    TRISA = 0b11110000;
}
void initializePORTB(){
    PORTB = 0;
    LATB = 0;
    ANSELB = 0;
    TRISB = 0b00000011;
}
void initializePORTC(){
    PORTC = 0;
    LATC = 0;
    ANSELC = 0;
    TRISC = 0;
}
void initializePORTD(){
    TRISD = 0;
    PORTD = 0;
    LATD = 0;
    ANSELD = 0;
}
void updatedSevenSeg(int rhsIndex, int lhsIndex){
    rhsSevenSeg(rhsIndex);
    __delay_ms(1);
    lhsSevenSeg(lhsIndex);
    __delay_ms(1);   
}

void rhsSevenSeg(int index){
    if(index < 0 || index > 9){
        index = 10;
    }
    PORTC = 0b00000100;
    PORTD = sevenSegValues[index];  
    __delay_ms(5);
}
void lhsSevenSeg(int index){
    if(index < 0){
        index = 10;
    }
    PORTC = 0b00001000;
    PORTD = sevenSegValues[1];
    __delay_ms(5);
}
int getKeypad(){
    int buttonNumber = -1;
    
    //Check Column 1
    PORTAbits.RA0 = 1;
    if(PORTAbits.RA4 == 1){
        buttonNumber = 1;
        while(PORTAbits.RA4 == 1);
    }
    if(PORTAbits.RA5 == 1){
        buttonNumber = 4;
        while(PORTAbits.RA5 == 1);
    }
    if(PORTBbits.RB0 == 1){
        buttonNumber = 7;
        while(PORTBbits.RB0 == 1);
    }
    PORTAbits.RA0 = 0;
    //Check Column 2
    PORTAbits.RA1 = 1;
    if(PORTAbits.RA4 == 1){
        buttonNumber = 2;
        while(PORTAbits.RA4 == 1);
    }
    if(PORTAbits.RA5 == 1){
        buttonNumber = 5;
        while(PORTAbits.RA5 == 1);
    }
    if(PORTBbits.RB0 == 1){
        buttonNumber = 8;
        while(PORTBbits.RB0 == 1);
    }
    if(PORTBbits.RB1 == 1){
        buttonNumber = 0;
        while(PORTBbits.RB1 == 1);
    }   
    PORTAbits.RA1 = 0;
    //Check Column 3
    PORTAbits.RA2 = 1;
    if(PORTAbits.RA4 == 1){
        buttonNumber = 3;
        while(PORTAbits.RA4 == 1);
    }
    if(PORTAbits.RA5 == 1){
        buttonNumber = 6;
        while(PORTAbits.RA5 == 1);
    }
    if(PORTBbits.RB0 == 1){
        buttonNumber = 9;
        while(PORTBbits.RB0 == 1);
    }
    if(PORTBbits.RB1 == 1){
        buttonNumber = 10;
        while(PORTBbits.RB1 == 1);
    }      
    PORTAbits.RA2 = 0;
    //Check Column 4
    PORTAbits.RA3 = 1;
    if(PORTAbits.RA4 == 1){
        buttonNumber = 11;
        while(PORTAbits.RA4 == 1);
    }
    if(PORTAbits.RA5 == 1){
        buttonNumber = 12;
        while(PORTAbits.RA5 == 1);
    }
    if(PORTBbits.RB0 == 1){
        buttonNumber = 13;
        while(PORTBbits.RB0 == 1);
    }
    if(PORTBbits.RB1 == 1){
        buttonNumber = 14;
        while(PORTBbits.RB1 == 1);
    }      
    PORTAbits.RA3 = 0;
    return buttonNumber;
}