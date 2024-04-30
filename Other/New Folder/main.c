/*
 * The purpose of this program is to demonstrate how a 20x2 LCD can be interfaced with a PIC processor. 
 * Refer to the lecture slides for more details on the physical connections.
 * You may want to consider changing this code, as needed, depending on your hardware. 
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
#include "C:/Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4

#define RS PORTBbits.RB0                   /* PORTD 0 pin is used for Register Select */
#define EN PORTBbits.RB1                   /* PORTD 1 pin is used for Enable */
#define ldata PORTD                 /* PORTB is used for transmitting data to LCD */

//#define LCD_Port TRISB              
//#define LCD_Control TRISD

void LCD_Init();
void LCD_Command(char );
void LCD_Char(char x);
void LCD_String(const char *);
void LCD_String_xy(char ,char ,const char*);
void MSdelay(unsigned int );
void lcd_set_cursor(char col, char line);

/*****************************Main Program*******************************/

void main(void)
{      
    PORTB = 0;
    TRISB = 0;
    LATB = 0;
    ANSELB = 0;
    
    PORTD = 0;
    TRISD = 0;
    LATD = 0;
    ANSELD = 0;
    //OSCCON=0x72;                   /* Use Internal Oscillator with Frequency 8MHZ */ 
    LCD_Init();                    /* Initialize 16x2 LCD */
    LCD_String("Hello World");
    lcd_set_cursor(3,2);
    LCD_String("Second line");
    //LCD_String_xy(1,0,"Anthony Meyer");    /* Display string at location(row,location). */
                                   /* This function passes string to display */
    //LCD_String_xy(2,0,"Row");   /*Display string at location(row,location). */
                                   /* This function passes string to display */    
    
    while(1);           
}

/****************************Functions********************************/
void LCD_Init()
{
    __delay_ms(1000); //MSdelay(15);           /* 15ms,16x2 LCD Power on delay */
//    LCD_Port = 0x00;       /* Set PORTB as output PORT for LCD data(D0-D7) pins */
//    LCD_Control = 0x00;    /* Set PORTD as output PORT LCD Control(RS,EN) Pins */
    
    LCD_Command(0b00111100);     /* clear display screen */
    LCD_Command(0b00001100);     /* uses 2 line and initialize 5*7 matrix of LCD */
    LCD_Command(0b00000110);     /* display on cursor off */
    //LCD_Command(0x06);     /* increment cursor (shift cursor to right) */
}

void LCD_Clear()
{
        LCD_Command(0x01); /* clear display screen */
}

void LCD_Command(char cmd )
{
    PORTD = cmd;            /* Send data to PORT as a command for LCD */   
    RS = 0;                /* Command Register is selected */
    EN = 1;                /* High-to-Low pulse on Enable pin to latch data */ 
    NOP();
    EN = 0;
    __delay_ms(15);//MSdelay(3); 
}

void LCD_Char(char dat)
{
    PORTD = dat;            /* Send data to LCD */  
    RS = 1;                /* Data Register is selected */
    EN=1;                  /* High-to-Low pulse on Enable pin to latch data */   
    NOP();
    EN=0;
    __delay_ms(15);//MSdelay(1);
}


void LCD_String(const char *msg)
{
    while((*msg)!=0)
    {       
      LCD_Char(*msg);
      msg++;    
    }
}

void LCD_String_xy(char row,char pos,const char *msg)
{
    char location=0;
    if(row<=1)
    {
        location=(0x80) | ((pos) & 0x0f); /*Print message on 1st row and desired location*/
        LCD_Command(location);
    }
    else
    {
        location=(0xC0) | ((pos) & 0x0f); /*Print message on 2nd row and desired location*/
        LCD_Command(location);    
    }  
    LCD_String(msg);

}
void lcd_set_cursor(char col, char line){
    if (line == 1){
        LCD_Command(0b10000000 | col);
    }else if(line == 2){
        LCD_Command(0b11000000 | col);
    }
}
/*********************************Delay Function********************************/
void MSdelay(unsigned int val)
{
     unsigned int i,j;
        for(i=0;i<val;i++)
            for(j=0;j<165;j++);      /*This count Provide delay of 1 ms for 8MHz Frequency */
}