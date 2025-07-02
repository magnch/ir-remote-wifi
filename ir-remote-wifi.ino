#include <Arduino.h>
#include <IRremote.hpp>
#include <WiFi.h>
#include <WebServer.h>
#include "samsung_codes.h"

#define REPEATS 0
#define IR_SEND_PIN 12

// WiFi credentials
const char* ssid = "H0409";
const char* password = "0x58887416";

WebServer server(80);

void setup() {

  pinMode(IR_SEND_PIN, OUTPUT);
  IrSender.begin(IR_SEND_PIN);

  Serial.begin(115200);
  
  // Initialize WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected. IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize server
  server.on("/", handle_root);
  server.on("/send", handle_IR_send);
  server.begin();
  Serial.println("HTTP server started");

  Serial.println("Setup complete");

}

void loop() {

  server.handleClient();

}

// Send IR based on query param
void handle_IR_send() {
  if (!server.hasArg("code")) {
    server.send(400, "text/plain", "Missing code parameter");
    return;
  }

  String code_str = server.arg("code");
  Code code = static_cast<Code>(code_str.toInt());
  int command = code_to_hex(code);
  IrSender.sendSamsung(ADDRESS, command, REPEATS);

  Serial.printf("Sent IR command: %s (0x%X)\n", code_to_string(code).c_str(), command);
  server.send(200, "text/plain", "Sent: " + code_to_string(code));
}

// Web UI
void handle_root() {
  String html = R"rawliteral(
    <html>
      <head>
        <title>IR Remote</title>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <style>
          body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f4;
            display: flex;
            flex-direction: column;
            align-items: center;
            padding: 20px;
            margin: 0;
          }

          h1 {
            color: #333;
            margin-bottom: 30px;
          }

          .button-container {
            display: flex;
            flex-direction: column;
            width: 100%;
            max-width: 300px;
            gap: 10px;
          }

          .remote-button {
            background-color: #4CAF50;
            color: white;
            padding: 14px;
            border: none;
            border-radius: 8px;
            font-size: 16px;
            cursor: pointer;
            transition: background-color 0.3s;
            width: 100%;
          }

          .remote-button:hover {
            background-color: #45a049;
          }

          #status {
            margin-top: 30px;
            font-size: 18px;
            color: #333;
            text-align: center;
          }
        </style>
        <script>
          function sendCode(code) {
            fetch("/send?code=" + code)
              .then(response => response.text())
              .then(data => {
                document.getElementById("status").innerText = data;
              })
              .catch(err => {
                document.getElementById("status").innerText = "Error sending code.";
              });
          }
        </script>
      </head>
      <body>
        <h1>Samsung Remote</h1>
        <div class="button-container">
  )rawliteral";

  for (int i = 0; i <= vol_down; ++i) {
    html += "<button class=\"remote-button\" onclick=\"sendCode(" + String(i) + ")\">" + code_to_string(static_cast<Code>(i)) + "</button>";
  }

  html += R"rawliteral(
        </div>
        <div id="status">Status: Ready</div>
      </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}
