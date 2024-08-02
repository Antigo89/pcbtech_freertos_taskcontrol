/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

Задание 2
Реализовать систему с периодическим удалением задачи. Основные условия:
1. В проекте должна использоваться схема управления памятью heap_4.
2. Система создаётся с единственной задачей 1 (приоритет 1).
3. Задача 1 выполняет вывод через UART строки сообщения «Task 1 – runing», после чего создаёт
задачу 2 (приоритет 2) и блокируется на период 500 мс.
4. Задача 2 выполняет вывод через UART двух строк «Task 2 – runing» и «Task 2 – deleting», после
чего уничтожает сама себя.
5. Выполнить сборку, запуск и тестирование проекта на отладочной плате.
Критерии готовности (Definition of Done)
1. Проект собирается и загружается в отладочную плату без ошибок и предупреждений.
2. По последовательному порту передаются сообщения от двух задач.
3. В реализации задач используются функции создания и удаления задач FreeRTOS.


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
//xTaskHandle xTask2Handle;

/****************************func************************************/
void vTask2(void * pvParameters){
  while(1){
    printf("%s\n","Task2 - runing");
    printf("%s\n","Task2 - deleting");
    vTaskDelete(NULL); //vTaskDelete(&xTask2Handle)
  }
  vTaskDelete(NULL);
}

void vTask1(void * pvParameters){
  portTickType lastTickCount = xTaskGetTickCount();
  while(1){
    printf("%s\n","Task1 - runing");
    xTaskCreate(vTask2, "vTask2", configMINIMAL_STACK_SIZE, NULL, 2, NULL); //NULL - &xTask2Handle
    vTaskDelayUntil(&lastTickCount, 500);
  }
  vTaskDelete(NULL);
}
/****************************main************************************/

int main(void) {
  usart1_init();
  printf("--- System startup ---\n");

  xTaskCreate(vTask1, "vTask1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

  vTaskStartScheduler();

  while(1){}
}

/*************************** End of file ****************************/
