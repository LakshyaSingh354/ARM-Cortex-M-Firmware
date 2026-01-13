#include "event.h"
#include "peripherals/gptm/timer.h"
#include "peripherals/gpio/gpio.h"

int main(void) {
    event_t evt;
    uint32_t tick_count = 0;

    gpio_init();
    timer_init();

    while (1) {
        if (event_get(&evt)) {
            if (evt.type == EVENT_TICK) {
                tick_count++;

                if (tick_count % 5 == 0) {
                    gpio_toggle_led();
                }
            }
        }
    }
}