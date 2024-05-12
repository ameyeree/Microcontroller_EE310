

#include <xc.h>
#include "config.h"
#include <stdio.h>
#define _XTAL_FREQ 4000000
#define FCY _XTAL_FREQ/4
#include "LCD.h"
#include "keypad.h"
#include "interrupt.h"
#include "servo.h"

int userGuess = 0;
int pword;
char d[10];
void main(void) {
    OSCSTATbits.HFOR =1; // enable  HFINTOSC Oscillator (see clock schematic))
    OSCFRQ=0x02; // 00=1 MHZ, 02=4MHZ internal - see page 106 of data sheet
    

    initializeKeypad();
    LCD_Init();
    INTERRUPT_Initialize(); 
    TMR2_Initialize();
    TMR2_StartTimer();
    PWM_Output_D8_Enable();
    PWM2_Initialize();

    
    LCD_String_xy(1, 0, "Opening Box...");
    openBox();
    for(int i = 0; i < 5 ; i++){
        sprintf(d,"%d",(6 - (i + 1)));
        LCD_String_xy(2, 0, d);
        __delay_ms(1000);
    }
    LCD_String_xy(1, 0, "Closing Box...");
    LCD_String_xy(2,0," ");
    closeBox();
    __delay_ms(1000);
    LCD_String_xy(1, 0, "Box closed...");
    
//    LCD_String_xy(1, 0, "Testing EEPRPOM:");
//    pword = (eepromRead(0) * 100) + eepromRead(2);
//    sprintf(d,"%d",pword);
//    LCD_String_xy(2, 0, d);
//    __delay_ms(3000);
//    
//    LCD_String_xy(1,0,"Testing Keypad: ");
//    userGuess = getValue();
//    
//    LCD_Clear();
//    LCD_String_xy(1,0,"User Guess: ");
//    sprintf(d,"%d",userGuess);
//    LCD_String_xy(2,0,d);
    while(1);
    return;
}
