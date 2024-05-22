#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#else
#error "Board not found"
#endif
#include <WebSocketsServer.h>
IPAddress local_IP(192, 168, 8, 1);
IPAddress gateway(192, 168, 8, 1);
IPAddress subnet(255, 255, 255, 0);
#define buzzer 13
char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <head>
        <title>LJ Wirelessbell </title>
        <style>
        html{ 
            font-family: Helvetica;
            margin:0px auto;
            text-align: center;
        }.button{
            font-size: 18px;
            border-radius: 10px;
            background-color: DodgerBlue;
            color: white;
            padding: 10px 15px;
            cursor: pointer;
            border: 4px solid DodgerBlue;
            width: 200px;
        }  
        </style>
        </head>
<script>
var connection = new WebSocket('ws://'+location.hostname+':81/');
function COME()
{
  console.log("COME");
  connection.send("COME");
}
function WATER()
{
console.log("WATER");
connection.send("WATER");
}
function TEA()
{
console.log("TEA");
connection.send("TEA");
}
function NEXT()
{
console.log("NEXT");
connection.send("NEXT");
}
</script>
<body>
<center>
<h1>Bell</h1><br><br>
<button onclick= "COME()" class="button">COME</button><br><br>
<button onclick="WATER()" class="button">WATER</button><br><br>
<button onclick="TEA()" class="button">TEA</button><br><br>
<button onclick="NEXT()" class="button">NEXT</button><br><br>
</center>
</body>
</html>
)=====";
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
AsyncWebServer server(80); // server port 80
WebSocketsServer websockets(81);
void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Page Not found");
}
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) 
  {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = websockets.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        websockets.sendTXT(num, "Connected from server");
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      String message = String((char*)( payload));
      Serial.println(message);
  
    if(message == "COME")
    {
    tone(buzzer,10,5000);
    }
     else if(message == "WATER")
     {
       tone(buzzer,30,5000);  
     }
    else if(message == "TEA")
    {
        tone(buzzer,50,5000);
    }
  else if(message == "NEXT")
  {
     tone(buzzer,80,5000);
  }
  }
}
void setup(void)
{ 
  Serial.begin(115200);
  pinMode(buzzer,OUTPUT);
  digitalWrite(buzzer, LOW);
  WiFi.softAP("Wirelessbell", "12345679");
WiFi.softAPConfig(local_IP, gateway, subnet);
  Serial.println("softap");
  Serial.println("");
  Serial.println(WiFi.softAPIP());  
  if (MDNS.begin("wirelessbell")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
  MDNS.addService("http", "tcp", 80);
  server.on("/", [](AsyncWebServerRequest * request)
  {   
  request->send_P(200, "text/html", webpage);
  });
   server.on("/led1/on", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    digitalWrite(buzzer,HIGH);
  request->send_P(200, "text/html", webpage);
  });
  server.onNotFound(notFound);
  server.begin();  // it will start webserver
  websockets.begin();
  websockets.onEvent(webSocketEvent);
}
void loop(void)
{
  MDNS.update();
 websockets.loop();
}