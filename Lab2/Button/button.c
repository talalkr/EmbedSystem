#include <avr/io.h> 
#include <avr/interrupt.h>


int main(void)
{
	
	DDRD |= (0 << PD2); //set PD2 as input
	DDRB |= (1 << PB0); //set PB0 as output	
	
	EICRA |= (1 << ISC01);    // set INT0 to trigger on falling edge
	EIMSK |= (1 << INT0);     // Turn on INT0

    sei();         //enable global interrupts

    while (1)
    {
        //main loop
    }
}

ISR(INT0_vect)
{
	cli(); //disable global interrupts
	PORTB ^= (1 << 0); //toggle LED
	sei();//reenable global interrupts
}
