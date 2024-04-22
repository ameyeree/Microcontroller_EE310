#include "initialization.h"


//Functions
void sevenSeg(int index, unsigned char isSecond);
void setSecretCode();
void getUserGuess();
void handleGuess();

void sevenSeg(int index, unsigned char isSecond){
    if(index < 0 || index > 9){
        index = 10;
    }
    //PORTC = 0b00000100;
    PORTD = sevenSegValues[index];  
    if(isSecond == 1){
        PORTD += 0b10000000;
    }
    //__delay_ms(5);
}
void setSecretCode(){
    pr1Count = 0;
    pr2Count = 0;
    done = 0;
    sevenSeg(pr1Count,0);
    while(done != 1){
        if(PORTBbits.RB0 == 1){
            __delay_ms(1000);
            while(PORTBbits.RB0 == 1){
                if(PORTBbits.RB1 == 1){
                    //while(PORTBbits.RB0 == 1);
                    done = 1;
                    __delay_ms(1000);
                }
            }
            if(done == 0){
                pr1Count++;
                sevenSeg(pr1Count,0);
            }
        }
    }
    done = 0;
    sevenSeg(pr2Count,1);
    while(done != 1){
        if(PORTBbits.RB1 == 1){
            __delay_ms(1000);
            while(PORTBbits.RB1 == 1){
                if(PORTBbits.RB0 == 1){
                    //while(PORTBbits.RB0 == 1);
                    done = 1;
                    __delay_ms(1000);
                }
            }
            if(done == 0){
                pr2Count++;
                sevenSeg(pr2Count,1);
            }
        }
    }
    PORTAbits.RA2 = 1;
    PORTD = 0;
    SECRET_CODE = (pr2Count * 16) | (pr1Count);
}
void getUserGuess(){
    pr1Count = 0;
    pr2Count = 0;
    done = 0;
    sevenSeg(pr1Count,0);
    while(done != 1){
        if(PORTBbits.RB0 == 1){
            __delay_ms(1000);
            while(PORTBbits.RB0 == 1){
                if(PORTBbits.RB1 == 1){
                    //while(PORTBbits.RB0 == 1);
                    done = 1;
                    __delay_ms(1000);
                }
            }
            if(done == 0){
                pr1Count++;
                sevenSeg(pr1Count,0);
            }
        }
    }
    done = 0;
    sevenSeg(pr2Count,1);
    while(done != 1){
        if(PORTBbits.RB1 == 1){
            __delay_ms(1000);
            while(PORTBbits.RB1 == 1){
                if(PORTBbits.RB0 == 1){
                    //while(PORTBbits.RB0 == 1);
                    done = 1;
                    __delay_ms(1000);
                }
            }
            if(done == 0){
                pr2Count++;
                sevenSeg(pr2Count,1);
            }
        }
    }
    PORTD = 0;
    __delay_ms(500);
    PORTD = 0xFF;
    __delay_ms(500);
    PORTD = 0;
    USER_GUESS = (pr2Count * 16) | (pr1Count);
}
void handleGuess(){
    if(SECRET_CODE != USER_GUESS){
        PORTAbits.RA0 = 1;
        __delay_ms(5000);
        PORTAbits.RA0 = 0;
    }else{
        PORTAbits.RA3 = 1;
        __delay_ms(5000);
        PORTAbits.RA3 = 0;
    }
    USER_GUESS = 0;
}

