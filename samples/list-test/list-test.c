#include <list/list.h>
#include <mid/mid.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BYTES_PER_LINE 16

int main( int argc, char *argv[] )
{
    FILE *mid_file;
    list_t *data,*sliced_data;
    int byte,i;

    /* Open file */
    if((mid_file = fopen(argv[1], "rb")) == NULL) {
        perror(argv[1]);
        return -1;
    }

    /* Read mid */
    data = list_dump_file(mid_file);
    fclose(mid_file);

    if (list_get_fixed(data, 4) == HEADER_SIGNATURE ) {
        puts("Header signature is correct!");
    } else {
        puts("Header signature is wrong!");
    }

    /* Reset data to start */
    list_set(data,0,LIST_FORW,LIST_BEG);

    /* Test list slicing by cutting of 2 bytes at start & end */
    sliced_data = list_slicing(data, 2, data->n - 2);

    /* Print mid data */
    while ((byte = list_get(sliced_data)) != EOL) {

	    printf("%02x ", byte);

        if (++i % BYTES_PER_LINE == 0) {
            putchar('\n');
        }
    }

    /* Deallocate data */
    list_free(data);
    list_free(sliced_data);


    printf("\n");

    return 0;
}
