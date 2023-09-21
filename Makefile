CC=gcc
CFLAGS=-Wall
SRC=main.c

all: knight_rider
run: knight_rider
	./$^ 6

knight_rider: $(SRC)
	$(CC) -DDEBUG $(CFLAGS) $^ -o $@

