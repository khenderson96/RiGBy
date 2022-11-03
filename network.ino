#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

/*
 * 
 */
void setupWifi() {
  delay(10);
  p("\n\nConnecting to ");
  p(SSID);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());
}

/*
 * 
 */
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32-lights2";
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("lights2/handshake", "1"); // publishing 1 to handshake 
      publishAll(withRoot("handshake"), "1");
      // ... and resubscribe (to all topics in topics array, see declarations
      client.subscribe(withRoot("time"));
      subscribeToTopics();

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/*
 * callback function called by client.setCallback(callback)
 * receieves messages for any topic subscribed to by reconnect()
 * converts payload from byte* to char*, prints, then passes
 * topic/message to handleMessage function to act on input
 */
void callback(char* topic, byte* payload, unsigned int length) {
  char message[64];
  for (int i = 0; i < length; i++){
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  Serial.print("ASSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS");
  printMessage(topic, message);
  String topicString = String(topic);

  byte i = 0;
  if (topicString.indexOf("/0/") >= 0){
    i = 0;
    topicString = topicString.substring(topicString.indexOf("/0/")+3);
  } else if (topicString.indexOf("/1/") >= 0){
    i = 1;
    topicString = topicString.substring(topicString.indexOf("/1/")+3);
  } else if (topicString.indexOf("/2/") >= 0){
    i = 2;
    topicString = topicString.substring(topicString.indexOf("/2/")+3);
  } else if (topicString.indexOf("/3/") >= 0){
    i = 3;
    topicString = topicString.substring(topicString.indexOf("/3/")+3);
  }

  int val = String(message).toInt();
  setSectionVariables(i, topic, val);
}


/*
 * 
 */
void subscribeToTopics(){
  printlnAll("\nSubscribing to global topics:");
  for (int i = 0; i < NUM_SINGLE_TOPICS; i++){
    printlnAll(withRoot(singleTopicSect[i]));
    client.subscribe(withRoot(singleTopicSect[i]));
  }

  printlnAll("\nSubscribing to section topics:");
  for (int i = 0; i < NUM_MULTI_TOPICS; i++){
    for (int j=0; j < NUM_SECTIONS; j++){
      static char buf[64];
      char intBuf[32];
      sprintf(intBuf, "%d", j);
      strcpy(buf, ROOT_TOPIC);
      strcat(buf, intBuf);
      strcat(buf, "/");
      strcat(buf, multiTopicSect[i]);
      printAll("    ");
      printlnAll(buf);
      client.subscribe(buf);
    }
  }
  subscribeToHSVarray(); //?
  printlnAll("");
}


/*
 * 
 */
void subscribeToHSVarray(){
  for (int i = 0; i < NUM_SECTIONS; i++){
    for (int j = 0; j < 3; j++){
      char *buff; // lights2/hsv/0/h
      buff = withRoot("hsv/");
      char numStr[1];
      // TODO: why is this ten? fix it
      strcat(buff, itoa(i, numStr, 10));
      if (j==0){
        strcat(buff, "/h");
      } else if (j==1){
        strcat(buff, "/s");
      } else if (j==2){
        strcat(buff, "/v");
      }   
    client.subscribe(buff);
    printAll("    ");
    printlnAll(buff);        
    }
  }
}
