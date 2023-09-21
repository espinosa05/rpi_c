#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>

#define ESPI_GPIO_IMPL
#include "espi_gpio.h"

#ifndef SPEED
#define SPEED 2
#endif /* SPEED */
_inline void blink(volatile GPIO_Mem *mem, size_t led, double dt);
void parse_args(int c, char **v);

int main(int argc, char **argv)
{
    /* Number of LEDs the PI should use */
    if (argc < 2) {
        printf("usage:\t%s [n pins] \n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t n_pins;

    n_pins = strtoul(argv[1], NULL, 10);
    assert_msg(is_valid_pin(n_pins), "invalid pin selected\n");
    double dt = (500 / n_pins / SPEED); /* 1 Hz / cycles (4) */
    /* pointer into the GPIO memory*/
    volatile GPIO_Mem *gpio;

    /* map the GPIO-MMIO pages into process memory */
    gpio = map_gpio();

    /* set the selected gpio pins to output */
    for (size_t i = 0; i < n_pins; ++i) {
        GPIO_OUTPUT(gpio, i);
    }

    while (1) {
	    /* run the lights to the right */
        for (size_t i = 0; i < n_pins; ++i) {
            blink(gpio, i, dt);
        }

        /* run them back to the left(excluding the last pin as it was already turned on ) */
        for (size_t i = n_pins - 2; i > 0; --i) {
            blink(gpio, i, dt);
        }
    }

    return EXIT_SUCCESS;
}

/* pulls a pin high for delta time (1Hz / sizeof pins) */
_inline void blink(volatile GPIO_Mem *mem, size_t led, double dt)
{
    debug_printf("pin number %02X\n", led);
    GPIO_SET(mem, led);
    usleep(dt * 1000);
    GPIO_CLR(mem,led);
}
