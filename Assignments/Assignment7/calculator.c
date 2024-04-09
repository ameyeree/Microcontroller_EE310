/*******************************************************************************
 * Title: Assignment 7: Calculator
 *******************************************************************************
 * Program Details:
 *  The purpose of this program is to utilize a matrix keypad and two seven
 * segment displays to perform mathematic calculations. The calculator will be
 * able to perform integer addition, subtraction, multiplication and division.
 * Constraints:
 *      *Values given by the user will be a maximum of two digits and will be
 *          positive values. Range is: [0,99]
 *      *Range for results is: [-99,99]
 *      *Can only perform integer calculations. In the case of division it will
 *          always round down.
 * Inputs: 
 *      Matrix Keypad: RA0-RA5, RB0-RB1
 * Outputs: 
 *      Two seven segment displays: RD0-RD7, RC2 & RC3
 * Setup: C- Simulator
 * Date: April 8th, 2024
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
 * Initialization/Headers
 **************************/
#include <xc.h>
#include "cnfg.h"
#include "calcFunctions.h"
#include "C:/Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Below is for delay
#define _XTAL_FREQ 4000000
#define FCY _XTAL_FREQ/4


int main(void) {
    //Initialize all variables and ports
    initializeAllPorts();
    int Operation_REG = 0;
    int X_Input_REG = -1;
    int Y_Input_REG = -1;
    int Display_Result_REG = 0;
    int isEqualSign = 0;
    
    //Start calculator program (infinite loop)
    while(1){
        //Re-initialize values once calculation is done
        Operation_REG = 0;
        isEqualSign = 0;
        Display_Result_REG = 0;
        X_Input_REG = -1;
        Y_Input_REG = -1;
        
        //Get first value from the user and store in X_Input_REG
        X_Input_REG = getValue(&Operation_REG,&isEqualSign);
        
        //If operator is not + - * or /, then wait until it is
        while(Operation_REG <= 10 || Operation_REG >= 15){
            dispConvNumber(X_Input_REG);
            Operation_REG = getKeypad();
        }
        
        //Get second value from the user and store in Y_Input_REG
        Y_Input_REG = getValue(&Operation_REG,&isEqualSign);
        
        //If operation is not = then wait until it is
        while(isEqualSign != 10){
            dispConvNumber(Y_Input_REG);
            isEqualSign = getKeypad();
        }

        //Calculate final result and store in Display_Result_REG
        Display_Result_REG = calculateFinalValue(Operation_REG,X_Input_REG,Y_Input_REG);
        
        //Display final result until operator is *
        while(Operation_REG != 15){
            dispConvNumber(Display_Result_REG);
            Operation_REG = getKeypad();
        }
    }
    return 0;
}
