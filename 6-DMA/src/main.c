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

#define BUFFER_SIZE              32
#define TIMEOUT_MAX              1000 /* Maximum timeout value */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

__IO TestStatus  TransferStatus = FAILED;

const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
                                    0x11111111,0x22222222,0x33333333,0x44444444,
                                    0x55555555,0x66666666,0x77777777,0x88888888,
                                    0x99999999,0xAAAAAAAA,0xBBBBBBBB,0xCCCCCCCC,
                                    0xDDDDDDDD,0xEEEEEEEE,0xFFFFFFFF,0x00000000,
                                    0xCAFFE000,0xDECADECA,0x2CAFFE00,0xDECADECA,
                                    0xBADBAD00,0xCADECADE,0x00FACCE0,0x12121212,
                                    0x23232323,0x34343434,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};
uint32_t aDST_Buffer[BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void DMA_Config(void);
void NVIC_config(void);
TestStatus Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength);
__IO uint32_t    Timeout = TIMEOUT_MAX;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       files (startup_stm32f40_41xxx.s/startup_stm32f427_437xx.s/startup_stm32f429_439xx.s)
       before to branch to application main.
     */

  /* Configure LEDs to monitor program status */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  Timeout = TIMEOUT_MAX;
  /* Configure and enable the DMA Stream for Memory to Memory transfer */
  DMA_Config();
  NVIC_config();

  /* Wait the end of transmission (the DMA Stream is disabled by Hardware at the
     end of the transfer) .
     There is also another way to check on end of transfer by monitoring the
     number of remaining data to be transferred. */

  /* while (DMA_GetCurrentMemoryTarget(DMA_STREAM) != 0) */  /* First method */

  while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE && (Timeout-- > 0))/* Second method */
  {
	  /* Check if a timeout condition occurred */
	  if (Timeout == 0)
	  {
	    /* Manage the error: to simplify the code enter an infinite loop */
	    while (1)
	    {
	    }
	  }
    /*
       Since this code present a simple example of how to use DMA, it is just
       waiting on the end of transfer.
       But, while DMA Stream is transferring data, the CPU is free to perform
       other tasks in parallel to the DMA transfer.
    */
  }

  /* Check if the transmitted and received data are equal */
  TransferStatus = Buffercmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE);
  /* TransferStatus = PASSED, if the transmitted and received data
     are the same */
  /* TransferStatus = FAILED, if the transmitted and received data
     are different */

  if (TransferStatus != FAILED)
  {
    /* Turn LED4 on: Transfer correct */
    STM_EVAL_LEDOn(LED4);
  }

  while (1)
  {
  }
}

/**
* @brief  Configure the DMA NVIC
* @param  None
* @retval None
*/
void NVIC_config(void){
	  NVIC_InitTypeDef NVIC_InitStructure;
	  /* Enable the DMA Stream IRQ Channel */
	  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configure the DMA controller according to the Stream parameters
  * @param  None
  * @retval None
  */
static void DMA_Config(void)
{
  DMA_InitTypeDef  DMA_InitStructure;

  /* Enable DMA clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

  /* Reset DMA Stream registers (for debug purpose) */
  DMA_DeInit(DMA2_Stream0);

  /* Check if the DMA Stream is disabled before enabling it.
     Note that this step is useful when the same Stream is used multiple times:
     enabled, then disabled then re-enabled... In this case, the DMA Stream disable
     will be effective only at the end of the ongoing data transfer and it will
     not be possible to re-configure it before making sure that the Enable bit
     has been cleared by hardware. If the Stream is used only once, this step might
     be bypassed. */
  while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE)
  {
  }

  /* Configure DMA Stream */
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)aSRC_Const_Buffer;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)aDST_Buffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;
  DMA_InitStructure.DMA_BufferSize = (uint32_t)BUFFER_SIZE;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);

  /* Enable DMA Stream Transfer Complete interrupt */
  DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);

  /* DMA Stream enable */
  DMA_Cmd(DMA2_Stream0, ENABLE);

  /* Check if the DMA Stream has been effectively enabled.
     The DMA Stream Enable bit is cleared immediately by hardware if there is an
     error in the configuration parameters and the transfer is no started (ie. when
     wrong FIFO threshold is configured ...) */
  while ((DMA_GetCmdStatus(DMA2_Stream0) != ENABLE))
  {  }

}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer, pBuffer1: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer identical to pBuffer1
  *         FAILED: pBuffer differs from pBuffer1
  */
TestStatus Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer != *pBuffer1)
    {
      return FAILED;
    }

    pBuffer++;
    pBuffer1++;
  }

  return PASSED;
}

/**
  * @brief  This function handles DMA Stream interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream0_IRQHandler(void)
{
  /* Test on DMA Stream Transfer Complete interrupt */
  if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0))
  {
    /* Clear DMA Stream Transfer Complete interrupt pending bit */
    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);

    /* Turn LED3 on: End of Transfer */
    STM_EVAL_LEDOn(LED3);
  }
}
