#include <OLED_I2C.h>

#include "display.h"
#include "buttons.h"
#include "buzzer.h"

void updateDisplay();

OLED  oled(SDA, SCL); // Remember to add the RESET pin if your display requires it...

BUTTONS buttons(8); // one button connected
BUZZER buzzer(9); // pin 9 to beeper

uint32_t timeUpdate = 0;
uint32_t timeChange = 0;

#include "datasource.h"

DataSource ds;

void setup()
{
  oled.begin(SSD1306_128X64);
  ds.begin();
  buzzer.begin();
  buttons.begin();
}

void loop()
{
  if ((millis() - timeUpdate) > 1000)
  {
    timeUpdate = millis();
    ds.update();
    screen_update(&oled, &ds);
    oled.update();
  }

  if ((millis() - timeChange) > 10000)
  {
    timeChange = millis();
    change_display_type();
  }

  buttons.update();
  bool pressed = buttons.wasTriggered();
  if (pressed)
  {
    buzzer.click();
    change_screen_mode();
    timeUpdate =0;
  }

  if (Serial.available() > 0)
    serialMode();
}
