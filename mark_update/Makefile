CC=gcc
CFLAGS=-std=gnu17 -pedantic -Wall
EXE=mark_update
IODBC_LDFLAGS=-liodbc

all: $(EXE)

$(EXE): mark_update.c util.c
	$(CC) $(CFLAGS) -o $@ $^ $(IODBC_LDFLAGS)

clean:
	rm -f $(EXE)
