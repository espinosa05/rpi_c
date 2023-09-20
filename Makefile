CC=gcc
CFLAGS=-Wall
SRC=main.c 
all: knight_rider
knight_rider: $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

