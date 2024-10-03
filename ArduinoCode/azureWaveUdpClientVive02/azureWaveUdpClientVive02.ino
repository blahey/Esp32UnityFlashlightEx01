#include <WiFi.h>
#include <WiFiUdp.h>

char ssid[] = "NETGEAR92";    // Set your Wi-Fi SSID
char password[] = "greatflower105";    // Set your Wi-Fi password
int status = WL_IDLE_STATUS;        // Indicator of Wi-Fi status
WiFiUDP udp;

const int udpPort = 4211;
const int buttonPin = 21;
const int potPin = A0;

const int ledPin = 4;
int ledBrightness;

void setup() {
    Serial.begin(115200);
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(potPin, INPUT);
    pinMode(ledPin, OUTPUT);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi connected.");
    udp.begin(udpPort);
}

void loop() {
    // 1. Send button state and potentiometer value to Unity
    int buttonState = digitalRead(buttonPin);
    int potValue = analogRead(potPin);

    char packetBuffer[50];
    sprintf(packetBuffer, "Button: %d, Pot: %d", buttonState, potValue);

    udp.beginPacket("192.168.0.8", 4211); // Replace with Unity IP address
    udp.write(packetBuffer);
    udp.endPacket();

    // 2. Receive position data from Unity
    int packetSize = udp.parsePacket();
    if (packetSize) {
        char packetBuffer[255];
        int len = udp.read(packetBuffer, 255);
        if (len > 0) {
            packetBuffer[len] = 0;
        }

        Serial.println(packetBuffer); // Display received position data

        // Parse position data
        float posX, posY, posZ;
        sscanf(packetBuffer, "X:%f,Y:%f,Z:%f", &posX, &posY, &posZ);

        // Example of using the position values
        Serial.print("Position - X: ");
        Serial.print(posX);
        Serial.print(" Y: ");
        Serial.print(posY);
        Serial.print(" Z: ");
        Serial.println(posZ);

        // You can use posX, posY, and posZ to control servos, motors, etc.
        ledBrightness = abs((int)posY %255);
        analogWrite(ledPin, ledBrightness);
    }

    delay(40); // Send/receive every 40ms
}
