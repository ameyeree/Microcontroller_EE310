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
#define	measuredTempInput   -20   ;Input value for temp sensor
#define refTempInput	    20	 ;Input value for keypad (user value)
    
;---------------------
; Registers
;--------------------- 
#define refTemp	   	    0x20    ;
#define measuredTemp	    0x21    ;
#define contReg		    0x22    ;
#define measuredDecHund	    0x72    ;
#define measuredDecTens	    0x71    ;
#define measuredDecOnes	    0x70    ;
#define refDecHund	    0x62    ;
#define refDecTens	    0x61    ;
#define refDecOnes	    0x60    ;    
numerator   EQU	    0x30    ;For decimal conversion
denominator EQU	    0x31    ;For decimal conversion
quotient    EQU	    0x33    ;For decimal conversion
    
;---------------------
; Program Outputs
;---------------------
#define	HEATER	PORTD,1
#define COOLER	PORTD,2

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
    MOVWF   PORTD,0

    MOVLW   measuredTempInput
    MOVWF   measuredTemp
    CALL    _convertMeasured
    
    MOVLW   refTempInput
    MOVWF   refTemp	    
    CALL    _convertReference
    
_compare:
    CPFSGT  measuredTemp,a	;if measuredTemp > refTemp go to _greater
    CPFSLT  measuredTemp,a	;if measuredTemp < refTemp go to _less
    BRA	    _greater
    CPFSEQ  measuredTemp,a	;if measuredTemp == refTemp go to equal
    BRA	    _less
    BRA	    _equal
    
_equal:			    ;if measuredTemp == refTemp go to equal
    MOVLW   0x00	    ;To set contReg to 0
    BCF	    HEATER
    BCF	    COOLER
    BRA	    _end
    
_greater:		    ;if measuredTemp > refTemp go to _greater   
    MOVLW   0x02	    ;To set contReg to 2
    BCF	    HEATER
    BSF	    COOLER
    BRA	    _end
    
_less:			    ;if measuredTemp < refTemp go to _less
    MOVLW   0x01	    ;To set contReg to 1
    BSF	    HEATER
    BCF	    COOLER
    BRA	    _end
    
    
_convertMeasured: ;Convert to decimal: Code taken from Mazid Pg 165
    ORG	    0x300
    
    MOVWF   numerator
    MOVLW   convDenom
    CLRF    quotient
_digit1m:
    INCF    quotient,F
    SUBWF   numerator
    BC	    _digit1m
    ADDWF   numerator
    DECF    quotient,F
    MOVFF   numerator,measuredDecOnes
    MOVFF   quotient,numerator
    CLRF    quotient
_digit2m:
    INCF    quotient,F
    SUBWF   numerator
    BC	    _digit2m
    ADDWF   numerator
    DECF    quotient,F
    MOVFF   numerator,measuredDecTens
    MOVFF   quotient,measuredDecHund
    RETURN
    
_convertReference: ;Convert to decimal: Code taken from Mazid Pg 165
    ORG	    0x350
    
    MOVWF   numerator
    MOVLW   convDenom
    CLRF    quotient
_digit1r:
    INCF    quotient,F
    SUBWF   numerator
    BC	    _digit1r
    ADDWF   numerator
    DECF    quotient,F
    MOVFF   numerator,refDecOnes
    MOVFF   quotient,numerator
    CLRF    quotient
_digit2r:
    INCF    quotient,F
    SUBWF   numerator
    BC	    _digit2r
    ADDWF   numerator
    DECF    quotient,F
    MOVFF   numerator,refDecTens
    MOVFF   quotient,refDecHund
    RETURN
    
_end:
    MOVWF   contReg,0
END