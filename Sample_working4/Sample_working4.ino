#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int buzzerPin = A0;
const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const int numLeds = 12;

unsigned long lastBuzzerTime = 0;
unsigned long buzzerInterval = 500;
int litLeds = 0;
int typingSpeed = 0;

String incomingData = "";
unsigned long lastQuoteChange = 0;
String currentQuote = "";

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("MockeyType");
  display.display();
  delay(1000);

  pinMode(buzzerPin, OUTPUT);
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  handleSerial();
  updateLEDs();
  updateBuzzer();
}

void handleSerial() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n') {
      typingSpeed = incomingData.toInt();
      incomingData = "";
      updateQuote(typingSpeed);
      updateOLED(typingSpeed);
      updateBuzzerInterval(typingSpeed);
    } else {
      incomingData += c;
    }
  }
}

void updateQuote(int speed) {
  if (millis() - lastQuoteChange > 2000) {
    if (speed < 80)
      currentQuote = "Typing with toes?";
    else if (speed < 140)
      currentQuote = "Grandma's faster!";
    else if (speed < 200)
      currentQuote = "Not bad... for a snail.";
    else
      currentQuote = "Whoa, calm down turbo!";
    lastQuoteChange = millis();
  }
}

void updateOLED(int speed) {
  display.clearDisplay();

  // Emoji - Main focus at center top
  display.setTextSize(4);
  display.setCursor((SCREEN_WIDTH / 2) - 20, 0); // Center horizontally
  if (speed < 80)
    display.print(":(");
  else if (speed < 140)
    display.print(":|");
  else if (speed < 200)
    display.print(":)");
  else
    display.print(":D");

  // Mocking Quote - Directly below emoji, centered
  display.setTextSize(1);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(currentQuote, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 40);
  display.print(currentQuote);

  display.display();

  // LED mapping
  litLeds = map(speed, 0, 200, 0, numLeds);
  litLeds = constrain(litLeds, 0, numLeds);
}

void updateLEDs() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], (i < litLeds) ? HIGH : LOW);
  }
}

void updateBuzzerInterval(int speed) {
  int maxSpeed = 200;
  int minInterval = 30;
  int maxInterval = 1000;

  speed = constrain(speed, 0, maxSpeed);
  buzzerInterval = map(speed, 0, maxSpeed, maxInterval, minInterval);
}

void updateBuzzer() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastBuzzerTime >= buzzerInterval) {
    tone(buzzerPin, 1000, 50);
    lastBuzzerTime = currentMillis;
  }
}

 
