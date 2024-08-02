/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

Задание 3
Реализовать систему с использованием механизма приостановки задач. Основные условия:
1. Система имеет 2 задачи с одинаковым приоритетом.
2. Задача 1 – переключает пользовательский светодиод на отладочной плате с периодичностью 1 с.
Запускается приостановленной (Suspended), светодиод выключен.
3. Задача 2 – читает состояние пользовательской кнопки на отладочной плате. При однократном
нажатии на кнопку задача должна возобновлять работу задачи 1, если последняя была
приостановлена (Suspended), или наоборот должна приостанавливать выполнение задачи 1.
4. Выполнить сборку, запуск и тестирование проекта на отладочной плате.
Критерии готовности (Definition of Done)
1. Проект собирается и загружается в отладочную плату без ошибок и предупреждений.
2. Начальные условия при включении системы соблюдаются.
3. Нажатия на кнопку обрабатываются в соответствии с условиями задания.
4. В реализации задач используются функции приостановки и возобновления задач FreeRTOS.


USING STM32F407VET6 board:

SWD interface
PE13 - output LED1
PE10 - input Key
*/
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>
#include <stdlib.h>
/************************global values********************************/
uint8_t ucTask1Status = 0;
xTaskHandle xTask1Handle;

/****************************func************************************/
void hwInit(void){
  //LED
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
  GPIOE->MODER |= GPIO_MODER_MODE13_0;
  GPIOE->BSRR |= GPIO_BSRR_BS13;
  //Key
  GPIOE->MODER &= ~(GPIO_MODER_MODE10);
}

void vTask1(void * pvParameters){
  while(1){
    GPIOE->BSRR |= GPIO_BSRR_BS13;
    vTaskDelay(1000);
    GPIOE->BSRR |= GPIO_BSRR_BR13;
    vTaskDelay(1000);
  }
  vTaskDelete(NULL);
}

void vTask2(void * pvParameters){
  while(1){
    if(!(GPIOE->IDR & GPIO_IDR_ID10)){
      vTaskDelay(100);
      if(!(GPIOE->IDR & GPIO_IDR_ID10)){
        if(ucTask1Status == 0){
          vTaskResume(xTask1Handle);
          ucTask1Status = 1;
        }else{
          vTaskSuspend(xTask1Handle);
          ucTask1Status = 0;
        }
      }
    }
  }
  vTaskDelete(NULL);
}
/****************************main************************************/

int main(void) {
  hwInit();
  printf("--- System startup ---\n");

  xTaskCreate(vTask1, "vTask1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle);
  vTaskSuspend(xTask1Handle);
  xTaskCreate(vTask2, "vTask2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  vTaskStartScheduler();

  while(1){}
}

/*************************** End of file ****************************/
