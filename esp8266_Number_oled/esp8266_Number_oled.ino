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
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h>




#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels



// Define OLED parameters
const int buzzerPin = D4;
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char webpage[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
body {
    background: rgb(246, 250, 255);
/* background: linear-gradient(90deg, rgba(50,130,255,1) 0%, rgba(61,244,255,1) 48%, rgba(50,130,255,1) 100%); */
    font-family: 'Arial', sans-serif;
    margin: 0;
    text-align: center;
}

.container {
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    height: 100vh;
    box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
}

.header {
    font-size: 36px;
    margin-bottom: 10px;
    color: #000; /* White text */
}
.input-container {
    margin-top: 10px;
}

input {
    font-size: 18px;
    padding: 12px;
    width: 250px;
    border: 2px solid #3282ff; /* Emerald Green */
    border-radius: 8px;
    outline: none;
    margin-bottom: 10px;
    transition: border-color 0.3s ease-in-out;
}

input:focus {
    border-color: #011585; /* Nephritis - Darker Green */
}

.button {
    font-size: 20px;
    border-radius: 12px;
    background-color: #3282ff; /* Emerald Green */
    color: #fff; /* White text */
    padding: 12px 20px;
    cursor: pointer;
    border: 4px solid #3282ff; /* Emerald Green */
    width: 250px;
    margin-bottom: 10px;
    transition: transform 0.3s ease-in-out, background-color 0.3s ease-in-out;
}

.button:hover {
    transform: scale(1.1);
    background-color: #3282ff; /* Nephritis - Darker Green */
    
}


    </style>
</head>
<body>
   
    <div class="container">
        <div class="header">
            <h1>LJ  WIRELESSBELL</h1>  
        </div>
        <div class="input-container">
            
            <input type="text" id="numberInput" name="numberInput" min="0" max="9" minlength="" maxlenght="2" placeholder="Type here any number">


        </div></br>

        <div class="button" onclick="sendMessage('come')">COME</div>
        <div class="button" onclick="sendMessage('Tea')">TEA</div>
        <div class="button" onclick="sendMessage('Coffee')">COFFEE</div>
        <div class="button" onclick="sendMessage('water')">WATER</div>
        <div class="button" onclick="sendMessage('next')">NEXT</div>
        <div class="button" onclick="sendMessage('emergency')">EMERGENCY</div>
        <div class="button" onclick="sendMessage('call')">CALL</div>
    </div>

    <script>
        var connection = new WebSocket('ws://' + location.hostname + ':81/');

        function sendMessage(message) 
        {
            var numberInput = document.getElementById('numberInput').value;
            var combinedMessage = numberInput ? `${numberInput} ${message}` : message;
            
            console.log(combinedMessage);
            connection.send(combinedMessage);
        }
    </script>
</body>
</html>

)=====";

AsyncWebServer server(80);
WebSocketsServer websockets(81);
void setupBuzzer() {
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);  // Set initial state to low
}

void beep(int frequency, int duration) {
  tone(buzzerPin, frequency, duration);
  delay(duration);
  noTone(buzzerPin);
}
void handleButtonClick(String message) {
  if (message == "come") {
    beep(1000, 500);  // Adjust frequency and duration as needed
  } else if (message == "Tea") {
    beep(1500, 500);
  } else if (message == "Coffee") {
    beep(2000, 500);
  } else if (message == "water") {
    beep(2500, 500);
  } else if (message == "next") {
    beep(3000, 500);
  } else if (message == "emergency") {
    beep(3500, 1000);
  } else if (message == "call") {
    beep(4000, 1000);
  }
}

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Page Not found");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) 
{
  switch (type) 
  {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: 
    {
        IPAddress ip = websockets.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        websockets.sendTXT(num, "Connected from server");
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      String message = String((char*)( payload));
      Serial.println(message);

     handleButtonClick(message);

        addMessageToBuffer(message);
      break;

  }
}
#define MAX_MESSAGES 4
String messageBuffer[MAX_MESSAGES];
uint8_t messageIndex = 0;

void printMessagesOnDisplay() 
{
   display.ssd1306_command(SSD1306_DISPLAYON);
  // Clear the display
  display.clearDisplay();

  // Set text properties
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  // Print messages on the display
  for (uint8_t i = 0; i < MAX_MESSAGES; i++) 
  {
    uint8_t index = (messageIndex + i) % MAX_MESSAGES;
    display.println(messageBuffer[index]);
  }

  // Update the display
  display.display();
  display.clearDisplay();
  delay(30000);
  display.ssd1306_command(SSD1306_DISPLAYOFF);
}

void addMessageToBuffer(const String &message) {
  // Add the message to the buffer
  messageBuffer[messageIndex] = message;

  // Increment the message index
  messageIndex = (messageIndex + 1) % MAX_MESSAGES;

  // Print the updated messages on the display
  printMessagesOnDisplay();
  
}



void setup(void) {
  Serial.begin(115200);
  setupBuzzer();

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
   {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) ;
  }

  display.display();
  display.display();
  delay(2000);
  display.clearDisplay();

  WiFi.softAP("techiesms", "");
  Serial.println("softap");
  Serial.println("");
  Serial.println(WiFi.softAPIP());

  if (MDNS.begin("ESP")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", webpage);
  });

  server.on("/oled/on", HTTP_GET, [](AsyncWebServerRequest * request) {
    // Update the display with your desired content
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Hello, OLED!");
    display.display();

    request->send_P(200, "text/html", webpage);
  });

  server.onNotFound(notFound);

  server.begin();
  websockets.begin();
  websockets.onEvent(webSocketEvent);

}

void loop(void) {
  websockets.loop();
}