#include <dht.h>
#include <LiquidCrystal.h>
dht DHT;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // RS, E, D4, D5, D6, D7
// DHT11 Sensor Pin
#define DHT11_PIN 7
// Potentiometer Pin
#define POTPIN 0
// Button pin
int BUTTONPIN = 8;
// Button State
int buttonState = 0;

// Functions for calculating values
float saturationPressure(float t_d_c) {
  // Taking celsius temp and getting Saturation Pressure
  return 6.112 * exp((17.67 * t_d_c) / (t_d_c + 243.5));
}

float actualVaporPressure(float p_ws, float rh) {
  return p_ws * (rh/100);
}

float dewPointApproximation(float t_d_f, float rh) {
  return t_d_f - ((100 - rh) / 5.0);
}

// functions for taking Temps
void checkTempF(float TDF) {
  
  lcd.setCursor(0,0);
  lcd.print(char(223));
  lcd.print("F");
  lcd.print(":");
  lcd.print(TDF);
}

void checkTempC(float TDC) {
  
  lcd.setCursor(0,1);
  lcd.print(char(223));
  lcd.print("C");
  lcd.print(":");
  lcd.print(TDC);
}

void checkHum(float HUM) {

  lcd.setCursor(0,0);
  lcd.print("Humidity:");
  lcd.print(HUM);
  lcd.print("%");
}

void checkSetPoint() {
  int setpoint = analogRead(POTPIN);
  setpoint = map(setpoint, 0, 1023, 70, 80);
  float deadband = 2;
  float diff = setpoint - deadband;

  lcd.setCursor(0, 1);
  lcd.print("Set Point:");
  lcd.print(diff);

}

void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTONPIN, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {
  // Check sensor then convert to F Degrees
  int chk = DHT.read11(DHT11_PIN);
  float TDC = DHT.temperature;
  float TDF = (DHT.temperature * 1.8) + 32;
  float HUM = DHT.humidity;

  // Button State Read
  buttonState = digitalRead(BUTTONPIN);

  if (buttonState == LOW) {
    lcd.clear();
    checkTempF(TDF);
    checkTempC(TDC);
  } else if (buttonState == HIGH) {
    lcd.clear();
    checkHum(HUM);
    checkSetPoint();
  }
  delay(1000);

}
