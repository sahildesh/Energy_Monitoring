/*-----------------------------System Includes-------------------------------*/
#define PART_TM4C123AE6PM

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <pin_map.h>
#include <tm4c123gh6pz.h>
#include <hw_memmap.h>
#include <sysctl.h>
#include <timer.h>
#include <interrupt.h>
#include <gpio.h>
#include <adc.h>
#include <eeprom.h>
#include <pwm.h>
#include <uart.h>
#include <IQmathLib.h>
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
void InitSystemClock(void)
{
	//Use external crystal and PLL to set clock speed to 50MHz
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //50mhz
	g_stApolloParms.m_ulSysClock = SysCtlClockGet();
}// End of function

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
void InitGPIOClock(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
}// End of function

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
void InitGPIO_Inputs(void)
{

	// Setup Input pins on PORTA
	// PA7 - Solenoid Detect 3
	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_7);
	GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_7,
											GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

	// Setup Input pins on PORTB
	// PB1 - LED1
	// PB2 - PST
	// PB3 - DSR
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
										GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_BOTH_EDGES);
	GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOIntRegister(GPIO_PORTB_BASE, PortB_Interrupt_Handler);
	IntEnable(INT_GPIOB);

	// Setup input pins on PORTC
	// PC4 - DIP41
	// PC5 - DIP42
	// PC6 - DIP43
	// PC7 - DIP44
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPadConfigSet(GPIO_PORTC_BASE,  GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
													GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);



#if 1
	//*********************************************************************************************//
	//*********************************************************************************************//

	//This part of code is only for testing purpose
	// Setup input pins on PORTD
	// PD0 - DIP8 Version Configuration
	// PD1 - DIP8 Enable Local Addressing
	// PD2 - DIP8 Slave Select
	// PD3 - DIP8 Slave Address bit 0
	// PD4 - DIP8 Slave Address bit 1
	// PD5 - DIP8 Slave Address bit 2
	// PD6 - DIP8 Slave Address bit 3
	// PD7 - DIP8 Slave Address bit 4

    GPIO_PORTD_LOCK_R = 0x4C4F434B; 	//Enable writing to GPIOCR registers
    GPIO_PORTD_CR_R = 0x000000FF;
    GPIO_PORTD_DIR_R = 0x00000000;		//0x0:all port pins are input
    GPIO_PORTD_AFSEL_R = 0x00000000; 	//0x0: The associated pin functions as a GPIO & is controlled by GPIO registers
    GPIO_PORTD_DR2R_R = 0x000000FF; 	//0x1:2mA drive
    GPIO_PORTD_PUR_R = 0x000000FF; 		//0x1:all pins have internal weak pull ups
    GPIO_PORTD_DEN_R = 0x000000FF; 		//0x1: digital enable

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);

	GPIOPinTypeGPIOInput(GPIO_PORTM_BASE, GPIO_PIN_0);
	GPIOPadConfigSet(GPIO_PORTM_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	//*********************************************************************************************//
	//*********************************************************************************************//
#endif

	// Setup inputs pins on PORTE
	// PE0 - External inductive sensor
	// PE1 - Push Button switch
	// PE2 - ASI Board detection switch
	// PE3 - ADC channel
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 |  GPIO_PIN_3);
	GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_RISING_EDGE);
	GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0);
	GPIOIntRegister(GPIO_PORTE_BASE, PortE_Interrupt_Handler);
	IntEnable(INT_GPIOE);

	// Setup input pins on PORTF
	// PF0 - PLC Trigger Solenoid 1
	// PF1 - PLC Trigger Solenoid 2
	// PF2 - PLC Trigger Solenoid 3
	// PF3 - PLC Auto Setup Command
	// PF4 - Solenoid Detect 1

    GPIO_PORTF_LOCK_R = 0x4C4F434B; 	//Enable writing to GPIOCR registers
    GPIO_PORTF_CR_R = 0x000000FF;
    GPIO_PORTF_DIR_R = 0x00000000;		//0x0:all port pins are input
    GPIO_PORTF_AFSEL_R = 0x00000000; 	//0x0: The associated pin functions as a GPIO & is controlled by GPIO registers
    GPIO_PORTF_DR2R_R = 0x000000FF; 	//0x1:2mA drive
    GPIO_PORTF_PUR_R = 0x000000FF; 		//0x1:all pins have internal weak pull ups
    GPIO_PORTF_DEN_R = 0x000000FF; 		//0x1: digital enable

	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_BOTH_EDGES);
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOIntRegister(GPIO_PORTF_BASE, PortF_Interrupt_Handler);
	IntEnable(INT_GPIOF);

	// Setup input pin on PORTG
	// PG5 - Solenoid Detect 2
	GPIOPinTypeGPIOInput(GPIO_PORTG_BASE, GPIO_PIN_5);
	GPIOPadConfigSet(GPIO_PORTG_BASE,  GPIO_PIN_5,	GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}// End of function

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
void InitGPIO_Outputs(void)
{
	// Setup output pins of PORTA
	// PA2 - Sensor Select Line 0
	// PA3 - Sensor Select Line 1
	// PA4 - Sensor Select Line 2
	// PA5 - Multiplexer Enable
	// PA6 - Heartbeat LED
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);

	// Setup output pins of PORTB
	// PB0 - Addressing channel for AS-I Chip
	// PB4 - Green LED
	// PB5 - Blue LED
	// PB6 - Green LED
	// PB7 - Solenoid 1
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

	// Setup output pins of PORTE
	// PE4 - Solenoid 2
	// PE5 - Solenoid 3
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

	// Setup output pins of PORTG
	// PG0 - PLC De energized
	// PG1 - PLC energized
	// PG2 - PLC Seat Lift Lower
	// PG3 - PLC Seat Lift Upper
	// PG4 - PLC Status/OK
	GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);

	// This is only for the testing purpose
#if 1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
#endif
}// End of function

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
void InitGPIO_SpecialPins(void)
{
	// Setup UART pins on PORTA
	// PA0 - RX
	// PA0 - TX
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	// Setup UART pins on PORTA
	// PA6 - PWM for heartbeat LED

	// Setup PWM pins on PORTB
	// PB4 - Solenoid 1
	// PB5 - Solenoid 2
	// PB6 - Solenoid 3
	// PB7 - Red LED
	GPIOPinConfigure(GPIO_PB4_M0PWM2);
	GPIOPinConfigure(GPIO_PB5_M0PWM3);
	GPIOPinConfigure(GPIO_PB6_M0PWM0);
	GPIOPinConfigure(GPIO_PB7_M0PWM1);
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

	// Setup ADC pin on PORTE
	// PE3 - Analog input from instrumentation amplifier
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);

	// Setup PWM pins on PORTE
	// PE4 - Green LED
	// PE5 - Blue LED
	GPIOPinConfigure(GPIO_PE4_M0PWM4);
	GPIOPinConfigure(GPIO_PE5_M0PWM5);
	GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

#if 0
	// This code was only written for test
	GPIOPinConfigure(GPIO_PD0_M1PWM0);
	GPIOPinConfigure(GPIO_PD1_M1PWM1);
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);
#endif

}// End of function

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
void InitGPIO(void)
{
	InitGPIOClock();
	InitGPIO_Inputs();
	InitGPIO_Outputs();
	InitGPIO_SpecialPins();
}// End of function

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
void InitTimerSystem(void)
{
	// Initialise the system timer
	// This timer generates an interrupt every 1 ms
	// It is used as a timer to handle all system level events
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerDisable(TIMER0_BASE, TIMER_BOTH);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
	TimerLoadSet(TIMER0_BASE, TIMER_BOTH, (g_stApolloParms.m_ulSysClock/1000));
	TimerIntRegister(TIMER0_BASE, TIMER_BOTH, ISR_TimerSystem);
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER0_BASE, TIMER_BOTH);
}// End of function

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
void InitTimerCommunications(void)
{
	// This timer is used for addressing line communication
	// This timer will only be used in the ASI model
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	TimerDisable(TIMER1_BASE, TIMER_BOTH);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_A_PERIODIC);
	TimerLoadSet(TIMER1_BASE, TIMER_A,(3 * g_stApolloParms.m_ulSysClock/1000000));
	TimerIntRegister(TIMER1_BASE, TIMER_A, ISR_TimerCommunications);
	IntEnable(INT_TIMER1A);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}// End of function

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
void InitTimerIndications(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	TimerDisable(TIMER2_BASE, TIMER_BOTH);
	TimerConfigure(TIMER2_BASE, TIMER_CFG_A_PERIODIC);
	TimerLoadSet(TIMER2_BASE, TIMER_BOTH, (g_stApolloParms.m_ulSysClock/100)); // 10 ms
	TimerIntRegister(TIMER2_BASE, TIMER_BOTH, ISR_TimerIndications);
	IntEnable(INT_TIMER2A);
	TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER2_BASE, TIMER_BOTH);
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
void InitTimerRead(void)
{
	// This timer is used for addressing line communication
	// This timer will only be used in the ASI model
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
	TimerDisable(TIMER3_BASE, TIMER_BOTH);
	TimerConfigure(TIMER3_BASE, TIMER_CFG_A_PERIODIC);
	TimerLoadSet(TIMER3_BASE, TIMER_A,(3 * g_stApolloParms.m_ulSysClock/1000000));
	TimerIntRegister(TIMER3_BASE, TIMER_A, ISR_TimerReadData);
	IntEnable(INT_TIMER3A);
	TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
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
void InitTimers(void)
{
	// Initialise both the timers required for the system and communications
	InitTimerSystem();
	InitTimerCommunications();
	InitTimerIndications();
	InitTimerRead();
}// End of function

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
#define PWM_FREQ 5000 //Hz
#define PWM_DUTY 7500
void InitPWM(void)
{
#if 0
	// Initialise PWM module 0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	//
	// Set the period. For a 50 KHz frequency, the period = 1/50,000, or 20
	// microseconds. For a 20 MHz clock, this translates to 400 clock ticks.
	// Use this value to set the period.
	//
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, 400);
	//
	// Set the pulse width of PWM0 for a 25% duty cycle.
	//
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 100);
	//
	// Set the pulse width of PWM1 for a 75% duty cycle.
	//
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, 300);
	//
	// Start the timers in generator 0.
	//
	PWMGenEnable(PWM1_BASE, PWM_GEN_0);
#endif

#if 1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, g_stApolloParms.m_ulSysClock/PWM_FREQ);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, g_stApolloParms.m_ulSysClock/PWM_FREQ);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, g_stApolloParms.m_ulSysClock/PWM_FREQ);

	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, g_stApolloParms.m_ulSysClock/PWM_DUTY);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, g_stApolloParms.m_ulSysClock/PWM_DUTY);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, g_stApolloParms.m_ulSysClock/PWM_DUTY);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, g_stApolloParms.m_ulSysClock/PWM_DUTY);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, g_stApolloParms.m_ulSysClock/PWM_DUTY);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, g_stApolloParms.m_ulSysClock/PWM_DUTY);

	PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	PWMGenEnable(PWM0_BASE, PWM_GEN_1);
	PWMGenEnable(PWM0_BASE, PWM_GEN_2);

	PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT | PWM_OUT_3_BIT | PWM_OUT_4_BIT | PWM_OUT_5_BIT, FALSE);
#endif
}// End of function

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
void InitADC(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PLL  | ADC_CLOCK_RATE_FULL, 5);
	ADCReferenceSet(ADC0_BASE, ADC_REF_EXT_3V);
	ADCSequenceConfigure(ADC0_BASE, SQNC_0, ADC_TRIGGER_PROCESSOR, PRIORITY_1);
	ADCSequenceStepConfigure(ADC0_BASE, SQNC_0, STEP_0, ADC_CTL_CH1);
	ADCSequenceStepConfigure(ADC0_BASE, SQNC_0, STEP_1, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);
	ADCHardwareOversampleConfigure(ADC0_BASE, 0);
	ADCIntEnable(ADC0_BASE, SQNC_0);
	ADCIntRegister(ADC0_BASE, SQNC_0, ISR_ADC);
	ADCSequenceEnable(ADC0_BASE, SQNC_0);
	ADCProcessorTrigger(ADC0_BASE, SQNC_0); // Run the ADC once and wait for the conversion to finish
}// End of function

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
void InitUART(void)
{

	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

	// UART initialisation routine here
	// 8 Bits
	// No Parity
	// One stop bit

	UARTConfigSetExpClk(UART0_BASE, g_stApolloParms.m_ulSysClock, 115200,
	(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
	UART_CONFIG_PAR_NONE));

	// UART Interrupt Enable
	UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
	UARTIntRegister(UART0_BASE, ISR_UART_Receive);
	IntEnable(INT_UART0);

	// Enable the UART
	UARTEnable(UART0_BASE);
}// End of function

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
U32 InitEEPROM(void)
{
//	Initialise Internal EEPROM1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
	return(EEPROMInit());
}// End of function

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
void GetDeviceVersion(void)
{
	U32 Loc_ulPinState;
	Loc_ulPinState = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3);
	if(Loc_ulPinState == LOW)
	{
		g_stApolloParms.m_etDeviceType = APOLLO_DIGITAL;
	}
	else
	{
		g_stApolloParms.m_etDeviceType = APOLLO_ASI;
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
#define OFFSET 4

void InitHardware(void)
{
	// RESET all global variables
	memset(&g_stApolloParms, RESET, sizeof(g_stApolloParms));

//	InitSystemClock();	// Set the system clock to 50Mhz - Do not use this
	if(EEPROM_INIT_OK != InitEEPROM())
	{
		return;
	}
	InitGPIO();			// Initialise all the GPIOs of the micro controller
	InitTimers();		// Initialise all the timers of the micro controller
	InitPWM();			// Initialise the PWM modules of the micro controller
	InitADC();			// Initialise the analogue to Digital Converter of the micro controller
	InitUART();			// Initialise the UART of the micro controller
	IntMasterEnable();	// Globally Enable Interrupts
	GetDeviceVersion();	// Check whether Apollo Device is digital or ASI version

	g_stApolloParms.m_etApolloStates = STARTUP;
}// End of function
