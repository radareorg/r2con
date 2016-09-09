#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int main (void)
{

	DDRB |= _BV(DDB7);
  	DDRD |= _BV(DDD7);
  	DDRD |= _BV(DDD6);

	while(1)
	{
		PORTD ^= _BV(PB6);
		PORTD ^= _BV(PB7);
		PORTB ^= _BV(PB7);
		
		_delay_ms(1000);
	}
}



