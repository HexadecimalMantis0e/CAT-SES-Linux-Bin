CC=gcc
CFLAGS=-std=gnu17 -pedantic -Wall
EXE=cmos_swap

all: $(EXE)

$(EXE): cmos_swap.c print.c util.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(EXE)
