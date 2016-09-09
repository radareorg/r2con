#define APNAME "esp_123"

#define F_CPU 16000000UL

#define MAX_SIZE 255

char webpage[] = "HTTP/1.1 200 Ok\r\n\r\n<html><h1>LED web server!</h1><form method=POST action=/><input type=radio name=color value=red>red<br><input type=radio name=color value=green>green<br><input type=submit value=Switch></form></html>\r\n";

volatile void switchorange(){
  Serial1.println("Switching to orange led...");
  PORTD ^= _BV(PB6);
}

volatile void switchred(){
  Serial1.println("Switching to red led...");
  PORTD ^= _BV(PB7);
}

volatile void switchgreen(){
  Serial1.println("Switching to green led...");
  PORTB ^= _BV(PB7);
}

void setup() {
  DDRB |= _BV(DDB7);
  DDRD |= _BV(DDB7);
  DDRD |= _BV(DDB6);

  Serial1.begin(19200);
  Serial.begin(19200);
  Serial1.println("Starting LED control webserver!");
  delay(100);

  Serial.setTimeout(100);

  Serial.println("AT");
  Serial1.println( Serial.readString() );

  Serial.println("AT+CWMODE=2");
  Serial1.println( Serial.readString() );

  Serial.print("AT+CWSAP=\"");
  Serial.print(APNAME);
  Serial.println("\",\"1234567890\",5,3");
  Serial1.println( Serial.readString() );

  Serial.println("AT+CIPMUX=1");
  Serial1.println( Serial.readString() );

  Serial.println("AT+CIPSERVER=1,80");
  Serial1.println( Serial.readString() );

}

void sendData(const char *command, const int timeout, boolean debug)
{
    char c;

    Serial.print(command);

    long int time = millis();

    while( (time+timeout) > millis())
    {
      while(Serial.available())
      {

        c = Serial.read();

        if(debug)
            Serial1.print(c);

      }
    }
}

void send_index(int connectionId){
    String cmd;

    cmd = "AT+CIPSEND=";
    cmd += connectionId;
    cmd += ",";
    cmd += strlen(webpage);
    cmd += "\r\n";

    sendData(cmd.c_str(), 1000, true);
    sendData(webpage, 1000, true);

    cmd = "AT+CIPCLOSE=";
    cmd += connectionId;
    cmd += "\r\n";

    sendData(cmd.c_str(), 1000, true);

    Serial1.println( Serial.readString() );
}

void parse_get(int connectionId){
    Serial1.println("It is GET!");

    Serial.readString();

    send_index(connectionId);

    Serial1.println( "Reply sent!" );

}

void parse_post(int connectionId){
    char color[7];

    Serial1.println("It is POST!");

    Serial.find("\r\n\r\n");
    Serial.find("color=");
    Serial.readBytesUntil('\n', color, MAX_SIZE);
    Serial1.print("Switching led with color ");
    Serial1.println(color);
    delay(10);

    if(color[0] == 'r')
        switchred();
    else if(color[0] == 'g')
        switchgreen();

    send_index(connectionId);
}

void parseCommand(char *command, int connectionId){
    char httpCommand[10];
    int i = 0;

    Serial1.println("Parsing http command...");

    while(command[i] != 10 && command[i] != 32 && command[i] != 13){
        httpCommand[i] = command[i];
        i++;
    }

    httpCommand[i] = '\0';

    //Serial1.println(httpCommand);

    if(strcmp(httpCommand, "GET") == 0)
        parse_get(connectionId);
    else if(strcmp(httpCommand, "POST") == 0)
        parse_post(connectionId);

}

void loop() {
  char command[MAX_SIZE];

  if(Serial.available() ) {

        if(Serial.find("+IPD,"))
        {
         delay(10);
         int connectionId = Serial.read()-48;

         Serial.find(":");

         Serial.readBytesUntil('\n', command, MAX_SIZE);

         parseCommand(command, connectionId);
    }
   }

  delay(1);
}

volatile void sleep(){
    for(;;)
        delay(1000);
}
