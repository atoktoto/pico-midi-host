#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#if CFG_TUH_MIDI

void led_blinking_task(void);
extern void midi_host_app_task(void);

int main(void) {

  board_init();
  printf("TinyUSB Host MIDI Example\r\n");
  tusb_init();

  while (1) {
    tuh_task();
    led_blinking_task();
    midi_host_app_task();
  }

  return 0;
}

#endif

void led_blinking_task(void) {
  const uint32_t interval_ms = 1000;
  static uint32_t start_ms = 0;

  static bool led_state = false;

  if ( board_millis() - start_ms < interval_ms) return;
  start_ms += interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state;
}

void vApplicationMallocFailedHook( void )
{
    /* Called if a call to pvPortMalloc() fails because there is insufficient
    free memory available in the FreeRTOS heap.  pvPortMalloc() is called
    internally by FreeRTOS API functions that create tasks, queues, software
    timers, and semaphores.  The size of the FreeRTOS heap is set by the
    configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */

    /* Force an assert. */
    configASSERT( ( volatile void * ) NULL );
}