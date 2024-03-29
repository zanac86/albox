#include "datasource.h"

#include <Wire.h>
#include <DS3231.h>
#include <SFE_BMP180.h>

RTClib rtc;
SFE_BMP180 bmp180;

char* daysLong[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
char* daysShort[] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
char* monthLong[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
char* monthShort[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

DataSource::DataSource()
{
    for (uint8_t i = 0; i < HISTORY_LEN; i++)
    {
        pressure_history[i] = 0;
        norm_history[i] = 0;
    }
}

void DataSource::begin()
{
    Wire.begin();
    bmp180.begin();
}

void DataSource::update(uint8_t update_what)
{
    if (update_what & UPDATE_RTC)
    {
        // read rtc
        t = rtc.now();
    }

    if (update_what & UPDATE_PRESSURE)
    {
        update_pressure();
    }

    if (update_what & UPDATE_PRESSURE_HISTORY)
    {
        update_pressure_history();
    }
}

void DataSource::update_pressure()
{
    //read pressure 1004.962 hPa, 753.762 mmHg, 27.53 C
    char status = bmp180.startTemperature();
    delay(status);
    status = bmp180.getTemperature(temperature);
    status = bmp180.startPressure(3);
    delay(status);
    status = bmp180.getPressure(pressure_hPa, temperature);
    pressure_mmHg = pressure_hPa / 1.33322;
    // давление в мм.рт.ст от 700 до 800
    if (pressure_mmHg < 700)
    {
        pressure_mmHg = 700;
    }
    if (pressure_mmHg > 800)
    {
        pressure_mmHg = 800;
    }
}

void DataSource::update_pressure_history()
{
    // самое старое в начале, так проще рисовать
    // а новое положить в последний индекс
    for (uint8_t i = 0; i < (HISTORY_LEN - 1); i++)
    {
        pressure_history[i] = pressure_history[i + 1];
    }
    pressure_history[HISTORY_LEN - 1] = (uint16_t)(pressure_mmHg * 10.0);

    // тест
    // const uint16_t u[] = {742, 740, 747, 750, 752, 753, 755, 757, 763, 762, 759, 757, 753, 750, 748, 770 };
    // const uint16_t u[] = {750, 751, 751, 751, 750, 750, 750, 750, 749, 750, 750, 750, 750, 750, 751, 761 };
    // for (uint8_t i = 0; i < HISTORY_LEN; i++)
    // {
    //     pressure_history[i] = u[i];
    // }
}

void DataSource::strDateTime(char* s)
{
    sprintf(s, "%02u/%02u/%04u  %02u:%02u:%02u", t.day(), t.month(), t.year(), t.hour(), t.minute(), t.second());
}

// день недели строкой 3 символа или полное название
void DataSource::strDayOfWeek(char* s, bool isLongName)
{
    if (isLongName)
    {
        strcpy(s, daysLong[t.dayOfTheWeek() - 1]);
    }
    else
    {
        strcpy(s, daysShort[t.dayOfTheWeek() - 1]);
    }
}

// месяц строкой 3 символа или полное название
void DataSource::strMonth(char* s, bool isLongName)
{
    if (isLongName)
    {
        strcpy(s, monthLong[t.month()]);
    }
    else
    {
        strcpy(s, monthShort[t.month()]);
    }
}

void DataSource::strDateLong(char* s)
{
    sprintf(s, "%02u/%02u/%04u %3s", t.day(), t.month(), t.year(), daysShort[t.dayOfTheWeek() - 1]);
}

void DataSource::strDateWithMonthName(char* s)
{
    sprintf(s, "%02u %s %04u", t.day(), monthLong[t.month()], t.year());
}

void DataSource::strDate(char* s)
{
    sprintf(s, "%02u-%02u-%04u", t.day(), t.month(), t.year());
}

void DataSource::strTime(char* s)
{
    sprintf(s, "%02u:%02u", t.hour(), t.minute());
}

void DataSource::strTimeHMS(char* s)
{
    sprintf(s, "%02u:%02u:%02u", t.hour(), t.minute(), t.second());
}

// давление в hpa c 1 или 2 знаками после точки
void DataSource::strPressure(char* s, uint8_t format)
{
    char ss[10];
    switch (format)
    {
        case PRESSURE_FORMAT_MMHG_INT:
            dtostrf(pressure_mmHg, 0, 0, ss);
            sprintf(s, "%s", ss);
            break;
        case PRESSURE_FORMAT_MMHG2:
            dtostrf(pressure_mmHg, 0, 2, ss);
            sprintf(s, "%s", ss);
            break;
        case PRESSURE_FORMAT_MMHG1:
            dtostrf(pressure_mmHg, 0, 1, ss);
            sprintf(s, "%s", ss);
            break;
        case PRESSURE_FORMAT_HPA_INT:
            dtostrf(pressure_hPa, 0, 0, ss);
            sprintf(s, "%s", pressure_hPa);
            break;
        case PRESSURE_FORMAT_HPA2:
            dtostrf(pressure_hPa, 0, 2, ss);
            sprintf(s, "%s", ss);
            break;
        case PRESSURE_FORMAT_HPA1:
            dtostrf(pressure_hPa, 0, 1, ss);
            sprintf(s, "%s", ss);
            break;
        case PRESSURE_FORMAT_MMHG_UNITS:
            dtostrf(pressure_mmHg, 0, 1, ss);
            sprintf(s, "%s mmHg", ss);
            break;
        default:
            sprintf(s, "xxxxxx");
    }
}

void DataSource::strTemperature(char* s)
{
    char ss[8];
    dtostrf(temperature, 4, 1, ss);
    sprintf(s, "%s"
            "\xb8"
            "C",
            ss);
}

void DataSource::normalize_history(uint8_t max_h_norm)
{
    uint16_t mn = pressure_history[HISTORY_LEN - 1];
    uint16_t mx = pressure_history[HISTORY_LEN - 1];
    // find min/max == range
    for (uint8_t i = 0; i < HISTORY_LEN; i++)
    {
        norm_history[i] = 0;
        if (pressure_history[i] == 0)
        {
            // 0 значений давления не должно быть и не учитывать
            // они и рисоваться не будут
            continue;
        }
        if (pressure_history[i] < mn)
        {
            mn = pressure_history[i];
        }
        if (pressure_history[i] > mx)
        {
            mx = pressure_history[i];
        }
    }

    if (((mn == 0) || (mx == 0)))
    {
        // все нули и рисовать нечего
        return;
    }

    if (mn == mx)
    {
        // если менялось мало и в целых значениях изменения нет
        // то вернем середину от нормировки
        for (uint8_t i = 0; i < HISTORY_LEN; i++)
        {
            if (pressure_history[i] != 0)
            {
                norm_history[i] = max_h_norm / 2;
            }
        }
        return;
    }

    // диапазон изменения истории
    uint16_t range = mx - mn;

    // +- 2 это в 20, так как в истории лежат  в мм.рт.ст*10
    if (range <= 20)
    {
        mn = mn - 20;
        mx = mx + 20;
        range = mx - mn;
    }

    for (uint8_t i = 0; i < HISTORY_LEN; i++)
    {
        if (pressure_history[i] == 0)
        {
            continue;
        }
        uint32_t tmp = (uint32_t)(pressure_history[i] - mn);
        tmp = (tmp * max_h_norm) << 10;
        tmp = (tmp / (range + 1));
        tmp = (tmp >> 10);
        if (tmp == 0)
        {
            tmp = 1;
        }
        norm_history[i] = (uint8_t) tmp;
    }
}

void DataSource::strPressureHistory(char* s, uint8_t index)
{
    char ss[10];
    double p = (double)(pressure_history[index]) / 10.0;
    dtostrf(p, 0, 1, ss);
    sprintf(s, "%s", ss);
}