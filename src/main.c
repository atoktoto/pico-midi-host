#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "bsp/board.h"
#include "tusb.h"

#define USBD_STACK_SIZE    (3*configMINIMAL_STACK_SIZE/2) * (CFG_TUSB_DEBUG ? 2 : 1)

void led_blinking_task(void *pvParams);
void tuh_loop_task(void *pvParams);
void midi_host_task(void *pvParams);

extern void midi_host_app_task(void);

int main(void) {

  board_init();
  printf("TinyUSB Host MIDI Example\r\n");
  tusb_init();

  xTaskCreate(led_blinking_task, "blink", 256, NULL, tskIDLE_PRIORITY, NULL);
  xTaskCreate(tuh_loop_task, "tuh", 256, NULL, configMAX_PRIORITIES-1, NULL);
  xTaskCreate(midi_host_task, "midi-host", 256, NULL, tskIDLE_PRIORITY, NULL);

  vTaskStartScheduler();
  return 0;
}

void tuh_loop_task(void *pvParams) {
  while(true) {
    tuh_task();
  }
}

void midi_host_task(void *pvParams) {
  while(true) {
    test_rx();
  }
}

void led_blinking_task(void *pvParams) {
  static bool led_state = false;
  while(true) {
    board_led_write(led_state);
    led_state = 1 - led_state;
    vTaskDelay(200);
  }
}


void vApplicationMallocFailedHook( void ) {
  printf("Malloc failed!\r\n");
  configASSERT( ( volatile void * ) NULL ); // Force an assert.
}


void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName ) {
  printf("Stack overflow!\r\n");
  configASSERT( ( volatile void * ) NULL ); // Force an assert. 
}