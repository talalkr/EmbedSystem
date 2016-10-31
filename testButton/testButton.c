#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
  DDRC |= (1<<PC0); //Nakes first pin of PORTC as Output
  // OR DDRC = 0x01;
  DDRD &= ~(1<<PD0);//Makes firs pin of PORTD as Input
  // OR DDRD = 0x00; //Makes all pins of PORTD input
  while(1) //infinite loop
  {
    if(PIND & (1<<PD0) == 1) //If switch is pressed
    {
      PORTC |= (1<<PC0); //Turns ON LED
      _delay_ms(3000); //3 second delay
      PORTC &= ~(1<<PC0); //Turns OFF LED
    }
  }
}