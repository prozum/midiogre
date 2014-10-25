#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "distance.h"

int l_dist(char str1[], char str2[])
{
    int len1, len2;
    int dist, i;

    if (strlen(str2) > strlen(str1)) {
        char *tmp;

        tmp = str1;
        str1 = str2;
        str2 = tmp;
    }

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

/* WIP */
/*
int wf_dist(char str1[], char str2[])
{
    int len1, len2;
    int dist, i, j;

    len1 = strlen(str1);
    len2 = strlen(str2);

    int d_arr[len1][len2];

    for (i = 0; i < len1; i++) {
        d_arr[i][0] = i;
        printf("%d\n", d_arr[i][0]);
    }

    for (i = 0; i < len2; i++) {
        d_arr[0][i] = i;
        printf("%d\n", d_arr[0][i]);
    }


    for (j = 1; j <= len2; j++) {
        for (i = 1; i <= len1; i++){
            if (str1[i - 1] == str2[j - 1]) {
                d_arr[i][j] = d_arr[i - 1][j - 1];
            } else {
                d_arr[i][j] = (d_arr[i-1][j] + 1) > (d_arr[i][j-1] + 1) ? (d_arr[i-1][j] + 1) : (d_arr[i][j-1] + 1);
                d_arr[i][j] = d_arr[i][j] > (d_arr[i-1][j-1] + 1) ? d_arr[i][j] : (d_arr[i-1][j-1] + 1);
            }
        }
    }

    return d_arr[len1 + 1][len2 + 1];
}
*/

void check_str(char *str1[], char *str2[])
{
    char **tmp;

    *tmp = *str1;
    *str1 = *str2;
    *str2 = *tmp;
}
