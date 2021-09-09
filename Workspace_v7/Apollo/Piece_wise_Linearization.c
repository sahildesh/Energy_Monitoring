#if 0
#if 0
	while(1)
	{
		ulLength = ApolloMeasurement();
	}
#endif
	float flADCValue = RESET;
	float flRadians = RESET;
	float flTemp;
	float flDivider = RESET;
	float flAngle;
	float flDistance;
	_iq24 IQ_ulAdcValue = RESET;
	float flDataArray[180];
	float flDataArrayA[180];
	U32 ulPointer = RESET;

	U32 ulData1[100];
	U32 ulData2[100];
	U32 ulToggle = RESET;
	char ucUART_Data[100];
	U32 ulDataPointer = RESET;
	U32 ulLength;
	static U32 ulPointer1 = RESET;
	static U32 ulPointer2 = RESET;
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
MEAS_REGION GetRegionOfMeasurement(U32 Arg_ulSensorValue)
{
	MEAS_REGION etRegion = NO_REGION;
	if((Arg_ulSensorValue < REGION_1_LOW) || (Arg_ulSensorValue > REGION_20_HIGH))
	{
		etRegion = NO_REGION;
	}
	else if((Arg_ulSensorValue >= REGION_1_LOW) && (Arg_ulSensorValue <= REGION_1_HIGH))
	{
		etRegion = REGION_1;
	}
	else if((Arg_ulSensorValue >= REGION_2_LOW) && (Arg_ulSensorValue <= REGION_2_HIGH))
	{
		etRegion = REGION_2;
	}
	else if((Arg_ulSensorValue >= REGION_3_LOW) && (Arg_ulSensorValue <= REGION_3_HIGH))
	{
		etRegion = REGION_3;
	}
	else if((Arg_ulSensorValue >= REGION_4_LOW) && (Arg_ulSensorValue <= REGION_4_HIGH))
	{
		etRegion = REGION_4;
	}
	else if((Arg_ulSensorValue >= REGION_5_LOW) && (Arg_ulSensorValue <= REGION_5_HIGH))
	{
		etRegion = REGION_5;
	}
	else if((Arg_ulSensorValue >= REGION_6_LOW) && (Arg_ulSensorValue <= REGION_6_HIGH))
	{
		etRegion = REGION_6;
	}
	else if((Arg_ulSensorValue >= REGION_7_LOW) && (Arg_ulSensorValue <= REGION_7_HIGH))
	{
		etRegion = REGION_7;
	}
	else if((Arg_ulSensorValue >= REGION_8_LOW) && (Arg_ulSensorValue <= REGION_8_HIGH))
	{
		etRegion = REGION_8;
	}
	else if((Arg_ulSensorValue >= REGION_9_LOW) && (Arg_ulSensorValue <= REGION_9_HIGH))
	{
		etRegion = REGION_9;
	}
	else if((Arg_ulSensorValue >= REGION_10_LOW) && (Arg_ulSensorValue <= REGION_10_HIGH))
	{
		etRegion = REGION_10;
	}
	else if((Arg_ulSensorValue >= REGION_11_LOW) && (Arg_ulSensorValue <= REGION_11_HIGH))
	{
		etRegion = REGION_11;
	}
	else if((Arg_ulSensorValue >= REGION_12_LOW) && (Arg_ulSensorValue <= REGION_12_HIGH))
	{
		etRegion = REGION_12;
	}
	else if((Arg_ulSensorValue >= REGION_13_LOW) && (Arg_ulSensorValue <= REGION_13_HIGH))
	{
		etRegion = REGION_13;
	}
	else if((Arg_ulSensorValue >= REGION_14_LOW) && (Arg_ulSensorValue <= REGION_14_HIGH))
	{
		etRegion = REGION_14;
	}
	else if((Arg_ulSensorValue >= REGION_15_LOW) && (Arg_ulSensorValue <= REGION_15_HIGH))
	{
		etRegion = REGION_15;
	}
	else if((Arg_ulSensorValue >= REGION_16_LOW) && (Arg_ulSensorValue <= REGION_16_HIGH))
	{
		etRegion = REGION_16;
	}
	else if((Arg_ulSensorValue >= REGION_17_LOW) && (Arg_ulSensorValue <= REGION_17_HIGH))
	{
		etRegion = REGION_17;
	}
	else if((Arg_ulSensorValue >= REGION_18_LOW) && (Arg_ulSensorValue <= REGION_18_HIGH))
	{
		etRegion = REGION_18;
	}
	else if((Arg_ulSensorValue >= REGION_19_LOW) && (Arg_ulSensorValue <= REGION_19_HIGH))
	{
		etRegion = REGION_19;
	}
	else if((Arg_ulSensorValue >= REGION_20_LOW) && (Arg_ulSensorValue <= REGION_20_HIGH))
	{
		etRegion = REGION_20;
	}
	return(etRegion);
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
U32 ApplyR1_R11_Linearization(U32 Arg_ulSlope, U32 Arg_ulConstanst, U32 Arg_ulSensorValue)
{
	U32 ulLinearizedValue = RESET;

	// The equation for linearizing output from regions 1 to 11 is
	// Distance = (ADC Normalised output + Constant Value) / Section Slope

	ulLinearizedValue = Arg_ulSensorValue + Arg_ulConstanst;
	ulLinearizedValue /=  Arg_ulSlope;

	return(ulLinearizedValue);
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
U32 ApplyR12_R20_Linearization(U32 Arg_ulSlope, U32 Arg_ulConstanst, U32 Arg_ulSensorValue)
{

	// The equation for linearizing output from regions 1 to 11 is
	// Distance = (ADC Normalised output - Constant Value) / Section Slope

	U32 ulLinearizedValue = RESET;

	ulLinearizedValue = Arg_ulSensorValue - Arg_ulConstanst;
	ulLinearizedValue /=  Arg_ulSlope;

	return(ulLinearizedValue);
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
U32 GetLinearizedValue(U32 Arg_ulSensorValue, MEAS_REGION Arg_etRegion)
{
	U32 ulSlope = RESET;
	U32 ulConstant = RESET;
	U32 ulLinearizedValue = RESET;
	switch(Arg_etRegion)
	{
	case NO_REGION:
		//This is the case where magnet will be out of the sensing range
		//TODO

	break;

	case REGION_1:
		ulSlope = REGION_1_SLOPE;
		ulConstant = REGION_1_CONSTANT;
	break;

	case REGION_2:
		ulSlope = REGION_2_SLOPE;
		ulConstant = REGION_2_CONSTANT;
	break;

	case REGION_3:
		ulSlope = REGION_3_SLOPE;
		ulConstant = REGION_3_CONSTANT;
	break;

	case REGION_4:
		ulSlope = REGION_4_SLOPE;
		ulConstant = REGION_4_CONSTANT;
	break;

	case REGION_5:
		ulSlope = REGION_5_SLOPE;
		ulConstant = REGION_5_CONSTANT;
	break;

	case REGION_6:
		ulSlope = REGION_6_SLOPE;
		ulConstant = REGION_6_CONSTANT;
	break;

	case REGION_7:
		ulSlope = REGION_7_SLOPE;
		ulConstant = REGION_7_CONSTANT;
	break;

	case REGION_8:
		ulSlope = REGION_8_SLOPE;
		ulConstant = REGION_8_CONSTANT;
	break;

	case REGION_9:
		ulSlope = REGION_9_SLOPE;
		ulConstant = REGION_9_CONSTANT;
	break;

	case REGION_10:
		ulSlope = REGION_10_SLOPE;
		ulConstant = REGION_10_CONSTANT;
	break;

	case REGION_11:
		ulSlope = REGION_11_SLOPE;
		ulConstant = REGION_11_CONSTANT;
	break;

	case REGION_12:
		ulSlope = REGION_12_SLOPE;
		ulConstant = REGION_12_CONSTANT;
	break;

	case REGION_13:
		ulSlope = REGION_13_SLOPE;
		ulConstant = REGION_13_CONSTANT;
	break;

	case REGION_14:
		ulSlope = REGION_14_SLOPE;
		ulConstant = REGION_14_CONSTANT;
	break;

	case REGION_15:
		ulSlope = REGION_15_SLOPE;
		ulConstant = REGION_15_CONSTANT;
	break;

	case REGION_16:
		ulSlope = REGION_16_SLOPE;
		ulConstant = REGION_16_CONSTANT;
	break;

	case REGION_17:
		ulSlope = REGION_17_SLOPE;
		ulConstant = REGION_17_CONSTANT;
	break;

	case REGION_18:
		ulSlope = REGION_18_SLOPE;
		ulConstant = REGION_18_CONSTANT;
	break;

	case REGION_19:
		ulSlope = REGION_19_SLOPE;
		ulConstant = REGION_19_CONSTANT;
	break;

	case REGION_20:
		ulSlope = REGION_20_SLOPE;
		ulConstant = REGION_20_CONSTANT;
	break;
	}
	switch(Arg_etRegion)
	{

	//For regions 1 to 11, there is a different equation for linearization

	case REGION_1:
	case REGION_2:
	case REGION_3:
	case REGION_4:
	case REGION_5:
	case REGION_6:
	case REGION_7:
	case REGION_8:
	case REGION_9:
	case REGION_10:
	case REGION_11:
		ulLinearizedValue = ApplyR1_R11_Linearization(ulSlope, ulConstant, Arg_ulSensorValue);
	break;

	//For regions 12 to 20 there is a different linearisation equation
	case REGION_12:
	case REGION_13:
	case REGION_14:
	case REGION_15:
	case REGION_16:
	case REGION_17:
	case REGION_18:
	case REGION_19:
	case REGION_20:
		ulLinearizedValue = ApplyR12_R20_Linearization(ulSlope, ulConstant, Arg_ulSensorValue);
	break;
	}

	return(ulLinearizedValue);
}
#endif
#if 0
	while(1)
	{
		if(g_stApolloParms.m_ul100MS_Timer == SET)
		{
			g_stApolloParms.m_ul100MS_Timer = RESET;
//			GPIO_PORTA_DATA_R ^= 0x40;
			if(g_stApolloParms.m_etConversionStatusMR == INCOMPLETE)
			{
				ADCProcessorTrigger(ADC0_BASE, SQNC_0);
			}
		}
		if(g_stApolloParms.m_etConversionStatusMR == COMPLETE)
		{
			if(ulPointer1 < 101)
			{
				ulData1[ulPointer1] = g_stApolloParms.m_stADC_Parms.m_ulADC_RawValue[0];
				ulData2[ulPointer1] = g_stApolloParms.m_stADC_Parms.m_ulADC_RawValue[1];
				ulTemp[ulPointer1] = g_stApolloParms.m_stADC_Parms.m_ulADC_RawValue[1] * 2250;
				ulTemp[ulPointer1] = 6040125 - ulTemp[ulPointer1];
				ulTemp[ulPointer1] = (ulTemp[ulPointer1] * 100)/(40950 * 166);
				g_stApolloParms.m_etConversionStatusMR = INCOMPLETE;
				ulPointer1++;
			}
			else
			{
				ulPointer1 = 101;
			}
		}
	}
#endif
#if 0
	while(1)
	{
		flADCValue = 2374;
		flTemp = (flADCValue * 9012.63) - 16777020;
		flRadians = _IQ24asin(flTemp);
		flDivider = 583847.1;
		flAngle = flRadians/flDivider;
		flAngle = 0.01744 * flAngle;
		//This is the code for testing of trigonometric functions from the IQmath library

/*
		GPIO_PORTA_DATA_R = 0x40;
		flRadians = flADCValue * 0.0174 * 2;
		IQ_ulAdcValue = _IQ24(flRadians);
		flDataArray[ulPointer] = _IQ24sin(IQ_ulAdcValue);
		flTemp = _IQ24asin(flDataArray[ulPointer]);
		flDataArrayA[ulPointer] = flTemp;
		flADCValue++;
		ulPointer++;
		GPIO_PORTA_DATA_R = 0x00;

*/
		if(ulPointer > 91)
		{
			while(1)
			{

			}
		}

	}// EO while
#endif
#if 0 //Piece wise linearization
	//Piece wise linearization over here
	etMeasurementRegion = GetRegionOfMeasurement(ulSelectedSensorValue);
	g_stApolloParms.m_etMeasurementRegion = etMeasurementRegion;
	ulLinearizedValue = GetLinearizedValue(ulSelectedSensorValue, etMeasurementRegion);
#endif
