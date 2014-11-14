#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "popularity.h"

/*The functions below are used for the reddit algorith*/

#define TIMECONSTANT 45000

/*Print current time in Unix Time in secounds*/
void testTime(){
    printf("%d sekunder\n",getUnixTimeStamp());
}

/*Get Unix Time, and returns it in seconds*/
unsigned int getUnixTimeStamp(){
    return (unsigned)time(NULL);
}

/*Calculates diffrence between the starttime(time of creation) of the song and current time*/
unsigned int compareTime(unsigned int startTime, unsigned int currentTime){
    return currentTime-startTime;
}

/*Checks if the song has been played*/
int isPlayed(int plays){
    if(plays>0)
        return 1;
    else if(plays == 0)
        return 0;
    else
        return -1;
}

/*Ensures the playcount always is possitive or equal 0*/
int positivePlayCount(int playCount){
    if(playCount >= 1)
        return playCount;
    else
        return 1;
}

/*Calculates the score af the song*/
double weigth(unsigned int time, int playCount){
    return log10((double)positivePlayCount(playCount))+(isPlayed(playCount)*time)/TIMECONSTANT;
}
