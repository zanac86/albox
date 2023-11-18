#ifndef DATASOURCE_H
#define DATASOURCE_H

#define PRESSURE_FORMAT_MMHG_INT 1
#define PRESSURE_FORMAT_MMHG2 2
#define PRESSURE_FORMAT_MMHG1 3
#define PRESSURE_FORMAT_HPA_INT 4
#define PRESSURE_FORMAT_HPA2 5
#define PRESSURE_FORMAT_HPA1 6
#define PRESSURE_FORMAT_MMHG_UNITS 7

#include <DS3231.h>

#define HISTORY_LEN 16

#define UPDATE_RTC       1
#define UPDATE_PRESSURE  2
#define UPDATE_PRESSURE_HISTORY  4

class DataSource
{
    public:
        DataSource();
        void begin();

        void update(uint8_t update_what);

        void strDateLong(char* s);
        void strDateWithMonthName(char* s);
        void strDateTime(char* s);
        void strDate(char* s);
        void strDayOfWeek(char* s, bool isLongName);
        void strMonth(char* s, bool isLongName);
        void strTime(char* s);
        void strTimeHMS(char* s);
        void strPressure(char* s, uint8_t format);
        void strTemperature(char* s);

        //
        void normalize_history(uint8_t max_h_norm);

        uint8_t norm_history[HISTORY_LEN];

    private:

        void update_pressure();
        void update_pressure_history();

        DateTime t;
        double pressure_mmHg;
        double pressure_hPa;
        double temperature;

        // история давления в целых мм рт.ст.
        uint16_t pressure_history[HISTORY_LEN];


};

#endif
