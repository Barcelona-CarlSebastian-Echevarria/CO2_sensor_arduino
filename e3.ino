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
int redPin = 3;    // Red LED (High CO2)
int greenPin = 2;  // Green LED (Safe CO2)

// Calibration Constants
#define MIN_CALIBRATION 255   // Ensuring readings do not go below this value
#define RZERO_CALIBRATION 76.63  // Adjust based on actual sensor calibration
#define CO2_BASELINE 400         // Outdoor air CO2 concentration in ppm
#define CO2_THRESHOLD 1000       // Alert threshold for CO2 (adjustable)

// Temperature & Humidity Correction Factors
float temperature = 25.0;  // Default temperature (update with a sensor)
float humidity = 50.0;     // Default humidity (update with a sensor)

void setup() {
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    mq135.begin();

    pinMode(buzzPin, OUTPUT);
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);

    // Initial LED & Buzzer Test
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

    Serial.println("Warming up sensor (24hr recommended)...");
    lcd.setCursor(0, 0);
    lcd.print("Warming Sensor...");
    delay(30000);  // Minimum 30s warm-up, ideally 24h (86400000ms)
    lcd.clear();
}

void loop() {
    // Read CO2 Levels with Calibration
    float co2 = mq135.getCorrectedPPM(temperature, humidity);
    
    // Ensure CO2 readings do not go below 255 (minimum calibration value)
    if (co2 < MIN_CALIBRATION) {
        co2 = MIN_CALIBRATION;
    }

    Serial.print("CO2: ");
    Serial.print(co2);
    Serial.println(" ppm");

    lcd.setCursor(0, 0);
    lcd.print("CO2: ");
    lcd.print(co2);
    lcd.print(" ppm");

    // CO2 Detection Logic
    if (co2 < CO2_THRESHOLD) {
        digitalWrite(greenPin, HIGH);
        digitalWrite(redPin, LOW);
        digitalWrite(buzzPin, LOW);
    } else {
        digitalWrite(greenPin, LOW);
        digitalWrite(redPin, HIGH);

        // Buzzer rings in a warning pattern
        for (int i = 0; i < 3; i++) {
            digitalWrite(buzzPin, HIGH);
            delay(250);
            digitalWrite(buzzPin, LOW);
            delay(250);
        }
    }

    delay(1000);
}
