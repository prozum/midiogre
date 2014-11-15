#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "popularity.h"

/*The functions below are used for the reddit algorith*/

#define TIMECONSTANT 45000

/*Print current time in Unix Time in secounds*/
void testTime(){
    printf("%d sekunder\ns",getUnixTimeStamp());
}

/*Get Unix Time, and returns it in seconds*/
unsigned int getUnixTimeStamp(){
    return (unsigned)time(NULL);
}

/*Calculates diffrence between the starttime(time of creation) of the song and current time*/
unsigned int compareTime(unsigned int startTime){
    return (unsigned)time(NULL)-startTime;
}

/*Checks if the song has been played*/
int isPlayed(int plays){
    if(plays>0)
        return 1;
    else
        return 0;
}

/*Ensures the playcount always is possitive or equal 0*/
int absPlayCount(int playCount){
    if(abs(playCount) >= 1)
        return abs(playCount);
    else
        return 1;
}

/*Calculates the score af the song*/
/*No matter how old the song is, it wont get any score before it is viewed at least once*/
double weigth(unsigned int startTime, int playCount){
    return log10((double)absPlayCount(playCount))+(isPlayed(playCount)*compareTime(startTime))/TIMECONSTANT;
}
