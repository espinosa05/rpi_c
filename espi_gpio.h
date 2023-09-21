#ifndef ESPI_GPIO_H
#define ESPI_GPIO_H

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/mman.h>

#define assert_syscall(sc) do { if ((sc) < 0) { printf ("syscall failed("#sc")\n"); exit(EXIT_FAILURE);} } while(0)

#define array_size(a) (sizeof(a) / sizeof(*a))
#define assert_msg(cnd, msg) do { if ((!cnd)) { puts(msg); exit(EXIT_FAILURE);}}while (0)
#define _inline __attribute__((always_inline))

#define BCM2708_PERI_BASE 0x3F000000
#define GPIO_BASE (BCM2708_PERI_BASE + 0x200000)

#define GPIO_INPUT(m, pin) *(m + ((pin)/10)) &= ~(7<<(((pin)%10)*3))
#define GPIO_OUTPUT(m, pin) *(m + ((pin)/10)) |= (1<<(((pin)%10)*3))
#define GPIO_SET(m, pin) *(m + 7) = 1<<pin
#define GPIO_CLR(m, pin) *(m + 10) = 1<<pin
#define GPIO_READ(m, pin) (*(m + 13) & (1<<pin))

#ifdef DEBUG
#define debug_printf(...) printf("[DEBUG]\t"); printf(__VA_ARGS__)
#else
#define debug_printf(...)
#endif /* DEBUG */


#define RPI_3_PINS  {               \
    2, 	3,  4,  17, 27, 22, 10,		\
    9, 	11, 0,  5,  6,  13, 19,		\
    26, 14, 15, 18, 23, 24, 25,		\
    8,  7,  1,  12, 16, 20, 21,		\
}

enum modes {
    GPIO_HIGH = 0,
    GPIO_LOW,
    GPIO_IN,
    GPIO_OUT,
};

typedef uint32_t GPIO_Mem;

volatile GPIO_Mem *map_gpio();
void gpio_set_output(const size_t gpio_pin);
int is_valid_pin(size_t pin);
size_t get_hardware_pin(size_t logical);

#ifdef ESPI_GPIO_IMPL

#if 1
#define MEM_PATH "/dev/gpiomem"
#else
#define MEM_PATH "/dev/mem"
#endif
static const size_t gpio_pins[] = RPI_3_PINS;

volatile GPIO_Mem *map_gpio()
{
    ssize_t gpio_fd;
    volatile GPIO_Mem *mem;

    assert_syscall(gpio_fd = open(MEM_PATH, O_RDWR | O_SYNC));
    assert_syscall(mem = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, gpio_fd, GPIO_BASE));

    return mem;
}

int is_valid_pin(size_t pin)
{
    return (pin <= array_size(gpio_pins));
}

size_t get_hardware_pin(size_t logical)
{
#ifdef DEBUG
    printf("GPIO pin %d mapped to %d\n",logical, gpio_pins[logical]);
#endif
    return gpio_pins[logical];
}

#endif /* ESPI_GPIO_IMPL */
#endif /* ESPI_GPIO_H */
