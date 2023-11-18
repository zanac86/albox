#include "display.h"

extern uint8_t MsxFont[];
extern uint8_t BigNumbers[];
extern uint8_t MediumNumbers[];

const uint8_t max_count_display_type = 4;
volatile uint8_t display_type = 0;

void change_display_type()
{
    display_type = (display_type + 1) % max_count_display_type;
}

void screen_update(OLED* o, DataSource* ds)
{
    switch (display_type)
    {
        case 0:
            screen0(o, ds);
            break;
        case 1:
            screen1(o, ds);
            break;
        case 2:
            screen2(o, ds);
            break;
        case 3:
            screen3(o, ds);
            break;
    }
}

// график истории, время, дата и текущее
void screen0(OLED* o, DataSource* ds)
{
    char s[20];
    o->clrScr();
    o->setFont(MsxFont);

    ds->strDateLong(s);
    o->print(s, 2, 3);
    ds->strTime(s);
    o->print(s, 97, 3);

    ds->strPressure(s, PRESSURE_FORMAT_MMHG_INT);
    o->setFont(MediumNumbers);
    o->print(s, 91, 15);

    o->setFont(MsxFont);
    o->print("mmHg", 103, 33);
    ds->strPressure(s, PRESSURE_FORMAT_HPA1);
    o->print(s, 91, 45);
    o->print("hPa", 109, 54);

    for (uint8_t x = 0; x < 5; x++)
    {
        uint8_t xx = 3 + x * 21;
        for (uint8_t y = 0; y < 8; y++)
        {
            o->setPixel(xx, 14 + y * 7);
        }
    }
    o->drawLine(3, 63, 87, 63);

    uint8_t h_max = 49;
    uint8_t y0 = 14;
    ds->normalize_history(h_max);

    for (uint8_t i = 0; i < HISTORY_LEN; i++)
    {
        // каждый столбик 3 пикс, пробел 2 пикс
        uint8_t x = 5 + i * 5 + (i / 4);
        // для вертикальной линии y2 должен быть на 1 больше
        // при рисовании там условие <(y2-y1)

        if (ds->norm_history[i] == 0)
        {
            continue;
        }

        uint8_t y = y0 + (h_max - ds->norm_history[i]);

        o->drawLine(x, y, x, 63);
        o->drawLine(x + 1, y, x + 1, 63);
        o->drawLine(x + 2, y, x + 2, 63);
    }


    o->update();

}

void screen1(OLED* o, DataSource* ds)
{
    char s[20];
    o->clrScr();
    o->setFont(BigNumbers);
    ds->strTime(s);
    o->print(s, CENTER, 4);

    o->setFont(MsxFont);
    ds->strDateWithMonthName(s);
    o->print(s, CENTER, 34);
    ds->strDayOfWeek(s, true);
    o->print(s, CENTER, 44);
    ds->strPressure(s, PRESSURE_FORMAT_MMHG_UNITS);
    o->print(s, CENTER, 56);
    o->update();
}

void screen2(OLED* o, DataSource* ds)
{
    char s[20];
    o->clrScr();

    o->setFont(MediumNumbers);
    ds->strPressure(s, PRESSURE_FORMAT_MMHG1);
    o->print(s, RIGHT, 6);
    ds->strPressure(s, PRESSURE_FORMAT_HPA1);
    o->print(s, RIGHT, 28);

    o->setFont(MsxFont);
    o->print("mmHg", 20, 10);
    o->print("hPa", 20, 32);

    o->setFont(MsxFont);
    ds->strDateTime(s);
    o->print(s, CENTER, 56);
    o->update();
}

void screen3(OLED* o, DataSource* ds)
{
    char s[20];
    o->clrScr();

    o->setFont(BigNumbers);
    ds->strPressure(s, PRESSURE_FORMAT_MMHG_INT);
    o->print(s, CENTER, 10);
    o->setFont(MsxFont);
    o->print("mmHg", CENTER, 40);

    o->setFont(MsxFont);
    ds->strDateTime(s);
    o->print(s, CENTER, 56);
    o->update();
}

void show_message(OLED* o, char* s)
{
    o->clrScr();
    o->setFont(MsxFont);
    o->print(s, CENTER, 10);
    o->update();
}
