;---------------------
; Title: Assignment 4
;---------------------
;Program Details:
; Receives a measured value (currently hard coded), and a reference value
; (also currently hard coded) for the temperature in Celsius. 
; Stores these values in decimal form in specified registers (3 for each).
; If a value is negative, will store 0x01 in the "hundreds" position.
; Compares the values and turns HEATER or COOLER off depending on measurements:
; -If the temperatures are equal, will set HEATER and COOLER to zero (off),
; and controller register to 0.
; -If the measured > reference, will set HEATER to one (on) and COOLER to
; zero (off), and controller register to 1.
; -If the measured > reference, will set HEATER to zero (off) and COOLER to
; one (on), and controller register to 2.
; Program Constraints: 
;   measured temp: between -20 and +60 degrees Celsius
;   reference temp: between +10 and +50 degrees Celsius
;   only using PORTD
;   must start from register 0x20				    
				    
; Inputs: measuredTempInput, refTempInput
; Outputs: HEATER (RD1), COOLER (RD2)
; Date: March 4th, 2024
; File Dependencies / Libraries: None 
; Compiler: ASM 2.46
; Author: Anthony MeyerSlechta
; Versions:
;       V1.0: Original Program

;---------------------
; Initialization
;---------------------
#include "C:\Users\James\Documents\EE310\FirstAssemblyMPLAB.X\AssemblyConfig.inc"
#include <xc.inc>

;---------------------
; Program Inputs
;---------------------
#define	measuredTempInput   -5	    ;Input value for temp sensor
				    ;Value is assumed to be between -20 and 60
#define refTempInput	    15	    ;Input value for keypad (user value)
				    ;Value is assumed to be between 10 and 50
    
;---------------------
; Registers
;--------------------- 
refTemp		EQU	    0x20    ;Reference Temp given by user
measuredTemp	EQU	    0x21    ;Measured temp "given by temp sensor"				    
contReg		EQU	    0x22    ;Controller register
measuredDecHund	EQU	    0x72    ;Convert to decimal (hundreds position)
measuredDecTens	EQU	    0x71    ;Convert to decimal (tens position)
measuredDecOnes EQU	    0x70    ;Convert to decimal (ones position)
refDecHund	EQU	    0x62    ;Convert to decimal (hundreds position)
refDecTens	EQU	    0x61    ;Convert to decimal (tens position)
refDecOnes	EQU	    0x60    ;Convert to decimal (ones position)
numerator	EQU	    0x30    ;For decimal conversion
denominator	EQU	    0x31    ;For decimal conversion
quotient	EQU	    0x33    ;For decimal conversion
    
;---------------------
; Program Outputs
;---------------------
#define	HEATER	PORTD,1		    ;PORT D1
#define COOLER	PORTD,2		    ;PORT D2

;---------------------
; Program Constants
;---------------------
#define	convDenom	10	    ;Used for dividing to convert to decimal
    
;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld	    ;Do not change
    
    ORG          0		    ;Reset vector
    GOTO        _start1

    ORG          0x20		    ;Begin assembly at 0x20
    
_start1:			    ;Initializes PORTD
    MOVLW   0x00
    MOVWF   TRISD,0
    MOVWF   PORTD,0
    
_measuredTempInput:
    MOVLW   measuredTempInput
    MOVWF   measuredTemp
    BTFSS   measuredTemp,7	    ;If negative,convert to "magnitude" of number
    GOTO    _convMCall		
    NEGF    WREG
    
_convMCall:			    ;Convert measured temp to decimal and save
    CALL    _convertMeasured	    ; in registers 0x70,0x71,0x72
    MOVLW   measuredTempInput
    
_referenceTempInput:
    MOVLW   refTempInput	    
    MOVWF   refTemp	  
    
_convRCall:			    ;Convert reference temp to decimal and save
    CALL    _convertReference	    ; in registers 0x60,0x61,0x62
    MOVLW   refTempInput
    
_isNegative:			;Assuming ref temp can not be negative
    BTFSC   measuredTemp,7	; if measuredTemp is negative, then 
    GOTO    _heater		; go to heater
    
_compare:
    CPFSGT  measuredTemp,a	;if measuredTemp > refTemp go to cooler
    CPFSLT  measuredTemp,a	;if measuredTemp < refTemp go to heater
    BRA	    _cooler
    CPFSEQ  measuredTemp,a	;if measuredTemp == refTemp go to off
    BRA	    _heater
    BRA	    _off
  
_off:				;if measuredTemp == refTemp go to off
    MOVLW   0x00		;To set contReg to 0
    BCF	    HEATER
    BCF	    COOLER
    BRA	    _end
    
_cooler:			;if measuredTemp > refTemp go to cooler   
    MOVLW   0x02		;To set contReg to 2
    BCF	    HEATER
    BSF	    COOLER
    BRA	    _end
    
_heater:			;if measuredTemp < refTemp go to heater
    MOVLW   0x01		;To set contReg to 1
    BSF	    HEATER
    BCF	    COOLER
    BRA	    _end
    
    
_convertMeasured:	    ;Convert to decimal: Code taken from Mazid Pg 165
    ORG	    0x300	    ; Uses division (in the form of subtraction)
    
    MOVWF   numerator
    MOVLW   convDenom
    CLRF    quotient
_digit1m:				;Converts hex to decimal (in ones pos)
    INCF    quotient,F
    SUBWF   numerator
    BC	    _digit1m
    ADDWF   numerator
    DECF    quotient,F
    MOVFF   numerator,measuredDecOnes
    MOVFF   quotient,numerator
    CLRF    quotient
_digit2m:				;Converts hex to decimal (in tens pos)
    INCF    quotient,F
    SUBWF   numerator
    BC	    _digit2m
    ADDWF   numerator
    DECF    quotient,F
    MOVFF   numerator,measuredDecTens
    BTFSS   measuredTemp,7		;If negative, store 0x01 in hundreds pos
    RETURN
    MOVLW   0x01
    MOVWF   measuredDecHund
    ;MOVFF   quotient,measuredDecHund	;Code used for if decimal > 99
    RETURN
    
_convertReference: ;Convert to decimal: Code taken from Mazid Pg 165
    ORG	    0x350
    
    MOVWF   numerator
    MOVLW   convDenom
    CLRF    quotient
_digit1r:			    ;Converts hex to decimal (in ones pos)
    INCF    quotient,F
    SUBWF   numerator
    BC	    _digit1r
    ADDWF   numerator
    DECF    quotient,F
    MOVFF   numerator,refDecOnes
    MOVFF   quotient,numerator
    CLRF    quotient
_digit2r:			    ;Converts hex to decimal 
    INCF    quotient,F		    ; (in tens and hunds pos)
    SUBWF   numerator
    BC	    _digit2r
    ADDWF   numerator
    DECF    quotient,F
    MOVFF   numerator,refDecTens
    MOVFF   quotient,refDecHund	    ;Should not currently be used based on 
    RETURN			    ; constraints
    
_end:
    MOVWF   contReg,0
END