#include <stdio.h>
#include <stdint.h>
#include "midi.h"


int main(void)
{
    //header_t test;
    FILE *mid_file;
    uint8_t *tmp;

    // Open file
    mid_file = fopen("test.mid", "rb");

    // Where to read?
    int offset = 0;
    // How much?
    int size = 4;

    // Read the first 4 bytes from mid_file.
    tmp = ffread(mid_file,offset,size);

    // Print bytes as hex
    printp(tmp,size,"%x");

    // Deallocate bytes
    free(tmp);
    // Close mid_file
    fclose(mid_file);
    return 0;
}
