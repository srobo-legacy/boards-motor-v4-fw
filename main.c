#include <stdio.h>
#include <stdlib.h>

#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>

#include "led.h"
#include "output.h"
#include "usart.h"
#include "analogue.h"
#include "fw_ver.h"

void init(void) {
	rcc_clock_setup_in_hse_8mhz_out_24mhz();

	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN);
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN);
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPCEN);
	led_init();
	output_init();
	usart_init();
	//analogue_init();
}

int main(void) {
	int i;

	init();

	while (1) {
		int c = usart_get_char();
		switch (c) {
			case 'V':
				printf("MCV4B:%i\n", firmware_version);
				break;
		};
	}

	return 0;
}
