#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void setup() {
  Serial.begin(9600);
  delay(1);
  pinMode(13, OUTPUT);
  delay(1);
  digitalWrite(13, HIGH);
  Serial.println("Example 0.0. Hello AVR exploits.");
  Serial.println("Let's go!");
}

volatile int switchon(){
  Serial.println("LED on");
  digitalWrite(13, HIGH);
  return 0;
}

volatile int switchoff(){
  Serial.println("LED off");
  digitalWrite(13, LOW);
  return 0;
}

int choose(char *reply, int len){
  char buf[5];
  int i = 0;

  for(i = 0; i < len; i++){
    buf[i] = reply[i];
  }

  buf[i] = '\0';

  if(strcmp(buf, "on") == 0)
    switchon();
  else if(strcmp(buf, "off") == 0)
    switchoff();
  else
    Serial.println("Incorrect choice, use only on or off.");

  return i;
}

void loop() {
  int i = 0;
  int len = 0;
  char reply[48];

  Serial.println("Use on/off to turn on/off light. You have only 3 seconds! ");
  Serial.setTimeout(3000);

  for(i = 0; i < 48; i++)
    reply[i] = '\0';

  len = Serial.readBytesUntil('\n', reply, 48);

  if(len > 1)
    i = choose(reply, len);

  delay(3000);
}

