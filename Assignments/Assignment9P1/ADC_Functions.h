void ADC_Init(void);

void ADC_Init(void)
{
       //Setup ADC
    //DO: using ADCON0 set right justify
    ADCON0bits.FM = 1;
    //DO: Using ADCON0 set ADCRC Clock
    ADCON0bits.CS = 1;
    //DO: Using ADPCH register set RA0 as Analog channel
    ADPCHbits.PCH = 0x000000;
    //DO: Set RA0 to input
    TRISAbits.TRISA0 = 1;
    //DO: Set RA0 to analog
    ANSELAbits.ANSELA0 = 1;
    //DO: Turn ADC On on register ADCON0
    ADCON0bits.ON = 1;   
    //DO: set ADC CLOCK Selection register to zero
    ADCLKbits.CS = 0x0;
    //DO: Clear ADC Result registers
    ADRESL = 0x0;
    ADRESH = 0x0;
   //DO: set precharge select to 0 in register ADPERL & ADPERH
    ADPREL = 0;
    ADPREH = 0;
    //DO: set acquisition low and high byte to zero 
    ADACQL = 0;
    ADACQL = 0;
   
}