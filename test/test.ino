#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial    
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <DHT.h>
#include <SimpleTimer.h>
#include <Adafruit_BMP085.h>
#define DHTPIN  14
#define DHTPIN1 2
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
DHT dht1(DHTPIN1, DHTTYPE);
SimpleTimer timer;
Adafruit_BMP085 bmp;


char auth[] = "";
char ssid[] = "";
char pass[] = "";
WidgetLED snow(V5);
WidgetLED low(V6);
WidgetLED ice(V7);
WidgetLED rain(V8);


void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  dht1.begin();
  timer.setInterval(60000, readtemp);
  lcd.begin();
  lcd.backlight();
  bmp.begin();

}

void readtemp()
{

  int h = dht.readHumidity();//outside
  Blynk.virtualWrite(V1, h);

  int t = dht.readTemperature();//outside
  Blynk.virtualWrite(V2, t);

  int h1 = dht1.readHumidity();
  Blynk.virtualWrite(V3, h1);

  int t1 = dht1.readTemperature();
  Blynk.virtualWrite(V4, t1);
  float  prs = bmp.readPressure() / 133.33;
  Blynk.virtualWrite(V9, prs);
  
  //LCD part
  lcd.setCursor(0, 0);
  lcd.print("IN ");
  lcd.print(t, 1);
  lcd.setCursor(5, 0);
  lcd.print("C");
  lcd.setCursor(6, 0);
  lcd.print("#");
  lcd.setCursor(7, 0);
  lcd.print(h, 1);
  lcd.setCursor (9, 0);
  lcd.print("%");
  lcd.setCursor (10, 0);
  lcd.print("#");
  lcd.setCursor (11, 0);
  lcd.print(prs);
  lcd.setCursor (15, 0);
  lcd.print("H");
  //down row
  lcd.setCursor(0, 1);
  lcd.print("OUT ");
  lcd.print(t1);
  lcd.setCursor(7, 1);
  lcd.print("C");
  lcd.setCursor (8, 1);
  lcd.print("#");
  lcd.setCursor (9, 1);
  lcd.print(h1);
  lcd.setCursor (11, 1);
  lcd.print("%");



  if (h1 > 70  && t1 < -2 ) {
    snow.on();
    ice.off();
    rain.off();
     lcd.setCursor (12, 1);
     lcd.print("SNOW");
  }
  else if (h1 < 70 && t1 < -10) {
    ice.on();
    snow.off();
    rain.off();
    lcd.setCursor (12, 1);
     lcd.print("ICE");

  }

  else if (h1 > 70 && t1 > 5)
  { rain.on();
    ice.off();
    snow.off();
    lcd.setCursor (12, 1);
     lcd.print("RAIN");
  }
  else
  {
    rain.off();
    ice.off();
    snow.off();
    lcd.setCursor (12, 1);
     lcd.print("SUN");
  }

}

BLYNK_WRITE(V10)
{
  readtemp();
}
void loop()
{
  Blynk.run();
  timer.run(); // SimpleTimer is working


}

