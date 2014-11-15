#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "popularity.h"

/*The functions below are used for the reddit algorith*/

#define TIMECONSTANT 45000

/*Get Unix Time, and returns it in seconds*/
unsigned int getUnixTimeStamp(){
    return (unsigned int)time(NULL);
}

/*Calculates diffrence between the starttime(time of creation) of the song and current time*/
unsigned int compareTime(unsigned int startTime){
    return (unsigned int)time(NULL)-startTime;
}

/*Checks if the song has been played*/
int isPlayed(unsigned int plays){
    if(plays>0)
        return 1;
    else
        return 0;
}

/*Prevents the program from taking log10(0)*/
unsigned int playCount(unsigned int plays){
    if(plays == 0)
        return 1;
    else
        return plays;
}

/*Calculates the score af the song*/
/*No matter how old the song is, it wont get any weigth before it is viewed more than once, or once and has been around for a longer time*/
double weigth(unsigned int startTime, int plays){
    return log10((double)playCount(plays))+(isPlayed(plays)*compareTime(startTime))/TIMECONSTANT;
}
