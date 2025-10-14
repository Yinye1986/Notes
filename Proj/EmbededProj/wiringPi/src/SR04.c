#include "SR04.h"

void SR04_Init(int TPIN, int EPIN) {
    pinMode(TPIN, OUTPUT);
    pinMode(EPIN, INPUT);
}

float SR04_getDistance(int TPIN,int EPIN) {
    SR04_triggerPulse(TPIN);
    float travelTime = SR04_getTraveltime(EPIN);
    float distance = (travelTime * 34.3) / 2.0; // 距离转换为厘米
    return distance;
}
void SR04_triggerPulse(int TPIN) {
    digitalWrite(TPIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TPIN, LOW);
}

float SR04_getTraveltime(int EPIN) {
    while (digitalRead(EPIN) == LOW);
    float startTime = micros();
    while (digitalRead(EPIN) == HIGH);
    float travelTime = micros() - startTime;
    return travelTime;
}
