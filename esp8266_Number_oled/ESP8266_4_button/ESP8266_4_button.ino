#include <ESP8266WebServer.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
const char* ssid = "R&D";
const char* password = "12345679";
IPAddress local_IP(192, 168, 4, 100);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 0, 0);
WiFiServer server(80);
String header;
String outputRedState = "off";
String outputGreenState = "off";
String outputYellowState = "off";
String outputBlueState = "off";
const int buzzer = 2;
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

void setup() {
Serial.begin(115200);
if (!WiFi.config(local_IP, gateway, subnet))//, primaryDNS, secondaryDNS))
{
  Serial.println("STA Failed to configure");
}
WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("WiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  if (MDNS.begin("wirelessbell")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
  MDNS.addService("http", "tcp", 80);
}
void loop(){
  MDNS.update(); 
digitalWrite(buzzer, LOW);
WiFiClient client = server.available(); // Listen for incoming clients
if (client) { // If a new client connects,
Serial.println("New Client."); // print a message out in the serial port
String currentLine = ""; // make a String to hold incoming data from the client
currentTime = millis();
previousTime = currentTime;
while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
currentTime = millis(); 
if (client.available()) { // if there's bytes to read from the client,
char c = client.read(); // read a byte, then
Serial.write(c); // print it out the serial monitor
header += c;
if (c == '\n') { // if the byte is a newline character
if (currentLine.length() == 0) {
client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();
if (header.indexOf("GET /2/on") >= 0) 
{
  Serial.println("COME Button PRESSED");
  outputRedState = "on";
   digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(200);

  delay(300);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(200);

  delay(300);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(200);

  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(200);

}
else if (header.indexOf("GET /3/on") >= 0) 
{
  Serial.println("WATER Button Pressed");
  outputGreenState = "on";

  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(200);

  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(200);

  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(200);

  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(200);

  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(200);
} 
else if (header.indexOf("GET /4/on") >= 0) 
{
  Serial.println("TEA Button Pressed");
  outputYellowState = "on";
   digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(200);

  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(200);

  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(200);

} 
else if (header.indexOf("GET /5/on") >= 0) 
{
  Serial.println("Next Button Pressed");
  outputBlueState = "on";
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(200);

  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(200);

  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(200);

  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(200);

} 
client.println("<!DOCTYPE html><html>");
client.println("<head><tittle>L J Wireless </tittle><meta name=\"viewport\"content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"icon\" href=\"data:,\">");

client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
client.println(".buzzer { background-color: #2E86C1; font-weight:700px; text-transform:uppercase; border: none; color: white; padding: 14px 24px; border-radius: 50px;");
client.println("text-decoration: none; font-size: 18px; margin: 2px; cursor: pointer;border-radius: 6px;}");
client.println("</style></head>");
client.println("<body><h1>BELL</h1>");

client.println("<p><a href=\"/2/on\"><button class=\"button buzzer\"> COME </button></a></p>");
client.println("<p><a href=\"/3/on\"><button class=\"button buzzer \">WATER</button></a></p>");
client.println("<p><a href=\"/4/on\"><button class=\"button buzzer \">  TEA  </button></a></p>");

client.println("<p><a href=\"/5/on\"><button class=\"button buzzer \"> NEXT </button></a></p>");
client.println("</body></html>");
client.println();
break;
} else { // if you got a newline, then clear currentLine
currentLine = "";
}
} else if (c != '\r') { // if you got anything else but a carriage return character,
currentLine += c; // add it to the end of the currentLine
}
}
}
header = "";
}
}