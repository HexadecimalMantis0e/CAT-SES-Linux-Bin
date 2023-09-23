CC=gcc
CFLAGS=-std=gnu17 -pedantic -Wall
EXE=make_playlist
IODBC_LDFLAGS=-liodbc

all: $(EXE)

$(EXE): make_playlist.c util.c shared.c
	$(CC) $(CFLAGS) -o $@ $^ $(IODBC_LDFLAGS)

clean:
	rm -f $(EXE)
