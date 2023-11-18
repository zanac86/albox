#include <Arduino.h>

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

void setupTimeFromSerial()
{
    // Wait $2211301830453$
    //       YYMMDDHHMMSSbrightness 1..7
    Serial.println("CMD waiting...");
    show_message(&oled, "Seting RTC...");
    bool cmdReady = false;
    unsigned long timeToBreak = millis() + 10000;
    char str[20];
    int cmdLen = 15;
    int i = 0;
    while (!cmdReady)
    {
        if (millis() > timeToBreak)
        {
            break;
        }
        if (Serial.available())
        {
            byte c = Serial.read();
            str[i] = c;
            i++;
            if (i == cmdLen)
            {
                Serial.println("CMD read Ok");
                cmdReady = true;
            }
        }
    }

    if (cmdReady && (str[0] == '$') && (str[cmdLen - 1] == '$'))
    {
        byte year = ((byte)str[1] - 48) * 10 + (byte)str[2] - 48;
        byte month = ((byte)str[3] - 48) * 10 + (byte)str[4] - 48;
        byte date = ((byte)str[5] - 48) * 10 + (byte)str[6] - 48;
        byte hour = ((byte)str[7] - 48) * 10 + (byte)str[8] - 48;
        byte minute = ((byte)str[9] - 48) * 10 + (byte)str[10] - 48;
        byte second = ((byte)str[11] - 48) * 10 + (byte)str[12] - 48;

        uint8_t bright = (byte)str[13] - 48;
        if (bright > 7)
        {
            bright = 7;
        }
        // EEPROM.update(EEPROM_ADDR_BRIGHTNESS, bright);

        DS3231 rtc;
        rtc.setClockMode(false);  // set to 24h
        rtc.setYear(year);
        rtc.setMonth(month);
        rtc.setDate(date);
        rtc.setHour(hour);
        rtc.setMinute(minute);
        rtc.setSecond(second);
        rtc.enableOscillator(true, false, 0);  // enable osc on battery, without external power

        show_message(&oled, "RTC set Ok");
    }
    else
    {
        show_message(&oled, "NO CMD");
    }
}

void setup()
{
    Serial.begin(115200);  // Initialize serial port for debugging.
    delay(200);            // Soft startup to ease the flow of electrons.

    oled.begin(SSD1306_128X64);
    oled.setBrightness(32);
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

    delay(1);

    if (Serial.available())
    {
        setupTimeFromSerial();
        buzzer.accord();
    }

}
