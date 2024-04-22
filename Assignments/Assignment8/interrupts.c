/*
 * File:   interrupts.c
 * Author: James
 *
 * Created on April 11, 2024, 10:42 AM
 */


#include <xc.h>
#include "cnfg.h"
#include "functions.h"
//#include "initialization.h"
#include "C:/Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"



 
void main(void) {

    // Initializes interrupts and all ports
    //When system is on SYS_LED should be on
    initializeALL();
    setSecretCode();
    
    while(1){
        getUserGuess();
        handleGuess();   
    }

}

