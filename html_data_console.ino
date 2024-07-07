#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

// Replace with your network credentials
const char* ssid = "Nagarjuna";
const char* password = "123456789";

// Parameter to save the input string
const char* PARAM_INPUT = "inputString";

// Create an instance of the server
AsyncWebServer server(80);

// Variable to store the input data
String inputData;

void writeFile(fs::FS &fs, const char * path, const char * message) {
  File file = fs.open(path, "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

String readFile(fs::FS &fs, const char * path) {
  File file = fs.open(path, "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return String();
  }
  String fileContent;
  while (file.available()) {
    fileContent += file.readStringUntil('\n');
  }
  file.close();
  return fileContent;
}

void setup() {
  // Start Serial Monitor
  Serial.begin(115200);

  // Initialize LittleFS
  if (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting LittleFS");
    return;
  }
  Serial.println("LittleFS mounted successfully");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Define the route for the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", R"rawliteral(
      <!DOCTYPE HTML>
      <html>
      <head>
        <title>NodeMCU Project</title>
        <style>
          .button {padding: 50px 50px; font-size: 48px; margin: 5px; cursor: pointer;}
          .on-button {background-color: green; color: white;}
          .off-button {background-color: red; color: white;}
          body {width: 100%; height: 100vh; margin: 0; background-color: #FFD1DC; color: #000000; font-family: Cursive; font-size: 16px; background-repeat: no-repeat; background-size: 100% 100%; text-align: center;}
          .emoji {font-size: 100px;}
        </style>
      </head>
      <body>
        <h1>SMART OLED</h1>
        <button class="button on-button">WiFi On</button>
        <button class="button off-button">WiFi Off</button><br><br>
        <input id="userInput" placeholder="Enter your Quote"><br><br>
        <button onclick="sendData()">Submit</button>
        <h1 id="message"></h1>
        <script>
          function sendData() {
            let userInput = document.getElementById('userInput').value;
            let xhr = new XMLHttpRequest();
            xhr.open("GET", "/get?inputString=" + userInput, true);
            xhr.send();
            document.getElementById('message').innerText = 'Your Quote is: ' + userInput;
          }
        </script>
      </body>
      </html>
    )rawliteral");
  });

  // Handle the data received from the HTML input
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->hasParam(PARAM_INPUT)) {
      inputData = request->getParam(PARAM_INPUT)->value();
      writeFile(LittleFS, "/inputString.txt", inputData.c_str());
    }
    request->send(200, "text/plain", "Data received");
  });

  // Start server
  server.begin();
}

void loop() {
  // Read the file and print its contents to the Serial Monitor
  String fileContent = readFile(LittleFS, "/inputString.txt");
  if (fileContent.length() > 0) {
    Serial.println("Content of /inputString.txt:");
    Serial.println(fileContent);
  }
  delay(5000); // Print every 5 seconds
}
