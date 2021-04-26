#include "buttons.h"

BUTTONS::BUTTONS(uint8_t pin1)
{
  buttons[0].pin = pin1;
  begin();
}

void BUTTONS::begin()
{
  triggered = false;
  for (int i = 0; i < BUTTONS_COUNT; i++)
  {
    buttons[i].state = false;
    buttons[i].triggered = false;
    pinMode(buttons[i].pin, INPUT_PULLUP); // turn on internal pull-up resistor
  }
}

bool BUTTONS::wasTriggered(int buttonNumber)
{
  if ((buttonNumber >= 0) and (buttonNumber < BUTTONS_COUNT))
  {
    return buttons[buttonNumber].triggered;
  }
  else
  {
    return false;
  }
}

bool BUTTONS::wasTriggered()
{
  return triggered;
}

void BUTTONS::update()
{
  triggered = false;
  for (int i = 0; i < BUTTONS_COUNT; i++)
  {
    // Keep in mind the pullup means the pushbutton's
    // logic is inverted. It goes HIGH when it's open,
    // and LOW when it's pressed.
    bool b = (digitalRead(buttons[i].pin) != HIGH); // LOW means BUTTON PRESSED
    // compare previous state and current
    // true - only when new state is pressed and previous not pressed
    buttons[i].triggered = ((b != buttons[i].state) and (b == true));
    // collect triggeres flagsto global flag
    triggered |= buttons[i].triggered;
    buttons[i].state = b;
  }
  delay(10);
}
