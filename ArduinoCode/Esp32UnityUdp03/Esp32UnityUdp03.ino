#include <Bounce2.h>

#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <elapsedMillis.h>

// Bounce setup
const int FLASHLIGHT_BUTTON_PIN = 26;
Bounce flashlightButton = Bounce(); // Instantiate a Bounce object

// Replace with your network credentials and target IP
const char* ssid = "entanglement";
const char* password = "BU@37ajMVAg2";
const char* udpAddress = "192.168.0.8"; // Replace with your Unity IP address
const int udpPort = 4210;
const int receivePort = 4211;

WiFiUDP udp;
WiFiUDP receiveUdp;
int sensorValue32 = 0;
int sensorValue33 = 0;
int sensorValue34 = 0;
int sensorValue35 = 0;
int flashlightButtonState = 1;
int rssi = 0;

// Timers for intervals
elapsedMillis sensorValueTimer;
elapsedMillis rssiValueTimer;

// Intervals in milliseconds
const unsigned long sensorValueInterval = 40;
const unsigned long rssiValueInterval = 5000;

float posX, posY, posZ;



void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Wait for the connection to the Wi-Fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Start UDP
  receiveUdp.begin(receivePort);
  pinMode(26, INPUT);

  //Bounce setup
  flashlightButton.attach(FLASHLIGHT_BUTTON_PIN, INPUT);
  flashlightButton.interval(25); 

}

void loop() {
  // Update sensorValues at the specified interval
  if (sensorValueTimer >= sensorValueInterval) {
    sensorValue32 = analogRead(32); // Read from analog pin 32
    sensorValue33 = analogRead(33); // Read from analog pin 33
    sensorValue34 = analogRead(34); // Read from analog pin 34
    sensorValue35 = analogRead(35); // Read from analog pin 35

    flashlightButton.update(); //Update the Bounce instance
    if(flashlightButton.fell() ) {
      flashlightButtonState = 0; // Send a 0 if the button was pressed (transition from HIGH to LOW)
    }
    else {
      flashlightButtonState = 1; // Send a 1 if the button state did not transition fron HIGH to LOW
    }
    //switchValue26 = digitalRead(26); // Read from digital pin 26
    sensorValueTimer = 0; // Reset the timer
  }

  // Update rssiValue at the specified interval
  if (rssiValueTimer >= rssiValueInterval) {
    rssi = WiFi.RSSI();
    rssiValueTimer = 0; // Reset the timer
  }

  // Send sensor data via UDP
  sendSensorData();

  // Receive position data from Unity
  receivePositionData();

}

void sendSensorData() {
  // Create a JSON document and populate it with sensor data
  DynamicJsonDocument doc(1024);
  doc["sensorValue32"] = sensorValue32;
  doc["sensorValue33"] = sensorValue33;
  doc["sensorValue34"] = sensorValue34;
  doc["sensorValue35"] = sensorValue35;
  doc["flashlightButtonState"] = flashlightButtonState;
  doc["rssi"] = rssi;

  // Serialize JSON document to a string
  char output[256];
  size_t len = serializeJson(doc, output);

  // Send the JSON string via UDP
  udp.beginPacket(udpAddress, udpPort);
  udp.write((uint8_t*)output, len);
  udp.endPacket();

  // Debugging: print the sent data to the Serial Monitor
  Serial.println("Data sent: " + String(output));
}

void receivePositionData() {
  int packetSize = receiveUdp.parsePacket();
  if (packetSize) {
    char incomingPacket[255];
    int len = receiveUdp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }
    Serial.printf("Received packet of size %d: %s\n", len, incomingPacket);

    // Parse the incoming JSON data
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, incomingPacket);
    if (!error) {
      posX = doc["x"];
      posY = doc["y"];
      posZ = doc["z"];
      Serial.printf("Received position data - X: %f, Y: %f, Z: %f\n", posX, posY, posZ);
    } else {
      Serial.print("Failed to parse JSON: ");
      Serial.println(error.c_str());
    }
  }
}


