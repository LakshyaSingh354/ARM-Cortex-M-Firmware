#include "event.h"
#include "peripherals/gptm/timer.h"
#include "peripherals/gpio/gpio.h"
#include "peripherals/uart/uart.h"

int main(void) {
    event_t evt;

    timer_init();
    gpio_init();
    uart_init();

    while (1) {
        if (event_get(&evt)) {
            switch (evt.type) {
                case EVENT_TICK:
                    // heartbeat logic
                    break;

                case EVENT_UART_RX:
                    // Put breakpoint here
                    volatile uint8_t c = evt.data;
                    break;
            }
        }
    }
}