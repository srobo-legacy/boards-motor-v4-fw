#include <stdio.h>
#include <stdlib.h>

#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/cm3/scb.h>

#include "led.h"
#include "output.h"
#include "usart.h"
#include "analogue.h"
#include "fw_ver.h"

uint32_t *top_of_ram = ((uint32_t *)0x20001FF0);
#define BOOTLOADER_MAGIC 0xFACEBEE5

void init(void) {
	rcc_clock_setup_in_hse_8mhz_out_24mhz();

	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN);
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN);
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPCEN);
	led_init();
	output_init();
	usart_init();
	//analogue_init();

	led_set(LED_M0_R);
}

void print_version(void) {
	printf("MCV4B:%i\n", firmware_version);
}

void enter_bootloader(void) {
	printf("Entering bootloader\n");
	*top_of_ram = BOOTLOADER_MAGIC;
	scb_reset_system();
}

void set_output_mode(int channel, int8_t mode) {
	switch (mode) {
		case 'f':
			output_enable(channel);
			output_direction(channel, DIR_FWD);
			break;
		case 'r':
			output_enable(channel);
			output_direction(channel, DIR_REV);
			break;
		case 'h':
			output_enable(channel);
			output_direction(channel, DIR_HALT);
			break;
		case 'd':
			output_disable(channel);
			break;
		default:
			break;
	}
}

typedef enum {
	STATE_INIT,
	STATE_SPEED0,
	STATE_SPEED1,
	STATE_MODE0,
	STATE_MODE1
} state_t;

void fsm(int c) {
	static state_t state = STATE_INIT;
	if (c == 0) {
		state = STATE_INIT;
		return;
	}
	switch (state) {
		case STATE_INIT:
			switch (c) {
				case 1:
					state = STATE_INIT;
					print_version();
					break;
				case 4:
					enter_bootloader();
					break;
				case 's':
					state = STATE_SPEED0;
					break;
				case 'S':
					state = STATE_SPEED1;
					break;
				case 'm':
					state = STATE_MODE0;
					break;
				case 'M':
					state = STATE_MODE1;
					break;
				default:
					state = STATE_INIT;
					break;
			}
			break;
		case STATE_SPEED0:
			state = STATE_INIT;
			output_speed(0, c);
			break;
		case STATE_SPEED1:
			state = STATE_INIT;
			output_speed(1, c);
			break;
		case STATE_MODE0:
			state = STATE_INIT;
			set_output_mode(0, c);
			break;
		case STATE_MODE1:
			state = STATE_INIT;
			set_output_mode(1, c);
			break;
		default:
			state = STATE_INIT;
			break;
	}
}


int main(void) {
	/* Check to see if we should jump into the bootloader */
	if (*top_of_ram == BOOTLOADER_MAGIC) {
		*top_of_ram = 0;
		asm("ldr r0, =0x1FFFF000\n\t" \
		    "ldr sp,[r0, #0]\n\t" \
		    "ldr r0,[r0, #4]\n\t" \
		    "bx r0");
	}

	init();

	while (1) {
		int c = usart_get_char();
		fsm(c);
	}

	return 0;
}
