#include <tm4c123gh6pz.h>
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
**************************************************************************/
U32 CheckBit(U32 Arg_unCheckBit)
{
	U32 Loc_unBit = RESET;

	if(Arg_unCheckBit & BIT_VALUE_ONE != NO)
	{
		Loc_unBit = MAN_BIT_VALUE_ONE;
		g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulOneCounter++;
	}
	else
	{
		Loc_unBit = MAN_BIT_VALUE_ZERO;
	}
	return(Loc_unBit);
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
U32 GetParity(U16 Arg_unOneCounter)
{
	U32 Loc_ulParity = RESET;
	if(Arg_unOneCounter % IS_NUM_EVEN == NO)
	{
	Loc_ulParity = SET_EVEN_PARITY;
	}
	else
	{
	Loc_ulParity = SET_ODD_PARITY;
	}
	return(Loc_ulParity);
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
U32 GetManchesterDataValue(U32 Arg_ulManchesterData)
{

	//Here we are converting 14 bit data into 28 bit data using the following code
	// For every bit that is one we use 01
	// For every bit that is zero we use 10
	//The routine works in the following manner
	/*
	 * 1. Gets the data
	 * 2. Passes to a function to check the bit
	 * 3. Function returns with either 01 or 10
	 * 4. Data is left shifted by two places to accomodate data for next bit
	 * 5. Variable containing 14 bit data is shifted to the right to scan the next LSB
	 */

	//Start bit, stop bit and parity bit are calculated differently, hence what remains is the 11 bit data
	U32 Loc_ulManchesterData = RESET;
	Loc_ulManchesterData |= CheckBit(Arg_ulManchesterData); //0
	Loc_ulManchesterData <<= LEFT_SHIFT_BY_TWO;
	Arg_ulManchesterData >>= RIGHT_SHIFT_BY_ONE;
	Loc_ulManchesterData |= CheckBit(Arg_ulManchesterData); //1
	Loc_ulManchesterData <<= LEFT_SHIFT_BY_TWO;
	Arg_ulManchesterData >>= RIGHT_SHIFT_BY_ONE;
	Loc_ulManchesterData |= CheckBit(Arg_ulManchesterData); //2
	Loc_ulManchesterData <<= LEFT_SHIFT_BY_TWO;
	Arg_ulManchesterData >>= RIGHT_SHIFT_BY_ONE;
	Loc_ulManchesterData |= CheckBit(Arg_ulManchesterData); //3
	Loc_ulManchesterData <<= LEFT_SHIFT_BY_TWO;
	Arg_ulManchesterData >>= RIGHT_SHIFT_BY_ONE;
	Loc_ulManchesterData |= CheckBit(Arg_ulManchesterData); //4
	Loc_ulManchesterData <<= LEFT_SHIFT_BY_TWO;
	Arg_ulManchesterData >>= RIGHT_SHIFT_BY_ONE;
	Loc_ulManchesterData |= CheckBit(Arg_ulManchesterData); //5
	Loc_ulManchesterData <<= LEFT_SHIFT_BY_TWO;
	Arg_ulManchesterData >>= RIGHT_SHIFT_BY_ONE;
	Loc_ulManchesterData |= CheckBit(Arg_ulManchesterData); //6
	Loc_ulManchesterData <<= LEFT_SHIFT_BY_TWO;
	Arg_ulManchesterData >>= RIGHT_SHIFT_BY_ONE;
	Loc_ulManchesterData |= CheckBit(Arg_ulManchesterData); //7
	Loc_ulManchesterData <<= LEFT_SHIFT_BY_TWO;
	Arg_ulManchesterData >>= RIGHT_SHIFT_BY_ONE;
	Loc_ulManchesterData |= CheckBit(Arg_ulManchesterData); //8
	Loc_ulManchesterData <<= LEFT_SHIFT_BY_TWO;
	Arg_ulManchesterData >>= RIGHT_SHIFT_BY_ONE;
	Loc_ulManchesterData |= CheckBit(Arg_ulManchesterData); //9
	Loc_ulManchesterData <<= LEFT_SHIFT_BY_TWO;
	Arg_ulManchesterData >>= RIGHT_SHIFT_BY_ONE;
	Loc_ulManchesterData |= CheckBit(Arg_ulManchesterData); //10
	Loc_ulManchesterData <<= LEFT_SHIFT_BY_TWO;
	return(Loc_ulManchesterData);
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
U32 EncodeToManchester(U32 Arg_ulManchesterData)
{
	U32 Loc_ulManchesterData = RESET;										//Reset data variable
	U32 Loc_ulParity = RESET;												//Reset parity variable
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulOneCounter = RESET;		//Reset the "One Counter" used for calculating parity

	Loc_ulManchesterData = Arg_ulManchesterData;							//Copy data into local variable
	Loc_ulManchesterData = GetManchesterDataValue(Loc_ulManchesterData); 	//Encode data into manchester code
	Loc_ulParity = GetParity(g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulOneCounter);	//Get the value of parity bit from encoded data
	Loc_ulManchesterData |= Loc_ulParity;	//Or data with parity
	Loc_ulManchesterData |= STOP_BIT;		//Or data with STOP bit
	Loc_ulManchesterData |= START_BIT;		//Or data with Start bit
	return(Loc_ulManchesterData);			//Okay, now our data is ready to be stored in array
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
LOC_ADDR_STATES StoreGblManchesterData(void)
{
	//Store constant values of magic sequence in Array
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulManchesterMagicSequence = EncodeToManchester(MANCHESTER_MAGIC_SEQUENCE);
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataArray[MAGIC_SEQUENCE] = g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulManchesterMagicSequence;

	//Store constant value of Delete Slave address call in array
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDeleteAddress = EncodeToManchester(MANCHESTER_DEL_ADDR);
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataArray[DELETE_SLV_ADDR] = g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDeleteAddress;

	//If local addressing is enabled only then store the slave address in array
	if(g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulLocalAddressing == IS_ENABLED)
	{
		g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulSlaveAddress = EncodeToManchester(g_stApolloParms.m_stASI_Parms.m_stASI_DIP_Switches.m_ulSlaveAddress);
		g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataArray[SLAVE_ADDR] = g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulSlaveAddress;
	}
	//Enter program mode
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulEnterProgramMode = EncodeToManchester(ENTER_PRGM_MODE);
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataArray[ENTER_PROGRAM_MODE] = g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulEnterProgramMode;

	//Send version ID code
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_Code = EncodeToManchester(g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_Code);
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataArray[ID_1_CODE] = g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_Code;

	//Send version ID_2 code
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_Code2 = EncodeToManchester(g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_Code2);
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataArray[ID_2_CODE] = g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_Code2;

	//Send version IO code
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulIO_Code = EncodeToManchester(g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulIO_Code);
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataArray[IO_CODE] = g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulIO_Code;


	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_CodeExt = EncodeToManchester(g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_CodeExt);
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataArray[ID_EXT_CODE] = g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulID_CodeExt;

	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulBroadcastCall = EncodeToManchester(RESET_SLAVE);
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataArray[RESET_SINGLE] = g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulBroadcastCall;

	//Change the state to MANCHESTER CODE
	g_stApolloParms.m_etStartupStates = LOCAL_ADDRESSING;

	//Reset the timer
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulManchesterTimer = RESET;

	//Change state to send data
	g_stApolloParms.m_etLocAddrStates = SEND_DATA;
	return(g_stApolloParms.m_etLocAddrStates);
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
U32 DecodeDataFrame(U32 Arg_ulReadData)
{
	U32 Arg_ulActualData = RESET;
	Arg_ulReadData &= 0x03;

	if(Arg_ulReadData == 2)
	{
		Arg_ulActualData = LOW;
	}

	if(Arg_ulReadData == 1)
	{
		Arg_ulActualData = HIGH;
	}
	return(Arg_ulActualData);
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
void ProcessReadData(void)
{
	U32 ulReadData = g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulReadDataFromPort;
	U32 ulActualData = RESET;
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulProcessedData = RESET;

	ulReadData = ~(ulReadData);
	ulActualData = DecodeDataFrame(ulReadData);
	ulReadData >>= RIGHT_SHIFT_BY_TWO;			//Bit 0
	ulActualData = DecodeDataFrame(ulReadData);
	ulReadData >>= RIGHT_SHIFT_BY_TWO;			//Bit 1
	ulActualData = DecodeDataFrame(ulReadData);
	ulReadData >>= RIGHT_SHIFT_BY_TWO;			//Bit 2
	ulActualData = DecodeDataFrame(ulReadData);
	ulReadData >>= RIGHT_SHIFT_BY_TWO;			//Bit 3
	ulActualData = DecodeDataFrame(ulReadData);
	ulReadData >>= RIGHT_SHIFT_BY_TWO;			//Bit 4
	ulActualData = DecodeDataFrame(ulReadData);
	ulReadData >>= RIGHT_SHIFT_BY_TWO;			//Bit 5
	ulActualData = DecodeDataFrame(ulReadData);

	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulProcessedData = ulActualData;
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
LOC_ADDR_STATES SendLocalAddrData(void)
{
	static U32 unBitCounter = RESET;;
	static ASI_MSG etPointer = MAGIC_SEQUENCE;
	static U32 ulPauseTime = RESET;
	U32 ulSlaveAddress = RESET;
	U32 ulSlave = RESET;

	if(g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulManchesterTimer == SET)	//We want a timed event, so wait for flag to be set
	{
		if(unBitCounter < BIT_COUNTER)		//Always check if number of bits transferred are less than 28
		{
			g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulManchesterTimer = RESET;						//Reset the timer
			GPIO_PORTB_DATA_R = g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataArray[etPointer];	//Output data on port
			g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataArray[etPointer] >>= RIGHT_SHIFT_BY_ONE;	//Make ready the next bit to be sent
			unBitCounter++;																				//Increase the counter
		}
		else
		{
			if(g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulIsAddrChannelLocked == NO)
			{
				if(g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulInputDSR_Pin == NO)
				{
					SET_IRD_PIN_HIGH;
					if(ulPauseTime < MANCHESTER_PAUSE_TIME)
					{
						ulPauseTime++;		//If magic sequences are pending, wait for pause time and send again
					}
					else
					{
						etPointer = MAGIC_SEQUENCE;
						unBitCounter = RESET;		//When pause time is over, reset all counters to send magic sequence again
						ulPauseTime = RESET;
					}
				}//End of DSR Input if
				else
				{
					SET_IRD_PIN_HIGH;
					g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulInputDSR_Pin = NO;
					g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulIsAddrChannelLocked = YES;
					etPointer = READ_SLV_A_1;
				} //End of DSR input else
			} //End of Addr Channel lock if
			else
			{
				if(etPointer >= READ_SLV_A_1 && etPointer <= READ_SLV_B_31)
				{
					if(g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulInputDSR_Pin == NO)
					{
						etPointer++;
					}
					else
					{
						ulSlaveAddress = etPointer;
						ulSlaveAddress <<= LEFT_SHIFT_BY_FIVE;
						if(etPointer >= READ_SLV_A_1 && etPointer <= READ_SLV_A_31)
						{
							ulSlave = SLAVE_A;
						}
						else if(etPointer >= READ_SLV_B_1 && etPointer <= READ_SLV_B_31)
						{
							ulSlave = SLAVE_B;
						}
						ulSlave <<= LEFT_SHIFT_BY_THREE;
						g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDeleteAddress = MANCHESTER_DEL_ADDR;
						g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDeleteAddress |= ulSlaveAddress;
						g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDeleteAddress |= ulSlave;
						g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataArray[DELETE_SLV_ADDR] = g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDeleteAddress;
						etPointer = DELETE_SLV_ADDR;
					}//End of no DSR input when reading status else
				}// End of if for reading slave address

				else
				{
					if(g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataAvailable == NO)
					{
						// Wait here until the slave response occurs
					}
					else if(g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataAvailable == YES)
					{
						ProcessReadData();
					}
				}
			} //End of Addr channel lock else
		}
	}
	return(g_stApolloParms.m_etLocAddrStates);
}
