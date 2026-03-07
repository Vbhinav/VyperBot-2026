#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

/* WIFI CREDENTIALS */
const char* ssid = "your_wifi_name";
const char* password = "your_password";

/* LEADERBOARD SERVER */
const char* leaderboardIP = "10.25.106.120";   // change to your server IP
const int leaderboardPort = 9000;
const char* espID = "ESP_1";

/* PINS */
#define FLASH_BUTTON D3   // GPIO0 (FLASH button)
#define LED_BUILTIN D4    // GPIO2 (Onboard LED)

/* UDP */
WiFiUDP udp;

/* BUTTON STATE */
bool lastButtonState = HIGH;

/* SETUP */
void setup() {

  Serial.begin(115200);

  pinMode(FLASH_BUTTON, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH); 

  /* WiFi */
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("Robot IP: ");
  Serial.println(WiFi.localIP());
}

/* LOOP */
void loop() {

  bool buttonState = digitalRead(FLASH_BUTTON);

// Detect button press (edge detection) 
  if (buttonState == LOW && lastButtonState == HIGH) {

    Serial.println("FLASH button pressed!");
    sendScoreUDP();

    digitalWrite(LED_BUILTIN, LOW);  
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
  }

  lastButtonState = buttonState;
}

/* SEND SCORE */
void sendScoreUDP() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("✗ WiFi lost — reconnecting");
    WiFi.reconnect();
    return;
  }

  char msg[32];

  snprintf(msg, sizeof(msg), "SCORE,%s", espID);

  udp.beginPacket(leaderboardIP, leaderboardPort);
  udp.write((uint8_t*)msg, strlen(msg));
  udp.endPacket();

  Serial.println("✓ SCORE SENT");
}
