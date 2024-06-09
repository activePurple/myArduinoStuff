#include <dht.h>
#include <LiquidCrystal.h>
dht DHT;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // RS, E, D4, D5, D6, D7

#define DHT11_PIN 7

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

// Calc humidity ratio and enthalpy using the dry-bulb temp. actual pressure and atmosphereic pressure
void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
}

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
  delay(2000);
  // clear lcd for next display
  lcd.clear();

  // Getting Sat Pressure
  float s_P = saturationPressure(t_d_C);

  // Getting Actual Vapor Pressure Value
  float v_P = actualVaporPressure(t_d_F, humidity);
  
  // Set new position
  lcd.setCursor(0, 0);

  // Measured in inch of Mercury
  lcd.print("Sat Pr: ");
  lcd.print(s_P);
  lcd.print("mb");

  lcd.setCursor(0,1);
  
  // Measured in inch of Mercury
  lcd.print("Act Pr: ");
  lcd.print(v_P);
  lcd.print("mb");
  delay(5000);
  lcd.clear();
  
  // Dew Point in C
  float dp = dewPointApproximation(t_d_F, humidity);
  lcd.print("Apprx DP: ");
  lcd.setCursor(0,1);
  lcd.print(dp);
  lcd.print(char(223));
  lcd.print("F");

  delay(5000);
  lcd.clear();
}
