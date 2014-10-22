CC=gcc
 CFLAGS=-c -Wall
SOURCES=main.c midi.c
DEP=midi.h
OBJ=$(SOURCES:.c=.o)
EXE=midiparo

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
