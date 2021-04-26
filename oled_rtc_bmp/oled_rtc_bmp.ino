#include <OLED_I2C.h>
#include <Ticker.h>

#include "display.h"
#include "buttons.h"
#include "buzzer.h"

void updateDisplay();

Ticker tickerDisplay(requestUpdateScreen, 1000);
Ticker tickerChangeDisplayType(change_display_type, 20000);

OLED  oled(SDA, SCL); // Remember to add the RESET pin if your display requires it...

BUTTONS buttons(8); // one button connected
BUZZER buzzer(9); // pin 9 to beeper

volatile bool need_update = true;

#include "datasource.h"

DataSource ds;

void setup()
{
  oled.begin(SSD1306_128X64);
  ds.begin();
  buzzer.begin();
  buttons.begin();

  tickerDisplay.start();
  tickerChangeDisplayType.start();
}

void loop()
{
  tickerDisplay.update();
  tickerChangeDisplayType.update();
  updateDisplay();
  buttons.update();
  bool pressed = buttons.wasTriggered();
  if (pressed)
  {
    buzzer.click();
    change_screen_mode();
    //    change_display_type();
    need_update = true;
  }
  if (Serial.available() > 0)
    serialMode();

}

void requestUpdateScreen()
{
  need_update = true;
}

void updateDisplay()
{
  if (need_update)
  {
    ds.update();
    screen_update(&oled, &ds);
    oled.update();
    need_update = false;
  }
}
