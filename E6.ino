
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MQ135.h>

#define anInput A0     // MQ135 analog pin
#define buzzer 4       // Buzzer pin
#define redLed 3       // Red LED pin
#define greenLed 2     // Green LED pin
#define co2Zero 100    // Adjust based on calibration

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD address 0x27 for a 16x2 display

int lastCo2ppm = 0;  // Store previous CO2 reading

void setup() {
    Serial.begin(9600);  // Start Serial communication for Serial Plotter
    
    // Initialize LCD
    lcd.init();
    lcd.backlight();

    // Initialize Pins
    pinMode(anInput, INPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);

    // LED & Buzzer Startup Test
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, LOW);
    digitalWrite(buzzer, LOW);

    // Countdown / Progress Bar for Preheat
    lcd.print("Warming up...");
    for (int i = 0; i <= 100; i += 1) {
        lcd.setCursor(12, 0);
        if (i < 100) lcd.print(" ");
        if (i < 10) lcd.print(" ");
        lcd.print(i);
        lcd.print("%");
        delay(500);
    }
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Ready to Read!");
    delay(2000);
    lcd.clear();

    Serial.println("Time (s), CO2 (PPM), Change (PPM)"); // Column headers for Serial Plotter
}

void loop() {
    int co2now[10];               
    int co2raw = 0;               
    int co2comp = 0;              
    int co2ppm = 0;               
    int zzz = 0;                  

    for (int x = 0; x < 10; x++) { 
        co2now[x] = analogRead(anInput);
        delay(200);
    }

    for (int x = 0; x < 10; x++) { 
        zzz += co2now[x];
    }

    co2raw = zzz / 10;                            
    co2comp = co2raw - co2Zero;                   
    co2ppm = map(co2comp, 0, 1023, 400, 5000);    

    int changeInCo2 = co2ppm - lastCo2ppm; // Calculate increase/decrease
    lastCo2ppm = co2ppm; // Update previous reading

    // Log to Serial Plotter
    Serial.print(millis() / 1000); // Time in seconds
    Serial.print(",");
    Serial.print(co2ppm); // CO2 ppm reading
    Serial.print(",");
    Serial.println(changeInCo2); // Change in CO2 ppm

    // Display CO2 ppm and status
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CO2: ");
    lcd.print(co2ppm);
    lcd.print("ppm");

    // Show change in ppm with "PPM" label
    lcd.setCursor(0, 1);
    lcd.print(changeInCo2);  // Display the difference
    lcd.print(" PPM");

    // Set Good or Bad label based on CO2 level
    if (co2ppm <= 1000) {
        lcd.print(" ACCEPTABLE");
    } else {
        lcd.print(" WARNING");
    }

    // Set LEDs and Buzzer based on CO2 levels
    if (co2ppm > 1000) {  
        digitalWrite(redLed, HIGH);
        digitalWrite(greenLed, LOW);
        digitalWrite(buzzer, HIGH);
    } else {
        digitalWrite(redLed, LOW);
        digitalWrite(greenLed, HIGH);
        digitalWrite(buzzer, LOW);
    }
    
    delay(2000); // Update every 2 seconds
}