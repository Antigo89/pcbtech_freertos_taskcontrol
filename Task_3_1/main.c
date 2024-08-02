/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

Задание 1
Реализовать систему с динамическим переключением приоритетов задач. Основные условия:
1. Система имеет две задачи: задача 1 (приоритет 1) и задача 2 (приоритет 2).
2. Задача 2 выполняет вывод через UART строки сообщения «Task 2 – runing», после чего повышает
приоритет задачи 1 до значения 3.
3. Задача 1 выполняет вывод через UART двух строк «Task 1 – runing» и «Task 1 – lower prio», после
чего понижает собственный приоритет до значения 1.
4. Выполнить сборку, запуск и тестирование проекта на отладочной плате.
Критерии готовности (Definition of Done)
1. Проект собирается и загружается в отладочную плату без ошибок и предупреждений.
2. По последовательному порту передаются сообщения от двух задач.
3. В реализации задач используются функции изменения приоритета задач FreeRTOS.


USING STM32F407VET6 board:

SWD interface
UART1 (PA9-TX, PA10-RX)

*/
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart_dbg.h"

#include <stdio.h>
#include <stdlib.h>
/************************global values********************************/
xTaskHandle xTask1Handle;

/****************************func************************************/
void vTask1(void * pvParameters){
  unsigned portBASE_TYPE uxPriority;
  while(1){
    printf("%s\n","Task1 - runing");
    printf("%s\n","Task1 - lower prio");
    vTaskPrioritySet(NULL, 1); 
  }
  vTaskDelete(NULL);
}

void vTask2(void * pvParameters){
  unsigned portBASE_TYPE uxPriority;
  while(1){
    printf("%s\n","Task2 - runing");
    vTaskPrioritySet(xTask1Handle, 3);
  }
  vTaskDelete(NULL);
}
/****************************main************************************/

int main(void) {
  usart1_init();
  printf("--- System startup ---\n");

  xTaskCreate(vTask1, "vTask1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle);
  xTaskCreate(vTask2, "vTask2", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
  vTaskStartScheduler();

  while(1){}
}

/*************************** End of file ****************************/
