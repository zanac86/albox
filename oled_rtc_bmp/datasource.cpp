#include "datasource.h"

char *daysLong[]  = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
char *daysShort[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
char *monthLong[]  = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
char *monthShort[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

DataSource::DataSource()
{

}

void DataSource::begin()
{
  Wire.begin();
  bmp180.begin();
}

void DataSource::update()
{
  // read rtc
  t = rtc.now();

  //read pressure 1004.962 hPa, 753.762 mmHg, 27.53 C
  char  status = bmp180.startTemperature();
  delay(status);
  status = bmp180.getTemperature(temperature);
  status = bmp180.startPressure(3);
  delay(status);
  status = bmp180.getPressure(pressure_hPa, temperature);
  pressure_mmHg = pressure_hPa / 1.33322;
}

void DataSource::strDateTime(char *s)
{
  sprintf(s, "%02u/%02u/%04u %02u:%02u:%02u", t.day(), t.month(), t.year(), t.hour(), t.minute(), t.second());
}

void DataSource::strDayOfWeek(char *s, bool isLongName)
{
  if (isLongName)
  {
    strcpy(s, daysLong[t.dayOfTheWeek() - 1]);
  } else
  {
    strcpy(s, daysShort[t.dayOfTheWeek() - 1]);
  }
}

void DataSource::strMonth(char *s, bool isLongName)
{
  if (isLongName)
  {
    strcpy(s, monthLong[t.month()]);
  } else
  {
    strcpy(s, monthShort[t.month()]);
  }
}

void DataSource::strDate(char *s)
{
  sprintf(s, "%02u-%02u-%04u", t.day(), t.month(), t.year());
}

void DataSource::strTime(char *s)
{
  sprintf(s, "%02u:%02u", t.hour(), t.minute());
}

void DataSource::strTimeHMS(char *s)
{
  sprintf(s, "%02u:%02u:%02u", t.hour(), t.minute(), t.second());
}

void DataSource::strPressure(char *s, uint8_t format)
{
  char ss[10];
  switch (format)
  {
    case PRESSURE_FORMAT_MMHG_INT:
      dtostrf(pressure_mmHg, 0, 0, ss);
      sprintf(s, "%s", ss);
      break;
    case PRESSURE_FORMAT_MMHG:
      dtostrf(pressure_mmHg, 0, 2, ss);
      sprintf(s, "%s", ss);
      break;
    case PRESSURE_FORMAT_HPA_INT:
      dtostrf(pressure_hPa, 0, 0, ss);
      sprintf(s, "%s", pressure_hPa);
      break;
    case PRESSURE_FORMAT_HPA:
      dtostrf(pressure_hPa, 0, 2, ss);
      sprintf(s, "%s", ss);
      break;
    default:
      sprintf(s, "xxxxxx");
  }
}

void DataSource::strTemperature(char *s)
{
  char ss[8];
  dtostrf(temperature, 4, 1, ss);
  sprintf(s, "%s""\xb8""C", ss);
}
