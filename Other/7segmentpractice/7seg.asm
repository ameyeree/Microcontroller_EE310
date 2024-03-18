;Testing 7 segment display code
; Will Count from 0 to F on 7 segment display, and repeat
; Needs to use delay, and will use PORTD.
; NOTE: 7 segment display being used is CATHODE
; PORTD mapping:
;   RD0-A
;   RD1-B
;   RD2-C
;   RD3-D
;   RD4-E
;   RD5-F
;   RD6-G
    
;---------------------
; Initialization
;---------------------
#include "C:\Users\James\Documents\EE310\FirstAssemblyMPLAB.X\AssemblyConfig.inc"
#include <xc.inc>
    

;---------------------
; Program Constants
;---------------------
counter	    EQU	0x20	;7 seg
lowerAddr   EQU	0x00	;For Table pointer, 7 seg value locations
higherAddr  EQU	0x01
upperAddr   EQU	0x00
REG10	    EQU 0x10   // in HEX
REG11	    EQU 0x11
Inner_loop  EQU 255 // in decimal
Outer_loop  EQU 255
   
;---------------------
; Constants
;---------------------
#define	count	0x10
	
;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld        ; Do not change
    
    ORG          0                ;Reset vector
    GOTO        _initialization

    ORG          0020H           ; Begin assembly at 0020H
    
_initialization:
    RCALL   _initializePortD
    RCALL   _initializeTablePointer
    MOVLW   count
    MOVWF   counter
    
_main:
    
    MOVFF   TABLAT, PORTD
    RCALL   loopDelay
    RCALL   loopDelay
    MOVLW   0x01
    ADDWF   TBLPTRL
    TBLRD*
    DECF    counter,1,0
    BNZ	    _main
    RCALL   _initializeTablePointer ;Reset to 0
    MOVLW   count
    MOVWF   counter
    BRA	    _main
    
_initializePortD:
    BANKSEL	PORTD 
    CLRF	PORTD		;Init PORTD
    BANKSEL	LATD		;Data Latch
    CLRF	LATD
    BANKSEL	ANSELD		;digital I/O
    CLRF	ANSELD		
    BANKSEL	TRISD 
    MOVLW	0b10000000	;Set RD0 to RD6 as outputs
    MOVWF	TRISD
    RETURN
    
_initializeTablePointer:
    MOVLW	lowerAddr
    MOVWF	TBLPTRL
    MOVLW	higherAddr
    MOVWF	TBLPTRH
    MOVLW	upperAddr
    MOVWF	TBLPTRU
    TBLRD*
    RETURN
    
;-----The Delay Subroutine    
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
    DECF        REG11,1 // outer loop
    BNZ        _loop1
    RETURN
    
_7segValues:
    ORG	0x100
    DB	0x3F, 0x30, 0x5B, 0x4F	;0-3
    DB	0x66, 0x6D, 0x7D, 0x07	;4-7
    DB	0x7F, 0x67, 0x77, 0x7F	;8-B
    DB	0x39, 0x3F, 0x79, 0x71	;C-F
    
END