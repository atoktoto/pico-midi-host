#include <sys/cdefs.h>
#include <pico/platform.h>
#include <sys/cdefs.h>
#include <pico/platform.h>
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"

#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "bsp/board.h"
#include "tusb.h"
#include "midi_app.h"

#define USBD_STACK_SIZE    (3*configMINIMAL_STACK_SIZE/2) * (CFG_TUSB_DEBUG ? 2 : 1)

_Noreturn void led_blinking_task(void *pvParams);
_Noreturn void midi_host_task(void *pvParams);

int main(void) {

    stdio_init_all();
    board_init();
    printf("TinyUSB Host MIDI Example\n");

    xTaskCreate(led_blinking_task, "blink", 256, NULL, tskIDLE_PRIORITY, NULL);

    TaskHandle_t tuh_task_handle;
    xTaskCreate(midi_host_task, "tuh", USBD_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, &(tuh_task_handle));
    // UBaseType_t uxCoreAffinityMask;
    // uxCoreAffinityMask = ( ( 1 << 0 ));`
    // vTaskCoreAffinitySet( tuh_task_handle, uxCoreAffinityMask );

    vTaskStartScheduler();
    return 0;
}

_Noreturn void midi_host_task(void *pvParams) {
    tusb_init();
    while (true) {
        tuh_task();
        midi_host_app_task();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

_Noreturn void led_blinking_task(void *pvParams) {
    static bool led_state = false;
    while (true) {
        printf("Blink\n");
        board_led_write(led_state);
        led_state = 1 - led_state;
        vTaskDelay(200);
    }
}

void vApplicationMallocFailedHook(void) {
    TU_LOG1("Malloc failed!\r\n");
    configASSERT((volatile void *) NULL); // Force an assert.
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    TU_LOG1("Stack overflow!\r\n");
    configASSERT((volatile void *) NULL); // Force an assert.
}