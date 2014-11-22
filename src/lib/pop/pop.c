#include "pop.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

unsigned int calcAge(unsigned int postTime){
    return postTime - BASETIME;
}

int isPlayed(unsigned int plays){
    if(plays>0)
        return 1;
    else
        return 0;
}

unsigned int playCount(unsigned int plays){
    if(plays == 0)
        return 1;
    else
        return plays;
}

double weight(unsigned int age, int plays){
    return log10((double)playCount(plays))+(isPlayed(plays)*calcAge(age))/TIMECONSTANT;
}
