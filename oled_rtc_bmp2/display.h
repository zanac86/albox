#include <OLED_I2C.h>
#include "datasource.h"

void change_display_type();
void change_screen_mode();

void screen_update(OLED* o, DataSource* ds);

void screen0(OLED* o, DataSource* ds);
void screen1(OLED* o, DataSource* ds);
void screen2(OLED* o, DataSource* ds);
void screen3(OLED* o, DataSource* ds);

void show_message(OLED* o, char* s);