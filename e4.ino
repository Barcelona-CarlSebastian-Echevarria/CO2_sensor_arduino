#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MQ135.h"

// LCD I2C Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// MQ135 Gas Sensor
MQ135 gasSensor = MQ135(A0);
#define sensorPin A0

// LED & Buzzer Pins
#define buzzPin 4   // Buzzer
#define redPin 3    // Red LED (High CO2)
#define greenPin 2  // Green LED (Safe CO2)

// CO2 Thresholds
#define CO2_THRESHOLD 1000  // High CO2 warning
#define CO2_BASELINE 400    // Minimum CO2 level

// Custom LCD Characters
byte smiley[8] = {
  0b00000,
  0b01010,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

byte sad[8] = {
  0b00000,
  0b01010,
  0b01010,
  0b00000,
  0b00000,
  0b01110,
  0b10001,
  0b00000
};

void setup() {
    Serial.begin(9600);
    
    // Initialize LCD
    lcd.init();
    lcd.backlight();

    // Initialize Pins
    pinMode(sensorPin, INPUT);
    pinMode(buzzPin, OUTPUT);
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);

    // LED & Buzzer Startup Test
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, HIGH);
    digitalWrite(buzzPin, HIGH);
    delay(1000);
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, LOW);
    digitalWrite(buzzPin, LOW);

    // LCD Startup Message
    lcd.setCursor(0, 0);
    lcd.print("CO2 Detector");
    delay(2000);
    lcd.clear();

    // Progress Bar (0–100%)
    lcd.print("Initializing...");
    for (int i = 0; i <= 100; i += 10) {
        lcd.setCursor(12, 0);
        if (i < 100) lcd.print(" ");
        if (i < 10) lcd.print(" ");
        lcd.print(i);
        lcd.print("%");
        delay(500);
    }
    lcd.clear();

    // Load Custom Characters
    lcd.createChar(0, smiley);
    lcd.createChar(1, sad);
}

void loop() {
    int val = analogRead(sensorPin);
    float ppm = gasSensor.getPPM();

    Serial.print("Raw Sensor Value: ");
    Serial.println(val);
    Serial.print("CO2 PPM: ");
    Serial.println(ppm);

    // Ensure CO2 value does not go below baseline
    if (ppm < CO2_BASELINE) {
        ppm = CO2_BASELINE;
    }

    // Display CO2 Level on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CO2 level:");
    lcd.setCursor(0, 1);
    lcd.print(ppm);
    lcd.print(" ppm");

    // CO2 Alert System
    if (ppm < CO2_THRESHOLD) {
        digitalWrite(greenPin, HIGH);
        digitalWrite(redPin, LOW);
        digitalWrite(buzzPin, LOW);
        lcd.print("  Good ");
        lcd.write(byte(0));  // Smiley face
    } 
    else if (ppm >= CO2_THRESHOLD && ppm <= 2000) {
        digitalWrite(greenPin, LOW);
        digitalWrite(redPin, HIGH);
        digitalWrite(buzzPin, HIGH);
        lcd.print("  Bad ");
        lcd.write(byte(1));  // Sad face
    } 
    else {
        digitalWrite(greenPin, LOW);
        digitalWrite(redPin, HIGH);
        lcd.print("  Danger!");

        // Buzzer warning pattern
        for (int i = 0; i < 3; i++) {
            digitalWrite(buzzPin, HIGH);
            delay(250);
            digitalWrite(buzzPin, LOW);
            delay(250);
        }
    }

    delay(2000);  // Delay for readability
}
