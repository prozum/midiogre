#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void check_str(char *str1[], char *str2[]);

int l_dist(char str1[], char str2[])
{
    int len1, len2;
    int dist, i;

    if (strlen(str2) > strlen(str1)) {
        char *tmp;

        tmp = str1;
        str1 = str2;
        str2 = tmp;

    dist = 0;
    len1 = strlen(str1);
    len2 = strlen(str2);

    for (i = 0; i <= len2; i++) {
        if (str1[i] != str2[i] && str2[i] != '\0') {
            dist++;
        }
    }

    return dist + (len1 - len2);
}

int wf_dist(char str1[], char str2[])
{
    int len1, len2;
    int dist, i, j;
    
    
}

void check_str(char *str1[], char *str2[])
{
    char **tmp;

    *tmp = *str1;
    *str1 = *str2;
    *str2 = *tmp;
}

int main(int argc, char *argv[])
{
    int dist = l_dist(argv[1], argv[2]);

    printf("%d\n", dist);

    return 0;
}
