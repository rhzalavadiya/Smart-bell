#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h> 
#include "SSD1306Spi.h"

// Change here the ssid and password to your own.
const char *ssid = "SSID";
const char *password = "password";

String ss = ssid;
String pw = password;

// The OLED display width and height
#define int DISPLAY_HEIGHT = 64;
#define int DISPLAY_WIDTH = 128;

// Your webpage to show in string form.
String webPage = "<html>\
  <head>\
    <title>Melvin ESP8266 Demo</title>\
  </head>\
  <body>\
    <h1>Hello from Melvin's ESP8266!</h1>\
    <p>You can enter text here and it will output on the oled screen </p>\
    <form action='http://192.168.4.1/submit' method='POST'>\
      Input: <input type='text' name='input'><br>\
      <input type='submit' value='Submit'>\
    </form>\
  </body>\
</html>";

ESP8266WebServer server(80); // webserver on port 80
SSD1306Spi display(0,16,2); // RESET, DC, CS

// Send the webpage when going to ESP8266s ip address
void handleRoot() {
  server.send(200, "text/html", webPage);
}

// Handle the submit Button
void handleSubmit(){
  if (server.args() > 0 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "input") {
         // If the argumentName is input print out result on OLED Screen.
         display.clear();
         display.drawString(DISPLAY_WIDTH/2, 10, "ssid: " + ss);
         display.drawString(DISPLAY_WIDTH/2, 20, "password: " + pw);
         display.drawString(DISPLAY_WIDTH/2, 30, "IP address: " + WiFi.softAPIP().toString());
         display.drawString(DISPLAY_WIDTH/2, 40, "output: " + server.arg(i));
         display.display();
      }
    }
  }
  // Send the webpage again so users can enter text again.
  server.send(200, "text/html", webPage);
}

void setup() {
  // Init and flip the OLED display
  display.init();
  display.flipScreenVertically();
  // Set contrast and font
  display.setContrast(255);
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);

  // Startup Wifi Access Point
  WiFi.softAP(ssid, password);
  delay(500);

  // Display some info on the OLED
  display.clear();
  display.drawString(DISPLAY_WIDTH/2, 10, "ssid: " + ss); // X,Y,String
  display.drawString(DISPLAY_WIDTH/2, 20, "password: " + pw);
  display.drawString(DISPLAY_WIDTH/2, 30, "IP address: " + WiFi.softAPIP().toString()); // The Ip of the ESP itself so you know where to go when connected. 
  display.display();

  // Start WebSockets on '/' and '/submit' for the form submit.
  server.on("/", handleRoot);
  server.on("/submit", handleSubmit);
  server.begin();
}

// handle websocket requests forever.
void loop() {
  server.handleClient();
}