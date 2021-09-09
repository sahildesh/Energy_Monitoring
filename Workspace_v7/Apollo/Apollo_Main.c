#include <stdint.h>
#include <stdbool.h>
#include <hw_memmap.h>
#include <tm4c123gh6pz.h>
#include <eeprom.h>
#include "Apollo_Header.h"
#include "Apollo_Functions.h"

GLOBAL_PARMS g_stApolloParms;
/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name:
Function Description:
**************************************************************************/
void main(void)
{
    InitHardware();

    while(1)
    {
    	APSM();
    } //End of while(1)
} //End of main
