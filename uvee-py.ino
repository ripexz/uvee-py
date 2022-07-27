#include <Adafruit_NeoPixel.h>
#include <Adafruit_LTR390.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datashebet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define UV_SENSITIVITY 2300
#define WINDOW_FACTOR 1

Adafruit_LTR390 ltr = Adafruit_LTR390();

// additional data for UV sensor
float gain_factor[5] = {1, 3, 6, 9, 18};
float res_factor[6] = {4, 2, 1, 0.5, 0.25, 0.03125};

// set up built-in neopixel on PIN_NEOPIXEL
Adafruit_NeoPixel pixels(1, PIN_NEOPIXEL);

void setup() {
  Serial.begin(115200);

  delay(200); // delay before display init

  pixels.begin();
  pixels.setBrightness(25);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1) showPixelErrorLoop();
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000);
  display.clearDisplay();

  if (!ltr.begin()) {
    showText("Couldn't find LTR sensor!", 1);
    while (1) showPixelErrorLoop();
  }
  Serial.println("Found LTR sensor!");
  showText("Found LTR sensor!", 1);

  ltr.setMode(LTR390_MODE_UVS);
  ltr.setGain(LTR390_GAIN_18);
  ltr.setResolution(LTR390_RESOLUTION_20BIT);
  ltr.setThresholds(100, 1000);
  ltr.configInterrupt(true, LTR390_MODE_UVS);
}

void loop() {
  if (ltr.newDataAvailable()) {
    uint32_t raw = ltr.readUVS();
    uint8_t _gain = (uint8_t)ltr.getGain();
    uint8_t _resolution = (uint8_t)(ltr.getResolution());

    Serial.print("UV data: ");
    Serial.println(raw);

    float uvi = (float)(raw) / ((gain_factor[_gain] / gain_factor[LTR390_GAIN_18]) * (res_factor[_resolution] / res_factor[LTR390_RESOLUTION_20BIT]) * (float)(UV_SENSITIVITY)) * (float)(WINDOW_FACTOR);

    String text = "UVI: ";
    text += uvi;
    text += "\n" + getUVRatingText(uvi);
    showText(text, 2);
  }

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

void showText(String text, int textSize) {
  display.clearDisplay();
  display.setTextSize(textSize);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.cp437(true);

  display.println(text);

  display.display();
  delay(100);
}

void showPixelErrorLoop() {
  showPixelColor(255, 0, 0);
  delay(200);
  showPixelColor(255, 0, 255);
  delay(100);
  pixels.clear();
  pixels.show();
  delay(500);
}

void showPixelColor(int r, int g, int b) {
  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.show();
  delay(1000);
}
