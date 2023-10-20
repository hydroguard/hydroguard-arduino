#include <Wire.h>
#include <SSD1306Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#include "boards.h"

SSD1306Wire display(0x3c, I2C_SDA, I2C_SCL);
Adafruit_BMP280 bmp;


void setup() {
  initBoard();

  display.init();
  display.flipScreenVertically();
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  
  unsigned status;
  if (!(status = bmp.begin(0x76))) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); 
    Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);
}

void loop() {
  display.clear();
  // drawCircleDemo();

  display.drawString(0, 10, "Temperature: " + String(bmp.readTemperature()) + " C");
  display.drawString(0, 20, "Pressure: " + String(bmp.readPressure()) + " Pa");
  display.drawString(0, 30, "Approx altitude: " + String(bmp.readAltitude(1013.25)) + " m");

  // write the buffer to the display
  display.display();

  delay(10);
}
