// Display – https://pmdway.com/collections/oled-displays/products/0-49-64-x-32-white-graphic-oled-i2c
// Guide – https://pmdway.com/blogs/product-guides-for-arduino/tutorial-using-the-0-49-64-x-32-graphic-i2c-oled-display-with-arduino

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_64X32_1F_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

int uvPin = A0;

void setup() {
//  Serial.begin(9600);
  u8g2.begin();
}

// fonts https://github.com/olikraus/u8g2/wiki/fntlistall#4-pixel-height

void loop() {
  float uvValue = analogRead(uvPin);
//  Serial.println(uvValue);
  uvValue *= 5; // Uno reference voltage
  uvValue /= 1023.0; // map to voltage range
  uvValue *= 10.0; // calculate UVI based on first stage of LM358 opamp
  uvValue /= 6.1; // negate 2nd stage of LM358 opamp

  drawScreen(uvValue, getUVRatingText(uvValue));
  delay(100);

//  drawScreen(1.32, "Low");
//  delay(1000);
//
//  drawScreen(4.20, "Mod.");
//  delay(1000);
//
//  drawScreen(7.6912, "High");
//  delay(1000);
//
//  drawScreen(9.00, "V.High");
//  delay(1000);
//
//  drawScreen(11.32, "Xtreme");
//  delay(1000);
}

void drawScreen(float value, String text) {
  u8g2.clearBuffer();
  drawVal(value);
  u8g2.setFont(u8g2_font_t0_15_tr);
  u8g2.drawStr(0, 28, text.c_str());
  drawBatt();
  u8g2.sendBuffer();
}

void drawVal(float value) {
  u8g2.setFont(u8g2_font_t0_22_tf);
  u8g2.setCursor(0, 14);
  u8g2.print(value);
}

void drawBatt() {
  u8g2.drawBox(58, 1, 4, 2);
  u8g2.drawBox(56, 3, 8, 28);
}

String getUVRatingText(float uvi) {
  if (uvi < 3) {
    return "Low";
  } else if (uvi < 6) {
    return "Mod";
  } else if (uvi < 8) {
    return "High";
  } else if (uvi < 11) {
    return "V. High";
  }
  return "Xtreme";
}
