CC=gcc
CFLAGS=-std=gnu17 -pedantic -Wall
EXE=ses_log

all: $(EXE)

$(EXE): ses_log.c log_main.c util.c shared.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(EXE)
