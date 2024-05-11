

#include <xc.h>
#include "config.h"
#include <stdio.h>
#define _XTAL_FREQ 4000000
#define FCY _XTAL_FREQ/4
#include "LCD.h"
#include "keypad.h"
#include "interrupt.h"
int userGuess = 0;
//int password;
void main(void) {
    initializeKeypad();
    LCD_Init();
    INTERRUPT_Initialize();
    
    LCD_String_xy(1,0,"Testing Keypad: ");
    userGuess = getValue();
    
    while(1);
    return;
}
