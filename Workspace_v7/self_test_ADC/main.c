#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "Ark_functions.h"


double rms_calculator(double temp){
    return (new_sqrt(temp/1000));
}


/**
 * main.c
 */
int main(void)
{
    uint32_t ui32ADC0Value[8];
    double voltage;
    double reference;
    float ADC_value;
    double current;
    float ADC_current;
    double power;
    int counter=0;
    double temp_volt_rms=0;
    double temp_cur_rms=0;
    double rms_volt_val=0;
    double rms_cur_val=0;
    double rms_val=0;
    int temp=1;

    //SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    InitHardware();

    while(1)
        {
            ADCIntClear(ADC0_BASE, 0);
            ADCProcessorTrigger(ADC0_BASE, 0);

            while(!ADCIntStatus(ADC0_BASE, 0, false))
            {
            }

            ADCSequenceDataGet(ADC0_BASE, 0, ui32ADC0Value);

            reference = ui32ADC0Value[0] *(3.3/4096);

            // Read voltage
            voltage = (ui32ADC0Value[1]*(3.3/4096)-reference)*267;
            ADC_value = ui32ADC0Value[1];

            // Read Current
            current = ui32ADC0Value[2] * 3.3 / 4096;
            ADC_current = ui32ADC0Value[2];

            // RMS value calculate
            temp_volt_rms = temp_volt_rms + (voltage*voltage);
            temp_cur_rms = temp_cur_rms + (current*current);

            counter = counter + 1;
            if(counter % 1000 == 0){
                temp=temp+1;
                //rms_val=new_sqrt(temp_volt_rms/1000);
                rms_volt_val = rms_calculator(temp_volt_rms);
                rms_cur_val = rms_calculator(temp_cur_rms);
                temp_volt_rms=0;
                temp_cur_rms=0;
                //counter=0;
            }

        }
}
