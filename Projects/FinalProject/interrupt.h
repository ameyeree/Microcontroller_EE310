

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