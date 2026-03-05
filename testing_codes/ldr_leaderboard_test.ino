#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

/* WIFI CONFIG */
const char* ssid = "SAC-08(2.4GHz)";
const char* password = "sac@1964";

/* SCORE UDP CONFIG */
WiFiUDP scoreUdp;

const char* scoreServerIP = "192.168.1.169";
const unsigned int scoreServerPort = 9000;

/* ESP ID */
const char* espID = "ESP_1";

/* LDR */
#define LDR_PIN A0

/* LDR SETTINGS */
#define LDR_THRESHOLD 1000

bool wasAboveThreshold = false;

void setup() {

  Serial.begin(115200);
  delay(500);

  Serial.println("\n=== LDR LEADERBOARD TEST ===");

  pinMode(LDR_PIN, INPUT);

  /* ---------- WIFI ---------- */
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(ssid, password);

  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println("\nWiFi CONNECTED");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  scoreUdp.begin(0);
}

void loop() {

  /* READ LDR */

  int ldrValue = 0;

  for (int i = 0; i < 5; i++)
    ldrValue += analogRead(LDR_PIN);

  ldrValue /= 5;

  Serial.print("LDR: ");
  Serial.println(ldrValue);

  /* HIT DETECTION */

  if (ldrValue >= LDR_THRESHOLD && !wasAboveThreshold) {

    Serial.println("HIT DETECTED -> Sending SCORE");

    sendScoreUDP();

    wasAboveThreshold = true;
  }

  if (ldrValue < LDR_THRESHOLD)
    wasAboveThreshold = false;

  delay(100);
}

/* SEND SCORE */

void sendScoreUDP() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost!");
    return;
  }

  char msg[32];
  snprintf(msg, sizeof(msg), "SCORE,%s", espID);

  scoreUdp.beginPacket(scoreServerIP, scoreServerPort);
  scoreUdp.write((uint8_t*)msg, strlen(msg));
  scoreUdp.endPacket();

  Serial.println("Score packet sent!");
}
