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
Function Name:
Function Description:
**************************************************************************/
void EEPROMWriteOperation(U32 *Arg_pParmRAM_Addr, U32 Arg_ParmEEPROM_Addr, U32 Arg_ucSizeInBytes)
{
	U32 ulStatus = RESET;

//	write once
	ulStatus = EEPROMProgram(Arg_pParmRAM_Addr, Arg_ParmEEPROM_Addr, Arg_ucSizeInBytes);
	if(ulStatus != RESET)
	{
//		failure occur once; try again
		ulStatus = EEPROMProgram(Arg_pParmRAM_Addr, Arg_ParmEEPROM_Addr, Arg_ucSizeInBytes);
		if(ulStatus != RESET)
		{
//			failure twice; try once more
			ulStatus = EEPROMProgram(Arg_pParmRAM_Addr, Arg_ParmEEPROM_Addr, Arg_ucSizeInBytes);
		}
	}

	switch(ulStatus)
	{
		case EEPROM_RC_INVPL: //0x00000100
			/*
			 * failed to write a value due to the voltage level dropping below
			 * that	required for EEPROM programming
			 */
		case EEPROM_RC_WRBUSY: //0x00000020
			/*
			 * an attempt was made to read from the EEPROM while
			 * a write operation was in progress
			 */
			break;

		case EEPROM_RC_NOPERM: //0x00000010
			/*
			 * an attempt was made to write a value but the destination
			 * permissions disallow write operations.  This may be due to
			 * the destination block being locked, access protection set
			 * to prohibit writes or an attempt to write a password when one is
			 * already written
			 */
		case EEPROM_RC_WKCOPY: //0x00000008
			/*
			 * EEPROM programming state machine is currently copying to or
			 * from the internal copy buffer to make room for newly written value.
			 * It is provided as a status indicator and does not indicate an error
			 */
		case EEPROM_RC_WKERASE: //0x00000004
			/*
			 * EEPROM programming state machine is currently erasing
			 * the internal copy buffer.  It is provided as a status indicator
			 * and does not indicate an error
			 */

		case EEPROM_RC_WORKING: //0x00000001
			/*
			 * the EEPROM programming state machine is currently working.
			 * No new write operations should be attempted until this bit is clear.
			 */
			break;

		default:
			break;
	} // End of switch for EEPROM errors
} //End of function


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
void SaveCalibrationData(void)
{
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulPosEnergised), EE_ADDR_ENERGIZED, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulPosDeEnergised), EE_ADDR_DE_ENERGIZED, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulPosLowSeatLift), EE_ADDR_LSL_VALUE, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulPosUppSeatLift), EE_ADDR_USL_VALUE, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulTolEnHigh), EE_ADDR_ENG_TOL_HIGH, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulTolEnLow), EE_ADDR_ENG_TOL_LOW, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulTolDeEnHigh), EE_ADDR_DNG_TOL_HIGH, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulTolDeEnLow), EE_ADDR_DNG_TOL_LOW, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulTolSL_High), EE_ADDR_SL_TOL_HIGH, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulTolSL_Low), EE_ADDR_SL_TOL_LOW, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_etValveType), EE_ADDR_VALVE_TYPE, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulMinutes), EE_ADDR_OPERATION_TIME, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulStrokeCount), EE_ADDR_NO_OF_STROKES, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulDate), EE_ADDR_DATE_FW, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulFirmwareVersion), EE_ADDR_FW_REVISION, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulPCB_Version), EE_ADDR_PCB_REVISION, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulMaximumTemparature), EE_ADDR_MAX_TEMP, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulSensorStateActivated), EE_ADDR_EXTSEN_ON, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulSensorStateDeActivated), EE_ADDR_EXTSEN_OFF, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulSensorStateDeEnergised), EE_ADDR_EXTSEN_DEN, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulSensorStateEnergised), EE_ADDR_EXTSEN_EN, SIZEOF_LONG);
	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulSensorStateLowerSeatLift), EE_ADDR_EXTSEN_LSL, SIZEOF_LONG);

	EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulCalibrationFlag), EE_ADDR_CALIBRATION, SIZEOF_LONG);

	if(g_stApolloParms.m_stUART_Parms.ulAutoSetupFromUART == TRUE)
	{
		sprintf(g_stApolloParms.m_stUART_Parms.ucUART_Data, "C");
		SendUART_String(g_stApolloParms.m_stUART_Parms.ucUART_Data, WITH_CARRIAGE_RETURN);
		g_stApolloParms.m_stUART_Parms.ulAutoSetupFromUART = FALSE;
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
void SetToleranceBands(VALVES Arg_etValveType)
{
	DEVICE_DATA *stDeviceData = &(g_stApolloParms.m_stApolloDeviceData);

	if(g_stApolloParms.m_stApolloDeviceData.m_etValveType == SEAT_VALVE)
	{
		stDeviceData->m_ulTolDeEnLow = stDeviceData->m_ulPosDeEnergised - 1400;		//Tolerance band TBU TODO
		stDeviceData->m_ulTolDeEnHigh = stDeviceData->m_ulPosDeEnergised + 1400;
		stDeviceData->m_ulTolEnLow = stDeviceData->m_ulPosEnergised - 1400;
		stDeviceData->m_ulTolEnHigh = stDeviceData->m_ulPosEnergised + 1400;
		stDeviceData->m_ulTolSL_Low = stDeviceData->m_ulPosEnergised - 1000;
		stDeviceData->m_ulTolSL_High = stDeviceData->m_ulPosEnergised + 1000;
	}

	if(g_stApolloParms.m_stApolloDeviceData.m_etValveType == BUTTERFLY_VALVE)
	{
		stDeviceData->m_ulTolDeEnLow = stDeviceData->m_ulPosDeEnergised - 5000;		//Tolerance band TBU TODO
		stDeviceData->m_ulTolDeEnHigh = stDeviceData->m_ulPosDeEnergised + 5000;
		stDeviceData->m_ulTolEnLow = stDeviceData->m_ulPosEnergised - 5000;
		stDeviceData->m_ulTolEnHigh = stDeviceData->m_ulPosEnergised + 5000;
		stDeviceData->m_ulTolSL_Low = stDeviceData->m_ulPosEnergised - 5000;
		stDeviceData->m_ulTolSL_High = stDeviceData->m_ulPosEnergised + 5000;
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
void LED_Control(LED_STATE Arg_etLED_State, U32 Arg_ulLED_PWM)
{
	g_stApolloParms.m_stLED_Control.m_ulLED_PWM = Arg_ulLED_PWM;
	g_stApolloParms.m_stLED_Control.m_etLED_State = Arg_etLED_State;
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
AUTO_CALIBRATION_STATES SolenoidTerminalCheck(void)
{
	U32 ulSolenoidInputsOff = RESET;
	if((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == LOW) && (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1) == LOW) &&
			(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2) == LOW))
	{
		ulSolenoidInputsOff = YES;
	}
	if(ulSolenoidInputsOff == YES)
	{
		g_stApolloParms.m_stAutoCalParms.m_ulIsMagnetSteady = NO;
		g_stApolloParms.m_stAutoCalParms.m_ulAutoCalP1Over = NO;
		g_stApolloParms.m_stAutoCalParms.m_ulAutoCalP2Over = NO;
		g_stApolloParms.m_stAutoCalParms.m_ulAutoCalP3Over = NO;
		g_stApolloParms.m_etAutoCalStates = MAGNET_RANGE_CHECK;
	}
	return(g_stApolloParms.m_etAutoCalStates);
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
AUTO_CALIBRATION_STATES MagnetRangeCheck(void)
{
	if(g_stApolloParms.m_stApolloDeviceData.m_ulPositionOfMagnet < LOWER_POS_LIMIT ||
		g_stApolloParms.m_stApolloDeviceData.m_ulPositionOfMagnet > HIGH_POS_LIMIT)
	{
		g_stApolloParms.m_etAutoCalStates = AUTO_CAL_ERROR;
	}
	else
	{
		g_stApolloParms.m_etAutoCalStates = AUTO_CAL_PART_ONE;
	}
	return(g_stApolloParms.m_etAutoCalStates);
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
U32 MagnetSteadyCheck(void)
{
	static U32 ulPrvsDistance = RESET;
	U32 ulDistanceError = RESET;
	U32 ulIsMagnetSteady = RESET;
	U32 ulCurrentDistance = g_stApolloParms.m_stApolloDeviceData.m_ulPositionOfMagnet;

	if(g_stApolloParms.m_ul50MS_Timer == SET)
	{
		g_stApolloParms.m_ul50MS_Timer = RESET;

		if(ulCurrentDistance > ulPrvsDistance)
		{
			ulDistanceError = ulCurrentDistance - ulPrvsDistance;
		}
		else
		{
			ulDistanceError = ulPrvsDistance - ulCurrentDistance;
		}

		if(ulDistanceError > 100)
		{
			ulIsMagnetSteady = NO;
		}
		else
		{
			ulIsMagnetSteady = YES;
		}
		ulPrvsDistance = ulCurrentDistance;
	}
	return(ulIsMagnetSteady);
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
void ActivateSolenoidValve(SOLENOIDS Arg_etSolenoids)
{
	static U32 ulIsSolenoidActivated = RESET;

	switch(Arg_etSolenoids)
	{
	case SOLENOID_1:
		if(ulIsSolenoidActivated == RESET)
		{
			PWMOutputState(PWM0_BASE, PWM_SOLENOID_1, TRUE);
			PWMOutputState(PWM0_BASE, PWM_SOLENOID_2, FALSE);
			PWMOutputState(PWM0_BASE, PWM_SOLENOID_3, FALSE);
			g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidOneActivations++;
			EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidOneActivations), EE_ADDR_TOTSV1_ACT, SIZEOF_LONG);
			ulIsSolenoidActivated = SET;
		}
	break;

	case SOLENOID_2:
		if(ulIsSolenoidActivated == RESET)
		{
			PWMOutputState(PWM0_BASE, PWM_SOLENOID_2, TRUE);
			PWMOutputState(PWM0_BASE, PWM_SOLENOID_1, FALSE);
			PWMOutputState(PWM0_BASE, PWM_SOLENOID_3, FALSE);
			g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidTwoActivations++;
			EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidTwoActivations), EE_ADDR_TOTSV2_ACT, SIZEOF_LONG);
			ulIsSolenoidActivated = SET;
		}
	break;

	case SOLENOID_3:
		if(ulIsSolenoidActivated == RESET)
		{
			PWMOutputState(PWM0_BASE, PWM_SOLENOID_3, TRUE);
			PWMOutputState(PWM0_BASE, PWM_SOLENOID_1, FALSE);
			PWMOutputState(PWM0_BASE, PWM_SOLENOID_2, FALSE);
			g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidThreeActivations++;
			EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidThreeActivations), EE_ADDR_TOTSV3_ACT, SIZEOF_LONG);
			ulIsSolenoidActivated = SET;
		}
	break;

	case SOLENOID_OFF:
	default:
		PWMOutputState(PWM0_BASE, PWM_SOLENOID_ALL, FALSE);
		ulIsSolenoidActivated = RESET;
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
Function Name:
Function Description:
**************************************************************************/
AUTO_CALIBRATION_STATES AutoCalibrationPartOne(void)
{
	U32 ulIsMagnetSteady = RESET;
	DEVICE_DATA *stDeviceData = &(g_stApolloParms.m_stApolloDeviceData);
	//TODO
	ulIsMagnetSteady = MagnetSteadyCheck();
	if(ulIsMagnetSteady == YES)
	{
		stDeviceData->m_ulPosDeEnergised = stDeviceData->m_ulPositionOfMagnet;
		stDeviceData->m_ulSensorStateDeEnergised = READ_SENSOR_STATE;
		ActivateSolenoidValve(SOLENOID_1);
		g_stApolloParms.m_stAutoCalParms.m_ulAutoCalP1Over = YES;
		LED_Control(TOGGLE_FAST, YELLOW_LED);
		stDeviceData->m_ulTolDeEnLow = stDeviceData->m_ulPosDeEnergised - 5000;		//Set the default tolerance band
		stDeviceData->m_ulTolDeEnHigh = stDeviceData->m_ulPosDeEnergised + 5000;
		g_stApolloParms.m_etAutoCalStates = AUTO_CAL_PART_TWO;
	}
	else
	{
		g_stApolloParms.m_etAutoCalStates = AUTO_CAL_PART_ONE;
	}
	return(g_stApolloParms.m_etAutoCalStates);
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
AUTO_CALIBRATION_STATES AutoCalibrationPartTwo(void)
{
	U32 ulIsMagnetSteady = RESET;
	U32 ulStrokeLength = RESET;
	DEVICE_DATA *stDeviceData = &(g_stApolloParms.m_stApolloDeviceData);
	static U32 ulTimeoutTimer = RESET;
	static U32 ulProcessStart = RESET;

	if(g_stApolloParms.m_ul100MS_Timer == SET)
	{
		if(ulProcessStart == NO)
		{
			ulIsMagnetSteady = MagnetSteadyCheck();
			if(ulIsMagnetSteady == YES)
			{
				// Get the energised position from the MR
				stDeviceData->m_ulPosEnergised = stDeviceData->m_ulPositionOfMagnet;
				stDeviceData->m_ulSensorStateEnergised = READ_SENSOR_STATE;

				// Get the stroke length from the de energsied and energised positions
				if(stDeviceData->m_ulPosEnergised > stDeviceData->m_ulPosDeEnergised)
				{
					ulStrokeLength = stDeviceData->m_ulPosEnergised - stDeviceData->m_ulPosDeEnergised;
				}
				else
				{
					ulStrokeLength = stDeviceData->m_ulPosDeEnergised - stDeviceData->m_ulPosEnergised;
				}
				if(ulStrokeLength < 7000)
				{
					// If the stroke length is less than 7mm that means there is an operation error
					// Which arises of a wrong air tube connected
					g_stApolloParms.m_etApolloStates = OPERATION_ERROR;
				}
				else
				{
					stDeviceData->m_ulStrokeLength = ulStrokeLength;
					if(ulStrokeLength > 40000)
					{
						// if the stroke length is greater than 40mm and either the energised or de energised
						// Positions are less than 10mm then it is a butterfly valve
						if(stDeviceData->m_ulPosDeEnergised <= 10000 || stDeviceData->m_ulPosEnergised <= 10000)
						{
							stDeviceData->m_etValveType = BUTTERFLY_VALVE;
						}
					}
					else
					{
						//Otherwise it is a seat valve
						stDeviceData->m_etValveType = SEAT_VALVE;
					}
					if(g_stApolloParms.m_ul100MS_Timer == SET)
					{
						LED_Control(TOGGLE_DOUBLE, YELLOW_LED);
						ulTimeoutTimer++;
						g_stApolloParms.m_ul100MS_Timer = RESET;
						if(ulTimeoutTimer > 20)
						{
							ulProcessStart = YES;
							ulTimeoutTimer = RESET;
						}
					}
				}
			}// End of part one magnet steady IF
		}
		// End of process start one if

		if(ulProcessStart == YES)
		{
			ActivateSolenoidValve(SOLENOID_OFF);
			LED_Control(TOGGLE_FAST, GREEN_LED);
			ulIsMagnetSteady = MagnetSteadyCheck();
			if(ulIsMagnetSteady == NO) // Magnet is not steady, means that magnet is moving
			{
				if(g_stApolloParms.m_ul100MS_Timer == SET)
				{
					g_stApolloParms.m_ul100MS_Timer = RESET;
					if(stDeviceData->m_ulPositionOfMagnet != stDeviceData->m_ulPosDeEnergised)
					{
						ulTimeoutTimer++;
						if(ulTimeoutTimer > stDeviceData->m_ulErrorTimeout)
						{
							// Go to error if magnet does not reach de energised position within 10s time
							ulTimeoutTimer = RESET;
							g_stApolloParms.m_etApolloStates = OPERATION_ERROR;
						}
					}
					else if(stDeviceData->m_ulPositionOfMagnet >= stDeviceData->m_ulTolDeEnLow
							&& stDeviceData->m_ulPositionOfMagnet <= stDeviceData->m_ulTolDeEnHigh)
					{
						// If magnet reaches the de energised position within 10s
						// Then double flash the green LED 2 times and exit to next calibration state
						LED_Control(TOGGLE_DOUBLE, GREEN_LED);
						ulTimeoutTimer++;
						g_stApolloParms.m_ul100MS_Timer = RESET;

						if(ulTimeoutTimer > 20)
						{
							ulProcessStart = YES;
							ulTimeoutTimer = RESET;
							if((g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidTwo == ABSENT) &&
								(g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidThree == ABSENT))
							{
								// Since solenoid 2 and 3 are not present we will not calibrate
								// Lower seat lift and upper seat lift
								g_stApolloParms.m_etAutoCalStates = AUTO_CAL_PART_FIVE;
							}
							else
							{
								if(g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidTwo == PRESENT)
								{
									g_stApolloParms.m_etAutoCalStates = AUTO_CAL_PART_THREE;
								}
								else if(g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidThree == PRESENT)
								{
									g_stApolloParms.m_etAutoCalStates = AUTO_CAL_PART_FOUR;
								}
							}
						}
					}
				}
			}
		}
	}
	return(g_stApolloParms.m_etAutoCalStates);
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
AUTO_CALIBRATION_STATES AutoCalibrationPartThree(void)
{
	U32 ulIsMagnetSteady = RESET;
	DEVICE_DATA *stDeviceData = &(g_stApolloParms.m_stApolloDeviceData);
	static U32 ulTimeoutTimer = RESET;
	static U32 ulProcessStart = RESET;
	static U32 ulProcessEnd = RESET;

	ActivateSolenoidValve(SOLENOID_2);
	LED_Control(TOGGLE_FAST, BLUE_LED);

	if(g_stApolloParms.m_ul100MS_Timer == SET)
	{
		g_stApolloParms.m_ul100MS_Timer = RESET;
		ulIsMagnetSteady = MagnetSteadyCheck();
		if(ulProcessStart == NO && ulProcessEnd == NO)
		{
			if(ulIsMagnetSteady == YES)
			{
				ulTimeoutTimer++;
				if(ulTimeoutTimer > stDeviceData->m_ulErrorTimeout)
				{
					ulTimeoutTimer = RESET;
					g_stApolloParms.m_etApolloStates = OPERATION_ERROR;
				}
			}
			else
			{
				ulProcessStart = YES; // Magnet has started moving, meaning calibration process has started
			}
		}
		else if(ulProcessStart == YES && ulProcessEnd == NO)
		{
			ulIsMagnetSteady = MagnetSteadyCheck();
			if(ulIsMagnetSteady == YES)
			{
				stDeviceData->m_ulPosLowSeatLift = stDeviceData->m_ulPositionOfMagnet;
				stDeviceData->m_ulSensorStateLowerSeatLift = READ_SENSOR_STATE;
				ulProcessStart = RESET;
				ulProcessEnd = YES;
			}
		}

		// Now we have got the lower seat lift position
		else if(ulProcessEnd == YES)
		{
			ActivateSolenoidValve(SOLENOID_OFF);
			LED_Control(TOGGLE_FAST, GREEN_LED);
			ulIsMagnetSteady = MagnetSteadyCheck();
			if(ulIsMagnetSteady == NO) // Magnet is not steady, means that magnet is moving
			{
				if(stDeviceData->m_ulPositionOfMagnet != stDeviceData->m_ulPosDeEnergised)
				{
					ulTimeoutTimer++;
					if(ulTimeoutTimer > stDeviceData->m_ulErrorTimeout)
					{
						// Go to error if magnet does not reach de energised position within 10s time
						ulTimeoutTimer = RESET;
						g_stApolloParms.m_etApolloStates = OPERATION_ERROR;
					}
				}
				else if(stDeviceData->m_ulPositionOfMagnet >= stDeviceData->m_ulTolDeEnLow
						&& stDeviceData->m_ulPositionOfMagnet <= stDeviceData->m_ulTolDeEnHigh)
				{
					// If magnet reaches the de energised position within 10s
					// Then double flash the green LED 2 times and exit to next calibration state
					LED_Control(TOGGLE_DOUBLE, GREEN_LED);
					ulTimeoutTimer++;
					if(ulTimeoutTimer > 20)
					{
						ulProcessStart = YES;
						ulTimeoutTimer = RESET;
						if(g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidThree == ABSENT)
						{
							g_stApolloParms.m_etAutoCalStates = AUTO_CAL_PART_FIVE;
						}
						else
						{
							g_stApolloParms.m_etAutoCalStates = AUTO_CAL_PART_FOUR;
						}
					}
				}
			}
		}
	}
	return(g_stApolloParms.m_etAutoCalStates);
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
AUTO_CALIBRATION_STATES AutoCalibrationPartFour(void)
{
	static U32 ulSensorState = RESET;
	U32 ulActivatedSensorState = RESET;
	DEVICE_DATA *stDeviceData = &(g_stApolloParms.m_stApolloDeviceData);
	static U32 ulTimeoutTimer = RESET;
	static U32 ulErrorTimeoutTimer = RESET;
	static U32 ulProcessStart = RESET;
	static U32 ulProcessEnd = RESET;

	ActivateSolenoidValve(SOLENOID_3);
	LED_Control(TOGGLE_FAST, WHITE_LED);
	if(g_stApolloParms.m_ul100MS_Timer == SET)
	{
		if((stDeviceData->m_ulPositionOfMagnet < stDeviceData->m_ulTolDeEnLow) ||
			(stDeviceData->m_ulPositionOfMagnet > stDeviceData->m_ulTolDeEnHigh))
		{
			g_stApolloParms.m_etApolloStates = OPERATION_ERROR;
		}
		g_stApolloParms.m_ul100MS_Timer = RESET;
		if(ulProcessStart == NO)
		{
			ulSensorState = READ_SENSOR_STATE;
			ActivateSolenoidValve(SOLENOID_3);
			LED_Control(TOGGLE_FAST, WHITE_LED);
			ulProcessStart = YES;
		}
		else if(ulProcessStart == YES)
		{
			if(ulProcessEnd == NO)
			{
				ulActivatedSensorState = READ_SENSOR_STATE;
				if(ulActivatedSensorState == ulSensorState)
				{
					ulTimeoutTimer++;
					if(ulTimeoutTimer > stDeviceData->m_ulErrorTimeout)
					{
						ulTimeoutTimer = RESET;
						g_stApolloParms.m_etApolloStates = OPERATION_ERROR;
					}
				}
				else
				{
					ulTimeoutTimer = RESET;
					stDeviceData->m_ulSensorStateActivated = ulActivatedSensorState;
					stDeviceData->m_ulSensorStateDeActivated = ulSensorState;
					ActivateSolenoidValve(SOLENOID_OFF);
					LED_Control(TOGGLE_FAST, GREEN_LED);
					ulProcessEnd = YES;
				}
			}
			else if(ulProcessEnd == YES)
			{
				ulActivatedSensorState = READ_SENSOR_STATE;
				if(ulActivatedSensorState != ulSensorState)
				{
					ulErrorTimeoutTimer++;
					if(ulTimeoutTimer > stDeviceData->m_ulErrorTimeout)
					{
						ulErrorTimeoutTimer = RESET;
						g_stApolloParms.m_etApolloStates = OPERATION_ERROR;
					}
				}
				else if(ulActivatedSensorState == ulSensorState)
				{
					LED_Control(TOGGLE_DOUBLE, GREEN_LED);
					ulTimeoutTimer++;
					if(ulTimeoutTimer > 20)
					{
						ulProcessStart = YES;
						ulTimeoutTimer = RESET;
						g_stApolloParms.m_etAutoCalStates = AUTO_CAL_PART_FIVE;
					}
				}
			}
		}
	}
	return(g_stApolloParms.m_etAutoCalStates);
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
AUTO_CALIBRATION_STATES AutoCalibrationPartFive(void)
{
	SetToleranceBands(g_stApolloParms.m_stApolloDeviceData.m_etValveType);
	g_stApolloParms.m_stApolloDeviceData.m_ulCalibrationFlag = CALIBRATED;
	SaveCalibrationData();
	return(g_stApolloParms.m_etAutoCalStates);
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
APOLLO_STATES ApolloAutoCalibration(void)
{
	switch(g_stApolloParms.m_etAutoCalStates)
	{
	case SOLENOID_TERMINAL_CHECK:
		g_stApolloParms.m_etAutoCalStates = SolenoidTerminalCheck();
		break;

	case MAGNET_RANGE_CHECK:
		g_stApolloParms.m_etAutoCalStates = MagnetRangeCheck();
		break;

	case AUTO_CAL_PART_ONE:
		g_stApolloParms.m_etAutoCalStates = AutoCalibrationPartOne();
		break;

	case AUTO_CAL_PART_TWO:
		g_stApolloParms.m_etAutoCalStates = AutoCalibrationPartTwo();
		break;

	case AUTO_CAL_PART_THREE:
		g_stApolloParms.m_etAutoCalStates = AutoCalibrationPartThree();
		break;

	case AUTO_CAL_PART_FOUR:
		g_stApolloParms.m_etAutoCalStates = AutoCalibrationPartFour();
		break;

	case AUTO_CAL_PART_FIVE:
		g_stApolloParms.m_etAutoCalStates = AutoCalibrationPartFive();
		break;
	}
	return(g_stApolloParms.m_etApolloStates);
}
