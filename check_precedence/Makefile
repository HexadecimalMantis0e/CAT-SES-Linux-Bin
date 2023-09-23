CC=gcc
CFLAGS=-std=gnu17 -pedantic -Wall
EXE=check_precedence
IODBC_LDFLAGS=-liodbc

all: $(EXE)

$(EXE): check_precedence.c util.c
	$(CC) $(CFLAGS) -o $@ $^ $(IODBC_LDFLAGS)

clean:
	rm -f $(EXE)
