#define CPU_16MHz 0x00
#define F_CPU 10000000

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usb_serial.h"
#include <string.h>
//#include <uart.h>

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

#include <string.h>
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)


//Declaration of our functions
void USART_init(void);
unsigned char USART_receive(void);
void USART_send(unsigned char data);
void USART_putstring(char* StringPtr);
void USART_Flush(void);
int setTimer(char timerChar);
void configurePWD(void);
void setFWD(char freq_ch);

void USART_init(){

	//
	//Declare variables for Serial Communication
	//
	UBRR0H  = (uint8_t)(BAUD_PRESCALE >> 8);
	UBRR0L  = (uint8_t)BAUD_PRESCALE;

    UCSR0C = (1<<UCSZ00) | (1<<UCSZ01); /* 8-bit data */
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);   /* Enable RX and TX */


}

void delay_ms(unsigned int time_ms)
{
    // _delay_ms() comes from <util/delay.h> and can only
    //  delay for a max of around 13 ms when the system
    //  clock is 20 MHz, so we define our own longer delay
    //  routine based on _delay_ms()
 
    unsigned int i;
 
    for (i = 0; i < time_ms; i++)
        _delay_ms(1);       
}

int main(void){

	USART_init();

	CPU_PRESCALE(CPU_16MHz);
	
	//char buffer [32];

	DDRD |= (1<<PD6) | (1<<PD7); //Pin 7 is used to test Timer+Frequency, Pin6 is used to solely test Timer.

	char freq_ch, timer_ch;
	int timer_val;
	int counter;
	//
	//Declare variables for Timer
	//
	TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1<< WGM00);
	TCCR0B |= (1 << CS00);
	
	PORTD |= (1 << PD6); 


		 	freq_ch = USART_receive(); //Stops loop and wait for  input for frequency
		 	USART_Flush();
			 timer_ch = USART_receive(); //Stops loop and wait for input for Timer

		 	setFWD(freq_ch); //Set new frequency value
		 	timer_val = setTimer(timer_ch); //Set time value

	while (1)
	{

			PORTD |= (1 << PD7); //Turn Pin 7 on
			delay_ms(timer_val); //Wait for timer_val miliseconds
			PORTD &= ~(1 << PD7); //Turn Pin 7 off
			delay_ms(timer_val); //Wait for timer_val miliseconds

	}
}

/*
	Function waits for user's side to be read

*/
void USART_send(unsigned char data){
 
 while(!(UCSR0A & (1<<UDRE0))); // Wait for empty transmit buffer
 UDR0 = data; //Puts data into buffer, sends the data
 
}

/*
	Function waits for a data input
	Returns data as char
*/
unsigned char USART_receive(void){
 
 while(!(UCSR0A & (1<<RXC0))); //Wait for data to be received
 return UDR0; //Get and return received data from buffer
 
}

/*
	Flush the receiver buffer. This empties the buffer for new data.
*/
void USART_Flush(void){
	unsigned char dummy;
	while (UCSR0A & (1<< RXC0))  dummy = UDR0;
}

void USART_putstring(char* StringPtr){
 
while(*StringPtr != 0x00){    //Here we check if there is still more chars to send, this is done checking the actual char and see if it is different from the null char
 USART_send(*StringPtr);    //Using the simple send function we send one char at a time
 StringPtr++;}        //We increment the pointer so we can read the next char
 
}

/*

	Function that utiilizes a switch case for Timer.
	Possible arguments consists of: [q,w,e,r]
	Each returning a unique time in miliseconds.

*/
int setTimer(char ch)
{
	int time= 0;
		switch(ch)
		{
			case 'q':
				time = 100;
				break;
			case 'w':
				time = 500;
				break;
			case 'e':
				time = 1000;
				break;
			case 'r':
				time = 2000;
				break;
		}
	
	return time;

}

/*

	Function that utiilizes a switch case for Frequency.
	Possible arguments consts of: [q,w,e,r,null]
	Each sets the OCR0A value to a unique number 

*/
void setFWD(char freq_ch)
{
			switch(freq_ch)
		{
			case 'q':
				OCR0A = 255;
				break;
			case 'w':
				OCR0A = 155;
				break;
			case 'e':
				OCR0A = 100;
				break;
			case 'r':
				OCR0A = 55;
				break;
			default:
				OCR0A = 200;
				break;
		}
}

