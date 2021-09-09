/*
 * Ark_Drivers.c
 *
 *  Created on: 13-Jan-2018
 *      Author: lavesh
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "Ark_functions.h"


/*
 * Function Description: Initializes clock to 40Mhz
 *   Configure the system clock to run using a 16MHz crystal on the main oscillator, driving the 400MHz PLL.
 *   There is a default /2 divider in the clock path and we are specifying another /5, which totals 10.
 *   That means the System Clock will be 40MHz.
 */
void InitSystemClock(){

    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
}
void InitGPIOClock(){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
}

void InitGPIO_Inputs(){

    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);                // Reference voltage    - channel 2

    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_5);                // Configure GPIO as ADC PE5-Voltage
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_4);                // PE-4 Current
}

void InitGPIO(){
    InitGPIOClock();
    InitGPIO_Inputs();
   // InitGPIO_Outputs();
}

void InitADC(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    ADCHardwareOversampleConfigure(ADC0_BASE, 16);              // Hardware averaging

    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);                    // 0 is the sequencer number

    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH2);                          // read from input channel 2
    ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH8);                          // read from input channel 8
    ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH9|ADC_CTL_IE|ADC_CTL_END);   // read from input channel 9
    ADCSequenceEnable(ADC0_BASE, 0);
}

double new_sqrt(float num){
    float i=1;
    float sqr=i*i;
    float Resolution=0.05;

    while(num>sqr){
        i++;
        sqr=i*i;
    }

    if(num==sqr)
        return i;
    i--;
    sqr=i*i;

    while(num>sqr){
        i=i+Resolution;
        sqr=i*i;
    }   // end while
        return (i-Resolution);
}

void InitHardware(void){

    //Initialize system clock
    InitSystemClock();          // System clock 40Mhz
    InitGPIO();
    InitADC();
}
