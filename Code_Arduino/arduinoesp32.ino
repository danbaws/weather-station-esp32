#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>
#include <SFE_BMP180.h>
#include <BlynkSimpleEsp32.h>
#include <WiFiClient.h>

#define LDR 4
#define TH 5
#define Rain 36

double T, P;
char status;

LiquidCrystal_I2C lcd(0x27, 16, 2);

SFE_BMP180 bmp;

DHT dht(TH, DHT11);

void setup() {
  Serial.begin(115200);
  bmp.begin();
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(LDR, INPUT);
  pinMode(Rain, INPUT);
  analogReadResolution(12);

  lcd.setCursor(0, 0);
  lcd.print("System");
  lcd.setCursor(4, 1);
  lcd.print("Loading..");
  delay(4000);
  lcd.clear();

}

void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t);

  lcd.setCursor(8, 0);
  lcd.print("H:");
  lcd.print(h);

}

void rainSensor() {
  int Rvalue = analogRead(Rain);
  Rvalue = map(Rvalue, 0, 4095, 0, 100);
  Rvalue = (Rvalue - 100) * -1;
  Rvalue = Rvalue-53;
  lcd.setCursor(0, 1);
  lcd.print("R:");
  lcd.print(Rvalue);
  lcd.print(" ");
  Serial.println(Rvalue);
}

void pressure() {
  status =  bmp.startTemperature();
  if (status != 0) {
    delay(status);
    status = bmp.getTemperature(T);

    status = bmp.startPressure(3);
    if (status != 0) {
      delay(status);
      status = bmp.getPressure(P, T);
      if (status != 0) {

      }
    }
  }

  lcd.setCursor(8, 1);
  lcd.print("P:");
  lcd.print(P);
}

void LDRsensor() {
  bool value = digitalRead(LDR);
  if (value == 1) {
    WidgetLED LED(V4);
    LED.on();
  } else {
    WidgetLED LED(V4);
    LED.off();
  }

}

void loop() {
  DHT11sensor();
  rainSensor();
  pressure();
  LDRsensor();
}