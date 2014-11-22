/** @pop.h Contains functions used in the reddit algorithm */

#ifndef _POPULARITY_H_
#define _POPULARITY_H_

#define TIMECONSTANT 45000
#define BASETIME 1416174518 /*Added 22:48 16/11/2014*/

/** Alt time calculator based on the actual model used på reddit as found here: amix.dk/blog/post/19588*/
unsigned int calcAge(unsigned int postTime);

/** Checks if the song has been played*/
int isPlayed(unsigned int plays);

/** Prevents the program from taking log10(0)*/
unsigned int playCount(unsigned int plays);

/** Calculates the score af the song.
No matter how old the song is, it wont get any weight before it is viewed more than once, or once and has been around for a longer time*/
double weigth(unsigned int startTime, int plays);

#endif
