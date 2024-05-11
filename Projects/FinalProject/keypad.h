/* 
 * File:   calcFunctions.h
 * Author: Anthony Meyer Slechta
 *
 * Created on April 2, 2024, 5:19 PM
 */
//#include "LCD.h"
void initializeKeypad();
void initializePORTA();
void initializePORTC();
int getKeypad();
int getValue();
//#define _XTAL_FREQ 4000000
//#define FCY _XTAL_FREQ/4


//Initialization Functions
void initializeKeypad(){
    initializePORTA();
    initializePORTC();
}
void initializePORTA(){
    PORTA = 0;
    LATA = 0;
    ANSELA = 0;
    TRISA = 0b11111000;
}
void initializePORTC(){
    //Only using RD2 as an input for keypad
    PORTCbits.RC4 = 0;
    LATCbits.LATC4 = 0;
    ANSELCbits.ANSELC4 = 0;
    TRISCbits.TRISC4 = 1;
}

//Gets the Key Pad value from the user and returns the value
// If no value selected, returns -1 to denote nothing given.
int getKeypad(){
    int buttonNumber = -1;
    
    //Check Column 1
    PORTAbits.RA0 = 1;
    if(PORTAbits.RA4 == 1){
        //Shows button pressed is = 1
        buttonNumber = 1;
        while(PORTAbits.RA4 == 1);
    }
    if(PORTAbits.RA5 == 1){
        //Shows button pressed is = 4
        buttonNumber = 4;
        while(PORTAbits.RA5 == 1);
    }
    if(PORTAbits.RA3 == 1){
        //Shows button pressed is = 7
        buttonNumber = 7;
        while(PORTAbits.RA3 == 1);
    }
    if(PORTCbits.RC4 == 1){
        //Shows button pressed is = *
        buttonNumber = 15;
        while(PORTCbits.RC4 == 1);
    }
    PORTAbits.RA0 = 0;
    
    //Check Column 2
    PORTAbits.RA1 = 1;
    if(PORTAbits.RA4 == 1){
        //Shows button pressed is = 2
        buttonNumber = 2;
        while(PORTAbits.RA4 == 1);
    }
    if(PORTAbits.RA5 == 1){
        //Shows button pressed is = 5
        buttonNumber = 5;
        while(PORTAbits.RA5 == 1);
    }
    if(PORTAbits.RA3 == 1){
        //Shows button pressed is = 8
        buttonNumber = 8;
        while(PORTAbits.RA3 == 1);
    }
    if(PORTCbits.RC4 == 1){
        //Shows button pressed is = 0
        buttonNumber = 0;
        while(PORTCbits.RC4 == 1);
    }   
    PORTAbits.RA1 = 0;
    
    //Check Column 3
    PORTAbits.RA2 = 1;
    if(PORTAbits.RA4 == 1){
        //Shows button pressed is = 3
        buttonNumber = 3;
        while(PORTAbits.RA4 == 1);
    }
    if(PORTAbits.RA5 == 1){
        //Shows button pressed is = 6
        buttonNumber = 6;
        while(PORTAbits.RA5 == 1);
    }
    if(PORTAbits.RA3 == 1){
        //Shows button pressed is = 9
        buttonNumber = 9;
        while(PORTAbits.RA3 == 1);
    }
    if(PORTCbits.RC4 == 1){
        //Shows button pressed is = #
        buttonNumber = 10;
        while(PORTCbits.RC4 == 1);
    }      
    PORTAbits.RA2 = 0;
    
    //Check Column 4 
    /* NOT USING COLUMN 4 */
//    PORTAbits.RA3 = 1;
//    if(PORTAbits.RA4 == 1){
//        //Shows button pressed is = +
//        buttonNumber = 11;
//        while(PORTAbits.RA4 == 1);
//    }
//    if(PORTAbits.RA5 == 1){
//        //Shows button pressed is = -
//        buttonNumber = 12;
//        while(PORTAbits.RA5 == 1);
//    }
//    if(PORTBbits.RB0 == 1){
//        //Shows button pressed is = *
//        buttonNumber = 13;
//        while(PORTBbits.RB0 == 1);
//    }
//    if(PORTBbits.RB1 == 1){
//        //Shows button pressed is = /
//        buttonNumber = 14;
//        while(PORTBbits.RB1 == 1);
//    }      
//    PORTAbits.RA3 = 0;
    
    return buttonNumber;
}
// Gets 4 digit pin number, returns in hex form 0x1234 for code 1, 2, 3, 4
int getValue(){
    int value1 = -1;
    int value2 = -1;
    int value3 = -1;
    int value4 = -1;
    int decimalValue = 0;
    char data[10];
    
    LCD_String_xy(2,0,"____");
    
    while(value1 < 0 || value1 > 9){
            value1 = getKeypad();
    }
    sprintf(data,"%d",value1);
    LCD_String_xy(2,0,data);
    
    while(value2 < 0 || value2 > 9){
        value2 = getKeypad();
    }
    sprintf(data,"%d",value2);
    LCD_String_xy(2,1,data);
    
    while(value3 < 0 || value3 > 9){
        value3 = getKeypad();
    }
    sprintf(data,"%d",value3);
    LCD_String_xy(2,2,data);
    
    while(value4 < 0 || value4 > 9){
        value4 = getKeypad();
    }
    sprintf(data,"%d",value4);
    LCD_String_xy(2,3,data);
    
    decimalValue = (value1 * 1000)+(value2 * 100)+(value3 * 10) + (value4);
    return decimalValue;
}




