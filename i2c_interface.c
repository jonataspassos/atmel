#include <avr/io.h>
#include "uart.c"
#include "i2c.c"

int main(void)
{
	int op = 0;
    USART_CONFIG();
	I2C_CONFIG();
	uint8_t addr=0;
	uint8_t reg=0;
	uint8_t val=0;
	int qtd=0;
	
    while (1) 
    {
		USART_TRANSFER_STRING("|-------------- I2C Interface --------------|\r\n");
		USART_TRANSFER_STRING("Options: ");
		USART_TRANSFER_STRING("\r\n\t1 - Set Address Device:    ");
		USART_TRANSFER_INT_HEX(addr);
		USART_TRANSFER_STRING("\r\n\t2 - Set Address Register:  ");
		USART_TRANSFER_INT_HEX(reg);
		USART_TRANSFER_STRING("\r\n\t3 - Set to:\r\n");
		USART_TRANSFER_STRING("\t4 - Show how many:\r\n");
		USART_TRANSFER_STRING("Option: ");
		op = USART_RECIVE_INT();
		USART_TRANSFER_INT(op);
		USART_TRANSFER_STRING("\r\n");
		switch(op){
		case 1:
			USART_TRANSFER_STRING("Old Address Device:\t");
			USART_TRANSFER_INT_HEX(addr);	
			USART_TRANSFER_STRING("\r\nNew Address Device:\t");
			addr = (uint8_t)USART_RECIVE_INT();
			USART_TRANSFER_INT_HEX(addr);
			USART_TRANSFER_STRING("\r\n");
			break;
		case 2:
			USART_TRANSFER_STRING("Old Address Register:\t");
			USART_TRANSFER_INT_HEX(reg);
			USART_TRANSFER_STRING("\r\nNew Address Register:\t");
			reg= (uint8_t)USART_RECIVE_INT();
			USART_TRANSFER_INT_HEX(reg);
			USART_TRANSFER_STRING("\r\n");
			break;
		case 3:
			USART_TRANSFER_STRING("Set in ");
			USART_TRANSFER_INT_HEX(addr);
			USART_TRANSFER_STRING(" device, in ");
			USART_TRANSFER_INT_HEX(reg);
			USART_TRANSFER_STRING(" register\r\n\tto value:\t");
			val= (uint8_t)USART_RECIVE_INT();
			USART_TRANSFER_INT_HEX(val);
			USART_TRANSFER_STRING("\r\n");
			USART_TRANSFER_STRING("Confirm? [y/n]");
			op = USART_RECIVE_CHAR();
			USART_TRANSFER(op);
			USART_TRANSFER_STRING("\r\n");
			if(op == 'y' || op=='Y'){
				I2C_PRINT_STATUS(I2C_SETREGISTERSLAVE(addr,reg,val));
			}else{
				USART_TRANSFER_STRING("Operation Canceled by User!\r\n");	
			}
			break;
		case 4:
			USART_TRANSFER_STRING("Get in ");
			USART_TRANSFER_INT_HEX(addr);
			USART_TRANSFER_STRING(" device, in ");
			USART_TRANSFER_INT_HEX(reg);
			USART_TRANSFER_STRING(" register, how many values:");
			qtd= (uint8_t)USART_RECIVE_INT();
			USART_TRANSFER_INT(qtd);
			USART_TRANSFER_STRING("\r\n");
			if(qtd){
				I2C_PRINTREGISTERSLAVE(addr,reg,qtd);
			}else{
				USART_TRANSFER_STRING("Operation Canceled by User!\r\n");
			}
			break;	
		case 0:
			break;
		default:
			USART_TRANSFER_STRING("Invalid Operation!\r\n");
		}
		
    }
}
