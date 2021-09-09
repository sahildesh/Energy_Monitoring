#include <stdint.h>
#include <stdbool.h>
#include <hw_types.h>
#include <hw_memmap.h>
#include <gpio.h>
#include <adc.h>
#include <timer.h>
#include <uart.h>
#include "Apollo_Header.h"
#include "Apollo_Functions.h"

extern GLOBAL_PARMS g_stApolloParms;
/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name:
Function Description:

Global Variables Referred:
1.
2.
3.

Global Variables Modified:
1.
2.
3.
**************************************************************************/
void ISR_ADC(void)
{
	ADCIntClear(ADC0_BASE, SQNC_0);
	g_stApolloParms.m_etConversionStatusMR = COMPLETE;
}

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
U32 ReadIncomingData(void)
{
	U32 ulData = RESET;
	static U32 ulPrvsData = RESET;
	static U32 ulDataBitCounter = RESET;
	static U32 ulEdgeDetect = RESET;

	ulData = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1);
	ulDataBitCounter++;

	if(ulDataBitCounter < 13)
	{
		if(ulPrvsData == ulData)
		{
			ulEdgeDetect++;
			if(ulEdgeDetect > 2)
			{
				g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataLost = YES;
			}
		}
		else
		{
			ulEdgeDetect = RESET;
		}
		ulPrvsData = ulData;
	}
	else if(ulDataBitCounter > 13)
	{
		g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataAvailable = YES;
		ulData = RESET;
		ulPrvsData = RESET;
	}
	return(ulData);
}

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
void PortB_Interrupt_Handler(void)
{
	U32 ulStatus = GPIOIntStatus(GPIO_PORTB_BASE, TRUE);
	static U32 ulProcessStarted = RESET;
	LOC_ADDR_PARMS *stLoc_Addr = &(g_stApolloParms.m_stASI_Parms.m_stLoc_Addr);
	switch(ulStatus)
	{
	case GPIO_PIN_1:
		g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulInputLED_Pin = YES;
		if(ulProcessStarted == RESET)
		{
			stLoc_Addr->m_ulReadDataFromPort = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1);
			TimerEnable(TIMER3_BASE, TIMER_BOTH);
			g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataReadTimer = RESET;
			stLoc_Addr->m_ulReadDataFromPort <<= LEFT_SHIFT_BY_ONE;
			ulProcessStarted = YES;
		}
		else
		{
			if(g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataReadTimer == SET)
			{
				g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataReadTimer = RESET;
				stLoc_Addr->m_ulReadDataFromPort = ReadIncomingData();
				stLoc_Addr->m_ulReadDataFromPort <<= LEFT_SHIFT_BY_ONE;
			}
		}
		break;

	case GPIO_PIN_2:
		g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulInputPST_Pin = YES;
		break;

	case GPIO_PIN_3:
		g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulInputDSR_Pin = YES;
		break;
	}
	GPIOIntClear(GPIO_PORTB_BASE, ulStatus);
}

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
void PortE_Interrupt_Handler(void)
{
	U32 ulStatus = GPIOIntStatus(GPIO_PORTE_BASE, TRUE);

	GPIOIntClear(GPIO_PORTE_BASE, ulStatus);
	g_stApolloParms.m_stApolloDeviceData.m_ulInputOnExtSensor = TRUE;
}

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
void PortF_Interrupt_Handler(void)
{
	U32 ulStatus = GPIOIntStatus(GPIO_PORTF_BASE, TRUE);
	switch(ulStatus)
	{
	case GPIO_PIN_0:
		if((GPIOPinRead(GPIO_PORTF_BASE, ulStatus)) != LOW)
		{
			g_stApolloParms.m_stApolloDeviceData.m_ulInputOnSV_One = TRUE;
			g_stApolloParms.m_stApolloDeviceData.m_ulInputNoSV = FALSE;
		}
		else
		{
			g_stApolloParms.m_stApolloDeviceData.m_ulInputOnSV_One = FALSE;
		}
		break;

	case GPIO_PIN_1:
		if((GPIOPinRead(GPIO_PORTF_BASE, ulStatus)) != LOW)
		{
			g_stApolloParms.m_stApolloDeviceData.m_ulInputOnSV_Two = TRUE;
			g_stApolloParms.m_stApolloDeviceData.m_ulInputNoSV = FALSE;
		}
		else
		{
			g_stApolloParms.m_stApolloDeviceData.m_ulInputOnSV_Two = FALSE;
		}
		break;

	case GPIO_PIN_2:
		if((GPIOPinRead(GPIO_PORTF_BASE, ulStatus)) != LOW)
		{
			g_stApolloParms.m_stApolloDeviceData.m_ulInputOnSV_Three = TRUE;
			g_stApolloParms.m_stApolloDeviceData.m_ulInputNoSV = FALSE;
		}
		else
		{
			g_stApolloParms.m_stApolloDeviceData.m_ulInputOnSV_Three = FALSE;
		}
		break;

	case GPIO_PIN_3:
		if((GPIOPinRead(GPIO_PORTF_BASE, ulStatus)) != LOW)
		{
			g_stApolloParms.m_stApolloDeviceData.m_ulInputOnAutoSetup = TRUE;
		}
		else
		{
			g_stApolloParms.m_stApolloDeviceData.m_ulInputOnAutoSetup = FALSE;
		}
		break;
	}

	if(g_stApolloParms.m_stApolloDeviceData.m_ulInputOnSV_One == FALSE)
	{
		if(g_stApolloParms.m_stApolloDeviceData.m_ulInputOnSV_Two == FALSE)
		{
			if(g_stApolloParms.m_stApolloDeviceData.m_ulInputOnSV_Three == FALSE)
			{
				g_stApolloParms.m_stApolloDeviceData.m_ulInputNoSV = TRUE;
			}
		}
	}
	GPIOIntClear(GPIO_PORTF_BASE, ulStatus);
}

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
void ISR_UART_Receive(void)
{
    U32 ulStatus = RESET;

    ulStatus = UARTIntStatus(UART0_BASE, true);

    UARTIntClear(UART0_BASE, ulStatus);

    if(UARTCharsAvail(UART0_BASE) == TRUE)
    {
    	g_stApolloParms.m_stUART_Parms.ulDataAvailableOnUART = YES;
    	g_stApolloParms.m_stUART_Parms.ulDataOnUART = UARTCharGetNonBlocking(UART0_BASE);
    	if(g_stApolloParms.m_stUART_Parms.etMenuStates == NO_MENU)
    	{
    		g_stApolloParms.m_stUART_Parms.etMenuStates = MAIN_MENU;
    	}
    }
}
