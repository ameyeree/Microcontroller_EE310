/* 
 * File:   calcFunctions.h
 * Author: Anthony Meyer Slechta
 *
 * Created on April 2, 2024, 5:19 PM
 */
void initializeAllPorts();
void initializePORTA();
void initializePORTB();
void initializePORTC();
void initializePORTD();
void updateSevenSeg(int lhs, int rhs);
void lhsSevenSeg(int index);
void rhsSevenSeg(int index);
void negSevenSeg(int lhs, int rhs);
int getKeypad();
int getValue(int* operation, int* isEqualSign);
void dispConvNumber(int decimalValue);
int calculateFinalValue(int operation, int valueX, int valueY);
int Addition_Op(int valueX, int valueY);
int Subtraction_Op(int valueX, int valueY);
int Multiplication_Op(int valueX, int valueY);
int Division_Op(int valueX, int valueY);
#define _XTAL_FREQ 4000000
#define FCY _XTAL_FREQ/4

unsigned char sevenSegValues[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x00,0x79};

//Initialization Functions
void initializeAllPorts(){
    initializePORTA();
    initializePORTB();
    initializePORTC();
    initializePORTD();
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
//This updates the seven segment display for two digits
void updateSevenSeg(int lhsIndex, int rhsIndex){
    rhsSevenSeg(rhsIndex);
    __delay_ms(1);
    lhsSevenSeg(lhsIndex);
    __delay_ms(1);   
}
//Updates only the seven segment display on the right hand side
void rhsSevenSeg(int index){
    if(index < 0 || index > 9){
        index = 10;
    }
    PORTC = 0b00000100;
    PORTD = sevenSegValues[index];  
    __delay_ms(5);
}
//Updates only the seven segment display on the right hand side
void lhsSevenSeg(int index){
    if(index < 0){
        index = 10;
    }
    PORTC = 0b00001000;
    PORTD = sevenSegValues[index];
    __delay_ms(5);
}
//This updates the seven segment display when displaying negative values
// RHS 7 seg will display the decimal point to show negative values
void negSevenSeg(int lhs, int rhs){
    if(lhs == 0){
        lhs = 10;
    }
    //LHS
    PORTC = 0b00001000;
    PORTD = sevenSegValues[lhs];
    __delay_ms(5);
    //RHS
    PORTC = 0b00000100;
    PORTD = sevenSegValues[rhs] + 0x80;  
    __delay_ms(5);
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
    if(PORTBbits.RB0 == 1){
        //Shows button pressed is = 7
        buttonNumber = 7;
        while(PORTBbits.RB0 == 1);
    }
    if(PORTBbits.RB1 == 1){
        //Shows button pressed is = *
        buttonNumber = 15;
        while(PORTBbits.RB1 == 1);
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
    if(PORTBbits.RB0 == 1){
        //Shows button pressed is = 8
        buttonNumber = 8;
        while(PORTBbits.RB0 == 1);
    }
    if(PORTBbits.RB1 == 1){
        //Shows button pressed is = 0
        buttonNumber = 0;
        while(PORTBbits.RB1 == 1);
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
    if(PORTBbits.RB0 == 1){
        //Shows button pressed is = 9
        buttonNumber = 9;
        while(PORTBbits.RB0 == 1);
    }
    if(PORTBbits.RB1 == 1){
        //Shows button pressed is = #
        buttonNumber = 10;
        while(PORTBbits.RB1 == 1);
    }      
    PORTAbits.RA2 = 0;
    
    //Check Column 4
    PORTAbits.RA3 = 1;
    if(PORTAbits.RA4 == 1){
        //Shows button pressed is = +
        buttonNumber = 11;
        while(PORTAbits.RA4 == 1);
    }
    if(PORTAbits.RA5 == 1){
        //Shows button pressed is = -
        buttonNumber = 12;
        while(PORTAbits.RA5 == 1);
    }
    if(PORTBbits.RB0 == 1){
        //Shows button pressed is = *
        buttonNumber = 13;
        while(PORTBbits.RB0 == 1);
    }
    if(PORTBbits.RB1 == 1){
        //Shows button pressed is = /
        buttonNumber = 14;
        while(PORTBbits.RB1 == 1);
    }      
    PORTAbits.RA3 = 0;
    
    return buttonNumber;
}
// Gets up to a two digit value given by the user one value at a time. 
// Will also update the operation value 
int getValue(int* operation, int* isEqualSign){
    int value1 = -1;
    int value2 = -1;
    int decimalValue = 0;
    
    updateSevenSeg(value1,value2);
    
    while(value1 < 0 || value1 > 9){
            value1 = getKeypad();
    }
    rhsSevenSeg(value1);
    
    while(value2<0){
        value2 = getKeypad();
    }
    if(*operation < 10){
        //Determine if Value 2 is operator or the second part of decimalValue
        //Handle accordingly
        if(value2 >= 0 && value2 <= 9){
                updateSevenSeg(value1,value2);
                decimalValue = (value1 * 10) + value2;
        }else{
                *operation = value2;
                decimalValue = value1;
                rhsSevenSeg(0);
        }
    }else{
        if(value2 == 10){
            *isEqualSign = value2;
            decimalValue = value1;
            rhsSevenSeg(0);
        }
        while(*isEqualSign != 10){
            if(value2 >= 0 && value2 <= 9){
                updateSevenSeg(value1,value2);
                decimalValue = (value1 * 10) + value2;
            }
            *isEqualSign = getKeypad();
        }

    }
    return decimalValue;
}
//Takes a decimal value given
//  If the decimal value is between 0 and 99, will display value
//  If the decimal value is larger than out of range (< -99 OR > 99) will
//      display an E for error
//  If the decimal value is negative will output the value with the DP lit up
//      on the RHS seven segment display
void dispConvNumber(int decimalValue){
    if(decimalValue >= 0 && decimalValue <= 99){
        if(decimalValue < 10){
            rhsSevenSeg(decimalValue);
        }else{
            updateSevenSeg(decimalValue/10,decimalValue%10);
        }
    }else if(decimalValue < -99 || decimalValue > 99){
        //Output error
        updateSevenSeg(11,11);
    }else{
        //Negative number within parameter!
        decimalValue *= -1;
        negSevenSeg(decimalValue/10,decimalValue%10);
    }

}
//Calculates the final value based on what operation is given
int calculateFinalValue(int operation, int valueX, int valueY){
    int finalValue = 0;
    switch (operation)
    {
        case 11:
            finalValue = Addition_Op(valueX,valueY);
            break;
        case 12:
            finalValue = Subtraction_Op(valueX,valueY);
            break;
        case 13:
            finalValue = Multiplication_Op(valueX,valueY);
            break;
        case 14:
            finalValue = Division_Op(valueX,valueY);
            break;
    }
    
    return finalValue;
}
//Performs addition and returns the value
int Addition_Op(int valueX, int valueY){
    return valueX + valueY;
}
//Performs subtraction and returns the value
int Subtraction_Op(int valueX, int valueY){
    return valueX - valueY;
}
//Performs multiplication and returns the value
int Multiplication_Op(int valueX, int valueY){
    return valueX * valueY;
}
//Performs division and returns the value
int Division_Op(int valueX, int valueY){
    return valueX / valueY;
}


