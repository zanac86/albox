#include "OLED_I2C.h"

#include "display.h"
#include "buttons.h"
#include "buzzer.h"
#include "everytime.h"

void updateDisplay();

OLED  oled(SDA, SCL); // Remember to add the RESET pin if your display requires it...

BUTTONS buttons(8); // one button connected
BUZZER buzzer(9); // pin 9 to beeper

#include "datasource.h"

DataSource ds;

void setup()
{
    Serial.begin(115200);  // Initialize serial port for debugging.
    delay(200);            // Soft startup to ease the flow of electrons.

    oled.begin(SSD1306_128X64);
    oled.setBrightness(1);
    ds.begin();
    buzzer.begin();
    buttons.begin();
    ds.update(UPDATE_PRESSURE | UPDATE_RTC | UPDATE_PRESSURE_HISTORY);
    screen_update(&oled, &ds);

}

void loop()
{
    EVERY_N_MINUTES(15)
    {
        ds.update(UPDATE_PRESSURE_HISTORY);
    }

    EVERY_N_SECONDS(1)
    {
        ds.update(UPDATE_PRESSURE | UPDATE_RTC);
        screen_update(&oled, &ds);
    }

    EVERY_N_SECONDS(30)
    {
        change_display_type();
    }

    buttons.update();
    bool pressed = buttons.wasTriggered();
    if (pressed)
    {
        buzzer.click();
        change_display_type();
        screen_update(&oled, &ds);
    }

    if (Serial.available() > 0)
    {
        serialMode();
    }
}
