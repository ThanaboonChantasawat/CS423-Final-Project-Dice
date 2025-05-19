#include <WiFi.h>
#include <HTTPClient.h>

#define BUTTON_PIN 4
int debouncedButtonState = LOW;
int lastRawButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
// WIFI
const char* ssid = "Thanaboon_2.4GHz";
const char* password = "0956247270aA!";
// IP
const char* serverName = "http://192.168.1.33:3000/api/dice_roll";

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 30) {
    delay(500);
    Serial.print(".");
    retries++;
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi successfully!");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway IP: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("Subnet Mask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("DNS Server IP: ");
    Serial.println(WiFi.dnsIP(0));
  } else {
    Serial.print("Failed to connect to WiFi. Last status: ");
    Serial.println(WiFi.status());
    Serial.println("Please check SSID, password, 2.4GHz compatibility, and router settings.");
    while(true) { delay(1000); }
  }

  randomSeed(analogRead(0));
  Serial.println("ESP32 IoT Dice Roller Ready!");
  Serial.println("Press the button to roll the dice.");
}

void loop() {
  int currentRawButtonState = digitalRead(BUTTON_PIN);

  if (currentRawButtonState != lastRawButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentRawButtonState != debouncedButtonState) {
      debouncedButtonState = currentRawButtonState;
      if (debouncedButtonState == HIGH) {
        Serial.println("----------------------------------");
        Serial.println("Button Pressed! (Debounced)");

        int diceRoll = random(1, 7);
        Serial.print("Dice Roll: ");
        Serial.println(diceRoll);

        sendDiceRoll(diceRoll);
        Serial.println("----------------------------------");
      }
    }
  }

  lastRawButtonState = currentRawButtonState;
}

void sendDiceRoll(int roll) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    http.setTimeout(10000);

    const char* host = "192.168.1.33";
    const int httpPort = 3000;
    const char* path = "/api/dice_roll";

    Serial.print("Connecting to host: "); Serial.print(host);
    Serial.print(" Port: "); Serial.print(httpPort);
    Serial.print(" Path: "); Serial.println(path);

    if (http.begin(client, host, httpPort, path)) {
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      String httpRequestData = "roll=" + String(roll);
      Serial.print("Request data: ");
      Serial.println(httpRequestData);

      int httpResponseCode = http.POST(httpRequestData);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        Serial.print("Server Response: ");
        Serial.println(response);
      } else {
        Serial.print("Error on sending POST. HTTP Code: ");
        Serial.println(httpResponseCode);
        Serial.printf("HTTP POST failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
      }
      http.end();
    } else {
      Serial.println("HTTP begin failed. Check host, port, or path.");
    }
  } else {
    Serial.println("WiFi Disconnected. Cannot send data.");
  }
}