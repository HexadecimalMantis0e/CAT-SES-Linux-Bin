CC=gcc
CFLAGS=-std=gnu17 -pedantic -Wall
EXE=client_update
IODBC_LDFLAGS=-liodbc

all: $(EXE)

$(EXE): client_update.c util.c
	$(CC) $(CFLAGS) -o $@ $^ $(IODBC_LDFLAGS)

clean:
	rm -f $(EXE)
