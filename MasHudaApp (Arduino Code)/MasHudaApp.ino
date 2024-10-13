#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_Fingerprint.h>

// WiFi credentials
const char* ssid = "Waifi123";
const char* password = "sekbentar";

// Server details
const char* serverName = "http://your-laravel-server.com/api/fingerprint";

// Hardware pins
#define FINGERPRINT_RX 14  // GPIO14 (D5 on NodeMCU)
#define FINGERPRINT_TX 12  // GPIO12 (D6 on NodeMCU)
#define RELAY_PIN 4        // GPIO4 (D2 on NodeMCU)
#define BUZZER_PIN 5       // GPIO5 (D1 on NodeMCU)

SoftwareSerial mySerial(FINGERPRINT_RX, FINGERPRINT_TX);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(115200);
  
  // Initialize fingerprint sensor
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Fingerprint sensor found!");
  } else {
    Serial.println("Fingerprint sensor not found :(");
    while (1) { delay(1); }
  }

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set up hardware pins
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  int fingerprintID = getFingerprintID();
  
  if (fingerprintID != -1) {
    // Fingerprint detected, send to server
    String response = sendFingerprintToServer(fingerprintID);
    processServerResponse(response);
  }
  
  delay(50);
}

int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;
  
  return finger.fingerID;
}

String sendFingerprintToServer(int fingerprintID) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<200> doc;
    doc["fingerprint_id"] = fingerprintID;
    String requestBody;
    serializeJson(doc, requestBody);

    int httpResponseCode = http.POST(requestBody);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      http.end();
      return response;
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
      http.end();
      return "Error";
    }
  }
  return "WiFi Disconnected";
}

void processServerResponse(String response) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, response);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  bool isApproved = doc["approved"];
  String userLevel = doc["user_level"];

  if (isApproved) {
    Serial.println("Access granted");
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    
    if (userLevel == "admin" || userLevel == "user") {
      digitalWrite(RELAY_PIN, HIGH);  // Activate magnetic lock
      delay(5000);  // Keep the lock open for 5 seconds
      digitalWrite(RELAY_PIN, LOW);   // Deactivate magnetic lock
    }
  } else {
    Serial.println("Access denied");
    // You could add a different buzzer pattern for denied access
  }
}
