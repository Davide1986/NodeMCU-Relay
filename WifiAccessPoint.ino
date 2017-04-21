/*
 * NodeMCU/ESP8266 act as AP (Access Point) and simplest Web Server
 * to control GPIO (on-board LED)
 * Connect to AP "davide-wifi", password = "password"
 * Open browser, visit 192.168.4.1
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>


const char *ssid = "davide-wifi";
const char *password = "password";
int stateLED = LOW;

int stateRelay1LED = HIGH;
int stateRelay2LED = HIGH;

int stateValuePIR;

ESP8266WebServer server(80);

void handleRoot() {
    response();
}

//Led NodeMCU

void handleLedOn() {
  stateLED = LOW;
  digitalWrite(LED_BUILTIN, stateLED);
  response();
}

void handleLedOff() {
  stateLED = HIGH;
  digitalWrite(LED_BUILTIN, stateLED);
  response();
}

//Relay 1

void relay1LedOn() {
  stateRelay1LED  = LOW;
  digitalWrite(12, stateRelay1LED);
  response();
}

void relay1LedOff() {
  stateRelay1LED  = HIGH;
  digitalWrite(12, stateRelay1LED);
  response();
}

//Relay 2

void relay2LedOn() {
  stateRelay2LED  = LOW;
  digitalWrite(13, stateRelay2LED);
  response();
}

void relay2LedOff() {
  stateRelay2LED  = HIGH;
  digitalWrite(13, stateRelay2LED);
  response();
}

//Motion
void readMotion(){
  stateValuePIR = digitalRead(14);
  //low = no motion, high = motion
  if (stateValuePIR == LOW)
  {
   Serial.println("No motion");
   relay2LedOff();
  }else{
   Serial.println("Motion detected  ALARM");
   relay2LedOn();
  }
}


const String HtmlHtml = "<html><head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /></head>";
const String HtmlHtmlClose = "</html>";
const String HtmlTitle = "<h1> davide-wifi: ESP8266 AP WebServer exercise</h1><br/>\n";
const String HtmlLedStateLow = "<big>LED NodeMCU is now <b>ON</b></big><br/>\n";
const String HtmlLedStateHigh = "<big>LED NodeMCU is now <b>OFF</b></big><br/>\n";
const String HtmlLedStateRelay1Low = "<big>LED Relay 1 is now <b>ON</b></big><br/>\n";
const String HtmlLedStateRelay1High = "<big>LED Relay 1 is now <b>OFF</b></big><br/>\n";
const String HtmlLedStateRelay2Low = "<big>LED Relay 2 is now <b>ON</b></big><br/>\n";
const String HtmlLedStateRelay2High = "<big>LED Relay 2 is now <b>OFF</b></big><br/>\n";
const String HtmlButtonsLED = 
    "<a href=\"LEDOn\"><button style=\"display: block; width: 100%;\">ON</button></a><br/>"
    "<a href=\"LEDOff\"><button style=\"display: block; width: 100%;\">OFF</button></a><br/>";
const String HtmlButtonsRelay1LED =     
    "<a href=\"Relay1LEDOn\"><button style=\"display: block; width: 100%;\">ON</button></a><br/>"
    "<a href=\"Relay1LEDOff\"><button style=\"display: block; width: 100%;\">OFF</button></a><br/>";
const String HtmlButtonsRelay2LED =       
    "<a href=\"Relay2LEDOn\"><button style=\"display: block; width: 100%;\">ON</button></a><br/>"
    "<a href=\"Relay2LEDOff\"><button style=\"display: block; width: 100%;\">OFF</button></a><br/>";

void response(){
  String htmlRes = HtmlHtml + HtmlTitle;
  if(stateLED == LOW){
    htmlRes += HtmlLedStateLow;
  }else{
    htmlRes += HtmlLedStateHigh;
  }
  
  htmlRes += HtmlButtonsLED;
  
  if(stateRelay1LED == LOW){
    htmlRes += HtmlLedStateRelay1Low;
  }else{
    htmlRes += HtmlLedStateRelay1High;
  }

  htmlRes += HtmlButtonsRelay1LED;
  
  if(stateRelay2LED == LOW){
    htmlRes += HtmlLedStateRelay2Low;
  }else{
    htmlRes += HtmlLedStateRelay2High;
  }
  
  htmlRes += HtmlButtonsRelay2LED;     
  htmlRes += HtmlHtmlClose;

  server.send(200, "text/html", htmlRes);
}

void setup() {
    delay(1000);
    Serial.begin(115200);

    //Connect to Wifi network
    Serial.println();

    WiFi.softAP(ssid, password);

    IPAddress apip = WiFi.softAPIP();
    Serial.print("visit: \n");
    Serial.println(apip);
    server.on("/", handleRoot);
    server.on("/LEDOn", handleLedOn);
    server.on("/LEDOff", handleLedOff);
    server.on("/Relay1LEDOn", relay1LedOn);
    server.on("/Relay1LEDOff", relay1LedOff);
    server.on("/Relay2LEDOn", relay2LedOn);
    server.on("/Relay2LEDOff", relay2LedOff);
    server.on("/StatusMotion", readMotion);
    //Start the server
    server.begin();
    Serial.println("HTTP server beginned");
    pinMode(LED_BUILTIN, OUTPUT); //LED NodeMCU
    pinMode(12, OUTPUT); //D6
    pinMode(13, OUTPUT); //D7
    pinMode(14, INPUT); //D5
    digitalWrite(12, stateRelay1LED);
    digitalWrite(13, stateRelay2LED);
    digitalWrite(LED_BUILTIN, stateLED);
}

void loop() {
    readMotion();
    server.handleClient();
}
