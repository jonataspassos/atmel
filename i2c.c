#ifndef I2C_H
#include <avr/io.h>
#define I2C_H
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

/*#include "usart.h"*/

#define I2C_READ 1
#define I2C_WRITE 0
//Master Transmiter
#define I2C_STATUS_STARTING 0x08
#define I2C_STATUS_STARTED 0x10
#define I2C_STATUS_ADDRESS_W_ACK 0x18
#define I2C_STATUS_ADDRESS_W_NACK 0x20
#define I2C_STATUS_DATA_SEND_ACK 0x28
#define I2C_STATUS_DATA_SEND_NACK 0x30
#define I2C_STATUS_ARBITRATION_LOST 0x38
//Master Reciver
/*#define I2C_STATUS_STARTING 0x08
#define I2C_STATUS_STARTED 0x10
#define I2C_STATUS_ARBITRATION_LOST 0x38*/
#define I2C_STATUS_ADDRESS_R_ACK 0x40
#define I2C_STATUS_ADDRESS_R_NACK 0x48
#define I2C_STATUS_DATA_RECIVE_ACK 0x50
#define I2C_STATUS_DATA_RECIVE_NACK 0x58
//Slave Transmiter
#define I2C_STATUS_ADDRESS_SLAVE_R_ACK 0xA8
#define I2C_STATUS_LOST_MASTER_R 0xB0
#define I2C_STATUS_DATA_SEND_SLAVE_ACK 0xB8
#define I2C_STATUS_DATA_SEND_SLAVE_NACK 0xC0
#define I2C_STATUS_DATA_SEND_LAST_SLAVE_ACK 0xC8
//Slave Reciver
#define I2C_STATUS_ADDRESS_SLAVE_W_ACK 0x60
#define I2C_STATUS_ADDRESS_SLAVE_W_NACK 0x68
#define I2C_STATUS_GENERAL_CALL 0x70
#define I2C_STATUS_LOST_MASTER_W 0x78
#define I2C_STATUS_DATA_SLAVE_W_ACK 0x80
#define I2C_STATUS_DATA_SLAVE_W_NACK 0x88
#define I2C_STATUS_DATA_GENERAL_W_ACK 0x90
#define I2C_STATUS_DATA_GENERAL_W_NACK 0x98

#define I2C_STATUS_OK 0xF8

uint8_t I2C_GETSTATUS(void){//Ler status TWI
	uint8_t status;
	status = TWSR & 0xF8;
	return status;
}

uint8_t I2C_CONFIG(void){
	//Clock TWI (SCL) em 400kHz
	TWSR = 0x00;
	TWBR = 0x0C;
	TWCR = (1<<TWEN); //Habilita TWI
	return I2C_GETSTATUS();
}

uint8_t I2C_START(void){//Envia bit start
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while((TWCR & (1<<TWINT)) == 0);
	return I2C_GETSTATUS();
}

uint8_t I2C_STOP(void){//Envia bit stop
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	return I2C_GETSTATUS();
}
uint8_t I2C_SEND(uint8_t u8data){//Envia dados
	TWDR = u8data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while((TWCR & (1<<TWINT))==0);
	return I2C_GETSTATUS();
}

uint8_t I2C_RECIVEACK(void){//Ler dados com ACK
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while((TWCR & (1<<TWINT))==0);
	return TWDR;
}

uint8_t I2C_RECIVENACK(void){//Ler dados com NACK
	TWCR = (1<<TWINT)|(1<<TWEN);
	while((TWCR & (1<<TWINT))==0);
	return TWDR;
}
void I2C_PRINT_STATUS(uint8_t status){
	USART_TRANSFER_STRING("\nI2C Status Code:\t");
	USART_TRANSFER_INT_HEX(status);
	USART_TRANSFER_STRING("\n");
}
uint8_t I2C_SETREGISTERSLAVE(uint8_t addr,uint8_t reg, uint8_t val){
	uint8_t status;
	I2C_START();
	status = I2C_SEND( addr | I2C_WRITE );
	if(status != I2C_STATUS_ADDRESS_W_ACK){
		return status;
	}
	status = I2C_SEND( reg );
	if(status != I2C_STATUS_DATA_SEND_ACK){
		return status;
	}
	status = I2C_SEND( val ); //Sleep OFF
	if(status != I2C_STATUS_DATA_SEND_ACK){
		return status;
	}
	return I2C_STOP();
}
void I2C_GETREGISTERSLAVE(uint8_t * container, uint8_t addr,uint8_t reg, int qtd){
	int i;
	I2C_START();
	I2C_SEND( addr | I2C_WRITE);
	I2C_SEND( reg );
	_delay_ms(20);
	I2C_START();
	
	I2C_SEND( addr | I2C_READ );
	qtd--;
	for(i=0;i<qtd;i++){
		container[i] = I2C_RECIVEACK();
	}
	container[i] = I2C_RECIVENACK();
}
void I2C_PRINTREGISTERSLAVE(uint8_t addr,uint8_t reg, int qtd){
	uint8_t container[30];
	int i;
	I2C_GETREGISTERSLAVE(container,addr,reg,qtd);
	USART_TRANSFER_STRING("In ");
	USART_TRANSFER_INT_HEX(addr);
	USART_TRANSFER_STRING("device: \r\n");
	for (i=0;i<qtd;i++){
			
		USART_TRANSFER_INT_HEX(reg + i);
		USART_TRANSFER_STRING(": ");
		USART_TRANSFER_INT_HEX(container[i]);
		USART_TRANSFER_STRING("\t");
		USART_TRANSFER_INT_BIN(container[i]);
		USART_TRANSFER_STRING("\r\n");
	}
}
void I2C_PRINT_IF_NOT_OK(uint8_t status){
	if(status != I2C_STATUS_OK){
		I2C_PRINT_STATUS(status);
	}
}
#endif