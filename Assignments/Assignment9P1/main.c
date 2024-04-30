/*******************************************************************************
 * Title: Assignment 9: ADC and LCD
 *******************************************************************************
 * Program Details:
 *  The purpose of this program is the get familiar with using an LCD and the
 * Analog to Digital converter. It is able to read voltage and also display 
 * lumens from a photoresistor.
 * Much of the code was given to us during the lab, or in this assignment.
 * 
 * Inputs: 
 *      ADC Reader: RA0 (Both Photoresistor and potentiometer can be connected)
 * Outputs: 
 *      LCD: RB0-RB7, RD0,RD1
 * Setup: C- Simulator
 * Date: April 29th, 2024
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
#include <xc.h> // must have this
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "LCD_Functions.h"
#include "ADC_Functions.h"
#include "cnfg.h"
//#include "../../../../../Program Files/Microchip/xc8/v2.40/pic/include/proc/pic18f46k42.h"
//#include "C:\Program Files\Microchip\xc8\v2.40\pic\include\proc\pic18f46k42"

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4


#define Vref 5.0 // voltage reference 
int digital; // holds the digital value 
float voltage; // hold the analog value (volt))
int lumens;
char data[10];
double avg;


void main(void)
{       
    LCD_Init();                    /* Initialize 16x2 LCD */
    ADC_Init();
    //LCD_String_xy(1,0,"Input Voltage:"); 
    LCD_String_xy(1,0,"The Input Light:");
    while(1){
        
        avg = 0;
        //DO: Set ADCON0 Go to start conversion
        for(int i = 0; i < 50; i++){
            ADCON0bits.GO = 1;
            
            while (ADCON0bits.GO); //Wait for conversion done
            digital = (ADRESH*256) | (ADRESL);/*Combine 8-bit LSB and 2-bit MSB*/
            // DO: define voltage = Vref/4096 (note that voltage is float type
            voltage = digital*( (float)Vref / (float)4096 );
            avg = avg + voltage;
        }
        voltage = avg/50.0;
        if(voltage >=0 && voltage <= 2){
            lumens = 79 * voltage;
        }else if(voltage > 2 && voltage <= 3.15){
            lumens = 540 * voltage - 1000;
        }else if(voltage > 3.15 && voltage <= 3.75){
            lumens = 1111 * voltage - 2735;
        }else if(voltage > 3.75){
            lumens = 3750 * voltage - 12700;
        }
        /*This is used to convert integer value to ASCII string*/
        /**********************************
         * For Lumens use below code
         **********************************/
        sprintf(data,"%d",lumens);
        strcat(data," LUX     ");
        
        /**********************************
         * For voltage use below code
         **********************************/
        //sprintf(data,"%.2f",voltage);
        //strcat(data," V");	/*Concatenate result and unit to print*/
        LCD_String_xy(2,2,data); 
        __delay_ms(1000);
    }             
}

