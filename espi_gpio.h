#ifndef ESPI_GPIO_H
#define ESPI_GPIO_H

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <mman.h>

#define assert_syscall(sc) do { if ((sc) < 0) { printf ("syscall failed("#sc")"\n); \
                                    exit(EXIT_FAILURE);} } while(0)

#define _inline __attribute__((always_inline))

#define BCM2708_PERI_BASE 0x3F000000
#define GPIO_BASE (BCM2708_PERI_BASE + 0x200000)

#define GPIO_INPUT(m, pin) *(m + ((pin)/10)) &= ~(7<<(((pin)%10)*3))
#define GPIO_OUTPUT(m, pin) *(m + ((pin)/10)) |= (1<<(((pin)%10)*3))
#define GPIO_SET(m, pin) *(m + 7) = 1<<pin
#define GPIO_CLR(m, pin) *(m + 10) = 1<<pin
#define GPIO_READ(m, pin) (*(m + 13) & (1<<pin))

enum modes {
    GPIO_HIGH = 0,
    GPIO_LOW,
    GPIO_IN,
    GPIO_OUT,
};

typedef uint32_t GPIO_Mem;

volatile GPIO_Mem *map_gpio();
void gpio_set_output(const size_t gpio_pin);

#ifdef ESPI_GPIO_IMPL

static char gpio_pins[] = {

};

/* static function declarations */
static void gpio_set(const GPIO_Mem *mem, const size_t gpio_pin, const size_t mode);

/* static function declarations end */

static void gpio_set(const GPIO_Mem *mem, const size_t gpio_pin, const size_t mode)
{

    switch(mode) {
    case GPIO_HIGH:
        GPIO_SET(mem, gpio_pin);
    break;
    case GPIO_LOW:
        GPIO_CLR(mem, gpio_pin);
    break;
    case GPIO_IN:
        GPIO_INPUT(mem, gpio_pin);
    break;
    case GPIO_OUT:
        GPIO_OUTPUT(mem, gpio_pin);
    break;
    }
}

volatile GPIO_Mem *map_gpio()
{
    ssize_t gpio_fd;
    volatile GPIO_Mem *mem;

    assert_syscall(gpio_fd = open("/dev/gpio_mem", O_RDWR | O_SYNC));
    assert_syscall(mem = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, gpio_fd, GPIO_BASE));

    return mem;
}

#endif /* ESPI_GPIO_IMPL */
#endif /* ESPI_GPIO_H */
