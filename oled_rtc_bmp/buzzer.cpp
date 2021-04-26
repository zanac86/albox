#include "buzzer.h"

BUZZER::BUZZER(uint8_t buzz_pin)
  :   pin(buzz_pin)
{
}

void BUZZER::begin()
{
  analogWrite(pin, 0);       // 0 turns it off
}

void BUZZER::click()
{
  analogWrite(pin, 100);      // Almost any value can be used except 0 and 255
  // experiment to get the best tone
  delay(3);          // wait for a delayms ms
  analogWrite(pin, 0);       // 0 turns it off
  delay(30);          // wait for a delayms ms
}

void BUZZER::accord()
{
  tone(NOTE_C3, 100);
  tone(NOTE_E3, 100);
  tone(NOTE_G3, 100);
}

void BUZZER::tone(long frequency, long length)
{
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  // 1 second's worth of microseconds, divided by the frequency, then split in half since
  // there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  // multiply frequency, which is really cycles per second, by the number of seconds to
  // get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++)   // for the calculated length of time...
  {
    digitalWrite(pin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(pin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
}
