#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "Trulifi-6014/6016-TestCenter";
const char* password = "Signify2021!";

String latency="";

ESP8266WebServer server(80);

const int led = LED_BUILTIN;
const int RELAY_1 = D5;
const int RELAY_2 = D6;
const int RELAY_3 = D7;
const int RELAY_4 = D8;

int count = 0;
void handleRoot() {
  digitalWrite(led, 1);
  Serial.println("Received GET");
  server.send(200, "text/plain", latency);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  Serial.print("Received Command: ");

  String command=server.uri();

  if(command=="True"){
    Serial.println("Relay ON");
    digitalWrite(led, 1);
    digitalWrite(RELAY_1, 0);
  delay(200);
  digitalWrite(RELAY_2, 0);
  delay(200);
  digitalWrite(RELAY_3, 0);
  delay(200);
  digitalWrite(RELAY_4, 0);
  }else{
    Serial.println("Relay OFF");
    digitalWrite(led, 0);
    digitalWrite(RELAY_1, 1);
  delay(200);
  digitalWrite(RELAY_2, 1);
  delay(200);
  digitalWrite(RELAY_3, 1);
  delay(200);
  digitalWrite(RELAY_4, 1);
  }

/*
  Serial.print(command.length());

  for(int i=0;i<command.length();i++){
    Serial.println(command.charAt(i));
  }
*/

  server.send(777, "text/plain", "OK");
  
}

void setup(void) {
  pinMode(led, OUTPUT);
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  digitalWrite(RELAY_1, 1);
  delay(200);
  digitalWrite(RELAY_2, 1);
  delay(200);
  digitalWrite(RELAY_3, 1);
  delay(200);
  digitalWrite(RELAY_4, 1);
  delay(500);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}


void loop(void) {
  server.handleClient();

  if(Serial.available()>0){
    delay(100);
    latency="";
    while(Serial.available()>0){
      latency+=(char)Serial.read();  
    }
    Serial.print("ECHO: ");
    Serial.println(latency);
  }
  
}