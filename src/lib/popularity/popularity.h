#ifndef _POPULARITY_H_
#define _POPULARITY_H_

void testTime();
unsigned int getUnixTimeStamp();
unsigned int startTime();
unsigned int compareTime(unsigned int startTime);
int isPlayed(int plays);
int absPlayCount(int playCount);
double weigth(unsigned int startTime, int playCount);

#endif
