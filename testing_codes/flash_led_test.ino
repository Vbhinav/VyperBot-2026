/* PIN DEFINITIONS */
#define FLASH_BUTTON D3    // GPIO0 (FLASH button)
#define LED_BUILTIN D4     // GPIO2 (Onboard LED)

/* SETUP */
void setup() {
  Serial.begin(115200);

  pinMode(FLASH_BUTTON, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);
}

/* LOOP */
void loop() {

  int buttonState = digitalRead(FLASH_BUTTON);

  if (buttonState == LOW) { 
    digitalWrite(LED_BUILTIN, LOW); 
    Serial.println("FLASH button pressed");
  } 
  else {
    digitalWrite(LED_BUILTIN, HIGH); 
  }

  delay(50);
}
