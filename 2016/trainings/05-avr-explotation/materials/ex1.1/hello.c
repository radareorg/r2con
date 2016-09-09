#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void send_byte(uint8_t byte){
	 while((UCSR1A &(1<<UDRE1)) == 0);
	 UDR1 = byte;
}


int main (void)
{

	UBRR1H = ( BAUD_PRESCALE >> 8) ;
	UBRR1L = ( BAUD_PRESCALE ) ; 
	UCSR1B |= (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);
	UCSR1C |= (1<<USBS1) | (3<<UCSZ10); 
	
	while(1)
	{
		send_byte('H');
		
		_delay_ms(1000);
	}
}



