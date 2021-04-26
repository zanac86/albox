#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <Wire.h>
#include <DS3231.h>
#include <SFE_BMP180.h>

#define PRESSURE_FORMAT_MMHG_INT 1
#define PRESSURE_FORMAT_MMHG     2
#define PRESSURE_FORMAT_HPA_INT  3
#define PRESSURE_FORMAT_HPA      4

class DataSource
{
  public:
    DataSource();
    void begin();

    void update();

    void strDateTime(char *s);
    void strDate(char *s);
    void strDayOfWeek(char *s, bool isLongName);
    void strMonth(char *s, bool isLongName);
    void strTime(char *s);
    void strTimeHMS(char *s);
    void strPressure(char *s, uint8_t units);
    void strTemperature(char *s);

  private:
    RTClib rtc;
    SFE_BMP180 bmp180;

    DateTime t;
    double pressure_mmHg;
    double pressure_hPa;
    double temperature;
};

#endif
