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

void publishAll(char* topic, char* message){
  client.publish(topic, message);
  printlnAll("<-- PUBLISHING:");
  printAll("    Topic   : ");
  printlnAll(topic);
  printAll("    Message : ");
  printlnAll(message);
  printlnAll("");
}

char* getMessage(byte* payload, unsigned int length){
  static char message[255];
  for (int i = 0; i < length; i++){
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  return message;
}

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
