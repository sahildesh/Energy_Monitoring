//------------------------------------------------------------------------------------------------------------------------//
// MR Array Sensing and Linearization
//------------------------------------------------------------------------------------------------------------------------//
#include <stdint.h>
#include <stdbool.h>
#include <tm4c123gh6pz.h>
#include <hw_memmap.h>
#include <adc.h>
#include <IQmathLib.h>
#include "Apollo_Header.h"
#include "Apollo_Functions.h"
#include "Linearization.h"
#include "EEPROM_Function.h"

extern GLOBAL_PARMS g_stApolloParms;

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name: ReadAndStoreSensorData
Function Description:
1. The current ADC value is read and MR sensor value is taken from the
array
2. Current MR value is gotten by subtracting ADC offset from raw MR value
**************************************************************************/
void ReadAndStoreSensorData(void)
{
	ADC_PARMS *stADC_Parms = &(g_stApolloParms.m_stADC_Parms);
	stADC_Parms->m_ulADC_NormalisedValue = stADC_Parms->m_ulADC_RawValue[MR_SENSOR]; // - stADC_Parms->m_ulADC_Offset;
	g_stApolloParms.m_stMR_ArrayParms.m_ulSensorValue[g_stApolloParms.m_etCurrentSensor] = stADC_Parms->m_ulADC_NormalisedValue;
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name: ConvertAndGetSensorValue
Function Description:
1. As per sensor values are read from the ADC FIFO and stored as per
respective sensor
**************************************************************************/
U32 ConvertAndGetSensorValue(U32 Arg_ulProcessStart)
{
	static U32 ulDelayTimer = RESET;
	ADC_PARMS *stADC_Parms = &(g_stApolloParms.m_stADC_Parms);

	if(ulDelayTimer < 25)
	{
		ulDelayTimer++;
	}
	else
	{

		//Trigger the ADC
		ADCProcessorTrigger(ADC0_BASE, SQNC_0);

		//Wait for the conversion to be complete, then read the ADC normalized value
		if(g_stApolloParms.m_etConversionStatusMR == COMPLETE)
		{
			ADCSequenceDataGet(ADC0_BASE, SQNC_0, stADC_Parms->m_ulADC_RawValue);
			g_stApolloParms.m_etConversionStatusMR = INCOMPLETE;	//Reset conversion flag
			ReadAndStoreSensorData();								//Read and store sensor data
			ulDelayTimer = RESET;									//Reset stability timer
			Arg_ulProcessStart = RESET;								//Reset process start flag
			g_stApolloParms.m_etCurrentSensor++;					//Update selection to the next sensor
			if(g_stApolloParms.m_etCurrentSensor == MAX_SENSORS)
			{
				g_stApolloParms.m_etCurrentSensor = SENSOR_1;
				g_stApolloParms.m_etMeasurementStates = SELECT_SENSOR;
				RESET_MUX_SIGNALS;
			}
		}
	}
	return(Arg_ulProcessStart);
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name: ReadMR_Sensors
Function Description:
1. Reads the ADC without any inputs from MR sensors
2. Calculates offset of ADC
3. Reads the MR sensors in an ascending order
**************************************************************************/
MEASUREMENT_STATES ReadMR_Sensors(void)
{
	static U32 ulDelayTimer = RESET;
	static U32 ulProcessStart = RESET;
	ADC_PARMS *stADC_Parms = &(g_stApolloParms.m_stADC_Parms);
	switch(g_stApolloParms.m_etCurrentSensor)
	{
	case NO_SENSOR:
		GPIO_PORTA_DATA_R = 0x40;
		if(ulProcessStart == NO)
		{
			// Disable the multiplexer outputs
			DISABLE_MUX;
			ulProcessStart = YES;
		}
		else
		{
			//Wait a settling time delay
			if(ulDelayTimer < 25)
			{
				ulDelayTimer++;
			}
			else
			{
				ADCProcessorTrigger(ADC0_BASE, SQNC_0);
				if(g_stApolloParms.m_etConversionStatusMR == COMPLETE)
				{
					ADCSequenceDataGet(ADC0_BASE, SQNC_0, stADC_Parms->m_ulADC_RawValue);
					stADC_Parms->m_ulADC_Offset = stADC_Parms->m_ulADC_RawValue[MR_SENSOR];
					g_stApolloParms.m_etConversionStatusMR = INCOMPLETE;
					ulDelayTimer = RESET;
					ulProcessStart = RESET;
					g_stApolloParms.m_etCurrentSensor++;
					ENABLE_MUX;
				}
			}
		}
		break;

	case SENSOR_1:
		// Select MUX channel 0
		if(ulProcessStart == NO)
		{
			RESET_MUX_SIGNALS;
			SELECT_SENSOR_1_MUX;
			ulProcessStart = YES;
		}
		else
		{
			// Insert a small delay for signals to be stable
			ulProcessStart = ConvertAndGetSensorValue(ulProcessStart);
		}
		break;

	case SENSOR_2:
		// Select MUX channel 1
		if(ulProcessStart == NO)
		{
			RESET_MUX_SIGNALS;
			SELECT_SENSOR_2_MUX;
			ulProcessStart = YES;
		}
		else
		{
			// Insert a small delay for signals to be stable
			ulProcessStart = ConvertAndGetSensorValue(ulProcessStart);
		}
		break;

	case SENSOR_3:
		// Select MUX channel 2
		if(ulProcessStart == NO)
		{
			RESET_MUX_SIGNALS;
			SELECT_SENSOR_3_MUX;
			ulProcessStart = YES;
		}
		else
		{
			// Insert a small delay for signals to be stable
			ulProcessStart = ConvertAndGetSensorValue(ulProcessStart);
		}
		break;

	case SENSOR_4:
		// Select MUX channel 3
		if(ulProcessStart == NO)
		{
			RESET_MUX_SIGNALS;
			SELECT_SENSOR_4_MUX;
			ulProcessStart = YES;
		}
		else
		{
			// Insert a small delay for signals to be stable
			ulProcessStart = ConvertAndGetSensorValue(ulProcessStart);
		}
		break;

	case SENSOR_5:
		// Select MUX channel 4
		if(ulProcessStart == NO)
		{
			RESET_MUX_SIGNALS;
			SELECT_SENSOR_5_MUX;
			ulProcessStart = YES;
		}
		else
		{
			// Insert a small delay for signals to be stable
			ulProcessStart = ConvertAndGetSensorValue(ulProcessStart);
		}
		break;

	case SENSOR_6:
		// Select MUX channel 5
		if(ulProcessStart == NO)
		{
			RESET_MUX_SIGNALS;
			SELECT_SENSOR_6_MUX;
			ulProcessStart = YES;
		}
		else
		{
			// Insert a small delay for signals to be stable
			ulProcessStart = ConvertAndGetSensorValue(ulProcessStart);
		}
		break;

	case SENSOR_7:
		// Select MUX channel 6
		if(ulProcessStart == NO)
		{
			RESET_MUX_SIGNALS;
			SELECT_SENSOR_7_MUX;
			ulProcessStart = YES;
		}
		else
		{
			// Insert a small delay for signals to be stable
			ulProcessStart = ConvertAndGetSensorValue(ulProcessStart);
		}
		break;

	case SENSOR_8:
		// Select MUX channel 7
		if(ulProcessStart == NO)
		{
			RESET_MUX_SIGNALS;
			SELECT_SENSOR_8_MUX;
			ulProcessStart = YES;
		}
		else
		{
			// Insert a small delay for signals to be stable
			ulProcessStart = ConvertAndGetSensorValue(ulProcessStart);
		}
		break;
	}
	return(g_stApolloParms.m_etMeasurementStates);
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name: GetFixedDistance
Function Description:
1. All sensors are at a distance of 12.5mm from each other
2. As the distance measured is the distance per sensor, we have to add
the position of the sensor do get the exact distance
**************************************************************************/
U32 GetFixedDistance(U32 Arg_etSensor)
{
	U32 ulFixedDistance = RESET;
	switch(Arg_etSensor)
	{
	case SENSOR_1:
		ulFixedDistance = SENSOR_1_FIXED_DISTANCE;
		break;

	case SENSOR_2:
		ulFixedDistance = SENSOR_2_FIXED_DISTANCE;
		break;

	case SENSOR_3:
		ulFixedDistance = SENSOR_3_FIXED_DISTANCE;
		break;

	case SENSOR_4:
		ulFixedDistance = SENSOR_4_FIXED_DISTANCE;
		break;

	case SENSOR_5:
		ulFixedDistance = SENSOR_5_FIXED_DISTANCE;
		break;

	case SENSOR_6:
		ulFixedDistance = SENSOR_6_FIXED_DISTANCE;
		break;

	case SENSOR_7:
		ulFixedDistance = SENSOR_7_FIXED_DISTANCE;
		break;

	case SENSOR_8:
		ulFixedDistance = SENSOR_8_FIXED_DISTANCE;
		break;
	}
	return(ulFixedDistance);
}
/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name: SelectMR_Sensors
Function Description:
1. Selects the pair of MR sensors in between which the magnet lies
2. Selects the MR sensor closest to the Magnet
**************************************************************************/
MEASUREMENT_STATES SelectMR_Sensors(void)
{
	SENSOR_PAIR etMagnetBetweenSensors = OUTSIDE_OF_RANGE;
	MR_ARRAY *stMR_ArrayParms = &(g_stApolloParms.m_stMR_ArrayParms);
	MR_SENSORS etSelectedSensor = NO_SENSOR;
	//Run a calibrated comparison routine here to select the sensor to which
	//the magnet is the closest to

	// Get the sensor to which the magnet is the nearest
	// This routine first selects the pair of sensors between which the magnet lies
	// According to the outputs of all the sensors, the magnet lies between
	// The Lower sensor having values between 150 and LOW_SEN_HIGH_VAL
	// and the sensor have values between 568 and 950
	// With the higher sensor having the higher value
	// For example:
	// The magnet lies between sensor 1 and 2 if
	// Value of sensor 2 lies between 150 and 600 //TODO modify these commented values
	// Value of sensor 1 lies between 568 and 950
	// And the value of sensor 1 is always higher than the value of sensor 2

	// Check if position is between Sensors 1 and 2
	if(stMR_ArrayParms->m_ulSensorValue[SENSOR_1] > LOW_SEN_LOW_VAL && stMR_ArrayParms->m_ulSensorValue[SENSOR_1] < LOW_SEN_HIGH_VAL &&
		stMR_ArrayParms->m_ulSensorValue[SENSOR_2] > HIGH_SEN_LOW_VAL && stMR_ArrayParms->m_ulSensorValue[SENSOR_2] < HIGH_SEN_HIGH_VAL)
	{
		if(stMR_ArrayParms->m_ulSensorValue[SENSOR_1] < stMR_ArrayParms->m_ulSensorValue[SENSOR_2])
		{
			etMagnetBetweenSensors = BETWEEN_SENSOR_1_2;
		}
	}

	// Check if position is between Sensors 2 and 3
	if(stMR_ArrayParms->m_ulSensorValue[SENSOR_2] > LOW_SEN_LOW_VAL && stMR_ArrayParms->m_ulSensorValue[SENSOR_2] < LOW_SEN_HIGH_VAL &&
		stMR_ArrayParms->m_ulSensorValue[SENSOR_3] > HIGH_SEN_LOW_VAL && stMR_ArrayParms->m_ulSensorValue[SENSOR_3] < HIGH_SEN_HIGH_VAL)
	{
		if(stMR_ArrayParms->m_ulSensorValue[SENSOR_2] < stMR_ArrayParms->m_ulSensorValue[SENSOR_3])
		{
			etMagnetBetweenSensors = BETWEEN_SENSOR_2_3;
		}
	}

	// Check if position is between Sensors 3 and 4
	if(stMR_ArrayParms->m_ulSensorValue[SENSOR_3] > LOW_SEN_LOW_VAL && stMR_ArrayParms->m_ulSensorValue[SENSOR_3] < LOW_SEN_HIGH_VAL &&
		stMR_ArrayParms->m_ulSensorValue[SENSOR_4] > HIGH_SEN_LOW_VAL && stMR_ArrayParms->m_ulSensorValue[SENSOR_4] < HIGH_SEN_HIGH_VAL)
	{
		if(stMR_ArrayParms->m_ulSensorValue[SENSOR_3] < stMR_ArrayParms->m_ulSensorValue[SENSOR_4])
		{
			etMagnetBetweenSensors = BETWEEN_SENSOR_3_4;
		}
	}

	// Check if position is between Sensors 4 and 5
	if(stMR_ArrayParms->m_ulSensorValue[SENSOR_4] > LOW_SEN_LOW_VAL && stMR_ArrayParms->m_ulSensorValue[SENSOR_4] < LOW_SEN_HIGH_VAL &&
		stMR_ArrayParms->m_ulSensorValue[SENSOR_5] > HIGH_SEN_LOW_VAL && stMR_ArrayParms->m_ulSensorValue[SENSOR_5] < HIGH_SEN_HIGH_VAL)
	{
		if(stMR_ArrayParms->m_ulSensorValue[SENSOR_4] < stMR_ArrayParms->m_ulSensorValue[SENSOR_5])
		{
			etMagnetBetweenSensors = BETWEEN_SENSOR_4_5;
		}
	}

	// Check if position is between Sensors 5 and 6
	if(stMR_ArrayParms->m_ulSensorValue[SENSOR_5] > LOW_SEN_LOW_VAL && stMR_ArrayParms->m_ulSensorValue[SENSOR_5] < LOW_SEN_HIGH_VAL &&
		stMR_ArrayParms->m_ulSensorValue[SENSOR_6] > HIGH_SEN_LOW_VAL && stMR_ArrayParms->m_ulSensorValue[SENSOR_6] < HIGH_SEN_HIGH_VAL)
	{
		if(stMR_ArrayParms->m_ulSensorValue[SENSOR_5] < stMR_ArrayParms->m_ulSensorValue[SENSOR_6])
		{
			etMagnetBetweenSensors = BETWEEN_SENSOR_5_6;
		}
	}

	// Check if position is between Sensors 6 and 7
	if(stMR_ArrayParms->m_ulSensorValue[SENSOR_6] > LOW_SEN_LOW_VAL && stMR_ArrayParms->m_ulSensorValue[SENSOR_6] < LOW_SEN_HIGH_VAL &&
		stMR_ArrayParms->m_ulSensorValue[SENSOR_7] > HIGH_SEN_LOW_VAL && stMR_ArrayParms->m_ulSensorValue[SENSOR_7] < HIGH_SEN_HIGH_VAL)
	{
		if(stMR_ArrayParms->m_ulSensorValue[SENSOR_6] < stMR_ArrayParms->m_ulSensorValue[SENSOR_7])
		{
			etMagnetBetweenSensors = BETWEEN_SENSOR_6_7;
		}
	}

	// Check if position is between Sensors 7 and 8
	if(stMR_ArrayParms->m_ulSensorValue[SENSOR_7] > LOW_SEN_LOW_VAL && stMR_ArrayParms->m_ulSensorValue[SENSOR_7] < LOW_SEN_HIGH_VAL &&
		stMR_ArrayParms->m_ulSensorValue[SENSOR_8] > HIGH_SEN_LOW_VAL && stMR_ArrayParms->m_ulSensorValue[SENSOR_8] < HIGH_SEN_HIGH_VAL)
	{
		if(stMR_ArrayParms->m_ulSensorValue[SENSOR_7] < stMR_ArrayParms->m_ulSensorValue[SENSOR_8])
		{
			etMagnetBetweenSensors = BETWEEN_SENSOR_7_8;
		}
	}

	// After selecting the pair of sensors near to the magnet
	// The following routine selects the sensor to which the magnet is the closest
	// For example:
	// The magnet is located between sensors 1 and 2
	// If the value of sensor 1 and sensor 2 are above 211 and below 650 repectively,
	// Then the magnet is closer to sensor 1
	// If the value of sensor 1 is above 211 and the value of sensor 2 is above 650
	// If all the cases fail, then the magnet is in the region of sensor 2
	// Search for the sensor to which the magnet is the nearest

	switch(etMagnetBetweenSensors)
	{
	// If the magnet lies between the sensors 1 and 2
	case BETWEEN_SENSOR_1_2:
		if(stMR_ArrayParms->m_ulSensorValue[SENSOR_1] > SENSOR_CALB_LOW && stMR_ArrayParms->m_ulSensorValue[SENSOR_2] < SENSOR_CALB_HIGH)
		{
			etSelectedSensor = SENSOR_2;
		}
		else if(stMR_ArrayParms->m_ulSensorValue[SENSOR_1] > SENSOR_CALB_LOW)
		{
			etSelectedSensor = SENSOR_1;
		}
		else
		{
			etSelectedSensor = SENSOR_2;
		}
		break;
	// If the magnet lies between the sensors 2 and 3
	case BETWEEN_SENSOR_2_3:
		if(stMR_ArrayParms->m_ulSensorValue[SENSOR_2] > SENSOR_CALB_LOW && stMR_ArrayParms->m_ulSensorValue[SENSOR_3] < SENSOR_CALB_HIGH)
		{
			etSelectedSensor = SENSOR_3;
		}
		else if(stMR_ArrayParms->m_ulSensorValue[SENSOR_2] > SENSOR_CALB_LOW)
		{
			etSelectedSensor = SENSOR_2;
		}
		else
		{
			etSelectedSensor = SENSOR_3;
		}
		break;

	// If the magnet lies between the sensors 3 and 4
	case BETWEEN_SENSOR_3_4:
		if(stMR_ArrayParms->m_ulSensorValue[SENSOR_3] > SENSOR_CALB_LOW && stMR_ArrayParms->m_ulSensorValue[SENSOR_4] < SENSOR_CALB_HIGH)
		{
			etSelectedSensor = SENSOR_4;
		}
		else if(stMR_ArrayParms->m_ulSensorValue[SENSOR_3] > SENSOR_CALB_LOW)
		{
			etSelectedSensor = SENSOR_3;
		}
		else
		{
			etSelectedSensor = SENSOR_4;
		}
		break;

	// If the magnet lies between the sensors 4 and 5
	case BETWEEN_SENSOR_4_5:
		if(stMR_ArrayParms->m_ulSensorValue[SENSOR_4] > SENSOR_CALB_LOW && stMR_ArrayParms->m_ulSensorValue[SENSOR_5] < SENSOR_CALB_HIGH)
		{
			etSelectedSensor = SENSOR_5;
		}
		else if(stMR_ArrayParms->m_ulSensorValue[SENSOR_4] > SENSOR_CALB_LOW)
		{
			etSelectedSensor = SENSOR_4;
		}
		else
		{
			etSelectedSensor = SENSOR_5;
		}
		break;
	// If the magnet lies between the sensors 5 and 6
	case BETWEEN_SENSOR_5_6:
		if(stMR_ArrayParms->m_ulSensorValue[SENSOR_5] > SENSOR_CALB_LOW && stMR_ArrayParms->m_ulSensorValue[SENSOR_6] < SENSOR_CALB_HIGH)
		{
			etSelectedSensor = SENSOR_6;
		}
		else if(stMR_ArrayParms->m_ulSensorValue[SENSOR_5] > SENSOR_CALB_LOW)
		{
			etSelectedSensor = SENSOR_5;
		}
		else
		{
			etSelectedSensor = SENSOR_6;
		}
		break;
	// If the magnet lies between the sensors 6 and 7
	case BETWEEN_SENSOR_6_7:
		if(stMR_ArrayParms->m_ulSensorValue[SENSOR_6] > SENSOR_CALB_LOW && stMR_ArrayParms->m_ulSensorValue[SENSOR_7] < SENSOR_CALB_HIGH)
		{
			etSelectedSensor = SENSOR_7;
		}
		else if(stMR_ArrayParms->m_ulSensorValue[SENSOR_6] > SENSOR_CALB_LOW)
		{
			etSelectedSensor = SENSOR_6;
		}
		else
		{
			etSelectedSensor = SENSOR_7;
		}
		break;
	// If the magnet is detected between sensors 7 and 8
	case BETWEEN_SENSOR_7_8:
		if(stMR_ArrayParms->m_ulSensorValue[SENSOR_7] > SENSOR_CALB_LOW && stMR_ArrayParms->m_ulSensorValue[SENSOR_8] < SENSOR_CALB_HIGH)
		{
			etSelectedSensor = SENSOR_8;
		}
		else if(stMR_ArrayParms->m_ulSensorValue[SENSOR_7] > SENSOR_CALB_LOW)
		{
			etSelectedSensor = SENSOR_7;
		}
		else
		{
			etSelectedSensor = SENSOR_8;
		}
		break;
	}
	g_stApolloParms.m_stLinearization.m_etSelectedSensor = etSelectedSensor;
	return(g_stApolloParms.m_etMeasurementStates);
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name: ConvertToIQ_Math
Function Description:
1. Converts the current sensor value to IQ Math value which is required
by the functions in the IQMath library
**************************************************************************/
F32 ConvertToIQ_Math(F32 Arg_flSensorValue)
{
	// Here we are changing the value of the measured sensor to that understood by the IQ math
	// library
	F32 flSineValue = RESET;
	flSineValue = (Arg_flSensorValue * 9012.63) - 16777020.0; // This value is not confirmed TODO
	return(flSineValue);
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name:
Function Description: LinearizeMR_Sensor
1. Linearizes the output of the selected MR sensor
2. Function uses inverse sine technique used in the Tiva IQmath library
**************************************************************************/
MEASUREMENT_STATES LinearizeMR_Sensor(void)
{
	MR_SENSORS etSelectedSensor = g_stApolloParms.m_stLinearization.m_etSelectedSensor;
	F32 flSelectedSensorValue = (F32)(g_stApolloParms.m_stMR_ArrayParms.m_ulSensorValue[etSelectedSensor]);
	F32 flSineIQ_MathValue = RESET;
	F32 flLinearizedValue = RESET;
	F32 flFixedDistance = RESET;

	//Apply sine inverse linearization technique here
	flSineIQ_MathValue = ConvertToIQ_Math(flSelectedSensorValue);
	flLinearizedValue = _IQ24asin(flSineIQ_MathValue);
	flLinearizedValue = flLinearizedValue + 26273120;

	// At the end of the routine complete the distance calculation using fixed values
	flFixedDistance = GetFixedDistance(etSelectedSensor);
	g_stApolloParms.m_stApolloDeviceData.m_ulPositionOfMagnet = flLinearizedValue - flFixedDistance;
	GPIO_PORTA_DATA_R = 0x00;
	return(g_stApolloParms.m_etMeasurementStates);
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name: TemperatureMeasurement
Function Description:
1. Current temperature measurement
2. Comparison with previous temperature
3. Stores max temperature in EEPROM
**************************************************************************/
MEASUREMENT_STATES TemperatureMeasurement(void)
{
	U32 ulReadTemperature = RESET;

	if(g_stApolloParms.m_ulMeasureTemp == SET)
	{
		ADC_PARMS *stADC_Parms = &(g_stApolloParms.m_stADC_Parms);
		stADC_Parms->m_ulADC_TemperatureSensor = stADC_Parms->m_ulADC_RawValue[TEMP_SENSOR];
		ulReadTemperature = ((6040125) - (2250 * stADC_Parms->m_ulADC_TemperatureSensor)) / 40950;
		if(ulReadTemperature > g_stApolloParms.m_stApolloDeviceData.m_ulMaximumTemparature)
		{
			g_stApolloParms.m_stApolloDeviceData.m_ulMaximumTemparature = ulReadTemperature;
			EEPROMWriteOperation((U32 *)(&g_stApolloParms.m_stApolloDeviceData.m_ulMaximumTemparature), EE_ADDR_MAX_TEMP, SIZEOF_LONG);
		}
	}
	g_stApolloParms.m_etApolloStates = g_stApolloParms.m_etPreviousState;
	return(g_stApolloParms.m_etMeasurementStates);
}

/**************************************************************************
Project ID: EMS3103
Project Name: Apollo Digital And ASI Development
Code Developed by: Makarand Deshmukh
Date modified: 28-05-2014

RS ID:
DLD ID:
Function Name: ApolloMeasurement
Function Description:
Sub state Machine for measurement of Magnet position and temperature of
device.
**************************************************************************/
APOLLO_STATES ApolloMeasurement(void)
{
	switch(g_stApolloParms.m_etMeasurementStates)
	{
	case READ_SENSORS:
		g_stApolloParms.m_etMeasurementStates = ReadMR_Sensors();
		break;

	case SELECT_SENSOR:
		g_stApolloParms.m_etMeasurementStates = SelectMR_Sensors();
		g_stApolloParms.m_etMeasurementStates = LINEARIZE_SENSOR;
		break;

	case LINEARIZE_SENSOR:
		g_stApolloParms.m_etMeasurementStates = LinearizeMR_Sensor();
		g_stApolloParms.m_etMeasurementStates = MEASURE_TEMPERATURE;
		break;

	case MEASURE_TEMPERATURE:
		g_stApolloParms.m_etMeasurementStates = TemperatureMeasurement();
		g_stApolloParms.m_etMeasurementStates = READ_SENSORS;
		break;
	}
	return(g_stApolloParms.m_etApolloStates);
}
