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


  volatile uint16_t CCR1_Val = 54618;
  volatile uint16_t CCR2_Val = 27309;
  volatile uint16_t CCR3_Val = 13654;
  volatile uint16_t CCR4_Val = 6826;


static void TIM_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint16_t PrescalerValue = 0;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;


  /* TIM Configuration */
  TIM_Config();

  /* -----------------------------------------------------------------------
  TIM3 Configuration: Output Compare Timing Mode:

  In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1),
  since APB1 prescaler is different from 1.
  TIM3CLK = 2 * PCLK1
  PCLK1 = HCLK / 4
  => TIM3CLK = HCLK / 2 = SystemCoreClock /2

  To get TIM3 counter clock at 500 KHz, the prescaler is computed as follows:
  Prescaler = (TIM3CLK / TIM3 counter clock) - 1
  Prescaler = ((SystemCoreClock /2) /500 KHz) - 1

  CC1 update rate = TIM3 counter clock / CCR1_Val = 9.154 Hz
  ==> Toggling frequency = 4.57 Hz

  C2 update rate = TIM3 counter clock / CCR2_Val = 18.31 Hz
  ==> Toggling frequency = 9.15 Hz

  CC3 update rate = TIM3 counter clock / CCR3_Val = 36.62 Hz
  ==> Toggling frequency = 18.31 Hz

  CC4 update rate = TIM3 counter clock / CCR4_Val = 73.25 Hz
  ==> Toggling frequency = 36.62 Hz

  ----------------------------------------------------------------------- */

  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 500000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);

  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;

  TIM_OC3Init(TIM3, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

  TIM_OC4Init(TIM3, &TIM_OCInitStructure);

  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);

  /* TIM Interrupts enable */
  TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);

  while (1);
}

/**
  * @brief  Configure the TIM IRQ Handler.
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Enable the TIM3 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Initialize Leds mounted on STM32F401C-DISCO board */
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);

  /* Turn on LED4, LED3, LED5 and LED6 */
  STM_EVAL_LEDOn(LED4);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED5);
  STM_EVAL_LEDOn(LED6);
}

/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
  uint16_t capture = 0;

  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);

    /* LED4 toggling with frequency = 4.57 Hz */
    STM_EVAL_LEDToggle(LED4);
	capture = TIM_GetCapture1(TIM3);
    TIM_SetCompare1(TIM3, capture + CCR1_Val);
  }
  else if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);

    /* LED3 toggling with frequency = 9.15 Hz */
    STM_EVAL_LEDToggle(LED3);
    capture = TIM_GetCapture2(TIM3);
    TIM_SetCompare2(TIM3, capture + CCR2_Val);
  }
  else if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);

    /* LED5 toggling with frequency = 18.31 Hz */
    STM_EVAL_LEDToggle(LED5);
    capture = TIM_GetCapture3(TIM3);
    TIM_SetCompare3(TIM3, capture + CCR3_Val);
  }
  else
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);

    /* LED6 toggling with frequency = 36.62 Hz */
    STM_EVAL_LEDToggle(LED6);
    capture = TIM_GetCapture4(TIM3);
    TIM_SetCompare4(TIM3, capture + CCR4_Val);
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
