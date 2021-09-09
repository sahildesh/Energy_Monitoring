//------------------------------------------------------------------------------------------------------------------------//
// Included header files
//------------------------------------------------------------------------------------------------------------------------//
#include <stdio.h>
#include <string.h>
#include <IQmathLib.h>
#include "Apollo_Typedef.h"

//------------------------------------------------------------------------------------------------------------------------//
// MACROs for Apollo Digital
//------------------------------------------------------------------------------------------------------------------------//
#define SENSOR_DISTANCE 1250

// PWM macros
#define PWM_SOLENOID_1		PWM_OUT_2_BIT //PWM_OUT_2_BIT - Actual
#define PWM_SOLENOID_2		PWM_OUT_3_BIT
#define PWM_SOLENOID_3		PWM_OUT_0_BIT
#define PWM_SOLENOID_ALL	PWM_OUT_2_BIT | PWM_OUT_3_BIT | PWM_OUT_0_BIT
#define RED_LED				PWM_OUT_1_BIT
#define GREEN_LED			PWM_OUT_4_BIT // PWM_OUT_4_BIT - Actual
#define BLUE_LED			PWM_OUT_5_BIT
#define YELLOW_LED 			PWM_OUT_1_BIT | PWM_OUT_4_BIT
#define VIOLET_LED  		PWM_OUT_1_BIT | PWM_OUT_5_BIT
#define WHITE_LED			PWM_OUT_1_BIT | PWM_OUT_4_BIT | PWM_OUT_5_BIT
#define ALL_LED				PWM_OUT_1_BIT | PWM_OUT_4_BIT | PWM_OUT_5_BIT
#define SLOW_TOGGLE			500
#define FAST_TOGGLE			200

// Digital mode feedback macros
#define TURN_OFF_ALL_FEEDBACK 	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4, 0x00)
#define TURN_ON_PLC_FB_DEN 			GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_0, 0x01)
#define TURN_ON_PLC_FB_EN 			GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0x02)
#define TURN_ON_PLC_FB_SLL 			GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x04)
#define TURN_ON_PLC_FB_SLU 			GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_3, 0x08)
#define TURN_ON_PLC_FB_SOK 			GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_4, 0x10)

// ASI feedback macros
#define TURN_ON_ASI_FB_DEN 			GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_0, 0x01)
#define TURN_ON_ASI_FB_EN 			GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0x02)
#define TURN_ON_ASI_FB_AUX 			GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x04)
#define TURN_ON_ASI_FB_SOK 			GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_4, 0x10)

// ASI for PMO feedback macros
#define TURN_ON_ASI_PMO_FB_DEN 		GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_0, 0x01)
#define TURN_ON_ASI_PMO_FB_SLL 		GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x04)
#define TURN_ON_ASI_PMO_FB_SLU 		GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_3, 0x08)
#define TURN_ON_ASI_PMO_FB_SOK 		GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_4, 0x10)
//TODO

// Sensor read MACRO
#define READ_SENSOR_STATE			GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0)

// Macros for testing the MUX
#define DISABLE_MUX				GPIO_PORTK_DATA_R &= 0x00
#define ENABLE_MUX				GPIO_PORTK_DATA_R |= 0x80
#define RESET_MUX_SIGNALS		GPIO_PORTK_DATA_R &= 0x00
#define SELECT_SENSOR_1_MUX		GPIO_PORTK_DATA_R |= 0x80
#define SELECT_SENSOR_2_MUX		GPIO_PORTK_DATA_R |= 0x90
#define SELECT_SENSOR_3_MUX		GPIO_PORTK_DATA_R |= 0xA0
#define SELECT_SENSOR_4_MUX		GPIO_PORTK_DATA_R |= 0xB0
#define SELECT_SENSOR_5_MUX		GPIO_PORTK_DATA_R |= 0xC0
#define SELECT_SENSOR_6_MUX		GPIO_PORTK_DATA_R |= 0xD0
#define SELECT_SENSOR_7_MUX		GPIO_PORTK_DATA_R |= 0xE8
#define SELECT_SENSOR_8_MUX		GPIO_PORTK_DATA_R |= 0xF0

// 4 Pole DIP switch Macros
#define SEAT_CLEAN 		0x01
#define VALIDATION_INT 	0x02
#define ASI_FOR_PMO		0x04

// Error Timeouts
#define TEN_SEC_ERROR_TIMEOUT 	 100
#define THIRTY_SEC_ERROR_TIMEOUT 300

//Modify these values according to sensor calibration
//TODO
#define LOW_SEN_LOW_VAL			1
#define LOW_SEN_HIGH_VAL		1
#define HIGH_SEN_HIGH_VAL		1
#define HIGH_SEN_LOW_VAL		1
#define SENSOR_CALB_LOW			1
#define SENSOR_CALB_HIGH		1
#define LOWER_POS_LIMIT			0
#define HIGH_POS_LIMIT			100000

//TODO
#define SENSOR_1_FIXED_DISTANCE		6250
#define SENSOR_2_FIXED_DISTANCE		18750
#define SENSOR_3_FIXED_DISTANCE		31250
#define SENSOR_4_FIXED_DISTANCE		43750
#define SENSOR_5_FIXED_DISTANCE		56250
#define SENSOR_6_FIXED_DISTANCE		68750
#define SENSOR_7_FIXED_DISTANCE		81250
#define SENSOR_8_FIXED_DISTANCE		93750

//------------------------------------------------------------------------------------------------------------------------//
// Enumerations for ASI
//------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------//
// MACROs for ASI Model
//------------------------------------------------------------------------------------------------------------------------//
#define SETUP_PORTD               	0x07    	//Set the Data Direction Register to 0
#define SETUP_PORTB               	0xF8    	//Set the Data Direction Register, Bit 1 to 1
#define ENABLE_PULLUP_PORTD       	0xF8    	//Enable pullups on the PORTD
#define ENABLE_PULLUP_PORTB       	0x07    	//Enable pullups on the PORTB
#define PORT_PC0_OUTPUT           	0x01    	//Set PC0 as output
#define SETUP_IC_CMOS             	0x01    	//Set PC0 High to get IC in CMOS
#define TEN_US_COMPARE            	160     	//Timer compare match value in decimal
#define TIMER1_CTRLA              	0x00    	//Timer control A
#define TIMER1_CTRLB              	0x09    	//Timer control B
#define TIMER1_CTRLC              	0x00    	//Timer control C
#define TIMER_INT_MASK            	0x02    	//Timer Interrupt setting
#define ONE_MS_COMPARE            	250     	//Timer 2 compare match value
#define ONE_SECOND                	1001    	//One second value at one mili second interrupt
#define SLAVE_ADDR_SET				0x1F

//Macro definitions for Apollo ASI
#define STOP_BIT                  	0x08000000  //STOP BIT for manchester telegram, can be ORed directly
#define START_BIT                 	0x01        //START BIT for manchester telegram, can be ORed directly
#define MANCHESTER_MAGIC_SEQUENCE 	0x400       //Magic sequence code for ASI chip, has to be processed
#define MANCHESTER_DEL_ADDR       	0x400     	//Delete address code, has to be processed
#define ENTER_PRGM_MODE         	0x41D       //Program mode code for ASI, has to be processed
#define BROADCAST_CALL_CODE       	0x7F5
#define MC_SLAVE_ADDRESS		  	0x1F
#define LOCAL_ADDR_SETTING			0x40
#define VERSION_CONF_SETTING		0x20
#define SLAVE_SEL_SETTING			0x80
#define READ_IC_STATUS				0x7FE
#define READ_IC_STATUS_2			0x5DE
#define READ_IC_STATUS_3			0x7EE
#define RESET_SLAVE					0x41C

#define ID_CODE_V_2_1             	0x07        //Version configuration 2.1, ID code
#define ID_CODE_2_V_2_1           	0x0F        //Version configuration 2.1, ID_2 code
#define IO_CODE_V_2_1             	0x0F        //Version configuration 2.1, IO code
#define ID_CODE_EXT_V_2_1         	0x0F        //Version configuration 2.1, ID_Ext code
#define ID_CODE_V_3_0             	0x07        //Version configuration 3.0, ID code
#define ID_CODE_2_V_3_0           	0x0A        //Version configuration 3.0, ID_2 code
#define IO_CODE_V_3_0             	0x07        //Version configuration 3.0, IO code
#define ID_CODE_EXT_V_3           	0x07        //Version configuration 3.0, ID_Ext code

#define SLAVE_SELECT_A            	0x00        //Slave select to be ORed in Ext ID code
#define SLAVE_SELECT_B           	0x08        //Slave select to be ORed in Ext ID code

#define NO_DATA                   	0x55555555

#define RIGHT_SHIFT_BY_ONE        	1           //Shifts right by one position
#define RIGHT_SHIFT_BY_TWO        	2           //Shifts right by two positions
#define RIGHT_SHIFT_BY_THREE      	3           //Shifts right by three positions
#define RIGHT_SHIFT_BY_FOUR      	3           //Shifts right by three positions
#define RIGHT_SHIFT_BY_FIVE			5
#define RIGHT_SHIFT_BY_SIX			6
#define RIGHT_SHIFT_BY_SEVEN		7
#define LEFT_SHIFT_BY_ONE			1			//Shifts left by one position
#define LEFT_SHIFT_BY_TWO         	2           //Shifts left by two positions
#define LEFT_SHIFT_BY_THREE        	3           //Shifts left by three positions
#define LEFT_SHIFT_BY_FIVE			5			//Shifts left by five positions

#define BINARY_ONE                	1           //Binary 1, for ORing/ANDing
#define BINARY_TWO               	2           //Binary 2, for ORing/ANDing
#define BINARY_FOUR               	4           //Binary 4, for ORing/ANDing

#define IS_NUM_EVEN               	2           //Remainder check if number is even

#define SLAVE_ADDR_LNIB_SLOT		0X010
#define SLAVE_ADDR_HNIB_SLOT		0X030
#define ID_MEMORY_SLOT            	0x110       //ID Code memory location in AS_I chip
#define ID_2_MEMORY_SLOT          	0x130       //ID_2 Code memory location in AS_I chip
#define IO_MEMORY_SLOT            	0x150       //IO Code memory location in AS_I chip
#define ID_EXT_MEMORY_SLOT        	0x440       //ID_Ext Code memory location in AS_I chip

#define PAUSE_TIME                	8       	//Time to wait for a pause after a telegram
#define PULSE_TIME                	1           //Time to wait for next pulse in telegram

#define BIT_COUNTER               	28          //No of manchester bits in telegram
#define MAGIC_SEQ_COUNTER         	3           //No of times to send magic sequence
#define MAN_DATA_COUNTER          	1           //No of times to send other manchester telegrams

#define SET_IRD_PIN_HIGH         	GPIO_PORTB_DATA_R |= 0x01      //Set PORT PIN PC0 high
#define SET_IRD_PIN_LOW				GPIO_PORTB_DATA_R &= 0xFE
#define SET_EVEN_PARITY           	0x01000000  //Set even parity if no of ones is odd
#define SET_ODD_PARITY            	0x02000000  //Set odd parity if no of ones is even

#define BIT_VALUE_ONE             	1           //Set value of LSB = 1
#define MAN_BIT_VALUE_ONE         	0x02        //Set for manchester one
#define MAN_BIT_VALUE_ZERO        	0x01        //Set for manchester zero
#define EEPROM_WRITE_TIME			15
#define ONE_SECOND_TIME				100000
#define MANCHESTER_PAUSE_TIME		12

//------------------------------------------------------------------------------------------------------------------------//
// Enumerations for ASI model
//------------------------------------------------------------------------------------------------------------------------//
typedef enum etASI_Profile
{
	VERSION_3_0 = 0,
	VERSION_2_1
}ASI_PROF;

typedef enum etASI_Slave
{
	SLAVE_A = 0,
	SLAVE_B
}ASI_SLAVE;

typedef enum etASI_Message
{
	NO_MESSAGE = 0,
	READ_SLV_A_1,
	READ_SLV_A_2,
	READ_SLV_A_3,
	READ_SLV_A_4,
	READ_SLV_A_5,
	READ_SLV_A_6,
	READ_SLV_A_7,
	READ_SLV_A_8,
	READ_SLV_A_9,
	READ_SLV_A_10,
	READ_SLV_A_11,
	READ_SLV_A_12,
	READ_SLV_A_13,
	READ_SLV_A_14,
	READ_SLV_A_15,
	READ_SLV_A_16,
	READ_SLV_A_17,
	READ_SLV_A_18,
	READ_SLV_A_19,
	READ_SLV_A_20,
	READ_SLV_A_21,
	READ_SLV_A_22,
	READ_SLV_A_23,
	READ_SLV_A_24,
	READ_SLV_A_25,
	READ_SLV_A_26,
	READ_SLV_A_27,
	READ_SLV_A_28,
	READ_SLV_A_29,
	READ_SLV_A_30,
	READ_SLV_A_31,
	READ_SLV_B_1,
	READ_SLV_B_2,
	READ_SLV_B_3,
	READ_SLV_B_4,
	READ_SLV_B_5,
	READ_SLV_B_6,
	READ_SLV_B_7,
	READ_SLV_B_8,
	READ_SLV_B_9,
	READ_SLV_B_10,
	READ_SLV_B_11,
	READ_SLV_B_12,
	READ_SLV_B_13,
	READ_SLV_B_14,
	READ_SLV_B_15,
	READ_SLV_B_16,
	READ_SLV_B_17,
	READ_SLV_B_18,
	READ_SLV_B_19,
	READ_SLV_B_20,
	READ_SLV_B_21,
	READ_SLV_B_22,
	READ_SLV_B_23,
	READ_SLV_B_24,
	READ_SLV_B_25,
	READ_SLV_B_26,
	READ_SLV_B_27,
	READ_SLV_B_28,
	READ_SLV_B_29,
	READ_SLV_B_30,
	READ_SLV_B_31,
	MAGIC_SEQUENCE,
	ENTER_PROGRAM_MODE,
	DELETE_SLV_ADDR,
	ID_1_CODE,
	ID_2_CODE,
	IO_CODE,
	ID_EXT_CODE,
	SLAVE_ADDR,
	READ_STATUS,
	RESET_SINGLE,
	RESET_ALL
}ASI_MSG;

enum
{
	IS_DISABLED = 0,
	IS_ENABLED
};

enum
{
	MR_SENSOR = 0,
	TEMP_SENSOR
};

enum
{
	ONCE = 0,
	CONTINUOUS
};

typedef enum etDeviceType
{
	APOLLO_DIGITAL = 0,
	APOLLO_ASI,
	APOLLO_ASI_PMO
}DEV_TYPE;

typedef enum etLocalAddrStates
{
	STORE_GBL_DATA = 0,
	SEND_DATA,
	SEND_COMPLETE
}LOC_ADDR_STATES;

typedef enum etApolloModel
{
	MODEL_DIGITAL = 0,
	MODEL_ASI,
	MODEL_ASI_PMO
}APOLLO_MODEL;

enum
{
	ABSENT = 0,
	PRESENT
};


enum
{
	NOT_CALIBRATED = 10,
	CALIBRATED = 20
};

typedef enum etErrorConditions
{
	NO_ERROR = 0,
	OPERATION_ERR_SOL1,
	OPERATION_ERR_SOL2,
	OPERATION_ERR_SOL3,
	AUTOCAL_ERR
}ERRORS;

typedef enum etConversionStatus
{
	INCOMPLETE = 0,
	COMPLETE
}STATUS;

typedef enum etApolloStates
{
	STARTUP = 0,
	NOT_CALIBRATED_MODE,
	AUTO_CALIBRATION,
	MEASUREMENT,
	OPERATION_MODE,
	MEASUREMENT_ERROR,
	OPERATION_ERROR,
	TEST_MODE
}APOLLO_STATES;

typedef enum etMagnetBetweenSensors
{
	OUTSIDE_OF_RANGE = 0,
	BETWEEN_SENSOR_1_2,
	BETWEEN_SENSOR_2_3,
	BETWEEN_SENSOR_3_4,
	BETWEEN_SENSOR_4_5,
	BETWEEN_SENSOR_5_6,
	BETWEEN_SENSOR_6_7,
	BETWEEN_SENSOR_7_8
}SENSOR_PAIR;

typedef enum etLED_State
{
	LED_OFF = 0,
	LED_ON,
	TOGGLE_SLOW,
	TOGGLE_FAST,
	TOGGLE_DOUBLE
}LED_STATE;

typedef enum etMeasurementStates
{
	READ_SENSORS = 0,
	SELECT_SENSOR,
	LINEARIZE_SENSOR,
	MEASURE_TEMPERATURE
}MEASUREMENT_STATES;

typedef enum etStartupStates
{
	READ_DIP_SWITCH_LOCAL = 0,
	READ_DIP_SWITCH_ASI,
	LOCAL_ADDRESSING,
	DETECT_SOLENOIDS,
	GET_CALIBRATION_DATA
}STARTUP_STATES;

typedef enum etOutputControl
{
	SOLENOID_CONTROL = 0,
	AUTO_SETUP,
	LIGHTING_CONTROL
}OUTPUT_CONTROL_STATES;

typedef enum etSolenoids
{
	SOLENOID_OFF = 0,
	SOLENOID_1 = 0x01,
	SOLENOID_2 = 0x02,
	SOLENOID_3 = 0x04
}SOLENOIDS;

typedef enum etPLC_Output
{
	NO_FEEDBACK = 0,
	ENERGIZED,
	DE_ENERGIZED,
	SEAT_LIFT_UPPER,
	SEAT_LIFT_LOWER,
	STATUS_OK
}PLC_OUTPUTS;

typedef enum etPLC_Input
{
	IN_NO_INPUT = 0,
	IN_SOLENOID_1,
	IN_SOLENOID_2,
	IN_SOLENOID_3,
	IN_AUTO_SETUP
}PLC_INPUTS;

typedef enum etValves
{
	BUTTERFLY_VALVE = 0,
	SEAT_VALVE
}VALVES;

typedef enum etAutoCalStates
{
	SOLENOID_TERMINAL_CHECK = 0,
	MAGNET_RANGE_CHECK,
	AUTOMATIC_CALIBRATION,
	AUTO_CAL_PART_ONE,
	AUTO_CAL_PART_TWO,
	AUTO_CAL_PART_THREE,
	AUTO_CAL_PART_FOUR,
	AUTO_CAL_PART_FIVE,
	AUTO_CAL_ERROR,
	EO_AUTO_CAL
}AUTO_CALIBRATION_STATES;

typedef enum etMenuStates
{
	NO_MENU = 0,
	MAIN_MENU,
	DIAGNOSTIC_MENU,
	SETUP_VALUES_MENU,
	MANUAL_OPERATION_MENU,
	MANUAL_SETUP_MENU,
	ADVACNED_SETTINGS_MENU,
	ADVANCED_DIAGNOSTIC_MENU
}UART_MENU_STATES;

//------------------------------------------------------------------------------------------------------------------------//
// Enumerations for ADC
//------------------------------------------------------------------------------------------------------------------------//
typedef enum etADC_Priority
{
	PRIORITY_1 = 0,
	PRIORITY_2,
	PRIORITY_3,
	PRIORITY_4
}ADC_PRIORITY;

typedef enum etADC_SequencerSteps
{
	STEP_0 = 0,
	STEP_1,
	STEP_2,
	STEP_3,
	STEP_4,
	STEP_5,
	STEP_6,
	STEP_7

}ADC_SEQ_STEP;

typedef enum etADC_Sequencers
{
	SQNC_0 = 0,
	SQNC_1,
	SQNC_2,
	SQNC_3,
	SQNC_4,
	SQNC_5,
	SQNC_6,
	SQNC_7
}ADC_SEQ;

typedef enum etADC_Channels
{
	CHANNEL_0 = 0,
	CHANNEL_1,
	CHANNEL_2,
	CHANNEL_3,
	CHANNEL_4,
	CHANNEL_5,
	CHANNEL_6,
	CHANNEL_7
}ADC_CH;

//------------------------------------------------------------------------------------------------------------------------//
// Enumerations for MR sensors
//------------------------------------------------------------------------------------------------------------------------//
typedef enum etMR_Sensors
{
	NO_SENSOR = 0,
	SENSOR_1,
	SENSOR_2,
	SENSOR_3,
	SENSOR_4,
	SENSOR_5,
	SENSOR_6,
	SENSOR_7,
	SENSOR_8,
	MAX_SENSORS
}MR_SENSORS;

typedef enum etTestModeStates
{
	TEST_ONE = 0,
	TEST_TWO,
	TEST_THREE,
	TEST_FOUR
}TEST_MODE_STATES;

typedef struct m_stASI_DIP_Switches
{
	U32 m_ulDIP_SwitchValue;
	U32 m_ulSlaveAddress;
	U32 m_ulIsLocalAddrEnabled;
	U32 m_ulVersionConfiguration;
	U32 m_ulSlaveSelect;

}ASI_DIP_SW;

typedef struct m_stUART_Parms
{
	UART_MENU_STATES etMenuStates;
	U8 ucUART_Data[100];
	U32 ulStringLength;
	U32 ulDataAvailableOnUART;
	U32 ulUART_ReceiveInt;
	U32 ulDataOnUART;
	U32 ulAutoSetupFromUART;
}UART_PARMS;

typedef struct stLED_Control
{
	LED_STATE m_etLED_State;
	U32 m_ulLED_PWM;
}LED_CON;

typedef struct m_stLoc_Addr
{
	U32 m_ulID_Code;
	U32 m_ulID_Code2;
	U32 m_ulIO_Code;
	U32 m_ulID_CodeExt;
	U32 m_ulLocalAddressing;
	U32 m_ulManchesterTimer;
	U32 m_ulManchesterSlaveAddress;
	U32 m_ulOneCounter;
	U32 m_ulManchesterData;
	U32 m_ulDataReadTimer;
	U32 m_ulParity;
	U32 m_ulSlaveAddress;
	U32 m_ulDataAvailable;
	U32 m_ulDataLost;
	U32 m_ulReadDataFromPort;
	U32 m_ulProcessedData;
	U32 m_ulManchesterMagicSequence;
	U32 m_ulEnterProgramMode;
	U32 m_ulDeleteAddress;
	U32 m_ulBroadcastCall;
	U32 m_ulReadStatus;
	U32 m_ulReceiveDataFlag;
	U32 m_ulReceiveData;
	U32 m_ulReceiveDataTemp;
	U32 m_ulSlaveAddressH_Nib;
	U32 m_ulSlaveAddressL_Nib;
	U32 m_ulArrayPointer;
	U32 m_ulInputDSR_Pin;
	U32 m_ulIsAddrChannelLocked;
	U32 m_ulInputPST_Pin;
	U32 m_ulInputLED_Pin;
	U32 m_ulDataArray[100];
}LOC_ADDR_PARMS;

typedef struct stASI_Parms
{
	ASI_DIP_SW m_stASI_DIP_Switches;
	LOC_ADDR_PARMS m_stLoc_Addr;
}ASI_PARMS;

typedef struct stDiagnostics
{
	U32 m_ulErrorNo;
	U32 m_ulErrorType;
	U32 m_ulErrorTime;
	U32 m_ulErrorDuration;
}DIAGNOSTICS;

typedef struct stADC_Parms
{
	U32 m_ulADC_RawValue[2];
	U32 m_ulADC_NormalisedValue;
	U32 m_ulADC_TemperatureSensor;
	U32 m_ulADC_Offset;
}ADC_PARMS;

typedef struct stMR_ArrayParms
{
	U32 m_ulSensorValue[12];
	U32 m_ulSensorOffset[12];
	U32 m_ulSensorPeak[12];
}MR_ARRAY;

typedef struct stLinearization
{
	U32 m_ulFixedDistance;
	U32 m_ulLinearizedValue;
	MR_SENSORS m_etSelectedSensor;
	U32 m_ulIsMagnetSteady;
}LINEARIZATION;

typedef struct stApolloDeviceData
{
	U32 m_ulSeconds;
	U32 m_ulMinutes;
	U32 m_ulHours;
	U32 m_ulDays;
	U32 m_ulDate;
	U32 m_ulYears;
	U32 m_ulOperationTimeHours;
	U32 m_ulStrokeCount;
	U32 m_ulStrokeLength;
	U32 m_ulWeek;
	U32 m_ulYear;
	U32 m_ulFirmwareVersion;
	U32 m_ulPCB_Version;
	U32 m_ulSolenoidOne;
	U32 m_ulSolenoidTwo;
	U32 m_ulSolenoidThree;
	U32 m_ulSolenoidOneActivations;
	U32 m_ulSolenoidTwoActivations;
	U32 m_ulSolenoidThreeActivations;
	U32 m_ulMaximumTemparature;
	U32 m_ulSensorStateActivated;
	U32 m_ulSensorStateDeActivated;
	U32 m_ulSensorStateDeEnergised;
	U32 m_ulSensorStateEnergised;
	U32 m_ulSensorStateLowerSeatLift;
	F32 m_ulPositionOfMagnet;
	U32 m_ulPosDeEnergised;
	U32 m_ulPosEnergised;
	U32 m_ulPosLowSeatLift;
	U32 m_ulPosUppSeatLift;
	U32 m_ulTolDeEnLow;
	U32 m_ulTolDeEnHigh;
	U32 m_ulTolEnLow;
	U32 m_ulTolEnHigh;
	U32 m_ulTolSL_Low;
	U32 m_ulTolSL_High;
	U32 m_ulIsMagnetSteady;
	U32 m_ulAutoSetupInterlock;
	U32 m_ulSeatCleanFunction;
	U32 m_ulValidationIntelligence;
	U32 m_ulASI_ForPMO;
	U32 m_ulCalibrationFlag;
	U32 m_ulInputOnSV_One;
	U32 m_ulInputOnSV_Two;
	U32 m_ulInputOnSV_Three;
	U32 m_ulInputNoSV;
	U32 m_ulMainCleanFunction;
	U32 m_ulInputOnAutoSetup;
	U32 m_ulInputOnExtSensor;
	U32 m_ulErrorTimeout;
	VALVES m_etValveType;
}DEVICE_DATA;

typedef struct stPushButton
{
	U32 m_ulPB_Press1S;
	U32 m_ulPB_PressL1S;
	U32 m_ulPB_Timer;
}PUSH_BUTTON;

typedef struct m_stAutoCalParms
{
	U32 m_ulIsMagnetSteady;
	U32 m_ulAutoCalP1Over;
	U32 m_ulAutoCalP2Over;
	U32 m_ulAutoCalP3Over;
}AUTO_CALIBRATION_PARMS;

//Global Structure
typedef struct g_stApolloParms
{
	U32 m_ulSysClock;
	U32 m_ul10MS_Timer;
	U32 m_ul50MS_Timer;
	U32 m_ulOneMS_Timer;
	U32 m_ul100MS_Timer;
	U32 m_ul200MS_Timer;
	U32 m_ul500MS_Timer;
	U32 m_ulOneS_Timer;
	U32 m_ul30S_Timer;
	U32 m_ulMeasureTemp;
	U32 m_NumberOfSolenoids;
	U32 m_ulIsSystemCalibrated;

	ERRORS m_etErrorConditions;
	STATUS m_etConversionStatusMR;
	STATUS m_etConversionStatusTS;
	LED_CON m_stLED_Control;
	DEV_TYPE m_etDeviceType;
	MR_ARRAY m_stMR_ArrayParms;
	ASI_PARMS m_stASI_Parms;
	ADC_PARMS m_stADC_Parms;
	SOLENOIDS m_etSolenoids;
	PLC_INPUTS m_etPLCInputs;
	UART_PARMS m_stUART_Parms;
	MR_SENSORS m_etCurrentSensor;
	DIAGNOSTICS m_stDiagnostics;
	PUSH_BUTTON m_stPushButton;
	DEVICE_DATA m_stApolloDeviceData;
	LINEARIZATION m_stLinearization;
	APOLLO_STATES m_etPreviousState;
	APOLLO_STATES m_etApolloStates;
	STARTUP_STATES m_etStartupStates;
	LOC_ADDR_STATES m_etLocAddrStates;
	TEST_MODE_STATES m_etTestModeStates;
	MEASUREMENT_STATES m_etMeasurementStates;
	OUTPUT_CONTROL_STATES m_etOutputControlStates;
	AUTO_CALIBRATION_PARMS m_stAutoCalParms;
	AUTO_CALIBRATION_STATES m_etAutoCalStates;

}GLOBAL_PARMS;
