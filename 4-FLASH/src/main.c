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
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/

#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_3   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_4   /* End @ of user Flash area */

/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */

#define DATA_32                 ((uint32_t)0x12345678) /*DATA TO BE WRITTEN*/


uint32_t  Address = 0, i = 0 ;
volatile uint32_t data32 = 0 , MemoryErrors = 0 ;


int main(void)
{

  /* Initialize LED4 and LED5 mounted on STM32F401C-DISCO board */
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);
  /* Unlock the Flash to enable the flash control register access *************/
  FLASH_Unlock();

  /* Erase the user Flash area
  (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  /* Clear pending flags (if any) */
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);



    /* Call EraseSectorFunction to clean the sector */
    if (FLASH_EraseSector(FLASH_Sector_3, VoltageRange_3) != FLASH_COMPLETE)
    {
      /* Error occurred while sector erase. LED ON */
      while (1)
      { STM_EVAL_LEDOn(LED5); }
    }

  /* Program the user Flash area word by word
  (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  Address = FLASH_USER_START_ADDR;

  while (Address < FLASH_USER_END_ADDR)
  {
    if (FLASH_ProgramWord(Address, DATA_32) == FLASH_COMPLETE)
    {
      Address = Address + 4;
    }
    else
    {
      /* Error occurred while writing data in Flash memory.LED ON */
      while (1)
      {STM_EVAL_LEDOn(LED5); }
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
  to protect the FLASH memory against possible unwanted operation) *********/
  FLASH_Lock();

  /* Check if the programmed data is OK*/
  Address = FLASH_USER_START_ADDR;
  MemoryErrors = 0x0;

  while (Address < FLASH_USER_END_ADDR)
  {
    data32 = *(volatile uint32_t*)Address;

    if (data32 != DATA_32)
    {
      MemoryErrors++;
      /* Data is not programmed correctly */
      STM_EVAL_LEDOn(LED5);
    }
    else
    {
      /* Data programmed correctly */
      STM_EVAL_LEDOn(LED4);
    }

    Address = Address + 4;
  }

  while (1)
  {
  }
}

