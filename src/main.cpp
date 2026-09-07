#include <Arduino.h>

#include <FastLED.h>
#include <SPI.h>
#include <Wire.h>
#include <TFT_eSPI.h>

/**
 * @brief Hardware configuration for the RGB LED, joystick, LCD, and touchscreen.
 */

/** @brief RGB LED configuration. */
#define NUM_LEDS 1
#define DATA_PIN 48

/** @brief Joystick configuration. */
#define JOYSTICK_X_PIN 2
#define JOYSTICK_Y_PIN 1
#define JOYSTICK_BUTTON_PIN 3
#define JOYSTICK_MAX_VALUE 4095
#define JOYSTICK_MIN_VALUE 0

/** @brief LCD configuration. */
#define LCD_CS_PIN 14
#define LCD_RST_PIN 13
#define LCD_DC_PIN 12
#define LCD_SPI_MOSI_PIN 11
#define LCD_SPI_SCK_PIN 10
#define LCD_SPI_MISO_PIN 9
#define LCD_LED_PIN 46

/** @brief LCD dimensions in pixels. */
#define LCD_WIDTH 320
#define LCD_HEIGHT 480

/** @brief FT6336 touchscreen configuration. */
#define TOUCH_IRQ_PIN 17
#define TOUCH_I2C_SDA_PIN 18
#define TOUCH_I2C_SCL_PIN 3
#define TOUCH_I2C_RST_PIN 8
#define TOUCH_I2C_ADDRESS 0x38
#define FT6336_REG_TD_STATUS 0x02
#define FT6336_REG_P1_XH 0x03


TFT_eSPI tft = TFT_eSPI(LCD_WIDTH, LCD_HEIGHT);


CRGB leds[NUM_LEDS];


/**
 * @brief Configure the joystick input pins.
 */
void initJoystick() {
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);
}

/**
 * @brief Read the joystick axes and button state.
 *
 * @param[out] xValue X-axis ADC reading.
 * @param[out] yValue Y-axis ADC reading.
 * @param[out] buttonPressed True when the joystick button is pressed.
 */
void readJoystick(int &xValue, int &yValue, bool &buttonPressed) {
  xValue = analogRead(JOYSTICK_X_PIN);
  yValue = analogRead(JOYSTICK_Y_PIN);
  buttonPressed = (digitalRead(JOYSTICK_BUTTON_PIN) == LOW);
}


/**
 * @brief Initialize the LCD and turn on its backlight.
 */
void initLCD() {
  pinMode(LCD_CS_PIN, OUTPUT);
  digitalWrite(LCD_CS_PIN, LOW);
  tft.init();
  tft.setRotation(0);
  pinMode(LCD_LED_PIN, OUTPUT);
  digitalWrite(LCD_LED_PIN, HIGH);
}

/**
 * @brief Reset the FT6336 and initialize its I2C bus.
 */
void initTouch() {
  pinMode(TOUCH_I2C_RST_PIN, OUTPUT);
  digitalWrite(TOUCH_I2C_RST_PIN, LOW);
  delay(5);
  digitalWrite(TOUCH_I2C_RST_PIN, HIGH);
  delay(50);

  Wire.begin(TOUCH_I2C_SDA_PIN, TOUCH_I2C_SCL_PIN);
}

/**
 * @brief Read the first FT6336 touch point.
 *
 * @param[out] x Touch X coordinate, or zero when no touch is detected.
 * @param[out] y Touch Y coordinate, or zero when no touch is detected.
 * @param[out] touched True when a valid touch point was read.
 */
void readTouch(uint16_t &x, uint16_t &y, bool &touched) {
  // Start each read with a known no-touch result. This also prevents stale
  // coordinates from being displayed after a finger leaves the screen.
  touched = false;
  x = 0;
  y = 0;

  // First read the number of active touch points from the FT6336 status
  // register. The repeated-start transaction keeps the register selection
  // and the following read together on the I2C bus.
  Wire.beginTransmission(TOUCH_I2C_ADDRESS);
  Wire.write(FT6336_REG_TD_STATUS);
  if (Wire.endTransmission(false) != 0 || Wire.requestFrom(TOUCH_I2C_ADDRESS, 1) != 1) {
    return;
  }

  uint8_t touchCount = Wire.read() & 0x0F;
  // The controller supports up to two points. Treat an invalid count as no
  // touch rather than attempting to parse an invalid coordinate packet.
  if (touchCount == 0 || touchCount > 2) {
    return;
  }

  // A valid point starts at the first point's X-high register. Four bytes
  // contain X-high, X-low, Y-high, and Y-low in that order.
  Wire.beginTransmission(TOUCH_I2C_ADDRESS);
  Wire.write(FT6336_REG_P1_XH);
  if (Wire.endTransmission(false) != 0 || Wire.requestFrom(TOUCH_I2C_ADDRESS, 4) != 4) {
    return;
  }

  // The upper nibble contains controller flags, so retain only the lower
  // twelve coordinate bits before combining each high and low byte.
  uint8_t xHigh = Wire.read();
  uint8_t xLow = Wire.read();
  uint8_t yHigh = Wire.read();
  uint8_t yLow = Wire.read();
  x = ((xHigh & 0x0F) << 8) | xLow;
  y = ((yHigh & 0x0F) << 8) | yLow;
  touched = true;
}
/**
 * @brief Clear and draw one dynamic value on the LCD.
 *
 * @param[in] text Text to draw.
 * @param[in] y Top coordinate of the value row.
 */
void drawValue(const char *text, int y) {
  tft.fillRect(165, y, 145, 20, TFT_BLACK);
  tft.drawString(text, 170, y);
}





/**
 * @brief Initialize the serial port and hardware peripherals.
 */
void setup() {
  Serial.begin(115200);
  while(!Serial) {
    ;
  }

  initLCD();
  initJoystick();
  initTouch();

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  leds[0] = CRGB::Red;
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("Joystick X:", 20, 20);
  tft.drawString("Joystick Y:", 20, 45);
  tft.drawString("Button:", 20, 70);
  tft.drawString("Touch X:", 20, 100);
  tft.drawString("Touch Y:", 20, 125);
  tft.drawString("Touch State:", 20, 150);
  tft.fillRect(200, 200, 40, 40, TFT_BLUE);
  
}

/**
 * @brief Read the inputs and refresh their values on the LCD.
 */
void loop() {
    int xValue;
    int yValue;
    uint16_t touchX, touchY;
    bool buttonState;

    // Keep serial logging independent from the display refresh rate. Native
    // USB output can block long enough to make the LCD appear less responsive.
    static unsigned long lastSerialTime = 0;

    // Sample the joystick before updating its three display fields.
    readJoystick(xValue, yValue, buttonState);

    bool touchState;
    // Read the FT6336 after the joystick. When no touch is present, readTouch
    // returns zero coordinates and sets touchState to false.
    readTouch(touchX, touchY, touchState);

    // Reuse one text buffer for numeric values to avoid repeated allocations.
    char value[32];
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    // Each drawValue call clears the old value area before drawing the new
    // text, so shorter values do not leave stale digits on the LCD.
    snprintf(value, sizeof(value), "%d", xValue);
    drawValue(value, 20);
    snprintf(value, sizeof(value), "%d", yValue);
    drawValue(value, 45);
    drawValue(buttonState ? "Pressed" : "Released", 70);
    snprintf(value, sizeof(value), "%u", touchX);
    drawValue(value, 100);
    snprintf(value, sizeof(value), "%u", touchY);
    drawValue(value, 125);
    drawValue(touchState ? "Touched" : "Not Touched", 150);

    // Report the current state once per second rather than on every display
    // refresh, which keeps serial traffic from competing with screen updates.
    if (millis() - lastSerialTime >= 1000) {
      lastSerialTime = millis();
      Serial.printf("Touch X: %u, Touch Y: %u, Touch: %s\n", touchX, touchY,
                    touchState ? "Touched" : "Not Touched");
      Serial.printf("Joystick X: %d, Y: %d, Button: %s\n", xValue, yValue,
                    buttonState ? "Pressed" : "Released");
    }

    // Limit the display refresh rate to ten updates per second.
    delay(100);
 
}