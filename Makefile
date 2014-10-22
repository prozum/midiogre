all: midiparo

midiparo: main.o midi.o
    gcc main.o midi.o -o midiparo

main.o: main.c
    gcc -c main.c

midi.o: midi.c
    gcc -c midi.c

clean:
    rm -rf *o midiparo
