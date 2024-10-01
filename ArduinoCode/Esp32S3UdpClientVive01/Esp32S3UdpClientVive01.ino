#include <WiFi.h>
#include <WiFiUdp.h>

// char ssid[] = "meshmeshmesh";    // Set your Wi-Fi SSID
// char password[] = "sparkyasu";    // Set your Wi-Fi password

char ssid[] = "NETGEAR92";    // Set your Wi-Fi SSID
char password[] = "greatflower105";    // Set your Wi-Fi password
int status = WL_IDLE_STATUS;        // Indicator of Wi-Fi status

WiFiUDP udp;

// Command Prompt to determine current IP address for your computer:
// Windows: Type cmd in the search box, press Enter, and then type "ipconfig /all" and press Enter. Your IP address will be the IPv4 address.
// Mac: Open Terminal. Type "ipconfig getifaddr en0" and press enter.


const char* udpAddress = "192.168.0.8";  // IP address of the computer running Unity (adjust if necessary)
const int udpPort = 4211;

const int buttonPin = 7;
const int potPin = A0;

void setup() {
    Serial.begin(115200);
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(potPin, INPUT);

    // Attempt to connect to Wi-Fi network
    Serial.print("Connecting to Wi-Fi: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    // Wait for the connection to establish
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nWi-Fi connected.");
    printWifiData();
}

void loop() {
    int buttonState = digitalRead(buttonPin);
    int potValue = analogRead(potPin);

    char packetBuffer[50];
    sprintf(packetBuffer, "Button: %d, Pot: %d", buttonState, potValue);

    udp.beginPacket(udpAddress, udpPort);
    udp.write(packetBuffer);
    udp.endPacket();

    delay(40); // Send data every 40ms

    // Optionally print network info every 10 seconds
    // delay(10000);
    // printCurrentNet();
}

void printWifiData() {
    // Print the Wi-Fi IP address
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // Print the subnet mask
    IPAddress subnet = WiFi.subnetMask();
    Serial.print("NetMask: ");
    Serial.println(subnet);

    // Print the gateway address
    IPAddress gateway = WiFi.gatewayIP();
    Serial.print("Gateway: ");
    Serial.println(gateway);
    Serial.println();
}
