#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <mid/mid.h>
#include <mid/mid-str.h>
#include <ext/ext.h>

int main(int argc, char* argv[])
{
    FILE *mid_file;
    mid_t *mid;

    int bitmap;

    /* Open file */
    mid_file = fopen(argv[1], "rb");
    if( mid_file == NULL ) {
        perror(argv[1]);
        return -1;
    }

    /* Read mid */
    mid = read_mid(mid_file);

    /* Close mid_file */
    fclose(mid_file);

    /* Extract instrument classes */
    bitmap = extract_instr_classes(mid);

    printf("\n\n\nThe sum  of bitmap is: %i\n",bitmap);

    printf("\n");
    return 0;
}
