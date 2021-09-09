#include "Apollo_Header.h"
#include "Apollo_Functions.h"
#include <timer.h>
#include <pwm.h>
#include <gpio.h>
#include <tm4c123gh6pz.h>
#include <hw_memmap.h>
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
void CheckPB_Press(void)
{
	static U32 ulPB_Timer = RESET;
	U32 ulPortStatus = RESET;

	ulPortStatus = GPIOPinRead(GPIO_PORTM_BASE, GPIO_PIN_0);

	if(ulPB_Timer < 1000)
	{

		if(ulPortStatus == LOW)
		{
			ulPB_Timer++;
		}
		else
		{
			ulPB_Timer = RESET;
		}
	}
	else
	{
		if(ulPortStatus == LOW)
		{
			LED_Control(LED_ON, YELLOW_LED);
			GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x04);
		}
		else if(ulPortStatus == HIGH)
		{
			GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x00);
			LED_Control(LED_OFF, YELLOW_LED);
			g_stApolloParms.m_stPushButton.m_ulPB_Press1S = TRUE;
			ulPB_Timer = RESET;
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
Function Description:
**************************************************************************/
void ISR_TimerSystem(void)
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	g_stApolloParms.m_ulOneMS_Timer++;

	if(g_stApolloParms.m_ulOneMS_Timer % 10 == RESET)
	{
		g_stApolloParms.m_ul10MS_Timer = SET;
		if(g_stApolloParms.m_etApolloStates != STARTUP)
		{
			if(g_stApolloParms.m_etApolloStates != MEASUREMENT)
			{
				g_stApolloParms.m_etPreviousState = g_stApolloParms.m_etApolloStates;
				g_stApolloParms.m_etApolloStates = MEASUREMENT;
			}
		}
	}

	if(g_stApolloParms.m_ulOneMS_Timer % 50 == RESET)
	{
		g_stApolloParms.m_ul50MS_Timer = SET;
	}

	if(g_stApolloParms.m_ulOneMS_Timer % 100 == RESET)
	{
		g_stApolloParms.m_ul100MS_Timer = SET;
		g_stApolloParms.m_ulMeasureTemp = SET;
	}

	if(g_stApolloParms.m_ulOneMS_Timer % 200 == RESET)
	{
		g_stApolloParms.m_ul200MS_Timer = SET;
	}

	if(g_stApolloParms.m_ulOneMS_Timer % 500 == RESET)
	{
		g_stApolloParms.m_ul500MS_Timer = SET;
	}

	if(g_stApolloParms.m_ulOneMS_Timer % 1000 == RESET)
	{
		g_stApolloParms.m_ulOneS_Timer = SET;
	}

	if(g_stApolloParms.m_ulOneMS_Timer % 30000 == RESET)
	{
		g_stApolloParms.m_ul30S_Timer = SET;
	}


	if(g_stApolloParms.m_ulOneMS_Timer > 50000)
	{
		g_stApolloParms.m_ulOneMS_Timer = RESET;
	}
	CheckPB_Press();
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
void ISR_TimerCommunications(void)
{
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulManchesterTimer = SET;
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
void ISR_TimerIndications(void)
{
	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

	static U32 ulCounter = RESET;
	static U32 ulToggle = RESET;
	static U32 ulToggleDouble = RESET;
	static U32 ulLED_Activated = RESET;

	U32 ulLED_PWM = g_stApolloParms.m_stLED_Control.m_ulLED_PWM;
	LED_STATE etLED_State = g_stApolloParms.m_stLED_Control.m_etLED_State;
	GPIO_PORTA_DATA_R ^= 0x40;

	ulCounter++;

	switch(etLED_State)
	{
	case LED_OFF:
		if(ulLED_Activated == SET)
		{
			PWMOutputState(PWM0_BASE, ALL_LED, FALSE);
			PWMOutputState(PWM0_BASE, ulLED_PWM, FALSE);
			ulLED_Activated = RESET;
		}
		break;

	case LED_ON:
		if(ulLED_Activated == RESET)
		{
			PWMOutputState(PWM0_BASE, ALL_LED, FALSE);
			PWMOutputState(PWM0_BASE, ulLED_PWM, TRUE);
			ulLED_Activated = SET;
		}
		break;

	case TOGGLE_SLOW:
		if(ulCounter == 50)
		{
			ulCounter = RESET;
			if(ulToggle == RESET)
			{
				PWMOutputState(PWM0_BASE, ALL_LED, FALSE);
				PWMOutputState(PWM0_BASE, ulLED_PWM, TRUE);
				ulToggle = SET;
			}
			else if(ulToggle == SET)
			{
				PWMOutputState(PWM0_BASE, ALL_LED, FALSE);
				PWMOutputState(PWM0_BASE, ulLED_PWM, FALSE);
				ulToggle = RESET;
			}
		}
		break;

	case TOGGLE_FAST:
		if(ulCounter == 20)
		{
			ulCounter = RESET;
			g_stApolloParms.m_ul200MS_Timer = RESET;
			if(ulToggle == RESET)
			{
				PWMOutputState(PWM0_BASE, ALL_LED, FALSE);
				PWMOutputState(PWM0_BASE, ulLED_PWM, TRUE);
				ulToggle = SET;
			}
			else if(ulToggle == SET)
			{
				PWMOutputState(PWM0_BASE, ALL_LED, FALSE);
				PWMOutputState(PWM0_BASE, ulLED_PWM, FALSE);
				ulToggle = RESET;
			}
		}
		break;

	case TOGGLE_DOUBLE:
		if(ulToggleDouble < 10)
		{
			PWMOutputState(PWM0_BASE, ALL_LED, FALSE);
			PWMOutputState(PWM0_BASE, ulLED_PWM, TRUE);
			ulToggleDouble++;
		}
		else if(ulToggleDouble < 20)
		{
			PWMOutputState(PWM0_BASE, ALL_LED, FALSE);
			PWMOutputState(PWM0_BASE, ulLED_PWM, FALSE);
			ulToggleDouble++;
		}
		else if(ulToggleDouble < 30)
		{
			PWMOutputState(PWM0_BASE, ALL_LED, FALSE);
			PWMOutputState(PWM0_BASE, ulLED_PWM, TRUE);
			ulToggleDouble++;
		}
		else if(ulToggleDouble >= 30 && ulToggleDouble <= 100)
		{
			PWMOutputState(PWM0_BASE, ALL_LED, FALSE);
			PWMOutputState(PWM0_BASE, ulLED_PWM, FALSE);
			ulToggleDouble++;
			if(ulToggleDouble > 100)
			{
				ulToggleDouble = RESET;
			}
		}
		break;

	default:
			PWMOutputState(PWM0_BASE, ALL_LED, FALSE);
	break;
	}// EO LED state switch
}//EOF

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
void ISR_TimerReadData(void)
{
	TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
	g_stApolloParms.m_stASI_Parms.m_stLoc_Addr.m_ulDataReadTimer = SET;
}
