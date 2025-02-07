#include <MQ135.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// I2C LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Sensor Pin
int Analog_Input = A0;
MQ135 mq135(Analog_Input);

// Buzzer and LED Pins
int buzzPin = 4;   // Buzzer
int redPin = 3;    // Red LED (CO2 detected)
int greenPin = 2;  // Green LED (Normal air)

#define CO2_THRESHOLD 1000  // Normal CO2 level

void setup() {
    Serial.begin(9600);
    lcd.init();      // Initialize LCD
    lcd.backlight();
    mq135.begin();   // Initialize MQ-135 sensor

    pinMode(buzzPin, OUTPUT);
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);

    // Test LEDs and buzzer
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, HIGH);
    digitalWrite(buzzPin, HIGH);
    delay(1000);
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, LOW);
    digitalWrite(buzzPin, LOW);

    lcd.setCursor(0, 0);
    lcd.print("CO2 Detector");
    delay(2000);
    lcd.clear();

    Serial.println("Warming up sensor...");
    lcd.setCursor(0, 0);
    lcd.print("Warming Sensor...");
    delay(5000);  // Sensor warm-up time
    lcd.clear();
}

void loop() {
    float co2 = mq135.getPPM(); // Read CO2 levels directly from MQ-135

    Serial.print("CO2: ");
    Serial.print(co2);
    Serial.println(" ppm");

    lcd.setCursor(0, 0);
    lcd.print("CO2: ");
    lcd.print(co2);
    lcd.print(" ppm ");

    // CO2 Detection Logic
    if (co2 < CO2_THRESHOLD) {
        // Safe level
        digitalWrite(greenPin, HIGH);
        digitalWrite(redPin, LOW);
        digitalWrite(buzzPin, LOW);
    } else {
        // High CO2 level detected
        digitalWrite(greenPin, LOW); // Turn off green LED
        digitalWrite(redPin, HIGH); // Turn on red LED

        // Buzzer rings in an interval
        for (int i = 0; i < 5; i++) {
            digitalWrite(buzzPin, HIGH);
            delay(300); // Reduced delay for faster buzzing
            digitalWrite(buzzPin, LOW);
            delay(300); // Reduced delay for faster buzzing
        }
    }

    delay(1000); // Closing brace for the delay function
}
