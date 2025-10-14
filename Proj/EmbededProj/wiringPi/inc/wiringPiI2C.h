#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int wiringPiI2CRead           (int fd) ;
extern int wiringPiI2CReadReg8       (int fd, int reg) ;
extern int wiringPiI2CReadReg16      (int fd, int reg) ;
extern int wiringPiI2CReadBlockData  (int fd, int reg, uint8_t *values, uint8_t size);  //Interface 3.3
extern int wiringPiI2CRawRead        (int fd, uint8_t *values, uint8_t size);           //Interface 3.3

extern int wiringPiI2CWrite          (int fd, int data) ;
extern int wiringPiI2CWriteReg8      (int fd, int reg, int data) ;
extern int wiringPiI2CWriteReg16     (int fd, int reg, int data) ;
extern int wiringPiI2CWriteBlockData (int fd, int reg, const uint8_t *values, uint8_t size);  //Interface 3.3
extern int wiringPiI2CRawWrite       (int fd, const uint8_t *values, uint8_t size);           //Interface 3.3

extern int wiringPiI2CSetupInterface (const char *device, int devId) ;
extern int wiringPiI2CSetup          (const int devId) ;

#ifdef __cplusplus
}
#endif
