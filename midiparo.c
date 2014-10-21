#include <stdio.h>
#include <stdint.h>
#include "midi.h"


int main(void)
{
    //header_t test;
    FILE *mid_file;
    header_t *header;

    // Open file
    mid_file = fopen("test.mid", "r");
    if( mid_file == NULL ) {
        perror("Error: ");
        return -1;
    }


    // Read midi header
    header = read_header(mid_file);
    if ( header == NULL) {
        return -1;
    }

    // Print info
    printf("Format:\t\t%i\n",header->format);
    printf("Tracks:\t\t%i\n",header->tracks);
    printf("Division:\t%i\n",header->division);

    // Deallocate bytes
    free(header);
    // Close mid_file
    fclose(mid_file);

    return 0;
}
