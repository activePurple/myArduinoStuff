#include <dht.h>
#include <LiquidCrystal.h>
dht DHT;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // RS, E, D4, D5, D6, D7

// DHT11 Sensor Pin
#define DHT11_PIN 7

// Potentiometer Pin
#define POTPIN 0

// DC Motor Pins
int MOTORPIN = 10;

// Fanstate Boolean Value for Temp check
bool fanState = false;


// Temp check function to operate on fanState
bool checkTemp(int setPoint, float Diff) {
  // Set Point Potentiometer mapping and reading
  int setpoint = analogRead(POTPIN);
  setpoint = map(setpoint, 0, 1023, 78, 85);
  float deadband = 1;
  float diff = setpoint - deadband;

  // Get Temp
  // Check sensor then convert to F Degrees
  int chk = DHT.read11(DHT11_PIN);
  float t_d_C = DHT.temperature;
  float t_d_F = (DHT.temperature * 1.8) + 32;
  float humidity = DHT.humidity;

  if (t_d_F < diff) {
    return fanState = false;
  } 
  else if (t_d_F > diff) {
    return fanState = true;
  }
  return fanState;
}

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


// Code Set-Up
void setup(){
  pinMode(MOTORPIN, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
}

// Loop Start
void loop(){

  // Check sensor then convert to F Degrees
  int chk = DHT.read11(DHT11_PIN);
  float t_d_C = DHT.temperature;
  float t_d_F = (DHT.temperature * 1.8) + 32;
  float humidity = DHT.humidity;

  // Start LCD Screen position
  lcd.setCursor(0,0); 

  // Print Temp in F to LCD Serial screen
  lcd.print("Temp: ");
  lcd.print(t_d_F);
  // Degree character
  lcd.print((char)223);
  lcd.print("F");

  // Set new LCF Position
  lcd.setCursor(0,1);

  // Print Humitidity RH; Density of Water Vapor / Density of water vapor at saturation
  // 100% is condensation levels
  // 0% is Completely Dry
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");

  // Clear screen after 2 second delay
  delay(2000);
  lcd.clear();

  // Set Point Potentiometer mapping and reading
  int setpoint = analogRead(POTPIN);
  setpoint = map(setpoint, 0, 1023, 70, 80);
  float deadband = 2;
  float diff = setpoint - deadband;

  // Set new LCF Position
  lcd.setCursor(0,0);

  // Print Setpoint reading from Potentiometer
  lcd.print("Setpoint: ");
  lcd.print(setpoint);

  // compare current temp to set point
  // Reading too hot if above and off if below
  lcd.setCursor(0,1);
  if (t_d_F > diff) {
  lcd.print("Too Hot! Fan On!");
  } 
  else if (t_d_F < diff) {
    lcd.print("Off");
  }

  // Clear screen after 2 second delay
  delay(2000);
  lcd.clear();

  // Getting Sat Pressure
  float s_P = saturationPressure(t_d_C);
  // Getting Actual Vapor Pressure Value
  float v_P = actualVaporPressure(t_d_F, humidity);
  
  // Set new Cursor Position
  lcd.setCursor(0, 0);

  // Measured in inch of Mercury
  lcd.print("Sat Pr: ");
  lcd.print(s_P);
  lcd.print("mb");
  
  // Set new Cursor Position
  lcd.setCursor(0,1);
  
  // Measured in inch of Mercury
  lcd.print("Act Pr: ");
  lcd.print(v_P);
  lcd.print("mb");
  delay(2000);
  lcd.clear();
  
  // Dew Point in C
  float dp = dewPointApproximation(t_d_F, humidity);
  lcd.print("Apprx DP: ");
  lcd.setCursor(0,1);
  lcd.print(dp);
  lcd.print(char(223));
  lcd.print("F");

  // Clear screen after 2 second delay
  delay(2000);
  lcd.clear();


  if (checkTemp(setpoint, diff) == false) {
    analogWrite(MOTORPIN, 0);
  } else if (checkTemp(setpoint, diff) == true) {
    analogWrite(MOTORPIN, 255);
   }

}
