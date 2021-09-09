#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <pin_map.h>
#include <tm4c123gh6pz.h>
#include <hw_memmap.h>
#include <gpio.h>
#include <uart.h>
#include "Apollo_Header.h"
#include "Apollo_Functions.h"
#include "UART_Header.h"

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
**************************************************************************/
void SendUART_String(U8 *Arg_ucUART_Data, U32 Arg_ulCarriageReturn)
{
	U32 ulDataPointer = RESET;
	U32 ulStringLength = RESET;
	ulStringLength = strlen(Arg_ucUART_Data);
	for(ulDataPointer = RESET; ulDataPointer < ulStringLength; ulDataPointer++)
	{
		UARTCharPut(UART0_BASE, Arg_ucUART_Data[ulDataPointer]);
	}
	if(Arg_ulCarriageReturn == WITH_CARRIAGE_RETURN)
	{
//		UARTCharPut(UART0_BASE, NEW_LINE);
		UARTCharPut(UART0_BASE, CARRIAGE_RETURN);
	}
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
UART_MENU_STATES UART_AdvancedDiagnosticMenu(void)
{
	MR_ARRAY *stMR_ArrayParms = &(g_stApolloParms.m_stMR_ArrayParms);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	// MR array data values
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Advanced Diagnostics");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "MR Sensor 1 (ADC Value): %4d", stMR_ArrayParms->m_ulSensorValue[SENSOR_1]);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "MR Sensor 2 (ADC Value): %4d", stMR_ArrayParms->m_ulSensorValue[SENSOR_2]);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "MR Sensor 3 (ADC Value): %4d", stMR_ArrayParms->m_ulSensorValue[SENSOR_3]);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "MR Sensor 4 (ADC Value): %4d", stMR_ArrayParms->m_ulSensorValue[SENSOR_4]);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "MR Sensor 5 (ADC Value): %4d", stMR_ArrayParms->m_ulSensorValue[SENSOR_5]);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "MR Sensor 6 (ADC Value): %4d", stMR_ArrayParms->m_ulSensorValue[SENSOR_6]);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "MR Sensor 7 (ADC Value): %4d", stMR_ArrayParms->m_ulSensorValue[SENSOR_7]);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "MR Sensor 8 (ADC Value): %4d", stMR_ArrayParms->m_ulSensorValue[SENSOR_8]);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Press any key to return to main menu");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	g_stApolloParms.m_stUART_Parms.etMenuStates = NO_MENU;
	return(g_stApolloParms.m_stUART_Parms.etMenuStates);
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
UART_MENU_STATES UART_AdvancedSettingsMenu(void)
{
	return(g_stApolloParms.m_stUART_Parms.etMenuStates);
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
UART_MENU_STATES UART_ManualSetupMenu(void)
{
	U32 ulFeedback = RESET;
	static U32 ulMenuDisplayed = RESET;

	if(ulMenuDisplayed == RESET)
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Manual Setup Menu");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "r - Reset Setup Values");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "a - Start Auto Setup");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "d - Read De Energised Feedback");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "e - Read Energised Feedback");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "l - Read AUX1 (LSL) Feedback");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "u - Read AUX2 (USL) Feedback");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "f - Set Solenoid Valve 1");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "v - Reset Solenoid Valve 1");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "g - Set Solenoid Valve 2");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "b - Reset Solenoid Valve 2");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "h - Set Solenoid Valve 3");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "n - Reset Solenoid Valve 3");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "z - Save MR Position as De energised");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "x - Save MR Position as Energised");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "c - Save MR Position as AUX2 (LSL)");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Y - Save MR Position as AUX2 (USL)");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "o - Set butterfly valve tolerance band");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "p - Set seat valve tolerance band");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Enter your choice or press any other key to go back to main menu: ");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = NO;
		ulMenuDisplayed = SET;
	}

	switch(g_stApolloParms.m_stUART_Parms.ulDataOnUART)
	{
	case ASCII_A:
		if(g_stApolloParms.m_stUART_Parms.ulAutoSetupFromUART == TRUE)
		{
			sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "I");
			SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		}
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = NO;
		g_stApolloParms.m_etApolloStates = AUTO_CALIBRATION;
		g_stApolloParms.m_stUART_Parms.ulAutoSetupFromUART = TRUE;
		g_stApolloParms.m_stUART_Parms.etMenuStates = MAIN_MENU;
		break;

	case ASCII_B:
		ActivateSolenoidValve(SOLENOID_OFF);
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "B");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		break;

	case ASCII_C:
		SaveMR_PositionAsCurrent(SEAT_LIFT_LOWER);
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "C");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		break;

	case ASCII_D:
		ulFeedback = ReadFeedback(DE_ENERGIZED);
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "De energised Feedback: %d", ulFeedback);
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "D");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		break;

	case ASCII_E:
		ulFeedback = ReadFeedback(ENERGIZED);
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Energised Feedback: %d", ulFeedback);
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "E");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		break;

	case ASCII_F:
		ActivateSolenoidValve(SOLENOID_1);
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "F");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		break;

	case ASCII_G:
		ActivateSolenoidValve(SOLENOID_2);
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "G");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		break;

	case ASCII_H:
		ActivateSolenoidValve(SOLENOID_3);
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "H");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		break;

	case ASCII_L:
		ulFeedback = ReadFeedback(SEAT_LIFT_LOWER);
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Lower Seat Lift Feedback: %d", ulFeedback);
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "L");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		break;

	case ASCII_N:
		ActivateSolenoidValve(SOLENOID_OFF);
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "N");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		break;

	case ASCII_O:
		SetToleranceBands(BUTTERFLY_VALVE);
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "O");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

		break;

	case ASCII_P:
		SetToleranceBands(SEAT_VALVE);
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "P");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		break;

	case ASCII_R:
		ResetAllCalibrationData(); //Resets all the calibration data and enters not calibrated mode
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "R");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		break;

	case ASCII_U:
		ulFeedback = ReadFeedback(SEAT_LIFT_UPPER);
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Upper Seat Lift: %d", ulFeedback);
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "U");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		break;

	case ASCII_V:
		ActivateSolenoidValve(SOLENOID_OFF);
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "V");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		break;

	case ASCII_Y:
		SaveMR_PositionAsCurrent(SEAT_LIFT_UPPER);
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Y");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		break;

	case ASCII_X:
		SaveMR_PositionAsCurrent(ENERGIZED);
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "X");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		break;

	case ASCII_Z:
		SaveMR_PositionAsCurrent(DE_ENERGIZED);
		g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Z");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		break;
	}
	return(g_stApolloParms.m_stUART_Parms.etMenuStates);
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
UART_MENU_STATES UART_ManualOperationMenu(void)
{
	static U32 ulMenuDisplayed = RESET;
	static U32 ulProcessStart = RESET;

	if(ulMenuDisplayed == RESET)
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Manual Operation Menu");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "f - Toggle Solenoid Valve 1");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "g - Toggle Solenoid Valve 2");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "h - Toggle Solenoid Valve 3");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "t - Toggle Test mode");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		ulMenuDisplayed = SET;
	}
	switch(g_stApolloParms.m_stUART_Parms.ulDataOnUART)
	{
		case ASCII_F:
			if(ulProcessStart == RESET)
			{
				ActivateSolenoidValve(SOLENOID_1);
				g_stApolloParms.m_stUART_Parms.ulDataOnUART = NO;
				ulProcessStart = SET;
			}
			else
			{
				ActivateSolenoidValve(SOLENOID_OFF);
				g_stApolloParms.m_stUART_Parms.ulDataOnUART = NO;
				ulProcessStart = RESET;
			}
			break;

		case ASCII_G:
			if(ulProcessStart == RESET)
			{
				ActivateSolenoidValve(SOLENOID_2);
				g_stApolloParms.m_stUART_Parms.ulDataOnUART = NO;
				ulProcessStart = SET;
			}
			else
			{
				ActivateSolenoidValve(SOLENOID_OFF);
				g_stApolloParms.m_stUART_Parms.ulDataOnUART = NO;
				ulProcessStart = RESET;
			}
			break;

		case ASCII_H:
			if(ulProcessStart == RESET)
			{
				ActivateSolenoidValve(SOLENOID_3);
				g_stApolloParms.m_stUART_Parms.ulDataOnUART = NO;
				ulProcessStart = SET;
			}
			else
			{
				ActivateSolenoidValve(SOLENOID_OFF);
				g_stApolloParms.m_stUART_Parms.ulDataOnUART = NO;
				ulProcessStart = RESET;
			}
			break;

		case ASCII_T:
			if(ulProcessStart == RESET)
			{
				if(g_stApolloParms.m_etApolloStates != MEASUREMENT)
				{
					g_stApolloParms.m_etApolloStates = TEST_MODE;
				}
				else
				{
					g_stApolloParms.m_etPreviousState = TEST_MODE;
				}
				g_stApolloParms.m_stUART_Parms.ulDataOnUART = NO;
				ulProcessStart = SET;
			}
			else
			{
				if(g_stApolloParms.m_stApolloDeviceData.m_ulCalibrationFlag == CALIBRATED)
				{
					g_stApolloParms.m_etPreviousState = OPERATION_MODE;
				}
				else
				{
					g_stApolloParms.m_etPreviousState = NOT_CALIBRATED_MODE;
				}
				g_stApolloParms.m_stUART_Parms.ulDataOnUART = NO;
				ulProcessStart = RESET;
			}
			break;
	}
		return(g_stApolloParms.m_stUART_Parms.etMenuStates);
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
UART_MENU_STATES UART_SetupValuesMenu(void)
{
	DEVICE_DATA *stApolloDeviceData = &(g_stApolloParms.m_stApolloDeviceData);

	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Setup Values");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	//Type of valve and tolerance band
	if(stApolloDeviceData->m_etValveType == SEAT_VALVE)
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Tolerance Band: +-1.4mm");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Valve Type: SEAT VALVE");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	}
	else
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Tolerance Band: +-5mm");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Valve Type: BUTTERFLY VALVE");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	}

	// Positions and external sensor states
	// De energised
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Position De Energised: %2.3f", (F32)(stApolloDeviceData->m_ulPosDeEnergised));
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "External Sensor: %d", stApolloDeviceData->m_ulSensorStateDeEnergised);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	// Energised
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Position De Energised: %2.3f", (F32)(stApolloDeviceData->m_ulPosEnergised));
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "External Sensor: %d", stApolloDeviceData->m_ulSensorStateEnergised);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	// Lower seat lift
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Position De Energised: %2.3f", (F32)(stApolloDeviceData->m_ulPosLowSeatLift));
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "External Sensor: %d", stApolloDeviceData->m_ulSensorStateLowerSeatLift);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Press any key to return to main menu");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	g_stApolloParms.m_stUART_Parms.etMenuStates = NO_MENU;
	return(g_stApolloParms.m_stUART_Parms.etMenuStates);
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
UART_MENU_STATES UART_Daignostic(void)
{
	U32 ulCurrentFeedback = RESET;
	LOC_ADDR_PARMS *stLoc_Addr = &(g_stApolloParms.m_stASI_Parms.m_stLoc_Addr);
	DEVICE_DATA *stApolloDeviceData = &(g_stApolloParms.m_stApolloDeviceData);

	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Show Diagnostic:");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	//Level 2 menu
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Status:");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	//Device type
	if(g_stApolloParms.m_etDeviceType == APOLLO_DIGITAL)
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Type: D70");
	}
	else if(g_stApolloParms.m_etDeviceType == APOLLO_ASI || g_stApolloParms.m_etDeviceType == APOLLO_ASI_PMO)
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Type: B70");
	}
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	//Current mode
	if(g_stApolloParms.m_stApolloDeviceData.m_ulCalibrationFlag == SET)
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Mode: Operation");
	}
	else
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Mode: Not Calibrated");
	}
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	//Current Position
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Position: %0.2f", g_stApolloParms.m_stApolloDeviceData.m_ulPositionOfMagnet/10);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	//Outputs
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Outputs:");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	ulCurrentFeedback = GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_0);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "De-energised Feedback: %1d", ulCurrentFeedback);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	ulCurrentFeedback = GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_1);
	ulCurrentFeedback >>= 1;
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Energised Feedback: %1d", ulCurrentFeedback);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	if(g_stApolloParms.m_etDeviceType == APOLLO_DIGITAL)
	{
		ulCurrentFeedback = GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_2);
		ulCurrentFeedback >>= 2;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Lower Seat Lift: %1d", ulCurrentFeedback);
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

		ulCurrentFeedback = GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_3);
		ulCurrentFeedback >>= 3;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Upper Seat Lift: %1d", ulCurrentFeedback);
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

		ulCurrentFeedback = GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_4);
		ulCurrentFeedback >>= 4;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Status: %1d", ulCurrentFeedback);
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	}

	if(g_stApolloParms.m_etDeviceType == APOLLO_ASI)
	{
		ulCurrentFeedback = GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_2);
		ulCurrentFeedback >>= 2;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Sum Auxiliary: %1d", ulCurrentFeedback);
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

		ulCurrentFeedback = GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_4);
		ulCurrentFeedback >>= 4;
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Status: %1d", ulCurrentFeedback);
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	}
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	//Inputs
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Inputs:");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	ulCurrentFeedback = RESET;
	ulCurrentFeedback = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Setup: %1d", ulCurrentFeedback);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	ulCurrentFeedback = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1);
	ulCurrentFeedback >>= 1;
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Solenoid 1: %1d", ulCurrentFeedback);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	ulCurrentFeedback = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2);

	ulCurrentFeedback >>= 2;
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Solenoid 2: %1d", ulCurrentFeedback);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	ulCurrentFeedback = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3);
	ulCurrentFeedback >>= 3;
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Solenoid 3: %1d", ulCurrentFeedback);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	//ASI Profile and Slave Address

	if((g_stApolloParms.m_etDeviceType == APOLLO_ASI) || (g_stApolloParms.m_etDeviceType == APOLLO_ASI_PMO))
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "ASI Profile: %x%x%x%x", stLoc_Addr->m_ulID_Code,
													stLoc_Addr->m_ulID_Code2, stLoc_Addr->m_ulIO_Code, stLoc_Addr->m_ulID_CodeExt);
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

		if(g_stApolloParms.m_stASI_Parms.m_stASI_DIP_Switches.m_ulSlaveSelect == SLAVE_A)
		{
			sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "ASI Slave Address: %dA", stLoc_Addr->m_ulSlaveAddress);
			SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		}
		else
		{
			sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "ASI Slave Address: %dB", stLoc_Addr->m_ulSlaveAddress);
			SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		}
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	}

	// External connections
	// External inductive sensor
	if(stApolloDeviceData->m_ulSensorStateActivated != stApolloDeviceData->m_ulSensorStateDeActivated)
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "External Sensor: Connected");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	}
	else
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "External Sensor: Not Connected");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	}
	//Solenoid 1
	if(stApolloDeviceData->m_ulSolenoidOne == PRESENT)
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Solenoid Valve 1: Connected");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	}
	else
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Solenoid Valve 1: Not Connected");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	}
	//Solenoid 2
	if(stApolloDeviceData->m_ulSolenoidOne == PRESENT)
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Solenoid Valve 2: Connected");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	}
	else
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Solenoid Valve 2: Not Connected");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	}
	//Solenoid 3
	if(stApolloDeviceData->m_ulSolenoidOne == PRESENT)
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Solenoid Valve 3: Connected");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	}
	else
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Solenoid Valve 3: Not Connected");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	}
	//Solenoid 1,2 and 3 activations
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Total SV1 Activations: %6d", stApolloDeviceData->m_ulSolenoidOneActivations);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Total SV2 Activations: %6d", stApolloDeviceData->m_ulSolenoidTwoActivations);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Total SV3 Activations: %6d", stApolloDeviceData->m_ulSolenoidThreeActivations);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
	// Total operation time
	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Total Operation Time: %3dY, %3dD, %3dH", stApolloDeviceData->m_ulYear,
																		stApolloDeviceData->m_ulDays, stApolloDeviceData->m_ulHours);
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Press any key to return to main menu");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "");
	SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);

	g_stApolloParms.m_stUART_Parms.etMenuStates = NO_MENU;

	return(g_stApolloParms.m_stUART_Parms.etMenuStates);
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
UART_MENU_STATES UART_MainMenu(void)
{
	static U32 ulMenuDisplayed = RESET;

	if(g_stApolloParms.m_stUART_Parms.ulDataAvailableOnUART == YES)
	{
		g_stApolloParms.m_stUART_Parms.ulDataAvailableOnUART = NO;
		if(ulMenuDisplayed == RESET)
		{
			g_stApolloParms.m_stUART_Parms.ulDataOnUART = RESET;
			sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "");
			SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
			sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "1: Show Diagnostic");
			SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
			sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "2: Show Setup Values");
			SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
			sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "3: Manual Operation");
			SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
			sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "4: Manual Setup");
			SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
			sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "5: Advanced Settings");
			SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
			sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "6: Advanced Diagnostic");
			SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
			sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "Please enter your choice: ");
			SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITHOUT_CARRIAGE_RETURN);
			sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "");
			SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
			ulMenuDisplayed = YES;
		}

		else if(ulMenuDisplayed == YES)
		{
			ulMenuDisplayed = NO;
			switch(g_stApolloParms.m_stUART_Parms.ulDataOnUART)
			{
				case ASCII_ONE:
					g_stApolloParms.m_stUART_Parms.etMenuStates = DIAGNOSTIC_MENU;
					break;

				case ASCII_TWO:
					g_stApolloParms.m_stUART_Parms.etMenuStates = SETUP_VALUES_MENU;
					break;

				case ASCII_THREE:
					g_stApolloParms.m_stUART_Parms.etMenuStates = MANUAL_OPERATION_MENU;
					break;

				case ASCII_FOUR:
					g_stApolloParms.m_stUART_Parms.etMenuStates = MANUAL_SETUP_MENU;
					break;

				case ASCII_FIVE:
					g_stApolloParms.m_stUART_Parms.etMenuStates = ADVACNED_SETTINGS_MENU;
					break;

				case ASCII_SIX:
					g_stApolloParms.m_stUART_Parms.etMenuStates = ADVANCED_DIAGNOSTIC_MENU;
					break;
			}
		}
	}
	return(g_stApolloParms.m_stUART_Parms.etMenuStates);
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
void SendUART_Menu(void)
{
	switch(g_stApolloParms.m_stUART_Parms.etMenuStates)
	{
	case MAIN_MENU:
		g_stApolloParms.m_stUART_Parms.etMenuStates = UART_MainMenu();
		break;

	case DIAGNOSTIC_MENU:
		g_stApolloParms.m_stUART_Parms.etMenuStates = UART_Daignostic();
		break;

	case SETUP_VALUES_MENU:
		g_stApolloParms.m_stUART_Parms.etMenuStates = UART_SetupValuesMenu();
		break;

	case MANUAL_OPERATION_MENU:
		g_stApolloParms.m_stUART_Parms.etMenuStates = UART_ManualOperationMenu();
		break;

	case MANUAL_SETUP_MENU:
		g_stApolloParms.m_stUART_Parms.etMenuStates = UART_ManualSetupMenu();
		break;

	case ADVACNED_SETTINGS_MENU:
		g_stApolloParms.m_stUART_Parms.etMenuStates = UART_AdvancedSettingsMenu();
		break;

	case ADVANCED_DIAGNOSTIC_MENU:
		g_stApolloParms.m_stUART_Parms.etMenuStates = UART_AdvancedDiagnosticMenu();
		break;

	default:
		break;
	}
}
