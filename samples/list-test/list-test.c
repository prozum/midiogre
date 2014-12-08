#include <list/list.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BYTES_PER_LINE 16

int main( int argc, char *argv[] )
{
    FILE *mid_file;
    list_t *data;
    int byte,i;

    /* Open file */
    if((mid_file = fopen(argv[1], "rb")) == NULL) {
        perror(argv[1]);
        return -1;
    }

    /* Read mid */
    data = list_dump_file(mid_file);
    fclose(mid_file);

    /* Print mid data */
    while ((byte = list_get(data)) != EOL) {

	    printf("%02x ", byte);

        if (++i % BYTES_PER_LINE == 0) {
            putchar('\n');
        }
    }

    /* Deallocate data */
    list_free(data);


    printf("\n");

    return 0;
}
