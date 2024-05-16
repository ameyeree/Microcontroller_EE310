/*******************************************************************************
 * Password Reset Module
 *******************************************************************************
 * Program Details:
 *  An interrupt that allows the user to save an updated password to EEPROM.
 *******************************************************************************/
int password;
void eepromWrite(uint8_t address, uint8_t data);
void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void)
{
        // Check if interrupt flag for INT0 is set to 1 - (note INT0 is your input)
            // if so, do something
   
    if(PORTBbits.RB0 == 1){
        LCD_Clear();
        LCD_String_xy(1, 0, "New Password:");
        
        password = getValue();
        __delay_ms(3000);
        LCD_Clear();
        LCD_String_xy(1, 1, "Password Reset");
        LCD_String_xy(2, 0, "****************");
        __delay_ms(3000);
        LCD_Clear();
        eepromWrite(0,password/100);
        eepromWrite(2,password%100);
        RESET();
    }    
    // always clear the interrupt flag for INT0 when done
    PIR1bits.INT0IF = 0;

}

void INTERRUPT_Initialize (void)
{
    PORTB = 0;
    TRISB = 0b11111111;
    LATB = 0;
    ANSELB = 0;
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

uint8_t eepromRead(uint8_t address){
    //Setup EEPROM access
    NVMCON1 = 0;
    
    //Setup address
    NVMADRL = address;
    
    //Set to read mode, and wait for data to be read and
    // put in register to access it
    NVMCON1bits.RD = 1;
    while(NVMCON1bits.RD);
    
    //Return data
    return NVMDAT;
}

void eepromWrite(uint8_t address, uint8_t data){
    //Setup EEPROM access
    NVMCON1 = 0;
    
    //Setup address
    NVMADRL = address;
    
    //Put data memory in write register
    NVMDAT = data;
    
    //Enable writes
    NVMCON1bits.WREN = 1;
    
    //Disable interrupts temporarily
    INTCON0bits.GIE = 0;
    
    //Unlock sequence to allow writing to address
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    
    //Enable write mode
    NVMCON1bits.WR = 1;
    while(NVMCON1bits.WR);
    
    //Re-enable interrupts
    INTCON0bits.GIE = 1;
    
    //Disable writes
    NVMCON1bits.WREN = 0;
}