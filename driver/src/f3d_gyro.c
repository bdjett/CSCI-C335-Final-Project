/****************************
 * f3d_gyro.c
 *
 * Driver functions for the gyroscope
 *
 * Created by: Brian Jett (bdjett) & Adam Combs (addcombs)
 * Created on: 28 February 2014
 * Last modified by: Brian Jett (bdjett)
 * Last modfiied on: 4 March 2014
 * Assignment: Lab 7
 */

#include <stm32f30x.h>
#include <f3d_led.h>
#include <f3d_uart.h>
#include <f3d_gyro.h>

#define GYRO_Sensitivity_500dps (float) 57.1429f

// initializes gyro interface
void f3d_gyro_interface_init() {
	
    /* Initialize the Peripheral Clocks
	   +--------------+------+
	   | Peripheral   | Bus  |
	   +--------------+------+
	   | GPIOA        | AHB  |
	   | GPIOE        | AHB  |
	   | SPI1         | APB2 |
	   +--------------+------+
	*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	/* Pin Configuration
	   +-------+--------------+
	   | Pin   | SPI Function |
	   +-------+--------------+
	   | PA5   | SCK          |
	   | PA6   | MISO         |
	   | PA7   | MOSI         |
	   +-------+--------------+
	*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Alternate Functions
	   Connect each pin to SPI1
	*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_5);
		
	/* GPIO Pin (used to select the gyro)
	   +-----+----------------+
	   | Pin | SPI Function   |
	   +-----+----------------+
	   | PE3 | CS             |
	   +-----+----------------+
	*/
	GPIO_InitTypeDef GPIO_InitStructure2;
	GPIO_StructInit(&GPIO_InitStructure2);
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure2.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure2);

	// set the pin high
	GPIO_SetBits(GPIOE, GPIO_Pin_3);
	
	// SPI peripheral config
	SPI_InitTypeDef SPI_InitStructure;
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
	SPI_Cmd(SPI1, ENABLE);
} 

// f3d_gyro_init initializes the gyroscope and sets the control registers
void f3d_gyro_init(void) {
	// initialize interface
	f3d_gyro_interface_init();

	uint8_t ctrl1, ctrl4;

	// CTRL1 Register
	// bit 7:6 data rate: datarate 0
	// bit 5:4 bandwidth: bandwith 3
	// bit 3: power mode: active
	// bit 2:0 axes enable: X,Y,Z enabled
	ctrl1 |= (uint8_t) (((uint8_t)0x00) |
						((uint8_t)0x30) |
						((uint8_t)0x08) |
						((uint8_t)0x07));

	// CTRL4 Register
	// bit 7 block update: continous
	// bit 6 endianess: LSB first
	// bit 5:4 full scale: 500 dps
	ctrl4 |= (uint8_t) (((uint8_t)0x00) |
						((uint8_t)0x00) |
						((uint8_t)0x10));

	// write control registers
	f3d_gyro_write(&ctrl1, 0x20, 1);
	f3d_gyro_write(&ctrl4, 0x23, 1);
}

// f3d_gyro_write takes a buffer, a write address and the number of bytes to write
void f3d_gyro_write(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite) {
	if (NumByteToWrite > 0x01) {
		// multiple bytes to write, set bit 6 of WriteAddr
		WriteAddr |= (uint8_t)(0x40);
	} 

	GYRO_CS_LOW();

	f3d_gyro_sendbyte(WriteAddr);
	
	while(NumByteToWrite > 0x00) {
		f3d_gyro_sendbyte(*pBuffer);
		NumByteToWrite--;
		pBuffer++;
	}

	GYRO_CS_HIGH();
}

// f3d_gyro_sendbyte takes the byte to send
static uint8_t f3d_gyro_sendbyte(uint8_t byte) {
	// poll the transmitter status prior to writing the byte
	// to send to the transmitter register
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_SendData8(SPI1, byte);

	// poll to confirm that received has fully received the character
	// prior to reading it
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return (uint8_t)SPI_ReceiveData8(SPI1);
}

// f3d_gyro_getdata takes a buffer to store the X,Y,Z data
void f3d_gyro_getdata(float *pfData) {
	uint8_t tmpbuffer[50];
	int16_t RawData[50];

	// read data stored on gyro
	f3d_gyro_read(tmpbuffer, 0x28, 6);
	
	int i; // loop invariant

	// data needs to be combined into 16 bit words
	for (i=0; i<3; i++) {
		RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i+1] << 8) + tmpbuffer[2*i]);
	}

	// raw data needs to be divided by senstivity value to get the final
	// value for angular rate
	for (i=0; i<3; i++) {
		pfData[i]=(float)RawData[i]/GYRO_Sensitivity_500dps;
	}
}

// f3d_gyro_read takes a buffer, an address to read and the number of bytes
// to read
void f3d_gyro_read(uint8_t *pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead) {
	if (NumByteToRead > 0x01) {
		// sending mroe than one byte set multibyte commands
		ReadAddr |= (uint8_t)(0x80 | 0x40);
	} else {
		// just set the read mode
		ReadAddr |= (uint8_t)(0x80);
	}

	GYRO_CS_LOW();

	// send byte(s)
	f3d_gyro_sendbyte(ReadAddr);

	while (NumByteToRead > 0x00) {
		*pBuffer = f3d_gyro_sendbyte(((uint8_t)0x00));
		NumByteToRead--;
		pBuffer++;
	}

	GYRO_CS_HIGH();
}
