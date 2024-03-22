;--------------------------------------
; Title: Assignment 6 -- Counter Design
;--------------------------------------
;Program Details:
; This program utilizes a seven segment display and a matrix keypad.
; When the 1 button is pressed and held the 7 Segment displays values
;   will increment from 0 to F and repeat until the button is released.
; When the 2 button is pressed and held the 7 segment displays values
;   will decrement from F to 0 and repeat until the button is released.
; When the 3 button is pressed (doesn't need to be held), the 7 segment
;   display is reset to 0.
; When no buttons are pressed the display will stay at whatever value
;   it was last at.
; This program uses the Table Pointer to change the values.
; NOTE: 7 segment display being used is CATHODE
; PORTD mapping (all Output):
;   RD0-A
;   RD1-B
;   RD2-C
;   RD3-D
;   RD4-E
;   RD5-F
;   RD6-G
; PORTB mapping (Key pad):
;   RB0- Column 1   (Output)
;   RB1- Column 2   (Output)
;   RB2- Column 3   (Output)
;   RB3- Row 1	    (Input)		    
; Date: March 22nd, 2024
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
; Program Constants
;---------------------
beginReg    EQU	0x20	;7 seg
endReg	    EQU	0x21	;7 seg
lowerAddr   EQU	0x00	;For Table pointer, 7 seg value locations
higherAddr  EQU	0x01
upperAddr   EQU	0x00
REG10	    EQU 0x10	// in HEX (For loop delays)
REG11	    EQU 0x11
Inner_loop  EQU 255	// in decimal (For loop delays)
Outer_loop  EQU 255
button	    EQU	0x22	//Which button is pressed
   
;---------------------
; Constants
;---------------------
#define	beginR	0x00	;TBLPTRL beginning value
#define endR	0x0F	;TBLPTRL end value
	
;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld	    ;Do not change
    
    ORG          0		    ;Reset vector
    GOTO        _initialization

    ORG          0020H		    ;Begin assembly at 0020H
    
_initialization:
    RCALL   _initializePortD
    RCALL   _initializePortB
    MOVLW   beginR
    MOVWF   beginReg
    MOVLW   endR
    MOVWF   endReg
rst:RCALL   _initializeTablePointer

    
_main:
    MOVFF   TABLAT, PORTD
    RCALL   tripleDelay
    RCALL   checkKeypad
    GOTO    whatButton
  
checkKeypad:
    MOVLW   0x00
    
    BSF	    PORTB,0	    ;Scan first column of keys
    BTFSC   PORTB,3	    ;If button 1 pressed
    MOVLW   0x01
    BCF	    PORTB,0
    
    BSF	    PORTB,1	    ;Scan second column of keys
    BTFSC   PORTB,3	    ;If button 2 pressed
    MOVLW   0x02
    BCF	    PORTB,1
    
    BSF	    PORTB,2	    ;Scan third colum of keys
    BTFSC   PORTB,3	    ;If button 3 pressed
    MOVLW   0x03
    BCF	    PORTB,2
    
    MOVWF   button
    RETURN

whatButton:
    MOVLW   0x03
    CPFSLT  button		;If button is 3, set to zero (reset)
    GOTO    rst
    BTFSC   button,0		;If button is 1, increment 
    RCALL   incrementPointer	
    BTFSC   button,1		;If button is 2, decrement
    RCALL   decrementPointer
    BRA	    _main
    
incrementPointer:
    MOVLW   endR
    CPFSEQ  TBLPTRL	    ;IF TBLPTRL == 0x0F, SKIP AND RESET TABLE PTR TO 0
    GOTO    _contIncrement
    MOVFF   beginReg,TBLPTRL
    TBLRD*
    RETURN
_contIncrement:
    INCF    TBLPTRL
    TBLRD*
    RETURN
    
decrementPointer:
    MOVLW   beginR
    CPFSEQ  TBLPTRL	    ;IF TBLTRL == 0x00, SKIP AND RESET TABLE PTR TO F
    GOTO    _contDecrement
    MOVFF   endReg,TBLPTRL
    TBLRD*
    RETURN
_contDecrement:
    DECF    TBLPTRL
    TBLRD*
    RETURN
    
_initializePortD:
    BANKSEL	PORTD 
    CLRF	PORTD
    BANKSEL	LATD
    CLRF	LATD
    BANKSEL	ANSELD
    CLRF	ANSELD		
    BANKSEL	TRISD 
    MOVLW	0b10000000	;Set RD0 to RD6 as outputs
    MOVWF	TRISD		;Note: RD7 will not be used
    RETURN
  
_initializePortB:
    BANKSEL	PORTB
    CLRF	PORTB
    BANKSEL	LATB
    CLRF	LATB
    BANKSEL	ANSELB
    CLRF	ANSELB
    BANKSEL	TRISB
    MOVLW	0b11111000	;Set RB0 to RD2 as Outputs, and RB3 as an input
    MOVWF	TRISB		;Note: RB4 to RB 7 will not be used
    RETURN
    
_initializeTablePointer:
    MOVLW	lowerAddr	;Initializing Table Pointer to initial 
    MOVWF	TBLPTRL		; address location
    MOVLW	higherAddr
    MOVWF	TBLPTRH
    MOVLW	upperAddr
    MOVWF	TBLPTRU
    TBLRD*
    RETURN
    
;-----The Delay Subroutine
tripleDelay:
    RCALL   loopDelay
    RCALL   loopDelay
    RCALL   loopDelay
    RETURN
loopDelay: 
    MOVLW       Inner_loop
    MOVWF       REG10
    MOVLW       Outer_loop
    MOVWF       REG11
_loop1:
    DECF        REG10,1
    BNZ         _loop1
    MOVLW	Inner_loop
    MOVWF	REG10
    DECF        REG11,1
    BNZ        _loop1
    RETURN
    
_7segValues:
    ORG	0x100
    DB	0x3F, 0x30, 0x5B, 0x4F	;0-3
    DB	0x66, 0x6D, 0x7D, 0x07	;4-7
    DB	0x7F, 0x67, 0x77, 0x7F	;8-B
    DB	0x39, 0x3F, 0x79, 0x71	;C-F
    
END


