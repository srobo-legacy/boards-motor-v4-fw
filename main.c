#include <stdio.h>
#include <stdlib.h>

#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>

#include "led.h"
#include "output.h"
#include "usart.h"
#include "analogue.h"

void init(void) {
	rcc_clock_setup_in_hse_8mhz_out_24mhz();

	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN);
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN);
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPCEN);
	led_init();
	output_init();
	usart_init();
	analogue_init();
}

int main(void) {
	int i;

	init();
	led_set(LED_M0_R);
	led_set(LED_M0_B);
	led_set(LED_M1_B);
	led_set(LED_M1_R);
	//while(1);

	output_enable(0);
	output_enable(1);
	output_direction(0, DIR_FWD);
	output_direction(1, DIR_FWD);
	output_speed(0, 0);
	output_speed(1, 0);
	//while(1);

	int d, j = 0;

	while (1) {
		led_set(LED_M0_R);
		led_set(LED_M0_B);
		led_set(LED_M1_B);
		led_set(LED_M1_R);
		output_direction(0, DIR_FWD);
		output_direction(1, DIR_REV);
		output_speed(1, j++);
		if (j==101) j = 0;
		d = 2500000;
		for (i = 0; i < d; i++)
			__asm__("nop");

		led_clear(LED_M0_R);
		led_clear(LED_M0_B);
		led_clear(LED_M1_B);
		led_clear(LED_M1_R);
		output_direction(0, DIR_REV);
		output_direction(1, DIR_FWD);
//		output_speed(1, 1);
		for (i = 0; i < d; i++)
			__asm__("nop");
		printf("boo\n");
	}

	return 0;
}
