// include the library code:
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <Wire.h> 
#include <Format.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

DeviceAddress sensorAddress;

#define REDLITE 3
#define GREENLITE 5
#define BLUELITE 6

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
 
// you can change the overall brightness by range 0 -> 255
int brightness = 255;
int last_time = 0;

byte symbol = 0;

byte degree[8] = {
B01100,
B10010,
B10010,
B01100,
B00000,
B00000,
B00000,
B00000
};

void setup() {
  lcd.createChar(symbol, degree);
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  
  sensors.begin();
  sensors.getAddress(sensorAddress, 0);
  sensors.setResolution(sensorAddress, 12);
  brightness = 100;
  printTemperatures();
}

void printTemperatures()
{
  sensors.requestTemperatures();
  float ftemp_c = sensors.getTempC(sensorAddress);
  float ftemp_f = DallasTemperature::toFahrenheit(ftemp_c);
  String stemp_c;
  String stemp_f;
  int width;
  stemp_c = formatFloat(ftemp_c, 1, &width);
  stemp_f = formatFloat(ftemp_f, 1, &width);
  
  lcd.clear();
  lcd.print("DS18B20 Temp");
  lcd.setCursor(0, 1);
  lcd.print(stemp_c);
  lcd.write(symbol);
  lcd.print("C ");
  lcd.print(stemp_f);
  lcd.write(symbol);
  lcd.print("F");
}

void loop() {
  int c_time = millis();
  if ((c_time - last_time) >= 10000)
  {
    printTemperatures();
    last_time = c_time;
  }
  
  for (int i = 0; i < 255; i++) {
    setBacklight(i, 0, 255-i);
    delay(5);
  }
  for (int i = 0; i < 255; i++) {
    setBacklight(255-i, i, 0);
    delay(5);
  }
  for (int i = 0; i < 255; i++) {
    setBacklight(0, 255-i, i);
    delay(5);
  }
}
 
 
 
void setBacklight(uint8_t r, uint8_t g, uint8_t b) {
  // normalize the red LED - its brighter than the rest!
  r = map(r, 0, 255, 0, 100);
  g = map(g, 0, 255, 0, 150);
 
  r = map(r, 0, 255, 0, brightness);
  g = map(g, 0, 255, 0, brightness);
  b = map(b, 0, 255, 0, brightness);
 
  // common anode so invert!
  r = map(r, 0, 255, 255, 0);
  g = map(g, 0, 255, 255, 0);
  b = map(b, 0, 255, 255, 0);
  Serial.print("R = "); Serial.print(r, DEC);
  Serial.print(" G = "); Serial.print(g, DEC);
  Serial.print(" B = "); Serial.println(b, DEC);
  analogWrite(REDLITE, r);
  analogWrite(GREENLITE, g);
  analogWrite(BLUELITE, b);
}
