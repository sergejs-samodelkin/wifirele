#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "WebSwitch.h"

const char* ssid = "podarok";
const char* password = "???????";

ESP8266WebServer server(80);

const int switchLength = 4;
const int switchPins[] = {5,4,14,12};
int state[] = {1,1,1,1};

void handleCheck() {
  String result("{");
  for(int i=0; i<switchLength; i++) {
    result += "'line";
    result += (i+1);
    result += "':";
    result += state[i] == 1? "0":"1";
    result +=",";
  }
  result+="'a':'a'}";
  server.send(200, "text/json", result);
}
void handleRoot() {
  //Content-Encoding  gzip ???
  server.send(200, "text/html", webpage);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
void handleCommand() {
  if(server.args()>0 && server.argName(0)=="c") {
    String f4dig = server.arg(0);
    Serial.print("Receive command: ");
    Serial.println(f4dig);
    for(int i=0; i<switchLength; i++) {
       state[i] = (f4dig[i]=='1')?0:1;
    }
  }
  
  for (int i=0; i<switchLength; i++) {
    digitalWrite(switchPins[i], state[i]);
  }
  handleCheck();
}
void setup(void){
  Serial.begin(115200);
  for (int i=0; i<switchLength; i++) {
    pinMode(switchPins[i], OUTPUT);
    digitalWrite(switchPins[i], state[i]);
  }
  
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/check", handleCheck);
  server.on("/do", handleCommand);
  server.onNotFound(handleRoot);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
