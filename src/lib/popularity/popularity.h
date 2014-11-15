#ifndef _POPULARITY_H_
#define _POPULARITY_H_

unsigned int getUnixTimeStamp();
unsigned int compareTime(unsigned int startTime);
int isPlayed(unsigned int plays);
unsigned int playCount(unsigned int plays);
double weigth(unsigned int startTime, int plays);

#endif
