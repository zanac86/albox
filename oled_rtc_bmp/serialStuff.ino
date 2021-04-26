#include <DS3231.h>

char uCase(char c)
{
  if ((c >= 'a') && (c <= 'z'))
    return (c - 0x20);
  else
    return c;
}

byte calcDOW(byte d, byte m, int y)
{
  int dow;
  byte mArr[12] = {6, 2, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

  dow = (y % 100);
  dow = dow * 1.25;
  dow += d;
  dow += mArr[m - 1];
  if (((y % 4) == 0) && (m < 3))
    dow -= 1;
  while (dow > 7)
    dow -= 7;

  return dow;
}

boolean serialValidateDate(byte d, byte m, word y)
{
  byte    mArr[12] = {31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  boolean ok = false;

  if (m == 2)
  {
    if ((y % 4) == 0)
    {
      if ((d > 0) and (d <= 29))
        ok = true;
    }
    else
    {
      if ((d > 0) and (d <= 28))
        ok = true;
    }
  }
  else
  {
    if ((d > 0) and (d <= mArr[m - 1]))
      ok = true;
  }

  return ok;
}

void serialSendDOW(byte dow)
{
  char* str[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

  Serial.print(str[dow - 1]);
  Serial.print(", ");
}

void serialSendMenu()
{
  Serial.println ("----------------------------");
  Serial.println ("Setup clock");
  Serial.println ("----------------------------");
  Serial.println ("S : Set date / time");
  Serial.println ("G : Get time / date");
  Serial.println ("? : Show help");
  Serial.println ("Q : Quit service mode");
  Serial.println ();
}

void serialSetTime()
{
  char buf[20];
  char tmp;
  int  cnt = 0;
  int Year, Month, Day, Hour, Minute, Second;

  Serial.print("Enter date/time format [YYMMDDhhmmss]: ");
  while (cnt < 12)
  {
    while (Serial.available() == 0) {};
    tmp = Serial.read();
    if ((tmp >= '0') and (tmp <= '9'))
    {
      buf[cnt] = tmp;
      cnt++;
      Serial.print(tmp);
    }
  }
  Serial.println();
  for (uint8_t i = 0; i < 12; i++)
  {
    buf[i] = buf[i] - '0';
  }
  Year = ((buf[0] ) * 10) + (buf[1]  );
  Month = ((buf[2]) * 10) + (buf[3]  );
  Day = ((buf[4] ) * 10) + (buf[5]  );
  Hour = ((buf[6] ) * 10) + (buf[7] );
  Minute = ((buf[8] ) * 10) + (buf[9] );
  Second = ((buf[10] ) * 10) + (buf[11] );

  DS3231 Clock;
  Clock.setYear(Year);
  Clock.setMonth(Month);
  Clock.setDate(Day);
  //    Clock.setDoW(DoW);
  Clock.setHour(Hour);
  Clock.setMinute(Minute);
  Clock.setSecond(Second);

}

void serialReadTimeDate()
{
  RTClib RTC;
  DateTime now = RTC.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  Serial.print(" since midnight 1/1/1970 = ");
  Serial.print(now.unixtime());
  Serial.print("s = ");
  Serial.print(now.unixtime() / 86400L);
  Serial.println("d");
}

void serialMode()
{
  boolean quitMode = false;
  char    cmd;

  while (Serial.available() > 0)
    Serial.read();

  serialSendMenu();
  Serial.print ("> ");

  while (quitMode == false)
  {
    if (Serial.available() > 0)
    {
      cmd = uCase(Serial.read());
      Serial.println(cmd);
      switch (cmd)
      {
        case 'S': serialSetTime();
          break;
        case 'G': serialReadTimeDate();
          break;
        case '?': serialSendMenu();
          break;
        case 'Q': Serial.println("Exiting service mode...");
          Serial.println();
          quitMode = true;
          break;
      }
      if (quitMode == false)
        Serial.print("> ");
    }
  }
  while (Serial.available() > 0)
  {
    Serial.read();
  }
  Serial.println("Press any button to enter service mode");
  Serial.println();
}
