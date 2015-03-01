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
			

int find_maximum(u8[], int);

int main() {
  int size, location, maximum;  //Int variables (32 bit)
  //Array of Unsigned 8 bit
  u8 array[]  = {3, 6, 3, 7, 12, 15, 1, 8, 256, 2, 763, 85};

  //Compute size of the array
  size = sizeof(array)/sizeof(array[0]);
  location = find_maximum(array, size-1);
  maximum  = array[location];

  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDOn(LED4);

  while(1);
}

int find_maximum(u8 a[], int n) {
  int c, max, index;
  max = a[0];
  index = 0;

  for (c = 1; c < n; c++) {
    if (a[c] > max) {
       index = c;
       max = a[c];
    }
  }
  return index;
}
