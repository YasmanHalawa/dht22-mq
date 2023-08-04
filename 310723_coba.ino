#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "GP2Y1010AU0F.h"

#define DHTPIN 27    // DHT22 data pin
#define DHTTYPE DHT22
#define sensorO2pin 32
#define sensorCOpin 35
#define dustSensorPin 34
const int analogCO2Pin = 33;

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  pinMode(sensorO2pin, INPUT);
  pinMode(sensorCOpin, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Penelitian");
  lcd.setCursor(1,1);
  lcd.print("Kualitas Udara");
  Wire.begin();
  dht.begin();
}

void loop() {
  // Read CO2 sensor data
  int sensorValue = analogRead(analogCO2Pin);
  float voltage = sensorValue * 5.0 / 4095.0;
  float co2Concentration = map(sensorValue, 0, 4095, 0, 5000); // Assuming 0-5000 ppm range, adjust accordingly

  Serial.print("CO2: ");
  Serial.print(co2Concentration);
  Serial.println("ppm");

  float sensorO2 = analogRead(sensorO2pin);
  float sensorO2_value = sensorO2 * 5.0 / 1023.0;
  Serial.print("SensorO2 Analog: ");
  Serial.println(sensorO2);
  Serial.print("SensorO2 : ");
  Serial.println(sensorO2_value);

  float sensorCO = analogRead(sensorCOpin);
  int CO = ((sensorCO * 18623) + 222.67);
  float sensorCO_value = sensorCO * 5.0 / 1023.0;
  Serial.print("SensorCO Analog: ");
  Serial.println(sensorCO);
  Serial.print("SensorCO : ");
  Serial.println(sensorCO_value);

  int rawValue = analogRead(dustSensorPin);
  // Convert the raw analog value to voltage (0 to 3.3V)
  float voltage_dust = (float)rawValue * 5.0 / 1024.0;
  float dustConcentration = 0.17 * voltage_dust - 0.1;

  Serial.print("Debu: ");
  Serial.print(dustConcentration);
  Serial.println("ug/m3");
  //0-50 Baik //51-150 Sedang //151-350 Tidak Sehat //351-420 Sangat Tidak Sehat //> 420 Berbahaya

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Gagal membaca data dari sensor DHT22!");
    return;
  }

  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.println("*C \t");

  Serial.print("Kelembaban: ");
  Serial.print(humidity);
  Serial.println("%");

  lcd.clear();  
  lcd.print("Suhu: ");
  lcd.print(temperature);
  lcd.write(B11011111);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Kelembaban:");
  lcd.print(humidity);
  lcd.print("%");

  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Debu:");
  lcd.print(dustConcentration);
  lcd.print(" ug/m3");

  lcd.setCursor(0, 1);
  lcd.print("CO2 : ");
  lcd.print(co2Concentration);
  lcd.print(" ppm");

  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("O2: ");
  lcd.print(sensorO2_value);
  lcd.print(" ppm");

  lcd.setCursor(0, 1);
  lcd.print("CO: ");
  lcd.print(sensorCO_value);
  lcd.print(" ppm");

  delay(5000);
}
