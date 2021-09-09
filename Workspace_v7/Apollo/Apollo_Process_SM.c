#include <stdint.h>
#include <stdbool.h>
#include <tm4c123gh6pz.h>
#include <hw_memmap.h>
#include <gpio.h>
#include <pwm.h>
#include <eeprom.h>
#include "Apollo_Header.h"
#include "Apollo_Functions.h"
#include "EEPROM_Function.h"
#include "UART_Header.h"

extern GLOBAL_PARMS g_stApolloParms;

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name: ReadAndProcessLocalDIP_Switch
Function Description:
The 4 pole DIP switch controls the following
1. Seat clean function
2. Validation intelligence
3. ASI for PMO
**************************************************************************/
U32 ReadFeedback(PLC_OUTPUTS Arg_etPLC_Output)
{
	U32 ulFeedback = RESET;

	switch(Arg_etPLC_Output)
	{
	case ENERGIZED:
		break;
	case DE_ENERGIZED:
		break;
	case SEAT_LIFT_LOWER:
		break;
	case SEAT_LIFT_UPPER:
		break;
	}

	return(ulFeedback);
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name: ReadAndProcessLocalDIP_Switch
Function Description:
The 4 pole DIP switch controls the following
1. Seat clean function
2. Validation intelligence
3. ASI for PMO
**************************************************************************/
void SaveMR_PositionAsCurrent(PLC_OUTPUTS Arg_etPLC_Output)
{
	DEVICE_DATA *stApolloDeviceData = &(g_stApolloParms.m_stApolloDeviceData);

	switch(Arg_etPLC_Output)
	{
	case ENERGIZED:
		stApolloDeviceData->m_ulPosEnergised = stApolloDeviceData->m_ulPositionOfMagnet;
		break;
	case DE_ENERGIZED:
		stApolloDeviceData->m_ulPosDeEnergised = stApolloDeviceData->m_ulPositionOfMagnet;
		break;
	case SEAT_LIFT_LOWER:
		stApolloDeviceData->m_ulPosLowSeatLift = stApolloDeviceData->m_ulPositionOfMagnet;
		break;
	case SEAT_LIFT_UPPER:
		stApolloDeviceData->m_ulSensorStateActivated = READ_SENSOR_STATE;
		break;
	}
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name: ReadAndProcessLocalDIP_Switch
Function Description:
The 4 pole DIP switch controls the following
1. Seat clean function
2. Validation intelligence
3. ASI for PMO
**************************************************************************/
void ResetAllCalibrationData(void)
{
	DEVICE_DATA *stApolloDeviceData = &(g_stApolloParms.m_stApolloDeviceData);

	//Reset all the values and enter not calibrated mode
	stApolloDeviceData->m_ulPosDeEnergised = RESET;
	stApolloDeviceData->m_ulPosEnergised = RESET;
	stApolloDeviceData->m_ulPosLowSeatLift = RESET;
	stApolloDeviceData->m_ulTolDeEnHigh = RESET;
	stApolloDeviceData->m_ulTolDeEnLow = RESET;
	stApolloDeviceData->m_ulTolEnHigh = RESET;
	stApolloDeviceData->m_ulTolEnLow = RESET;
	stApolloDeviceData->m_ulTolSL_High = RESET;
	stApolloDeviceData->m_ulTolSL_Low = RESET;
	stApolloDeviceData->m_ulSensorStateActivated = RESET;
	stApolloDeviceData->m_ulSensorStateDeActivated = RESET;

	g_stApolloParms.m_etApolloStates = NOT_CALIBRATED_MODE;
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name:
Function Description: GiveFeedbackToPLC
1. This routine controls outputs to the PLC for three different confgs.
2. Apollo Digital
	a.
	b.
	c.
	d.
	e.

3. Apollo ASI
	a.
	b.
	c.
	d.

4. Apollo ASI for PMO
	a.
	b.
	c.
	d.
**************************************************************************/
void GiveFeedbackToPLC(PLC_OUTPUTS Arg_etPLC_Output, DEV_TYPE Arg_etDevice)
{
	switch(Arg_etDevice)
	{
	case APOLLO_DIGITAL:
		switch(Arg_etPLC_Output)
		{
		case NO_FEEDBACK:
			TURN_OFF_ALL_FEEDBACK;
			break;

		case DE_ENERGIZED:
			TURN_ON_PLC_FB_DEN;
			break;

		case ENERGIZED:
			TURN_ON_PLC_FB_EN;
			break;

		case SEAT_LIFT_LOWER:
			TURN_ON_PLC_FB_SLL;
			break;

		case SEAT_LIFT_UPPER:
			TURN_ON_PLC_FB_SLU;
			break;

		case STATUS_OK:
			TURN_ON_PLC_FB_SOK;
			break;
		}
		break;

	case APOLLO_ASI:
		switch(Arg_etPLC_Output)
		{
		case NO_FEEDBACK:
			TURN_OFF_ALL_FEEDBACK;
			break;

		case DE_ENERGIZED:
			TURN_ON_ASI_PMO_FB_DEN;
			break;

		case ENERGIZED:
			TURN_ON_PLC_FB_EN;
			break;

		case SEAT_LIFT_LOWER:
			TURN_ON_ASI_FB_AUX;
			break;

		case STATUS_OK:
			TURN_ON_PLC_FB_SOK;
			break;
		}
		break;

	case APOLLO_ASI_PMO:
		switch(Arg_etPLC_Output)
		{
		case NO_FEEDBACK:
			TURN_OFF_ALL_FEEDBACK;
			break;

		case DE_ENERGIZED:
			TURN_ON_ASI_PMO_FB_DEN;
			break;

		case SEAT_LIFT_LOWER:
			TURN_ON_PLC_FB_SLL;
			break;

		case SEAT_LIFT_UPPER:
			TURN_ON_PLC_FB_SLU;
			break;

		case STATUS_OK:
			TURN_ON_PLC_FB_SOK;
			break;
		}
		break;
	}
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name:CheckForOperationErrors
Function Description:
Routine checks for any operational errors existing
**************************************************************************/
void ValveOperationMonitor(void)
{
	static U32 ulErrorTimeout = RESET;
	DEVICE_DATA *stDeviceData = &(g_stApolloParms.m_stApolloDeviceData);
	U32 ulMagnetPosition = g_stApolloParms.m_stApolloDeviceData.m_ulPositionOfMagnet;

	// Monitor when input on SV1
	if(stDeviceData->m_ulInputOnSV_One == TRUE)
	{
		if((ulMagnetPosition < stDeviceData->m_ulTolEnLow) &&
			(ulMagnetPosition > stDeviceData->m_ulTolEnHigh))
		{
			ulErrorTimeout++;
			if(ulErrorTimeout > 1500)
			{
				ulErrorTimeout = RESET;
				g_stApolloParms.m_etApolloStates = OPERATION_ERROR;
			}
		}
		else
		{
			ulErrorTimeout = RESET;
		}
	}

	// Monitor when input on SV2
	if(stDeviceData->m_ulInputOnSV_Two == TRUE)
	{
		if((ulMagnetPosition < stDeviceData->m_ulTolSL_Low) &&
			(ulMagnetPosition > stDeviceData->m_ulTolSL_High))
		{
			ulErrorTimeout++;
			if(ulErrorTimeout > 1500)
			{
				ulErrorTimeout = RESET;
				g_stApolloParms.m_etApolloStates = OPERATION_ERROR;
			}
		}
		else
		{
			ulErrorTimeout = RESET;
		}
	}

	// Monitor when input on SV3
	if(stDeviceData->m_ulInputOnSV_Three == TRUE)
	{
		if((READ_SENSOR_STATE == stDeviceData->m_ulSensorStateDeActivated) ||
			(READ_SENSOR_STATE == stDeviceData->m_ulSensorStateDeActivated))
		{
			ulErrorTimeout++;
			if(ulErrorTimeout > 1500)
			{
				ulErrorTimeout = RESET;
				g_stApolloParms.m_etApolloStates = OPERATION_ERROR;
			}
		}
		else
		{
			ulErrorTimeout = RESET;
		}
	}

	// Monitor Current MR Positions
	// Solenoid valve 1
	if((ulMagnetPosition >= stDeviceData->m_ulTolEnLow) &&
		(ulMagnetPosition <= stDeviceData->m_ulTolEnHigh))
	{
		if(stDeviceData->m_ulInputOnSV_One == FALSE)
		{
			g_stApolloParms.m_etApolloStates = OPERATION_ERROR;
		}
	}

	// Solenoid valve 2
	if((ulMagnetPosition >= stDeviceData->m_ulTolSL_Low) &&
		(ulMagnetPosition <= stDeviceData->m_ulTolSL_High))
	{
		if(stDeviceData->m_ulInputOnSV_Two == FALSE)
		{
			g_stApolloParms.m_etApolloStates = OPERATION_ERROR;
		}
	}

	// Solenoid valve 3
	if(READ_SENSOR_STATE == stDeviceData->m_ulSensorStateDeActivated)
	{
		if(stDeviceData->m_ulInputOnSV_Three == FALSE)
		{
			g_stApolloParms.m_etApolloStates = OPERATION_ERROR;
		}
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
Function Description: ApolloOperation
1. This is the main device operation mode function
2. PLC outputs as per position of magnet are controlled from here
3. PLC inputs are registered and a respective action is taken
4. LEDs are controlled as per position of magnet
5. Solenoids are controlled as per PLC inputs
**************************************************************************/
APOLLO_STATES ApolloOperation(void)
{
	F32 ulMagnetPosition = g_stApolloParms.m_stApolloDeviceData.m_ulPositionOfMagnet;
	DEV_TYPE etDevice = g_stApolloParms.m_etDeviceType;
	DEVICE_DATA *stDeviceData = &(g_stApolloParms.m_stApolloDeviceData);

	if(g_stApolloParms.m_ul10MS_Timer == SET)
	{
		g_stApolloParms.m_ul10MS_Timer = RESET;
		// If position is detected between de energised tolerance band
	//	if((ulMagnetPosition >= stDeviceData->m_ulTolDeEnLow) && (ulMagnetPosition <= stDeviceData->m_ulTolDeEnHigh))
	//	{
			GiveFeedbackToPLC(DE_ENERGIZED, etDevice);
			LED_Control(LED_ON, GREEN_LED);
	//	}
		// If position is detected between energised tolerance band
		if((ulMagnetPosition >= stDeviceData->m_ulTolEnLow) && (ulMagnetPosition <= stDeviceData->m_ulTolEnHigh))
		{
			GiveFeedbackToPLC(ENERGIZED, etDevice);
			LED_Control(LED_ON, YELLOW_LED);
		}
		// If position is detected as lower seat lift tolerance band
		else if((ulMagnetPosition >= stDeviceData->m_ulTolSL_Low) && (ulMagnetPosition <= stDeviceData->m_ulTolSL_High))
		{
			GiveFeedbackToPLC(SEAT_LIFT_LOWER, etDevice);
			LED_Control(LED_ON, BLUE_LED);
		}

		// If position is detected as upper seat lift
		// THis has to be changed as this is going to be input from an external sensor TODO
		else if(READ_SENSOR_STATE == stDeviceData->m_ulSensorStateActivated)
		{
			GiveFeedbackToPLC(SEAT_LIFT_UPPER, etDevice);
			LED_Control(LED_ON, WHITE_LED);
		}
		else
		{
			GiveFeedbackToPLC(NO_FEEDBACK, etDevice);
			LED_Control(LED_OFF, ALL_LED);
		}

		// Check for input on the PLC triggers here
		// First check input on solenoid valve 1
		if(stDeviceData->m_ulInputOnSV_One == TRUE)
		{
			ActivateSolenoidValve(SOLENOID_1);
			if(stDeviceData->m_ulInputOnAutoSetup == TRUE)
			{
				stDeviceData->m_ulMainCleanFunction = YES;
				stDeviceData->m_ulAutoSetupInterlock = YES; // Auto setup interlock prevents the unit from going to the Auto Setup mode
			}
		}
		else if(stDeviceData->m_ulInputOnSV_Two == TRUE)
		{
			ActivateSolenoidValve(SOLENOID_2);
		}
		else if(stDeviceData->m_ulInputOnSV_Three == TRUE)
		{
			ActivateSolenoidValve(SOLENOID_3);
		}
		else
		{
			ActivateSolenoidValve(SOLENOID_OFF);
		}

		if(stDeviceData->m_ulInputOnAutoSetup == TRUE)
		{
			if(stDeviceData->m_ulAutoSetupInterlock == NO)
			{
				g_stApolloParms.m_etApolloStates = AUTO_CALIBRATION;
			}
		}

		if(stDeviceData->m_ulInputOnSV_One == FALSE)
		{
			stDeviceData->m_ulMainCleanFunction = NO;
		}

		ValveOperationMonitor();
	}
	return(g_stApolloParms.m_etApolloStates);
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name:
Function Description: ApolloNotCalibrated
1. Controls functionality of the device when no calibration parameters are
set
**************************************************************************/
APOLLO_STATES ApolloNotCalibrated(void)
{
	// Here we are in not calibrated mode
	DEV_TYPE etDevice = g_stApolloParms.m_etDeviceType;
	if(g_stApolloParms.m_ul10MS_Timer == RESET)
	{
		g_stApolloParms.m_ul10MS_Timer = RESET;
		// Check if there's any input on a solenoid valve
		if(g_stApolloParms.m_stApolloDeviceData.m_ulInputNoSV == TRUE)
		{
			LED_Control(TOGGLE_SLOW, GREEN_LED);
		}
		else if(g_stApolloParms.m_stApolloDeviceData.m_ulInputOnSV_One == TRUE)
		{
			LED_Control(TOGGLE_SLOW, YELLOW_LED);
		}
		else if(g_stApolloParms.m_stApolloDeviceData.m_ulInputOnSV_Two == TRUE)
		{
			LED_Control(TOGGLE_SLOW, BLUE_LED);
		}
		else if(g_stApolloParms.m_stApolloDeviceData.m_ulInputOnSV_Three == TRUE)
		{
			LED_Control(TOGGLE_SLOW, WHITE_LED);
		}

		// Enable the status output of the PLC
		GiveFeedbackToPLC(STATUS_OK, etDevice);

		// Check for calibration flag to be set
		if(g_stApolloParms.m_stApolloDeviceData.m_ulCalibrationFlag == SET)
		{
			g_stApolloParms.m_etApolloStates = OPERATION_MODE;
		}

		//Check for the Setup flag to be set
		if(g_stApolloParms.m_stApolloDeviceData.m_ulInputOnAutoSetup == TRUE)
		{
			g_stApolloParms.m_etApolloStates = AUTO_CALIBRATION;
		}
	}
	return(g_stApolloParms.m_etApolloStates);
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name:
Function Description: ApolloOperationError
1. If any error occurs during operation code will flow to this routine
**************************************************************************/
APOLLO_STATES ApolloOperationError(void)
{
	static U32 ulTimeoutTimer = RESET;
	DEVICE_DATA *stDeviceData = &(g_stApolloParms.m_stApolloDeviceData);
	U32 ulMagnetPosition = g_stApolloParms.m_stApolloDeviceData.m_ulPositionOfMagnet;

	if(g_stApolloParms.m_ul10MS_Timer == SET)
	{
		g_stApolloParms.m_ul10MS_Timer = RESET;
		switch(g_stApolloParms.m_etErrorConditions)
		{
		case NO_ERROR:
			// If there is no error then restore state of unit to operation or not calbrated after checking for calibration flag
			if(g_stApolloParms.m_stApolloDeviceData.m_ulCalibrationFlag == CALIBRATED)
			{
				g_stApolloParms.m_etApolloStates = OPERATION_MODE;
			}
			else
			{
				g_stApolloParms.m_etApolloStates = NOT_CALIBRATED_MODE;
			}
			break;

		case OPERATION_ERR_SOL1:
			LED_Control(TOGGLE_DOUBLE, RED_LED);
			if((ulMagnetPosition >= stDeviceData->m_ulTolEnLow) && (ulMagnetPosition <= stDeviceData->m_ulTolEnHigh))
			{
				g_stApolloParms.m_etApolloStates = OPERATION_MODE;
				g_stApolloParms.m_etErrorConditions = NO_ERROR;
			}
			break;

		case OPERATION_ERR_SOL2:
			LED_Control(TOGGLE_DOUBLE, RED_LED);
			if((ulMagnetPosition >= stDeviceData->m_ulTolSL_Low) && (ulMagnetPosition <= stDeviceData->m_ulTolSL_High))
			{
				g_stApolloParms.m_etApolloStates = OPERATION_MODE;
				g_stApolloParms.m_etErrorConditions = NO_ERROR;
			}
			break;

		case OPERATION_ERR_SOL3:
			if(READ_SENSOR_STATE == stDeviceData->m_ulSensorStateActivated)
			{
				g_stApolloParms.m_etApolloStates = OPERATION_MODE;
				g_stApolloParms.m_etErrorConditions = NO_ERROR;
			}
			break;

		case AUTOCAL_ERR:
			LED_Control(TOGGLE_DOUBLE, RED_LED);
			ulTimeoutTimer++;
			if(ulTimeoutTimer > 200)
			{
				if(g_stApolloParms.m_stApolloDeviceData.m_ulCalibrationFlag == CALIBRATED)
				{
					g_stApolloParms.m_etApolloStates = OPERATION_MODE;
					ulTimeoutTimer = RESET;
				}
				else
				{
					g_stApolloParms.m_etApolloStates = NOT_CALIBRATED_MODE;
					ulTimeoutTimer = RESET;
				}
			}
			break;
		}
	}
	return(g_stApolloParms.m_etApolloStates);
}
/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name:
Function Description: ApolloOperationError
1. If any error occurs during operation code will flow to this routine
**************************************************************************/
APOLLO_STATES ApolloTestMode(void)
{
	DEV_TYPE etDevice = g_stApolloParms.m_etDeviceType;
	static U32 ulTimer = RESET;
	static U32 ulProcessStart = RESET;

	if(g_stApolloParms.m_ul100MS_Timer == SET)
	{
		g_stApolloParms.m_ul100MS_Timer = RESET;
		switch(g_stApolloParms.m_etTestModeStates)
		{
		case TEST_ONE:
			if(ulTimer < 10)
			{
				if(ulProcessStart == RESET)
				{
					LED_Control(LED_ON, GREEN_LED);
					GiveFeedbackToPLC(DE_ENERGIZED, etDevice);
					ulProcessStart = SET;
				}
				ulTimer++;
			}
			else
			{
				LED_Control(LED_OFF, ALL_LED);
				GiveFeedbackToPLC(NO_FEEDBACK, etDevice);
				ActivateSolenoidValve(SOLENOID_OFF);
				ulTimer = RESET;
				ulProcessStart = RESET;
				g_stApolloParms.m_etTestModeStates = TEST_TWO;
			}
			break;

		case TEST_TWO:
			if(ulTimer < 10)
			{
				if(ulProcessStart == RESET)
				{
					LED_Control(LED_ON, YELLOW_LED);
					GiveFeedbackToPLC(ENERGIZED, etDevice);
					ActivateSolenoidValve(SOLENOID_1);
					ulProcessStart = SET;
				}
				ulTimer++;
			}
			else
			{
				LED_Control(LED_OFF, ALL_LED);
				GiveFeedbackToPLC(NO_FEEDBACK, etDevice);
				ActivateSolenoidValve(SOLENOID_OFF);
				ulTimer = RESET;
				ulProcessStart = RESET;
				g_stApolloParms.m_etTestModeStates = TEST_THREE;
			}
			break;

		case TEST_THREE:
			if(ulTimer < 10)
			{
				if(ulProcessStart == RESET)
				{
					LED_Control(LED_ON, BLUE_LED);
					GiveFeedbackToPLC(SEAT_LIFT_LOWER, etDevice);
					ActivateSolenoidValve(SOLENOID_2);
					ulProcessStart = SET;
				}
				ulTimer++;
			}
			else
			{
				LED_Control(LED_OFF, ALL_LED);
				GiveFeedbackToPLC(NO_FEEDBACK, etDevice);
				ActivateSolenoidValve(SOLENOID_OFF);
				ulTimer = RESET;
				ulProcessStart = RESET;
				g_stApolloParms.m_etTestModeStates = TEST_FOUR;
			}
			break;

		case TEST_FOUR:
			if(ulTimer < 10)
			{
				if(ulProcessStart == RESET)
				{
					LED_Control(LED_ON, WHITE_LED);
					GiveFeedbackToPLC(SEAT_LIFT_UPPER, etDevice);
					ActivateSolenoidValve(SOLENOID_3);
					ulProcessStart = SET;
				}
				ulTimer++;
			}
			else
			{
				LED_Control(LED_OFF, ALL_LED);
				GiveFeedbackToPLC(NO_FEEDBACK, etDevice);
				ActivateSolenoidValve(SOLENOID_OFF);
				ulTimer = RESET;
				ulProcessStart = RESET;
				g_stApolloParms.m_etTestModeStates = TEST_ONE;
			}
			break;
		}
	}
	return(g_stApolloParms.m_etApolloStates);
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name:
Function Description: APSM
This is the main state machine for Apollo
**************************************************************************/
void UpdateSystemTime(void)
{
	if(g_stApolloParms.m_ul30S_Timer == SET)
	{
		g_stApolloParms.m_ul30S_Timer = RESET;
		g_stApolloParms.m_stApolloDeviceData.m_ulSeconds++;
		if(g_stApolloParms.m_stApolloDeviceData.m_ulSeconds % 2 == RESET)
		{
			g_stApolloParms.m_stApolloDeviceData.m_ulMinutes++;
			EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulMinutes), EE_ADDR_OPERATION_TIME, SIZEOF_LONG);
			g_stApolloParms.m_stApolloDeviceData.m_ulSeconds = RESET;
		}
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
Function Description: APSM
This is the main state machine for Apollo
**************************************************************************/
void CheckPushButtonPress(void)
{
	// Check if the push button for entering calibration mode has been pressed
	if(g_stApolloParms.m_stPushButton.m_ulPB_Press1S == TRUE)
	{
		// If the button has been pressed then check whether the current state is measurement
		// If yes, allow the unit to finish the measurement and update state to calibration later
		// If no, change the state to calibration instantly
		if(g_stApolloParms.m_etApolloStates == MEASUREMENT)
		{
			g_stApolloParms.m_etPreviousState = AUTO_CALIBRATION;
		}
		else
		{
			g_stApolloParms.m_etApolloStates = AUTO_CALIBRATION;
		}
		g_stApolloParms.m_stPushButton.m_ulPB_Press1S = FALSE;
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
Function Description: APSM
This is the main state machine for Apollo
**************************************************************************/
void APSM(void)
{
	// These functions have to be executed in all but the Startup state
	if(g_stApolloParms.m_etApolloStates != STARTUP)
	{
		UpdateSystemTime();
		CheckPushButtonPress();
		SendUART_Menu();
	}

	// This is the Apollo Main state machine
	// All the control modes for Apollo will be entered from here
	switch(g_stApolloParms.m_etApolloStates)
	{
	case STARTUP:
		g_stApolloParms.m_etApolloStates = ApolloStartup();
		break;

	case NOT_CALIBRATED_MODE:
		g_stApolloParms.m_etApolloStates = ApolloNotCalibrated();
		break;

	case AUTO_CALIBRATION:
		g_stApolloParms.m_etApolloStates = ApolloAutoCalibration();
		break;

	case MEASUREMENT:
		g_stApolloParms.m_etApolloStates = ApolloMeasurement();
		break;

	case OPERATION_MODE:
		g_stApolloParms.m_etApolloStates = ApolloOperation();
		break;

	case OPERATION_ERROR:
		g_stApolloParms.m_etApolloStates = ApolloOperationError();
		break;

	case TEST_MODE:
		g_stApolloParms.m_etApolloStates = ApolloTestMode();
		break;
	}
}
