#include "event.h"
#include "peripherals/gptm/timer.h"
#include "peripherals/gpio/gpio.h"
#include "peripherals/uart/uart.h"
#include "peripherals/uart/uart_rx_buf.h"
#include "peripherals/uart/uart_tx_buf.h"

int main(void) {
    event_t evt;
    uint32_t tick_count = 0;

    // driver initialization
    timer_init();
    gpio_init();
    uart_rx_buffer_init();
    uart_tx_buffer_init();
    uart_init();

    while (1) {
        if (event_get(&evt)) {
            switch (evt.type) {
                case EVENT_TICK:
                    // heartbeat logic
                    tick_count++;
                    if (tick_count % 5 == 0){
                        gpio_toggle_led(); // gpio demonstration
                    }
                    break;

                    case EVENT_UART_RX: {
                        uint8_t byte;
                        while (uart_rx_buffer_get(&byte)) {
                            uart_send_byte(byte); // echo
                        }
                        break;
                    }
            }
        }
    }
}