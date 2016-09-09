#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void send_byte(uint8_t byte){
	 while((UCSR0A &(1<<UDRE0)) == 0);
	 UDR0 = byte;
}


int main (void)
{

	UBRR0H = ( BAUD_PRESCALE >> 8) ;
	UBRR0L = ( BAUD_PRESCALE ) ; 
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	UCSR0C |= (1<<USBS0) | (3<<UCSZ00); 
	
	while(1)
	{
		send_byte('H');
		
		_delay_ms(1000);
	}
}



