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
#include "stm32f4xx.h"
#include "stm32f401_discovery.h"
			

void USART_Config(void);
void GPIO_init(void);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

char ch;
	GPIO_init() ;
	/* USART configuration */
	USART_Config();

  /* Output a message on Hyperterminal using printf function */
  printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");
  ch = getchar();
  ch = getchar();
  printf("\n\r USART2\n\r");
  ch = getchar();
   printf("\n\r USART3\n\r");

  while (1)
  {
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

int __io_getchar(int ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
	{}
		ch = USART_ReceiveData(USART2);

  /* Loop until the end of transmission */


  return ch;
}
