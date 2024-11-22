#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "Tec-IoT";
const char* password = "spotless.magnetic.bridge";

// Replace with the API URL you want to call
const char* apiEndpoint = "";

#define PIN_TRIG_1 D6 // 0 - right
#define PIN_ECHO_1 D7
#define PIN_TRIG_2 D2 // 1 - front
#define PIN_ECHO_2 D3
#define PIN_TRIG_3 D4 // 2 - left
#define PIN_ECHO_3 D5

uint8_t trigPins[] = {PIN_TRIG_1, PIN_TRIG_2, PIN_TRIG_3};
uint8_t echoPins[] = {PIN_ECHO_1, PIN_ECHO_2, PIN_ECHO_3};

void sendPOST(float distance, int deviceId) {

  // Make HTTP POST request
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;

    http.begin(client, apiEndpoint);  // Specify the URL

    // Specify content-type header (important for JSON payload)
    http.addHeader("Content-Type", "application/json");

    // Create JSON object payload
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["distance"] = distance;
    jsonDoc["deviceId"] = deviceId;

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
      Serial.printf("Error on HTTP request: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end(); // Close the connection
  } else {
    Serial.println("WiFi disconnected!");
  }
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 3; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected!");
}

float ultrasonicRead(int index) {
  digitalWrite(trigPins[index], LOW);  //para generar un pulso limpio ponemos a LOW 4us
  delayMicroseconds(4);
  
  digitalWrite(trigPins[index], HIGH);  //generamos Trigger (disparo) de 10us
  delayMicroseconds(10);
  digitalWrite(trigPins[index], LOW);
  
  float tiempo = pulseIn(echoPins[index], HIGH);
  return tiempo/58.3;
}

void loop() {
  for (int i = 0; i < 3; i++) {
    float distance = ultrasonicRead(i);
    Serial.print(i);
    Serial.print(" - ");
    Serial.println(distance);
    if (distance < 10 && distance > 0) {
      sendPOST(distance, i + 1);
      Serial.print("Movement detected on ");
      Serial.println(i);
    }
  } 
    delay(500);
}