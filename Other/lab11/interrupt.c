/*
 * The purpose of this program is demonstrate INT0 as an external interrupt. 
 * In this case when the signal changes state on RB0 (INT0) then D0 starts 
 * blinking for 4 seconds and then stops. The results can be simulated and
 * verified.  
 * 
 * Author: Farid Farahmand 
 */


// PIC18F46K42 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config FEXTOSC = LP     // External Oscillator Selection (LP (crystal oscillator) optimized for 32.768 kHz; PFM set to low power)
#pragma config RSTOSC = EXTOSC  // Reset Oscillator Selection (EXTOSC operating per FEXTOSC bits (device manufacturing default))

// CONFIG1H
#pragma config CLKOUTEN = OFF   // Clock out Enable bit (CLKOUT function is disabled)
#pragma config PR1WAY = ON      // PRLOCKED One-Way Set Enable bit (PRLOCK bit can be cleared and set only once)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)

// CONFIG2L
#pragma config MCLRE = EXTMCLR  // MCLR Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTS = PWRT_OFF // Power-up timer selection bits (PWRT is disabled)
#pragma config MVECEN = ON      // Multi-vector enable bit (Multi-vector enabled, Vector table used for interrupts)
#pragma config IVT1WAY = ON     // IVTLOCK bit One-way set enable bit (IVTLOCK bit can be cleared and set only once)
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit (ULPBOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)

// CONFIG2H
#pragma config BORV = VBOR_2P45 // Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR) set to 2.45V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config DEBUG = OFF      // Debugger Enable bit (Background debugger disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period selection bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled; SWDTEN is ignored)

// CONFIG3H
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4L
#pragma config BBSIZE = BBSIZE_512// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBEN = OFF       // Boot Block enable bit (Boot block disabled)
#pragma config SAFEN = OFF      // Storage Area Flash enable bit (SAF disabled)
#pragma config WRTAPP = OFF     // Application Block write protection bit (Application Block not write protected)

// CONFIG4H
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not write-protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
#pragma config WRTSAF = OFF     // SAF Write protection bit (SAF not Write Protected)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)

// CONFIG5L
#pragma config CP = OFF         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection disabled)

#include <xc.h> // must have this
//#include "../../../../../Program Files/Microchip/xc8/v2.40/pic/include/proc/pic18f46k42.h"
#include "C:/Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4

// Defining Interrupt ISR 
void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void)
{
        // Check if interrupt flag for INT0 is set to 1 - (note INT0 is your input)
            // if so, do something
                // e.g,blink an LED connected to  PORTDbits.RD0 for 10 times with a delay of __delay_ms(250)
   
    if(PORTBbits.RB0 == 1){
        PORTDbits.RD1 = 0;
        for(int i = 0; i < 10; i++){
            PORTDbits.RD0 = 1;
            __delay_ms(500);
            PORTDbits.RD0 = 0;
            __delay_ms(500);
        }
    }    
    // always clear the interrupt flag for INT0 when done
    PIR1bits.INT0IF = 0;
    PORTDbits.RD0 = 0;
        // turn off the led on PORTDbits.RD0 
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
    INTCON0bits.INT0EDG = 1;
    // Set the interrupt high priority (IP) for INT0 - INT0IP
    IPR1bits.INT0IP = 1;
    // Enable the interrupt (IE) for INT0
    PIE1bits.INT0IE = 1;
    //Clear interrupt flag for INT01
    PIR1bits.INT0IF = 0;
    
    // Change IVTBASE by doinG the following
    // Set IVTBASEU to 0x00 
    IVTBASEU = 0x00;
    // Set IVTBASEH to  0x40; 
    IVTBASEH = 0x40;
    // Set IVTBASEL to 0x08;
    IVTBASEL = 0x08;   
}
void main(void) {

    // Initialization  
    // set port B and D as outputs 
    // Port B should be inputs ??
    PORTB = 0;
    TRISB = 0b11111111;
    LATB = 0;
    ANSELB = 0;
    
    PORTD = 0;
    TRISD = 0;
    LATD = 0;
    ANSELD = 0;

    // enable the weak pull-ups are enabled for port B
    WPUB = 1;
    // initialize the interrupt_initialization by calling the proper function
    INTERRUPT_Initialize();
    
    // main code here 
    while(1){
        // blink an LED connected to RD1 every 2 seconds
        PORTDbits.RD1 = 1;
        __delay_ms(2000);
        PORTDbits.RD1 = 0;
        __delay_ms(2000);
    }

}