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
STARTUP_STATES ReadAndProcessLocalDIP_Switch(void)
{
	U32 ulDIP_SwtichValue = RESET;

	// Read the value of the DIP switch at power up
	ulDIP_SwtichValue = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	ulDIP_SwtichValue >>= 4;

	//Process the value of the DIP switch
	// Check if seat clean function is enabled
	if(GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4) != LOW)
	{
		g_stApolloParms.m_stApolloDeviceData.m_ulSeatCleanFunction = IS_DISABLED;
	}
	else
	{
		g_stApolloParms.m_stApolloDeviceData.m_ulSeatCleanFunction = IS_ENABLED;
	}

	//Check if validation intelligence is enabled
	if(GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5) != LOW)
	{
		g_stApolloParms.m_stApolloDeviceData.m_ulValidationIntelligence = IS_ENABLED;
	}
	else
	{
		g_stApolloParms.m_stApolloDeviceData.m_ulValidationIntelligence = IS_DISABLED;
	}

	//Check if ASI for PMO is enabled
	if(GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6) != LOW)
	{
		g_stApolloParms.m_stApolloDeviceData.m_ulASI_ForPMO = IS_DISABLED;
	}
	else
	{
		g_stApolloParms.m_stApolloDeviceData.m_ulASI_ForPMO = IS_ENABLED;
	}

	if(g_stApolloParms.m_etDeviceType == APOLLO_DIGITAL)
	{
		g_stApolloParms.m_etStartupStates = DETECT_SOLENOIDS;
	}
	else if(g_stApolloParms.m_etDeviceType == APOLLO_ASI)
	{
		if(g_stApolloParms.m_stApolloDeviceData.m_ulASI_ForPMO == IS_ENABLED)
		{
			g_stApolloParms.m_etDeviceType = APOLLO_ASI_PMO;
		}
		g_stApolloParms.m_etStartupStates = READ_DIP_SWITCH_ASI;
	}
	return(g_stApolloParms.m_etStartupStates);
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name: ReadAndProcessASI_DIP_Switch
Function Description:
Reads the DIP switch on the ASI daughter board for
1. Version of ASI
2. Enabled or disabled local addressing
3. Slave A or B
4. 5 bit slave address
**************************************************************************/
STARTUP_STATES ReadAndProcessASI_DIP_Switch(void)
{
	U32 ulASI_DIP_SwitchValue = RESET;
	g_stApolloParms.m_stASI_Parms.m_stASI_DIP_Switches.m_ulDIP_SwitchValue = GPIO_PORTD_DATA_R;
	ulASI_DIP_SwitchValue = g_stApolloParms.m_stASI_Parms.m_stASI_DIP_Switches.m_ulDIP_SwitchValue;


	//Read and set the slave address from DIP switch
	//Invert the read value  and AND with 31 to get actual address
	g_stApolloParms.m_stASI_Parms.m_stASI_DIP_Switches.m_ulSlaveAddress = ulASI_DIP_SwitchValue & MC_SLAVE_ADDRESS;
	g_stApolloParms.m_stASI_Parms.m_stASI_DIP_Switches.m_ulSlaveAddress = ~(g_stApolloParms.m_stASI_Parms.m_stASI_DIP_Switches.m_ulSlaveAddress);
	g_stApolloParms.m_stASI_Parms.m_stASI_DIP_Switches.m_ulSlaveAddress &= SLAVE_ADDR_SET;

	//Set the global address to either enabled or disabled as per DIP switch setting
	g_stApolloParms.m_stASI_Parms.m_stASI_DIP_Switches.m_ulIsLocalAddrEnabled = ulASI_DIP_SwitchValue & LOCAL_ADDR_SETTING;   //And with 0x02 to check whether addressing is enabled
	g_stApolloParms.m_stASI_Parms.m_stASI_DIP_Switches.m_ulIsLocalAddrEnabled >>= RIGHT_SHIFT_BY_SIX;

	//Set the version configuration profile according to DIP switch setting
	//For V2.1
	//ID Code - 0x07
	//ID_2 Code - 0x0F
	//IO Code - 0x0F
	//ID_Ext Code - 0x0F

	//For V3.0
	//ID Code - 0x07
	//ID_2 Code - 0x0A
	//IO Code - 0x07
	//ID_Ext Code - 0x07
	g_stApolloParms.m_stASI_Parms.m_stASI_DIP_Switches.m_ulVersionConfiguration = ulASI_DIP_SwitchValue & VERSION_CONF_SETTING; //Check version configuration
	g_stApolloParms.m_stASI_Parms.m_stASI_DIP_Switches.m_ulVersionConfiguration >>= RIGHT_SHIFT_BY_FIVE;
	g_stApolloParms.m_stASI_Parms.m_stASI_DIP_Switches.m_ulSlaveSelect = ulASI_DIP_SwitchValue & SLAVE_SEL_SETTING;         //Check current selected salve
	g_stApolloParms.m_stASI_Parms.m_stASI_DIP_Switches.m_ulSlaveSelect >>= RIGHT_SHIFT_BY_SEVEN;                                                   //Convert to boolean

	if(g_stApolloParms.m_stASI_Parms.m_stASI_DIP_Switches.m_ulVersionConfiguration == VERSION_2_1)
	{
		//If version is 2.1 then do not add a slave A or B condition
		g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_Code = ID_CODE_V_2_1;
		g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_Code2 = ID_CODE_2_V_2_1;
		g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulIO_Code = IO_CODE_V_2_1;
		g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_CodeExt = ID_CODE_EXT_V_2_1;
	}
	else
	{
		//If version is 3.0 check whether slave A or slave B is being addressed
		g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_Code = ID_CODE_V_3_0;
		g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_Code2 = ID_CODE_2_V_3_0;
		g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulIO_Code = IO_CODE_V_3_0;
		g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_CodeExt = ID_CODE_EXT_V_3;

		if(g_stApolloParms.m_stASI_Parms.m_stASI_DIP_Switches.m_ulSlaveSelect == SLAVE_A)
		{
			g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_CodeExt |= SLAVE_SELECT_A;
		}
		else
		{
			g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_CodeExt |= SLAVE_SELECT_B;
		}
	}

	//Add the memory locations to which the version configuration ID codes have to be stored
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_Code |= ID_MEMORY_SLOT;
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_Code2 |= ID_2_MEMORY_SLOT;
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulIO_Code |= IO_MEMORY_SLOT;
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_CodeExt |= ID_EXT_MEMORY_SLOT;

	//Check if local addressing is enabled, if yes, set the flag to be used later
	if(g_stApolloParms.m_stASI_Parms.m_stASI_DIP_Switches.m_ulIsLocalAddrEnabled == IS_DISABLED)
	{
		g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulLocalAddressing = IS_DISABLED;
	}
	else
	{
		g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulLocalAddressing = IS_ENABLED;
	}

	// Change the state to local addressing
	g_stApolloParms.m_etStartupStates = LOCAL_ADDRESSING;
	return(g_stApolloParms.m_etStartupStates);
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name: Local_Addressing
Function Description:
1. This is the main local addressing state machine
2. Converts data to manchester II format
3. Sends and receives local addressing data for ASI chip
4. This function is only avialable in the ASI model
**************************************************************************/
STARTUP_STATES Local_Addressing(void)
{
	U32 stat_Loc_ulPauseTime = RESET;
	switch(g_stApolloParms.m_etLocAddrStates)
	{
		case STORE_GBL_DATA:
			g_stApolloParms.m_etLocAddrStates = StoreGblManchesterData();
		break;

		case SEND_DATA:
			g_stApolloParms.m_etLocAddrStates = SendLocalAddrData();
		break;

		case SEND_COMPLETE:
			SET_IRD_PIN_HIGH;
			if(stat_Loc_ulPauseTime < ONE_SECOND_TIME)
			{
				stat_Loc_ulPauseTime++;
			}
			else
			{
				stat_Loc_ulPauseTime = RESET;
				GPIO_PORTG_DATA_R  ^= 0x04;
			}
		break;
	}
	return(g_stApolloParms.m_etStartupStates);
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name: DetectSolenoidValves
Function Description:
1. Detects the number of solenoid valves connected
2. Detects which solenoids are connected
**************************************************************************/
STARTUP_STATES DetectSolenoidValves(void)
{
	 static U32 ulCounter = RESET;
	 U32 ulTemp = RESET;

	// Enable all the sense MOSFETS so that solenoids will be detected
	GPIO_PORTB_DATA_R &= 0x8F;

	//Check if solenoids are present
	//Wait for a small amount of time before the check begins
	if(ulCounter < 25)
	{
		ulCounter++;
		g_stApolloParms.m_etStartupStates = DETECT_SOLENOIDS;
	}
	else
	{
		// Use macros for discrete figures TODO
		//Check for solenoid 1 first
		ulTemp = GPIO_PORTF_DATA_R & 0x10;
		if(ulTemp == 0x10)
		{
			g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidOne = PRESENT;
		}
		else
		{
			g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidOne = ABSENT;
		}
		EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidOne), EE_ADDR_SOLENOID_ONE, SIZEOF_LONG);

		ulTemp = GPIO_PORTG_DATA_R & 0x20;
		if(ulTemp == 0x20)
		{
			g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidTwo = PRESENT;
		}
		else
		{
			g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidTwo = ABSENT;
		}
		EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidTwo), EE_ADDR_SOLENOID_TWO, SIZEOF_LONG);

		ulTemp = GPIO_PORTA_DATA_R & 0x80;
		if(ulTemp == 0x80)
		{
			g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidThree = PRESENT;
		}
		else
		{
			g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidThree = ABSENT;
		}
		EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidThree), EE_ADDR_SOLENOID_THREE, SIZEOF_LONG);

		if((g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidOne == ABSENT) &&
			(g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidTwo == ABSENT) &&
			(g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidThree == ABSENT))
		{
			// If all solenoids are absent then change the Auto Cal Error time out to 30s instead of 10
			g_stApolloParms.m_stApolloDeviceData.m_ulErrorTimeout = THIRTY_SEC_ERROR_TIMEOUT;
		}
		else
		{
			g_stApolloParms.m_stApolloDeviceData.m_ulErrorTimeout = TEN_SEC_ERROR_TIMEOUT;
		}
		g_stApolloParms.m_etStartupStates = GET_CALIBRATION_DATA;
	}
	g_stApolloParms.m_etApolloStates = STARTUP;
	return(g_stApolloParms.m_etStartupStates);
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:+
Function Name: GetCalibrationData
Function Description:
1. Checks if unit has been calibrated by checking a flag
2. If yes, calibration data is recovered from EEPROM
3. If no, unit goes to not calibrated mode
**************************************************************************/
STARTUP_STATES GetCalibrationData(void)
{
	//Calibration data get code to be added here TODO

	U32 ulSolenoidOne = RESET;
	U32 ulSolenoidTwo = RESET;
	U32 ulSolenoidThree = RESET;

	//First check if the calibration flag is set
	EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulCalibrationFlag), EE_ADDR_CALIBRATION, SIZEOF_LONG);
	EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulMinutes), EE_ADDR_OPERATION_TIME, SIZEOF_LONG);
	g_stApolloParms.m_stApolloDeviceData.m_ulHours = g_stApolloParms.m_stApolloDeviceData.m_ulMinutes/60;
	g_stApolloParms.m_stApolloDeviceData.m_ulDays = g_stApolloParms.m_stApolloDeviceData.m_ulHours/24;
	g_stApolloParms.m_stApolloDeviceData.m_ulYears = g_stApolloParms.m_stApolloDeviceData.m_ulDays/365;
	EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidOneActivations), EE_ADDR_TOTSV1_ACT, SIZEOF_LONG);
	EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidTwoActivations), EE_ADDR_TOTSV2_ACT, SIZEOF_LONG);
	EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidThreeActivations), EE_ADDR_TOTSV3_ACT, SIZEOF_LONG);
	EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulMaximumTemparature), EE_ADDR_MAX_TEMP, SIZEOF_LONG);

	if(g_stApolloParms.m_stApolloDeviceData.m_ulCalibrationFlag == CALIBRATED)
	{

		// If the calibration flag is set then read the calibration data
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulPosEnergised), EE_ADDR_ENERGIZED, SIZEOF_LONG);
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulPosDeEnergised), EE_ADDR_DE_ENERGIZED, SIZEOF_LONG);
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulPosLowSeatLift), EE_ADDR_LSL_VALUE, SIZEOF_LONG);
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulPosUppSeatLift), EE_ADDR_USL_VALUE, SIZEOF_LONG);
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulTolEnHigh), EE_ADDR_ENG_TOL_HIGH, SIZEOF_LONG);
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulTolEnLow), EE_ADDR_ENG_TOL_LOW, SIZEOF_LONG);
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulTolDeEnHigh), EE_ADDR_DNG_TOL_HIGH, SIZEOF_LONG);
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulTolDeEnLow), EE_ADDR_DNG_TOL_LOW, SIZEOF_LONG);
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulTolSL_High), EE_ADDR_SL_TOL_HIGH, SIZEOF_LONG);
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulTolSL_Low), EE_ADDR_SL_TOL_LOW, SIZEOF_LONG);
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_etValveType), EE_ADDR_VALVE_TYPE, SIZEOF_LONG);
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulMinutes), EE_ADDR_OPERATION_TIME, SIZEOF_LONG);
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulStrokeCount), EE_ADDR_NO_OF_STROKES, SIZEOF_LONG);
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulDate), EE_ADDR_DATE_FW, SIZEOF_LONG);
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulFirmwareVersion), EE_ADDR_FW_REVISION, SIZEOF_LONG);
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulPCB_Version), EE_ADDR_PCB_REVISION, SIZEOF_LONG);
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulSensorStateActivated), EE_ADDR_EXTSEN_ON, SIZEOF_LONG);
		EEPROMRead((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulSensorStateDeActivated), EE_ADDR_EXTSEN_OFF, SIZEOF_LONG);
		EEPROMRead((U32 *)(&ulSolenoidOne), EE_ADDR_SOLENOID_ONE, SIZEOF_LONG);
		EEPROMRead((U32 *)(&ulSolenoidTwo), EE_ADDR_SOLENOID_TWO, SIZEOF_LONG);
		EEPROMRead((U32 *)(&ulSolenoidThree), EE_ADDR_SOLENOID_THREE, SIZEOF_LONG);

		if((ulSolenoidOne != g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidOne) ||
			(ulSolenoidTwo != g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidTwo) ||
			(ulSolenoidThree != g_stApolloParms.m_stApolloDeviceData.m_ulSolenoidThree))
		{
			g_stApolloParms.m_etApolloStates = OPERATION_ERROR;
		}
		else
		{
			g_stApolloParms.m_etApolloStates = OPERATION_MODE;
		}
	}

	else
	{
		g_stApolloParms.m_etApolloStates = NOT_CALIBRATED_MODE;
		g_stApolloParms.m_etApolloStates = OPERATION_MODE; // Only for testing
	}
	return(g_stApolloParms.m_etStartupStates);
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name: ApolloStartup
Function Description:
This is Apollo Unit startup state machine and does the following
1. Reads the local DIP switch
2. Reads the ASI DIP switch if an optional ASI board is present
3. Locally addresses the ASI chip if addressing is enabled
4. Detects the number of solenoid valves connected to the unit
5. Checks for availability of calibration data
6. Controls whether system will go to not calibrated mode or operation mode
**************************************************************************/
APOLLO_STATES ApolloStartup(void)
{
	switch(g_stApolloParms.m_etStartupStates)
	{
	case READ_DIP_SWITCH_LOCAL:
		g_stApolloParms.m_etStartupStates = ReadAndProcessLocalDIP_Switch();
		break;

	case READ_DIP_SWITCH_ASI:
		g_stApolloParms.m_etStartupStates = ReadAndProcessASI_DIP_Switch();
		break;

	case LOCAL_ADDRESSING:
		g_stApolloParms.m_etStartupStates = Local_Addressing();
		break;

	case DETECT_SOLENOIDS:
		g_stApolloParms.m_etStartupStates = DetectSolenoidValves();
		break;

	case GET_CALIBRATION_DATA:
		g_stApolloParms.m_etStartupStates = GetCalibrationData();
		break;
	}
	return(g_stApolloParms.m_etApolloStates);
}
