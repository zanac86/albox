#include "display.h"

extern uint8_t MsxFont[];
extern uint8_t BigNumbers[];
extern uint8_t MediumNumbers[];

const uint8_t max_count_display_type = 2;
volatile uint8_t display_type = 0;
volatile uint8_t screen_mode = 0;


/* display_type - clock or barometer

   screen_mode - for clock show clock and calendar info
   screen_mode - for barometer show date/time and mmHg or hPa

   support 2 mode for each display type

*/

void change_display_type()
{
  display_type = (display_type + 1) % max_count_display_type;
}

void change_screen_mode()
{
  screen_mode = (screen_mode + 1) % 2;
}

void screen_update(OLED *o, DataSource* ds)
{
  switch (display_type)
  {
    case 0:
      screen0(o, ds);
      break;
    case 1:
      screen1(o, ds);
      break;
  }
}

void screen0(OLED *o, DataSource* ds)
{
  char s[20];
  o->clrScr();
  o->setFont(BigNumbers);
  ds->strTime(s);
  o->print(s, CENTER, 4);
  switch (screen_mode)
  {
    case 0:
      o->setFont(MediumNumbers);
      ds->strDate(s);
      o->print(s, CENTER, 40);
      break;
    case 1:
      o->setFont(MsxFont);
      ds->strDate(s);
      o->print(s, CENTER, 36);
      ds->strMonth(s, true);
      o->print(s, CENTER, 46);
      ds->strDayOfWeek(s, true);
      o->print(s, CENTER, 56);
      break;
  }
  o->update();
}

void screen1(OLED *o, DataSource* ds)
{
  char s[20];
  o->clrScr();

  switch (screen_mode)
  {
    case 0:
      o->setFont(BigNumbers);
      ds->strPressure(s, PRESSURE_FORMAT_MMHG_INT);
      o->print(s, CENTER, 10);
      o->setFont(MsxFont);
      o->print("mmHg" , CENTER, 40);
      break;
    case 1:
      o->setFont(MediumNumbers);
      ds->strPressure(s, PRESSURE_FORMAT_MMHG);
      o->print(s, 0, 6);
      ds->strPressure(s, PRESSURE_FORMAT_HPA);
      o->print(s, 0, 28);
      o->setFont(MsxFont);
      o->print("mmHg" , 100, 10);
      o->print("hPa" , 100, 32);
      break;
  }

  o->setFont(MsxFont);
  ds->strDateTime(s);
  o->print(s, CENTER, 56);

  o->update();
}
