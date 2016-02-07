#ifndef switch_h
#define switch_h

static const uint8_t SWITCH_EVENT_IDLE = 1;
static const uint8_t SWITCH_EVENT_OFF = 1;
static const uint8_t SWITCH_EVENT_ON = 2;

/*
   We will check the switch every debounce_intervall_ms.
   If the logic value stays debounce_count times at a differnt level than the current one 
   an event is triggered and the new state is stored.
*/

class Switch
{

  private:

    static const uint8_t SWITCH_STATE_OFF = 0;
    static const uint8_t SWITCH_STATE_ON = 1;
    uint8_t state = SWITCH_STATE_OFF;

    unsigned long debounce_intervall_ms;
    unsigned long time_to_check_switch;

    bool polarity;

    uint8_t debounce_counter;
    uint8_t debounce_count;

    uint8_t pin;
    
    uint8_t event = SWITCH_EVENT_IDLE;

  public:

    Switch(uint8_t pin = 0, bool polarity = false, uint8_t debounce_count = 3, unsigned long debounce_intervall_ms = 5) {
      this->pin = pin;
      pinMode ( pin, INPUT );

      this->polarity = polarity;

      debounce_counter = 0;
      this->debounce_count = debounce_count;
      this->debounce_intervall_ms = debounce_intervall_ms;
      time_to_check_switch = millis() + debounce_intervall_ms;

      state = SWITCH_EVENT_OFF;
    }

    uint8_t getEvent(void) {
      uint8_t e = event;
      event = SWITCH_EVENT_IDLE;
      return e;
    }

    bool isOn() {
      return state == SWITCH_STATE_ON;
    }

    void loop() {
      if (millis() > time_to_check_switch) {

        time_to_check_switch = millis() + debounce_intervall_ms;

        switch (state) {
          
          case SWITCH_STATE_OFF:
            if ((digitalRead(pin) == HIGH && polarity == true) || (digitalRead(pin) == LOW && polarity == false) ) {
              debounce_counter++;
              if (debounce_counter >= debounce_count) {
                debounce_counter = 0;
                state = SWITCH_STATE_ON;
                event = SWITCH_EVENT_ON;
              }
            }
            else {
              debounce_counter = 0;
            }
            break;
            
          case SWITCH_STATE_ON:
          
            if ((digitalRead(pin) == LOW && polarity == true) || (digitalRead(pin) == HIGH && polarity == false) ) {
              debounce_counter++;
              if (debounce_counter >= debounce_count) {
                debounce_counter = 0;
                state = SWITCH_STATE_OFF;
                event = SWITCH_EVENT_OFF;
              }
            }
            else {
              debounce_counter = 0;
            }
            break;
            
        }
      }
    }
};

#endif /* switch_h */

