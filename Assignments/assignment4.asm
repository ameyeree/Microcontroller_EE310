;---------------------
; Title: Assignment 4
;---------------------
;Program Details:
; The purpose of this program is to load two bytes, Byte1 (F2H) and Byte2 (02H), in data registers REG0 
; The program may contain logical errors and should be debugged. Follow the comments and correct the errors

;-------
; Read This:
;-------
; You may need to change the path for the AssemblyConfig.inc; this file contains all the configuration bits
; To simulate go to Windows tab, then Target Memory View--> File Registers and view teh registers

		
; Inputs: Byte 1 and Byte 2
; Outputs: Register o
; Date: March 4th, 2024
; File Dependencies / Libraries: None 
; Compiler: ASM 2.4
; Author: Anthony MeyerSlechta
; Versions:
;       V1.0: Oriiginal Program

;---------------------
; Initialization
;---------------------
#include "C:\Users\James\Documents\EE310\FirstAssemblyMPLAB.X\AssemblyConfig.inc"
#include <xc.inc>

;---------------------
; Program Inputs
;---------------------
#define	measuredTempInput   45	    ;Input value for temp sensor
#define refTempInput	    25	    ;Input value for keypad (user value)
    
;---------------------
; Registers
;--------------------- 
#define refTemp	   	    0x20    ;
#define measuredTemp	    0x21    ;
#define contReg		    0x22    ;
#define measuredDecHund	    0x62    ;
#define measuredDecTens	    0x61    ;
#define measuredDecOnes	    0x60    ;
#define refDecHund	    0x72    ;
#define refDecTens	    0x71    ;
#define refDecOnes	    0x70    ;
#define	numerator	    0x30    ;
#define	denominator	    0x30    ;
    
;---------------------
; Program Outputs
;---------------------
#define	HEATER	PORTD,2
#define COOLER	PORTD,1

;---------------------
; Program Constants
;---------------------
#define	convDenom	10	    ;Used for dividing to convert
;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld	    ;Do not change
    
    ORG          0		    ;Reset vector
    GOTO        _start1

    ORG          0x20		    ;Begin assembly at 0x20
    
    
_start1:
    MOVLW   0x00
    MOVWF   TRISD,0
    MOVWF   PORTD
    MOVLW   measuredTempInput
    MOVWF   measuredTemp
    CALL    _convert,1
    MOVLW   refTempInput
    MOVWF   refTemp;
    CALL    _convert,1

_convert: ;Convert to decimal: Code taken from Mazid Pg 165
    
    RETURN
_compare:
    CPFSGT  measuredTemp,0
    CPFSLT  measuredTemp,0
    BRA	    _greater
    CPFSEQ  measuredTemp,0
    BRA	    _less
    BRA	    _equal
    
_equal:
    MOVLW   0x00
    BRA	    _end
    
_greater:
    MOVLW   0x02
    BRA	    _end
    
_less:
    MOVLW   0x01
    BRA	    _end
    
_end:
    MOVWF   contReg
    MOVWF   PORTD
    
END