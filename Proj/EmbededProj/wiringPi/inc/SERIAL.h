#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdio.h>
#include <stdint.h>

#define SERIAL_PORT "/dev/ttyS0"

extern int serialPort;

int initSerialPort(int baudrate);
void closeSerialPort(void);
void sendSerialData(const char *data);
char receiveSerialData(void);
void flushSerial(void);

#endif
