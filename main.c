#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ESPI_GPIO_IMPL
#include "espi_gpio.h"

_inline void blink(size_t led, double dt);

int main(int argc, char **argv)
{
    /* Number of LEDs the PI should use */
    if (argc < 2) {
        printf("usage %s: [n_LEDs]\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t n_pins = strtoul(argv[1]);
    size_t dt = (1000 / n_pins / 2); /* 1 Hz / cycles (4)*/

    /* pointer into the GPIO memory*/
    GPIO_Mem *gpio;

    /* map the GPIO-MMIO pages into process memory */
    gpio = map_gpio();

    for (size_t i = 0; i < n_pins; ++i) {
        gpio_setoutput(i);
    }

    while (1) {
        for (size_t i = 0; i < n_pins; ++i) {
            blink(i, dt);
        }

        for (size_t i = n_pins; i > 0; --i) {
            blink(i, dt);
        }
    }

    return EXIT_SUCCESS;
}

/* pulls a pin high for delta time (1Hz / sizeof pins) */
_inline void blink(size_t led, double dt)
{
    gpio_sethigh(led);
    sleep(dt);
    gpio_setlow(led);
}
