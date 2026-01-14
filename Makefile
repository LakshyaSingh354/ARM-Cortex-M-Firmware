CC=arm-none-eabi-gcc
CFLAGS=-mcpu=cortex-m3 -mthumb -ffreestanding -nostdlib -O0 -g

all: firmware.elf

firmware.elf: startup.s main.c event.c peripherals/gptm/timer.c peripherals/gpio/gpio.c peripherals/uart/uart.c linker.ld
	$(CC) $(CFLAGS) startup.s main.c event.c peripherals/gptm/timer.c peripherals/gpio/gpio.c peripherals/uart/uart.c -T linker.ld -o firmware.elf

clean:
	rm -f firmware.elf