#include <ESP8266WiFi.h>

const char* ssid = "MyAccessPointHome";
const char* password = "passwordHome";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  if (request.indexOf("/light1off") > 0)  {
    digitalWrite(13, HIGH);
  }
  if (request.indexOf("/light1on") > 0)  {
    digitalWrite(13, LOW);
  }

  if (request.indexOf("/light2on") > 0)  {
    digitalWrite(12, LOW);

  }
  if (request.indexOf("/light2off") > 0)  {
    digitalWrite(12, HIGH);
  }
  // Set ledPin according to the request
  //digitalWrite(ledPin, value);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
  client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
  client.println("</head>");
  client.println("<body bgcolor = \"#f7e6ec\">");
  client.println("<hr/><hr>");
  client.println("<h4><center> Esp8266 Electrical Device Control </center></h4>");
  client.println("<hr/><hr>");
  client.println("<br><br>");
  client.println("<br><br>");
  client.println("<center>");
  client.println("Device 1");
  client.println("<a href=\"/light1on\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/light1off\"\"><button>Turn Off </button></a><br />");
  client.println("</center>");
  client.println("<br><br>");
  client.println("<center>");
  client.println("Device 2");
  client.println("<a href=\"/light2on\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/light2off\"\"><button>Turn Off </button></a><br />");
  client.println("</center>");
  client.println("<br><br>");
  client.println("<center>");
  client.println("<table border=\"5\">");
  client.println("<tr>");
  
  //Check Light Status 
  if (digitalRead(13)){
    client.print("<td>Light 1 is ON</td>");
  }else{
    client.print("<td>Light 1 is OFF</td>");
  }

  if (digitalRead(12)){
    client.print("<td>Light 2 is ON</td>");
  }else{
    client.print("<td>Light 2 is OFF</td>");
  }

  client.println("</tr>");  
  client.println("</table>");
  client.println("</center>");
  client.println("</html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

}

