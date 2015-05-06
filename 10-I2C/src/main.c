/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f401_discovery.h"
#include "stm32f401_discovery_lsm303dlhc.h"
#include <stdio.h>
			
void I2C_Initialization(void);
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

void USART_Config(void);
void GPIO_init(void);
int __io_putchar(int ch);

__IO uint32_t TimingDelay;
volatile ErrorStatus result;

int main(void)
{
	uint8_t i;
	LSM303DLHCAcc_InitTypeDef LSM303DLHC_InitStruct;
	uint8_t Buffer_X[6];
	int16_t AccX, AccY, AccZ;
	char outString[30];

	I2C_Initialization();
	SysTick_Config(SystemCoreClock / 1000);

	GPIO_init() ;
		/* USART configuration */
	USART_Config();

	/* Fill the accelerometer structure */
	LSM303DLHC_InitStruct.Power_Mode = LSM303DLHC_NORMAL_MODE;
	LSM303DLHC_InitStruct.AccOutput_DataRate = LSM303DLHC_ODR_50_HZ;
	LSM303DLHC_InitStruct.Axes_Enable= LSM303DLHC_AXES_ENABLE;
	LSM303DLHC_InitStruct.AccFull_Scale = LSM303DLHC_FULLSCALE_2G;
	LSM303DLHC_InitStruct.BlockData_Update = LSM303DLHC_BlockUpdate_Continous;
	LSM303DLHC_InitStruct.Endianness=LSM303DLHC_BLE_LSB;
	LSM303DLHC_InitStruct.High_Resolution=LSM303DLHC_HR_ENABLE;

	LSM303DLHC_AccInit(&LSM303DLHC_InitStruct);


	while(1){
		//I2C registers are 8 bit long, it is thus necessary to concatenate Most significant byte and Least significant byte
		//Read fisrt least significant and then most significant
		result = LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_L_A, 1, &Buffer_X[0]);
		result = LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_H_A, 1, &Buffer_X[1]);
		AccX = Buffer_X[0] +((int16_t)Buffer_X[1]<<8);
		result = LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Y_L_A, 1, &Buffer_X[2]);
		result = LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Y_H_A, 1, &Buffer_X[3]);
		AccY = Buffer_X[2] +((int16_t)Buffer_X[3]<<8);
		result = LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Z_L_A, 1, &Buffer_X[4]);
		result = LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Z_H_A, 1, &Buffer_X[5]);
		AccZ = Buffer_X[4] +((int16_t)Buffer_X[5]<<8);
		Delay(50);

		//Send data via USART
		i = 0;
		sprintf(outString,"%d, %d, %d, \r\n",AccX,AccY,AccZ );
		while(outString[i] != '\n'){
			  USART_SendData(USART2, (uint8_t) outString[i]);
			  /* Loop until the end of transmission */
			  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			  {}
			  i++;
		}
	}
}




void SysTick_Handler(void)
{

	TimingDelay_Decrement();

}

void I2C_Initialization(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef  I2C_InitStructure;

	/* Enable the I2C periph */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	/* Enable SCK and SDA GPIO clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* I2C SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* I2C SDA pin configuration */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* I2C configuration -------------------------------------------------------*/
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;

	/* Apply LSM303DLHC_I2C configuration after enabling it */
	I2C_Init(I2C1, &I2C_InitStructure);

	/* LSM303DLHC_I2C Peripheral Enable */
	I2C_Cmd(I2C1, ENABLE);
}


/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}



/**
  * @brief  Configures the USART Peripheral.
  * @param  None
  * @retval None
  */
void USART_Config(void)
{
	USART_InitTypeDef USART_InitStructure;

	/* Enable UART clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* USARTx configured as follows:
		- BaudRate = 115200 baud
		- Word Length = 8 Bits
		- One Stop Bit
		- No parity
		- Hardware flow control disabled (RTS and CTS signals)
		- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* USART initialization */
	USART_Init(USART2, &USART_InitStructure);

	/* Enable USART */
	USART_Cmd(USART2, ENABLE);

}


void GPIO_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* Connect PXx to USARTx_Tx*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);

	/* Connect PXx to USARTx_Rx*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	/* Configure USART Tx as alternate function  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART Rx as alternate function  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);



}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */

int __io_putchar(int ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART2, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}

  return ch;
}
