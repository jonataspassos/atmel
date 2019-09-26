#ifndef SERIAL_PRINTER_H
#include <avr/io.h>
#define SERIAL_PRINTER_H
#include <avr/io.h>
#include <stdio.h>
//#include <stdlib.h>

void USART_CONFIG();

void USART_TRANSFER(uint8_t ender);
void USART_TRANSFER_STRING(char * text);
void USART_TRANSFER_INT(long int val);
void USART_TRANSFER_INT_HEX(long int val);
void USART_TRANSFER_INT_BIN(long int val);

uint8_t USART_RECIVE();
uint8_t USART_ISRECIVED();
void USART_RECIVE_STRING(char * text);
int USART_RECIVE_INT();

void USART_CONFIG(){
	UCSR0A = 0x40;
	UCSR0B = 0x18;//Transmiter and Reciver
	UCSR0C = 0x06;
	UBRR0L = 103;//9600 baud rate
	UBRR0H = 0;
}

void USART_TRANSFER(uint8_t ender){
	while((UCSR0A & 0x20)!=0x20){}
	UDR0 = ender;
}

uint8_t USART_RECIVE(){
	while((UCSR0A & 0x80)!=0x80){}
	return (uint8_t)UDR0;
}

uint8_t USART_ISRECIVED(){
	return UCSR0A & 0x80;
}

void USART_TRANSFER_STRING(char * text){
	int i=0;
	while(text[i]){
		USART_TRANSFER(text[i]);
		i++;
	}
}

void USART_RECIVE_STRING(char * text){
	int i=0;
	uint8_t temp = USART_RECIVE();
	while(temp != '\r'){
		text[i] = temp;
		i++;
		temp = USART_RECIVE();
	}
	temp = USART_RECIVE();
	text[i] = '\0';
}

void USART_TRANSFER_INT(long int val){
	char text[20];
	int i=18;
	do{
		text[i] = ((char)val%10)+'0';
		val/=10;
		i--;
	}while(val && (i>=2));
	text[19] = 0;
	USART_TRANSFER_STRING(text+i+1);
}

int USART_RECIVE_INT(){
	char text[50];
	int number = 0;
	USART_RECIVE_STRING(text);
	int i=0;
	int base = 10;
	int temp;
	int signal = 1;
	while(text[i]==' ' || text[i]=='\t' || text[i]=='\n'|| text[i]=='\r'){
		i++;
	}
	if(text[i]=='0'){
		if(text[i+1] == 'x' || text[i+1] == 'X'){//Hexa
			base = 16;
			i+=2;
		}else if(text[i+1] == 'b' || text[i+1] == 'B'){//Binário
			base = 2;
			i+=2;
		}else{//Octal
			base = 8;
			i+=1;
		}
	}
	if(text[i]=='-'){
		signal = -1;
	}
	while(text[i]){
		if(text[i]>='0' && text[i]<='9'){// '0' = 48; '9' = 57
			temp = text[i]-'0';
		}else if(text[i]>='a' && text[i]<='f'){
			temp = text[i]-'a'+10;
		}else if(text[i]>='A' && text[i]<='F'){
			temp = text[i]-'A'+10;
		}else{
			break;
		}
		if(temp >=base){
			USART_TRANSFER_STRING(" Número fora da base\r\n");
			return 0;
		}else{
			number = number*base + temp;
		}
		i++;
	}
	number = signal*number;
	return number;
}

char USART_RECIVE_CHAR(){
	char text[50];
	USART_RECIVE_STRING(text);
	return text[0];
}

void USART_TRANSFER_INT_HEX(long int val){
	char text[20];
	int i=18;
	char temp;
	do{
		temp = (char)val%16;
		if(temp<10)
			text[i] = temp+'0';
		else
			text[i] = temp+'A'-10;
		val/=16;
		i--;
	}while((val || (18-i)%2) && (i>=2));
	text[19] = 0;
	text[i--] = 'x';
	text[i] = '0';
	USART_TRANSFER_STRING(text+i);
}

void USART_TRANSFER_INT_BIN(long int val){
	char text[40];
	int i=38;
	char temp;
	do{
		temp = (char)val%2;
		text[i] = temp+'0';
		val/=2;
		i--;
	}while((val || (38-i)%8) && (i>=2));
	text[39] = 0;
	text[i--] = 'b';
	text[i] = '0';
	USART_TRANSFER_STRING(text+i);
}
/*int mul(int x, int y){
	int z=0;
	for(;y>0;y--){
		z+=x;
	}
	for(;y<0;y++){
		z-=x;
	}
	return z;
}

int pot(int x, int y){
	int z;
	for(z=1;y>0;y--){
		z= mul(z,x);
		USART_TRANSFER_INT(z);
		USART_TRANSFER_STRING("\n");
	}
	return z;
}

void USART_TRANSFER_FLOAT(float val){
	char text[20];
	/*int temp = (int)((val - (int)val)*pot(10,precision));
	USART_TRANSFER_INT((int)((val - (int)val)));
	USART_TRANSFER_STRING("\r\n");
	USART_TRANSFER_INT(temp);
	USART_TRANSFER_STRING("\r\n");
	USART_TRANSFER_INT(pot(10,precision));
	USART_TRANSFER_STRING("\r\n");
	text[18-precision]='.';
	int i = 18;
	do{
		text[i] = ((char)temp%10)+'0';
		temp/=10;
		i--;
	}while(temp  && (i>=2));
	temp = (int)val;
	i = 18-precision-1;
	do{
		text[i] = ((char)temp%10)+'0';
		temp/=10;
		i--;
	}while(temp  && (i>=2));
	text[19] = 0;
	sprintf(text,"%09.3f",val);
	USART_TRANSFER_STRING(text);//+i+1
}*/
#endif