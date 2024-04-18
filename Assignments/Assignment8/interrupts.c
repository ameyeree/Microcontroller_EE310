/*
 * File:   interrupts.c
 * Author: James
 *
 * Created on April 11, 2024, 10:42 AM
 */


#include <xc.h>
#include "cnfg.h"
#include "C:/Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4

//Note: INT2 is RB2

// Defining Interrupt ISR 
int count = 0;
unsigned char sevenSegValues[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x00,0x79};
  
void __interrupt(irq(IRQ_INT2),base(0x4008)) INT2_ISR(void)
{
    count++;
    if(count > 99){
        count =  0;
    }
        //update count
    // always clear the interrupt flag for INT2 when done
    
    PIR7bits.INT2IF = 0;
}

void INTERRUPT_Initialize (void)
{
    // Enable interrupt priority bit in INTCON0 (check INTCON0 register and find the bit)
    INTCON0bits.IPEN = 1;
    // Enable high priority interrupts using bits in INTCON0
    INTCON0bits.GIEH = 1;
    // Enable low priority interrupts using bits in INTCON0
    INTCON0bits.GIEL = 1;
    // Interrupt on rising edge of INT0 pin using bits in INTCON0
    INTCON0bits.INT2EDG = 1;
    // Set the interrupt high priority (IP) for INT0 - INT0IP
    IPR7bits.INT2IP = 1;
    // Enable the interrupt (IE) for INT0
    PIE7bits.INT2IE = 1;
    //Clear interrupt flag for INT01
    PIR7bits.INT2IF = 0;
    
    // Change IVTBASE by doinG the following
    // Set IVTBASEU to 0x00 
    IVTBASEU = 0x00;
    // Set IVTBASEH to  0x40; 
    IVTBASEH = 0x40;
    // Set IVTBASEL to 0x08;
    IVTBASEL = 0x08;   
}
void intializePorts();
void initializePORTA();
void initializePORTB();
void initializePORTD();
void initializePORTC();
void dispConvNumber(int decimalValue);
void updateSevenSeg(int lhsIndex, int rhsIndex);
void lhsSevenSeg(int index);
void rhsSevenSeg(int index);
void ledBCD(int decimalValue);


void main(void) {

    // initialize the interrupt_initialization by calling the proper function
    intializePorts();
    INTERRUPT_Initialize();
    // main code here 
    while(1){
        //Display count value
        dispConvNumber(count); 
        ledBCD(count);
    }

}

void intializePorts(){
    initializePORTA();
    initializePORTB();
    initializePORTD();
    initializePORTC();
}
void initializePORTA(){
    PORTA = 0;
    TRISA = 0;
    LATA = 0;
    ANSELA = 0;
}
void initializePORTB(){
    PORTB = 0;
    TRISB = 0b00000100;
    LATB = 0;
    ANSELB = 0;
    WPUB = 1;
}
void initializePORTD(){
    PORTD = 0;
    TRISD = 0;
    LATD = 0;
    ANSELD = 0;
}
void initializePORTC(){
    PORTC = 0;
    LATC = 0;
    ANSELC = 0;
    TRISC = 0;
}
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
        //decimalValue *= -1;
        //negSevenSeg(decimalValue/10,decimalValue%10);
    }

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
void ledBCD(int decimalValue){
    //decimalValue/10,decimalValue%10
    PORTA = decimalValue%10;
    int temp = decimalValue/10;
    temp <<= 4;
    PORTA = PORTA + temp;
}