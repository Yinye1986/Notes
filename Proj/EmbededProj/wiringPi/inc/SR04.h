#ifndef __SR04_H__
#define __SR04_H__

#include <wiringPi.h>

void SR04_Init(int TPIN, int EPIN);
float SR04_getDistance(int TPIN,int EPIN);
void SR04_triggerPulse(int TPIN);
float SR04_getTraveltime(int EPIN);

#endif
