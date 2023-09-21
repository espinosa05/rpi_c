# RPI-C

## Espi GPIO API overview
## peripheral access
Each GPIO macro and function takes the special GPIO\_Mem type that is defined as
```c
typedef uint32_t GPIO_Mem;
```
As each Memory-Mapped GPIO register has a size of 4 bytes starting at address GPIO\_BASE which is defined as
```c
BCM2708_PERI_BASE + 0x200000 // (0x3F000000 + 0x200000)
```
This special address is mapped into process memory via the mmap system call.<br>
## Input and Output macros
The GPIO interface is accessed via special macros that do bit manipulation to set the different modes of operation. <br>
```c
GPIO_INPUT(m, pin)
```
GPIO\_INPUT takes the specific pin number and sets it to input mode
```c
GPIO_OUTPUT(m, pin)
```
GPIO\_OUTPUT takes the specific pin number and sets it to output mode
```c
GPIO_SET(m, pin)
```
GPIO\_SET takes the specific pin number and sets the voltage to high
```c
GPIO_CLR(m, pin)
```
GPIO\_CLR takes the specific pin number and sets the voltage to low
```c
GPIO_READ(m, pin)
```
GPIO\_READ takes the specific pin number and measures the incoming voltage
```c
debug_printf(fmt, ...)
```
debug\_printf(fmt, ...) prints debugging information
## low-level library functions
```c
volatile GPIO_Mem *map_gpio(void);
```
map\_gpio loads the gpio address space into process memory and returns a pointer at the base I/O-base.
it opens the file located at "/dev/gpiomem" which is a special file that has the periphal address space mapped into it.

```c
int is_valid_pin(size_t pin);
```
is\_valid\_pin is used to check for a valid pin. it returns false if the selected pin is higher than the total number of pins
