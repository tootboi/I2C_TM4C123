// I2C.h
// Code for TM4C123GH6PM

void I2C_init(void);
// set slave address and read/write mode
void setSlaveAddr(unsigned char slave, unsigned char mode);
// write a byte of data, followed by conditions.
void writeByte(unsigned char data, unsigned char conditions);
