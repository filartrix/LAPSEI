/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f401_discovery.h"
			

void USART_Config(void);
void GPIO_init(void);
void USART_DMA_NVIC_Configuration(void);
void DMA_Configuration(char* BufferAddr, u16 dataSize);

#define BuffSize 100
char rx_buffer[BuffSize];   // Local holding buffer
u8 dataReady = 0;
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

	char buffer[BuffSize];
	int value = 0;
	u16 dataSize = 0;
	GPIO_init() ;
	/* USART configuration */
	USART_Config();

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	USART_DMA_NVIC_Configuration();

	dataSize = sprintf(buffer,"Initialization\r\n");
	DMA_Configuration(buffer, dataSize); //Configure and Start DMA Transfer

	while (1)
	{

		if (dataReady == 1){
			value = atoi(rx_buffer);  // Extract integer from RX buffer
			dataSize = sprintf(buffer,"You have entered the value: %d \r\n",value);  //Create a string
			DMA_Configuration(buffer, dataSize);// Send the string using DMA
			memset (rx_buffer,'\0',BuffSize);  	// Reset the RX Buffer
			dataReady = 0;					   	//Reset the Data Ready flag

		}
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

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // enable the USART2 receive interrupt


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

/**************************************************************************************/

void USART_DMA_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the USART2 RX DMA Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;             // we want to configure the USART1 interrupts
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         // this sets the priority group of the USART1 interrupts
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        // this sets the subpriority inside the group
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // the USART2 interrupts are globally enabled
  NVIC_Init(&NVIC_InitStructure);
}



/**************************************************************************************/

void DMA_Configuration(char* BufferAddr, u16 dataSize)
{
  DMA_InitTypeDef  DMA_InitStructure;

  DMA_DeInit(DMA1_Stream6);

  DMA_InitStructure.DMA_Channel = DMA_Channel_4;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; // Transmit
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)BufferAddr;
  DMA_InitStructure.DMA_BufferSize = dataSize;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  DMA_Init(DMA1_Stream6, &DMA_InitStructure);

  /* Enable the USART Tx DMA request */
  USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

  /* Enable DMA Stream Transfer Complete interrupt */
  DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);

  /* Enable the DMA RX Stream */
  DMA_Cmd(DMA1_Stream6, ENABLE);
}

u8 USART_DMA_TrasnferComplete;
void DMA1_Stream6_IRQHandler(void)
{
  /* Test on DMA Stream Transfer Complete interrupt */
  if (DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6))
  {
    /* Clear DMA Stream Transfer Complete interrupt pending bit */
    DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
    USART_DMA_TrasnferComplete = 1;
  }
}


void USART2_IRQHandler(void)
{
	static int rx_index = 0;

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // Received character?
	{
		char rx =  USART_ReceiveData(USART2);
		rx_buffer[rx_index++] = rx;
		if(rx == '\n' || rx == '\r' || rx_index ==BuffSize ){
			dataReady = 1;
			rx_index = 0;
		}
	}
}



/**************************************************************************************/


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



