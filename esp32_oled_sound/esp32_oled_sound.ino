#include <AsyncPrinter.h>
#include <DebugPrintMacros.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncTCPbuffer.h>
#include <SyncClient.h>
#include <async_config.h>
#include <tcp_axtls.h>


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
//#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h>




#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels



// Define OLED parameters
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char webpage[] PROGMEM = R"=====(

<!DOCTYPE html>
<html>

<head>
    <title>L J WirelessBell</title>
    <style>
        body {
            background: rgb(246, 250, 255);
            font-family: 'Arial', sans-serif;
            margin: 0;
            text-align: center;
        }

        table {
            padding: 10px;
        }

        td {
            padding: 10px;
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
            color: #000;
            /* White text */
        }

        .input-container {
            margin-top: 10px;
        }

        input {
            font-size: 18px;
            padding: 12px;
            width: 250px;
            border: 2px solid #3282ff;
            /* Emerald Green */
            border-radius: 8px;
            outline: none;
            margin-bottom: 10px;
            transition: border-color 0.3s ease-in-out;
        }

        input:focus {
            border-color: #011585;
            /* Nephritis - Darker Green */
        }

        .button {
            font-size: 20px;
            border-radius: 12px;
            background-color: #3282ff;
            /* Emerald Green */
            color: #fff;
            /* White text */
            padding: 12px 20px;
            cursor: pointer;
            border: 4px solid #3282ff;
            /* Emerald Green */
            width: 200px;
            margin-bottom: 10px;
            transition: transform 0.3s ease-in-out, background-color 0.3s ease-in-out;
        }

        .b1 {
            font-size: 20px;
            border-radius: 12px;
            background-color: #3282ff;
            /* Emerald Green */
            color: #fff;
            /* White text */
            padding: 12px 20px;
            cursor: pointer;
            border: 4px solid #3282ff;
            /* Emerald Green */
            width: 50px;
            margin-bottom: 10px;
            transition: transform 0.3s ease-in-out, background-color 0.3s ease-in-out;
        }

        .button:hover {
            transform: scale(1.1);
            background-color: #3282ff;
            /* Nephritis - Darker Green */

        }
    </style>
</head>

<body>
    <center>
        <h1> <h1>LJ WIRELESSBELL</h1></h1>
        <table>
            <tr>
                <td> <input type="text" id="numberInput" name="numberInput" min="0" max="9" minlength="" placeholder="Type here any number"></td>
                <td><button class="button" onclick="sendMessage()">SEND</button></td>
                <td><button class="button" onclick="repeatMessage()">REPEAT</button><br></td>
            </tr>
            <tr>
                <td>
                    <button style=" padding: 15px; margin: 15px;" class="b1" onclick="appendNumber('1')">1</button>
                    <button style="padding: 15px; margin: 15px;" class="b1" onclick="appendNumber('2')">2</button>
                    <button style="padding: 15px; margin: 15px;" class="b1" onclick="appendNumber('3')">3</button><br>
                </td>
                <td>
                    <button class="button" onclick="appendMessage('TEA ')">TEA</button><br>
                </td>
                <td>
                    <button class="button" onclick="appendMessage('COME ')">COME</button><br>
                </td>
            </tr>
            <tr>
                <td>
                    <button style=" padding: 15px; margin: 15px;" class="b1" onclick="appendNumber('4')">4</button>
                    <button style=" padding: 15px; margin: 15px;" class="b1" onclick="appendNumber('5')">5</button>
                    <button style=" padding: 15px; margin: 15px;" class="b1" onclick="appendNumber('6')">6</button><br>
                </td>
                <td>
                    <button class="button" onclick="appendMessage('COFFEE ')">COFFEE</button><br>
                </td>
                <td>
                    <button class="button" onclick="appendMessage('NEXT ')">NEXT</button><br>
                </td>
            </tr>
            <tr>
                <td>
                    <button style=" padding: 15px; margin: 15px;" class="b1" onclick="appendNumber('7')">7</button>
                    <button style=" padding: 15px; margin: 15px;" class="b1" onclick="appendNumber('8')">8</button>
                    <button style=" padding: 15px; margin: 15px;" class="b1" onclick="appendNumber('9')">9</button><br>
                </td>
                <td>
                    <button class="button" onclick="appendMessage('WATER ')">WATER</button><br>
                </td>
                <td>
                    <button class="button" onclick="appendMessage('EMERGENCY ')">EMERGENCY</button><br>
                </td>
            </tr>
            <tr>
                <td>
                    <center>
                        <button style=" padding: 15px; margin: 15px;" class="b1" onclick="appendNumber('0')">0</button>
                    </center>
                </td>
                <td>
                    <button class="button" onclick="appendMessage('COLD-DRINK ')">COLD-DRINK</button>
                </td>
                <td>
                    <button class="button" onclick="appendMessage('CALL ')">CALL</button>
                </td>
            </tr>
        </table>
    </center>
    <script>
     var connection = new WebSocket('ws://' + location.hostname + ':81/');
        let currentInput = '';
        let previousMessage = '';
    
        function appendNumber(number) {
            currentInput += number;
            updateInputField();
        }
    
        function appendMessage(message) {
            currentInput += ' ' + message;
            previousMessage = currentInput; // Save the current message before appending new message
            updateInputField();
        }
    
        function updateInputField() {
            document.getElementById('numberInput').value = currentInput;
        }
    
        function sendMessage() {
            // Access the currentInput variable
            console.log('Message sent:', currentInput);
    
            // Add your logic to handle sending messages
    
            // For now, let's reset the input field
            currentInput = '';
            updateInputField();
            connection.send(currentInput);
        }
    
        function repeatMessage() {
            currentInput = previousMessage; // Set the current message to the previous message
            updateInputField();
            console.log('Repeated message:', currentInput);
            connection.send(currentInput);
        }
    </script>
    
</body>

</html>

)=====";

AsyncWebServer server(80);
WebSocketsServer websockets(81);

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Page Not found");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
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

     
        addMessageToBuffer(message);
      break;
  }
}
#define MAX_MESSAGES 4
String messageBuffer[MAX_MESSAGES];
uint8_t messageIndex = 0;

void printMessagesOnDisplay() {
  // Clear the display
  display.clearDisplay();

  // Set text properties
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  // Print messages on the display
  for (uint8_t i = 0; i < MAX_MESSAGES; i++) {
    uint8_t index = (messageIndex + i) % MAX_MESSAGES;
    display.println(messageBuffer[index]);
  }

  // Update the display
  display.display();
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


  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
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