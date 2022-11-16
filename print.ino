//

/*
 * 
 */
void publishAll(char* topic, char* message){
  client.publish(topic, message);
  printlnAll("<-- PUBLISHING:");
  printAll("    Topic   : ");
  printlnAll(topic);
  printAll("    Message : ");
  printlnAll(message);
  printlnAll("");
}

/*
 * publishes message to topic every N seconds
 */
unsigned long publishEveryNsec(char* topic, char* message, int rate, unsigned long now, unsigned long prev){
  rate *= 1000;
  if (now - prev > rate) {
    prev = now;
    ++value;
    publishAll(topic, message);
  }
  return prev;
}

/*
 * converts byte array to null terminated char array
 */
char* getMessage(byte* payload, unsigned int length){
  static char message[255];
  for (int i = 0; i < length; i++){
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  return message;
}

/*
 * prints incoming message to Serial and/or Telnet
 */
void printMessage(char* topic, char* message){
  printlnAll("--> RECEIVING:");
  printAll("    Topic   : ");
  printlnAll(topic);
  printAll("    Message : ");
  printlnAll(message);
}

void printHSVarray(){
  for(int i = 0; i<NUM_SECTIONS;i++){
    Serial.print("S#");
    Serial.print(i);
    Serial.print(" ");
    Serial.print(sections[i].h);
    Serial.print(", ");
    Serial.print(sections[i].s);
    Serial.print(", ");
    Serial.println(sections[i].v);
  }
}

// see seetup.h to enable Serial printing via p() and pln() macro
// when DEBUG is defined, the functions below will print to Serial and Telnet
// glory to polymorphism :)

void printAll(char* message){
  p(message);
  TelnetStream.print(message);
}

void printAll(int message){
  p(message);
  TelnetStream.print(message);
}

void printlnAll(char* message){
  pln(message);
  TelnetStream.println(message);
}

void printlnAll(String message){
  pln(message);
  TelnetStream.println(message);
}

void printlnAll(int message){
  pln(message);
  TelnetStream.println(message);
}
