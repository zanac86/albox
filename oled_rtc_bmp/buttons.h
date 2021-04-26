#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <inttypes.h>
#include "Arduino.h"

#define BUTTONS_COUNT 1

/// Button state info
struct ButtonInfo
{
  /// Arduino pin number
  uint8_t pin;
  /// Button last state
  bool state;
  /// Flag button was pressed in prev update
  bool triggered;
};

/// Buttons control
class BUTTONS
{
  public:
    BUTTONS(uint8_t pin1);
    void begin();
    /// check button 0 or 1 was triggered
    bool wasTriggered(int buttonNumber);
    /// Flag true if any button was triggered
    bool wasTriggered();
    /// Update buttons state
    void update();

  protected:
    ButtonInfo buttons[BUTTONS_COUNT];
    /// GLobal flag for any button triggered since last update
    /// Next update will clear flag state
    bool triggered;
};

#endif
