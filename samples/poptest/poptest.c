#include <popularity/popularity.h>

#include <stdio.h>
#include <stdlib.h>

int main(void){

    //Testing value generated 15/11-14 kl. 19:30 - 1416076249

    unsigned int startTime = 1416076249; //This will be the value given to a song upon adding it to Midiogre(c)
    int viewCount = 0; //views

    double score = weigth(startTime, viewCount);
    printf("%f\n",score);

    return 0;
}
