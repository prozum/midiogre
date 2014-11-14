#ifndef _POPULARITY_H_
#define _POPULARITY_H_

void testTime();
unsigned int getUnixTimeStamp();
unsigned int startTime();
unsigned int compareTime(unsigned int startTime, unsigned int currentTime);
int isPlayed(int plays);
int positivePlayCount(int playCount);
double weigth(unsigned int time, int playCount);

#endif
