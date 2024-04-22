//Initialization values
unsigned char sevenSegValues[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x00,0x79};
int count = 0;
int pr1Count = 0;
unsigned char done = 0;
int pr2Count = 0;
int SECRET_CODE = 0x00;
int USER_GUESS = 0x00;
//Initialization functions
void initializeALL();
void __interrupt(irq(IRQ_INT2),base(0x4008)) INT2_ISR(void);
void INTERRUPT_Initialize (void);
void intializePorts();
void initializePORTA();
void initializePORTB();
void initializePORTD();
void initializePORTC();


void initializeALL(){
    intializePorts();
    INTERRUPT_Initialize();
    PORTAbits.RA1 = 1;
}
void __interrupt(irq(IRQ_INT2),base(0x4008)) INT2_ISR(void)
{
    PORTAbits.RA0 = 1;
    __delay_ms(500);
    PORTAbits.RA0 = 0;
    __delay_ms(500);
    PORTAbits.RA0 = 1;
    __delay_ms(1000);
    PORTAbits.RA0 = 0;
    __delay_ms(1000);
    PORTAbits.RA0 = 1;
    __delay_ms(1500);
    PORTAbits.RA0 = 0;
    __delay_ms(500);
    PORTAbits.RA0 = 1;
    __delay_ms(250);
    PORTAbits.RA0 = 0;
   
    PIR7bits.INT2IF = 0;
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
    INTCON0bits.INT2EDG = 1;
    // Set the interrupt high priority (IP) for INT0 - INT0IP
    IPR7bits.INT2IP = 1;
    // Enable the interrupt (IE) for INT0
    PIE7bits.INT2IE = 1;
    //Clear interrupt flag for INT01
    PIR7bits.INT2IF = 0;
    
    // Change IVTBASE by doinG the following
    // Set IVTBASEU to 0x00 
    IVTBASEU = 0x00;
    // Set IVTBASEH to  0x40; 
    IVTBASEH = 0x40;
    // Set IVTBASEL to 0x08;
    IVTBASEL = 0x08;   
}

void intializePorts(){
    initializePORTA();
    initializePORTB();
    initializePORTD();
    initializePORTC();
}
void initializePORTA(){
    PORTA = 0;
    TRISA = 0;
    LATA = 0;
    ANSELA = 0;
}
void initializePORTB(){
    PORTB = 0;
    TRISB = 0b00000111;
    LATB = 0;
    ANSELB = 0;
    WPUB = 0b00000100;
}
void initializePORTD(){
    PORTD = 0;
    TRISD = 0;
    LATD = 0;
    ANSELD = 0;
}
void initializePORTC(){
    PORTC = 0;
    LATC = 0;
    ANSELC = 0;
    TRISC = 0b00000100;
}