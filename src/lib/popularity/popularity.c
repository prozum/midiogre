#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "popularity.h"

/*The functions below are used for the reddit algorith*/

#define TIMECONSTANT 45000
#define BASETIME 1416174518 /*Added 22:48 16/11/2014*/

/*Alt time calculator based on the actual model used på reddit as found here: amix.dk/blog/post/19588*/
unsigned int calcAge(unsigned int postTime){
    return postTime - BASETIME;
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
double weigth(unsigned int age, int plays){
    return log10((double)playCount(plays))+(isPlayed(plays)*calcAge(age))/TIMECONSTANT;
}
