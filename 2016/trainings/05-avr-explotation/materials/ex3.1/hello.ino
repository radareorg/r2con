//#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//#define USART_BAUDRATE 9600
//#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

int i;
char password[] = "Micro";

void switchred();

void switchgreen();

int checkPassword(){
    char reply[48];
    char buf[16];
    int i,j = 0;

    Serial1.println("Enter password: ");
    for(i = 0; i < 48; i++)
        reply[i] = '\0';
    if(Serial1.readBytesUntil('\n', reply, 48) > 1) {
        while(reply[j] != '\0') {
          buf[j] = reply[j];
          j++;
        }
        buf[j] = '\0';
        if(strcmp(buf, password) == 0) {
            Serial1.println("Correct password!\n\nWait a few seconds...");
            return 1;
        }
    }
    Serial1.println("Wrong password!\n\nWait a few seconds...");
    return 0;
}

void choose(char *reply){
  char buf[7];
  int i = 0;

  while(reply[i] != '\0'){
    buf[i] = reply[i];
    i++;
  }
  buf[i] = '\0';

  if(strcmp(buf, "red") == 0) {
    if (checkPassword())
        switchred();
  } else if(strcmp(buf, "green") == 0)
    switchgreen();
}

void switchred(){
  Serial1.println("Red blink blink");
  PORTD ^= _BV(PB7);
}

void switchgreen(){
  Serial1.println("Green blink blink");
  PORTB ^= _BV(PB7);
}

void setup() {
  // initialize digital pin 13 as an output.
  DDRB |= _BV(DDB7);
  DDRD |= _BV(DDB7);
  DDRD |= _BV(DDB6);
  PORTD ^= _BV(PB7);
  PORTD ^= _BV(PB6);
  pinMode(13, OUTPUT);
  Serial1.begin(9600);
}

void loop() {
  char reply[48];

  digitalWrite(13, HIGH);
  Serial1.println("Choose your color to light on: red (* Need password), green. You have only 15 seconds!");
  Serial1.setTimeout(15000);

  for(i = 0; i < 48; i++)
    reply[i] = '\0';
  if(Serial1.readBytesUntil('\n', reply, 48) > 1)
    choose(reply);
  delay(1000);
}
