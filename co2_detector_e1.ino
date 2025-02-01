 
#include <LiquidCrystal.h>
#define sensor A0
#define buzz 4
#define red 6
#define green 5

int gas, co2lvl;

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

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

  //pinMode(pin, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(sensor, INPUT);
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setCursor (0, 0);
  lcd.print("                ");
  lcd.setCursor (0, 1);
  lcd.print("                ");
  lcd.setCursor (0, 0);
  lcd.print("  CO2 Meter   ");
  lcd.setCursor (0, 0);
  delay(1000);
  lcd.clear();
  lcd.print("Warming coil");

  for (int i = 0; i <= 5; i++)
  {
    lcd.setCursor(12, 0);
    if (i < 10) lcd.print(" ");
    lcd.print(i);
    lcd.print("%");
    delay(700);
  }
  lcd.createChar(0, smiley);
  lcd.createChar(1, sad);
}


void loop() {

  gas = analogRead(sensor);
  co2lvl = gas - 120;
  co2lvl = map(co2lvl, 0, 1024, 400, 5000);
  Serial.println(gas);
  lcd.setCursor (0, 0);
  lcd.print("CO2 level is:");
  lcd.setCursor(0, 1);
  lcd.print(co2lvl);
  lcd.print(" ppm");

  if ((co2lvl >= 350) && (co2lvl <= 1400))
  {
    lcd.print("  Good ");
    lcd.write(byte(0));
    digitalWrite(buzz, LOW);
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
  }

  else if ((co2lvl >= 1400) && (co2lvl <= 2000))
  {
    digitalWrite(buzz, HIGH);
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    lcd.print("  Bad ");
    lcd.write(byte(1));

  }
  else
  {
    lcd.print(" Danger!");
    digitalWrite(buzz, HIGH);
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
  }
  delay(300);
  lcd.clear();
}

