/*******************************************************************************
 * Title: Final Project--Electronic Safe
 *******************************************************************************
 * Program Details:
 *  Demonstrates an Electronic Safe utilizing an LCD, servo motor using 
 * PWM, Interrupt using a switch that allows the password to be changed and 
 * saved into EEPROM, and a matrix keypad allowing user inputs.
 * The servo motor will rotate approximately 90 degrees to open the lid to a
 * box.
 * 
 * Inputs: 
 *      Interrupt: RB0
 *      Matrix Keypad: RA0-RA4, RC4
 * Outputs: 
 *      LCD: RD0-RD7, RC2, RC3
 *      PWM: RC5
 * Setup: C- Simulator
 * Date: May 12th, 2024
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
#include "config.h"
#include <stdio.h>
#define _XTAL_FREQ 4000000
#define FCY _XTAL_FREQ/4
#include "LCD.h"
#include "keypad.h"
#include "interrupt.h"
#include "servo.h"
int userGuess = -1;
int pword;
char d[10];


void main(void) {
    //For PWM
    OSCSTATbits.HFOR =1; // enable  HFINTOSC Oscillator (see clock schematic))
    OSCFRQ=0x02; // 00=1 MHZ, 02=4MHZ internal - see page 106 of data sheet
    
    //Initialize all functions
    initializeKeypad();
    LCD_Init();
    INTERRUPT_Initialize(); 
    TMR2_Initialize();
    TMR2_StartTimer();
    PWM_Output_D8_Enable();
    PWM2_Initialize();
    
    //Get password from EEPROM
    pword = (eepromRead(0) * 100) + eepromRead(2);

    //Electronic Safe on
    LCD_String_xy(1, 0, "Electronic Safe");
    LCD_String_xy(2, 0, "  Initializing  ");
    __delay_ms(1000);
    LCD_Clear();
    
    //Confirm if a password has been set
    LCD_String_xy(1, 0, "Checking Passwrd");
    for(int i = 0; i < 16; i++){
        LCD_String_xy(2,i,".");
        __delay_ms(200);
    }
    LCD_Clear();
    //If password is set in EEPROM, let user know they need to set it
    if(pword > 9999){
        LCD_String_xy(1, 0, "Password is not");
        LCD_String_xy(2,0, "set please reset");
        __delay_ms(5000);
    }else{
        //If password is set in EEPROM, let user know
        LCD_String_xy(1, 0, "Password is set");
        __delay_ms(2000);
    }
    
    //Ask for password
    LCD_Clear();
    while(userGuess < 0){
        LCD_String_xy(1,0,"Enter Password: ");
        userGuess = getValue();
        __delay_ms(1000);
        LCD_Clear();
        
        //If password is correct, open box for 7 seconds total, then close box
        if(userGuess == pword){
            LCD_String_xy(1, 0, "Password is");
            LCD_String_xy(2,0, "correct");
            __delay_ms(1500);
            
            //temporarily disable interrupts
            INTCON0bits.GIE = 0;
            openBox();
            LCD_String_xy(1, 0, "Box open");
            __delay_ms(2000);
            
            closeBox();
            LCD_String_xy(1, 0, "Box closed");
            __delay_ms(2000);
            //Re-enable interrupts
            INTCON0bits.GIE = 1;
        }else{
            LCD_String_xy(1, 0, "Password is");
            LCD_String_xy(2,0, "incorrect...");
            __delay_ms(2000);
        }
        
        //Reset userGuess and clear LCD
        LCD_Clear();
        LCD_String_xy(1, 0, "Reinitializing");
        for(int i = 0; i < 16; i++){
            LCD_String_xy(2,i,".");
            __delay_ms(200);
        }
        LCD_Clear();
        userGuess = -1;
    }
    return;
}
