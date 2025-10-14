#include "SERIAL.h"

int serialPort = -1;

int initSerialPort(int baudrate) {
    if (serialPort >= 0) {
        printf("Serial port is already open.\n");
        return -1;
    }

    serialPort = serialOpen(SERIAL_PORT, baudrate);
    if (serialPort < 0) {
        printf("Unable to open serial device %s\n", SERIAL_PORT);
        return -1;
    }
    
    return 0;
}
