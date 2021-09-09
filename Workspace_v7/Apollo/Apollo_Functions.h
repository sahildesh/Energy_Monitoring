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
void InitHardware(void);
void InitSystemClock(void);
void InitGPIO(void);
void InitTimers(void);
void InitPWM(void);
void InitADC(void);
void InitUART(void);
U32 InitEEPROM(void);
U32 CheckSolenoidInputs(void);
U32 DecodeDataFrame(U32 Arg_ulReadData);
void InitGPIO_Inputs(void);
void InitGPIO_Outputs(void);
void InitGPIO_SpecialPins(void);
void InitTimerCommunications(void);
void InitTimerSystem(void);
void InitTimerIndications(void);
void ISR_TimerIndications(void);
void ISR_TimerCommunications(void);
void ISR_TimerSystem(void);
void ISR_TimerReadData(void);
void ISR_ADC(void);
void APSM(void);
void ISR_ADC_Temp(void);
void InitADC_Sqnc_MR_Array(void);
void InitADC_Sqnc_Temp(void);
void ISR_ADC_MR_Array(void);
void ISR_UART_Receive(void);
void PortF_Interrupt_Handler(void);
void PortB_Interrupt_Handler(void);
void SendUART_Menu(void);
void SetToleranceBands(VALVES );
void SendUART_String(U8 *, U32 );
void PortE_Interrupt_Handler(void);
void LED_Control(LED_STATE, U32 );
void ActivateSolenoidValve(SOLENOIDS );
void CheckKeyPress(void);
void ResetAllCalibrationData(void);
void SaveMR_PositionAsCurrent(PLC_OUTPUTS Arg_etPLC_Output);
U32 ReadFeedback(PLC_OUTPUTS Arg_etPLC_Feedback);
APOLLO_STATES ApolloOutputControl(void);
APOLLO_STATES ApolloMeasurement(void);
APOLLO_STATES ApolloStartup(void);
APOLLO_STATES ApolloAutoCalibration(void);
APOLLO_STATES ApolloOperationError(void);
APOLLO_STATES ApolloTestMode(void);

STARTUP_STATES ReadAndProcessLocalDIP_Switch(void);
STARTUP_STATES GetCalibrationData(void);
STARTUP_STATES DetectSolenoidValves(void);
STARTUP_STATES ReadAndProcessASI_DIP_Switch(void);
STARTUP_STATES Local_Addressing(void);

LOC_ADDR_STATES StoreGblManchesterData(void);
LOC_ADDR_STATES SendLocalAddrData(void);

UART_MENU_STATES UART_MainMenu(void);
UART_MENU_STATES UART_Daignostic(void);
UART_MENU_STATES UART_SetupValuesMenu(void);
UART_MENU_STATES UART_ManualOperationMenu(void);
UART_MENU_STATES UART_ManualSetupMenu(void);
UART_MENU_STATES UART_AdvancedSettingsMenu(void);
UART_MENU_STATES UART_AdvancedDiagnosticMenu(void);

MEASUREMENT_STATES ReadMR_Sensors(void);
MEASUREMENT_STATES LinearizeMR_Sensor(void);
MEASUREMENT_STATES SelectMR_Sensors(void);

OUTPUT_CONTROL_STATES EnterAutoSetup(void);
OUTPUT_CONTROL_STATES UpdateLightingStatus(void);
OUTPUT_CONTROL_STATES SolenoidControl(void);

AUTO_CALIBRATION_STATES SolenoidTerminalCheck(void);
AUTO_CALIBRATION_STATES MagnetRangeCheck(void);
U32 MagnetSteadyCheck(void);
AUTO_CALIBRATION_STATES AutomaticCalibration(void);
