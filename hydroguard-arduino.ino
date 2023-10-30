#include <Wire.h>
#include <SSD1306Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <ph4502c_sensor.h>

#include "loramac.h"
#include "boards.h"

#define OLED_ADDR 0x3c
#define BMP280_ADDR 0x76

#define PH4502C_TEMPERATURE_PIN 34
#define PH4502C_PH_PIN 35
#define PH4502C_PH_TRIGGER_PIN 14 
#define PH4502C_CALIBRATION 7.0f
#define PH4502C_READING_INTERVAL 100
#define PH4502C_READING_COUNT 10
#define ADC_RESOLUTION 4096.0f

#define TRS_LEFT_PIN 12
#define TRS_RIGHT_PIN 13

// Read data from TRS cable
// float read_trs_data(int pin) {
//   float data = 0.0f;
//   for (int i = 0; i < 10; i++) {
//     data += analogRead(pin);
//   }
//   return data / 10.0f;
// }

float read_trs_data(int pin) {
  return analogRead(pin);
}

SSD1306Wire display(OLED_ADDR, I2C_SDA, I2C_SCL);
Adafruit_BMP280 bmp;
PH4502C_Sensor ph4502c(
  PH4502C_PH_PIN,
  PH4502C_TEMPERATURE_PIN,
  PH4502C_CALIBRATION,
  PH4502C_READING_INTERVAL,
  PH4502C_READING_COUNT,
  ADC_RESOLUTION
);

void setup() {
  initBoard();

  Serial.begin(115200);
  display.init();
  display.flipScreenVertically();
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  
  bmp.begin(BMP280_ADDR);
  bmp.setSampling(
    Adafruit_BMP280::MODE_NORMAL,
    Adafruit_BMP280::SAMPLING_X2,
    Adafruit_BMP280::SAMPLING_X16,
    Adafruit_BMP280::FILTER_X16,
    Adafruit_BMP280::STANDBY_MS_500
  );

  ph4502c.init();
  delay(1500);
  setupLMIC();
}

void loop() {
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure();
  float altitude = bmp.readAltitude(1013.25);
  float ph = ph4502c.read_ph_level();
  float temperature_int = ph4502c.read_temp();

  float left_trs = read_trs_data(TRS_LEFT_PIN);
  float right_trs = read_trs_data(TRS_RIGHT_PIN);

  display.clear();

  display.drawString(0, 10, "Ext. Temperature: " + String(temperature) + " C");
  display.drawString(0, 20, "Ext. Pressure: " + String(pressure) + " hPa");
  display.drawString(0, 30, "Ext. Altitude: " + String(altitude) + " m");
  display.drawString(0, 40, "Internal pH: " + String(ph));
  // display.drawString(0, 50, "Internal Temperature: " + String(temperature_int) + " C");
  display.drawString(0, 50, "Left TRS: " + String(left_trs) + " | Right TRS: " + String(right_trs));
  Serial.println("Left TRS: " + String(left_trs) + " | Right TRS: " + String(right_trs));


  display.display();


  loopLMIC();
  delay(500);
}
