#include <TinyWireM.h>
// comment out the below line if using blue OLED screen
// see https://github.com/datacute/Tiny4kOLED
#define TINY4KOLED_QUICK_BEGIN
#include <Tiny4kOLED.h>

const float referenceVoltage = 2.56;
const float voltageRange = 1024.0;
const float opAmpMultiplier = 10.0; // if your sensor has LM358 op-amp, set this to (10.0 / 6.1)

const int uvPin = 3;

void setup() {
  pinMode(uvPin, INPUT);

  oled.begin(64, 32, sizeof(tiny4koled_init_64x32), tiny4koled_init_64x32);
  oled.clear();
  oled.on();
  oled.switchRenderFrame();
}

void loop() {
  float uvValue = analogRead(uvPin);
  float uvIndex = uvValue * referenceVoltage / voltageRange * opAmpMultiplier;

  updateScreen(uvIndex, getUVRatingText(uvIndex));

  delay(100);
}

String getUVRatingText(float uvi) {
  if (uvi < 3) {
    return "Low";
  } else if (uvi < 6) {
    return "Moderate";
  } else if (uvi < 8) {
    return "High";
  } else if (uvi < 11) {
    return "Very High";
  }
  return "Extreme";
}

void updateScreen(float uvIndex, String uvRating) {
  oled.clear();
  oled.setFont(FONT8X16);
  oled.setCursor(0, 0);
  oled.print(uvIndex);
  oled.setCursor(0, 2);
  oled.print(uvRating);
  oled.switchFrame();
}
