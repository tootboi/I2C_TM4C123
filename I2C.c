// I2C.c
// Code for TM4C123GH6PM

#include "tm4c123gh6pm.h"
#include "I2C.h"

// initialise I2C as master
void I2C_init(void){
	SYSCTL_RCGCGPIO_R |=  0x00000008;	// start clock on port D
	SYSCTL_RCGCI2C_R |= 0x00000008;		// start clock on I2C3
	GPIO_PORTD_DEN_R |= 0x03;			// enable PD0,1 digital I/O
	
	GPIO_PORTD_AFSEL_R |= 0x00000003;	// enalbe PD0,1 alt functions
	GPIO_PORTD_PCTL_R |= 0x00000033;	// configure PD0,1 as I2C3
	GPIO_PORTD_ODR_R |= 0x00000002;		// enalbe open drain on SDA (PD1)
	I2C3_MCR_R = 0x00000010;			// enalbe I2C master function
	I2C3_MTPR_R = 0x01;					// 312 Kbps I2C3 clock at default 12.5 Mhz system clock. (TM4C123GH6PM Datasheet Table 16-2)
}

// set slave address and configure transmit/receive
void setSlaveAddr(unsigned char slave, unsigned char mode) {
	while(I2C3_MCS_R&0x00000001){};	// wait for I2C ready
	I2C3_MSA_R = (slave << 1);
	if(mode==0) {
		I2C3_MSA_R &= ~0x1;	// clear bit0; transmit data
	} else {
		I2C3_MSA_R |= 0x1;	//set bit1; receive data
	}
}

// send a byte of data to slave
void writeByte(unsigned char data, unsigned char conditions) {
	I2C3_MDR_R = data&0xFF;		// trim and set data.
	I2C3_MCS_R = conditions;	// set conditions.
	while(I2C3_MCS_R&0x00000001){};// wait for transmission done
	if((I2C3_MCS_R&(0x00000008|0x00000004|0x00000002)) != 0) {	// check for error.
		I2C3_MCS_R = 0x00000004; // stop
	}
}
