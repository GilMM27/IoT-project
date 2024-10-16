#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "Tec-IoT";
const char* password = "spotless.magnetic.bridge";

// Replace with the API URL you want to call
const char* apiEndpoint = "https://kn8k5rd1-3000.usw3.devtunnels.ms/api/test";

WiFiClient wifiClient;

#define PIN_TRIG D5
#define PIN_ECHO D6

void sendPOST(float distance) {
  // Make HTTP POST request
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(wifiClient, apiEndpoint);  // Specify the URL

    // Specify content-type header (important for JSON payload)
    http.addHeader("Content-Type", "application/json");

    // Create JSON object payload
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["distance"] = distance;

    // Convert JSON document to string
    String requestBody;
    serializeJson(jsonDoc, requestBody);

    // Send the POST request with the payload
    int httpCode = http.POST(requestBody); 

    // Check the returning HTTP status code
    if (httpCode > 0) {
      // Get the request response payload
      String response = http.getString();
      Serial.println(httpCode);  // HTTP response code
      Serial.println(response);  // API response
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end(); // Close the connection
  } else {
    Serial.println("WiFi disconnected!");
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected!");
}

float ultrasonicRead() {
  digitalWrite(PIN_TRIG, LOW);  //para generar un pulso limpio ponemos a LOW 4us
  delayMicroseconds(4);
  
  digitalWrite(PIN_TRIG, HIGH);  //generamos Trigger (disparo) de 10us
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  float tiempo = pulseIn(PIN_ECHO, HIGH);
  return tiempo/58.3;
}

void loop() {
  float distance = ultrasonicRead();
  if (distance < 25) {
    sendPOST(distance);
    Serial.println("Movement detected!");
  }
  delay(500);
}