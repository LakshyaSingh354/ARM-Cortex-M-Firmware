#include "gpio.h"

#define GPIOF_BASE        0x40025000

#define GPIO_DIR          (*(volatile uint32_t *)(GPIOF_BASE + 0x400))
#define GPIO_DEN          (*(volatile uint32_t *)(GPIOF_BASE + 0x51C))
#define GPIO_DATA         (*(volatile uint32_t *)(GPIOF_BASE + 0x3FC))

#define SYSCTL_RCGCGPIO   (*(volatile uint32_t *)0x400FE108) // bit 5 for GPIOF

void gpio_init(void) {
    // Enable clock for GPIO Port F
    SYSCTL_RCGCGPIO |= (1 << 5);

    // Set PF0 as output
    GPIO_DIR |= (1 << 0);

    // Enable digital function
    GPIO_DEN |= (1 << 0);
}

void gpio_toggle_led(void) {
    GPIO_DATA ^= (1 << 0);
}