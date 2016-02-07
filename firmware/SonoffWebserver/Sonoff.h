
#ifndef Sonoff_h
#define Sonoff_h


static const uint8_t RELAY = 12; // MTDI
static const uint8_t SWITCH = 0; // GPIO0
static const uint8_t LED = 13; // MTCK

static const uint8_t SWITCH_OFF = 0;
static const uint8_t SWITCH_ON = 1;

static const uint8_t SWITCH_EVENT_IDLE = 1;
static const uint8_t SWITCH_EVENT_OFF = 1;
static const uint8_t SWITCH_EVENT_ON = 2;

class Switch
{

  private:

    unsigned long poll_intervall_ms;
    unsigned long time_to_poll;
    bool polarity;
    uint8_t sample_counter;
    uint8_t sample_count;

    uint8_t pin;
    uint8_t state = 0;

    uint8_t event = SWITCH_EVENT_IDLE;

  public:

    Switch(uint8_t pin = SWITCH, bool polarity = false, uint8_t sample_count = 3, unsigned long poll_intervall_ms = 5) {
      this->pin = pin;
      this->polarity = polarity;
      this->sample_count = sample_count;
      this->poll_intervall_ms = poll_intervall_ms;
      pinMode ( pin, INPUT );
      time_to_poll = millis() + poll_intervall_ms;
      state = SWITCH_EVENT_OFF;
      sample_counter = 0;
    }

    uint8_t getEvent(void) {
      uint8_t e = event;
      event = SWITCH_EVENT_IDLE;
      return e;
    }

    bool isOn() {
      return state == SWITCH_ON;
    }

    void loop() {
      if (millis() > time_to_poll) {

        time_to_poll = millis() + poll_intervall_ms;

        switch (state) {
          case SWITCH_OFF:
            if ((digitalRead(pin) == HIGH && polarity == true) || (digitalRead(pin) == LOW && polarity == false) ) {
              sample_counter++;
              if (sample_counter >= sample_count) {
                sample_counter = 0;
                state = SWITCH_ON;
                event = SWITCH_EVENT_ON;
              }
            }
            else {
              sample_counter = 0;
            }

            break;
          case SWITCH_ON:
            if ((digitalRead(pin) == LOW && polarity == true) || (digitalRead(pin) == HIGH && polarity == false) ) {
              sample_counter++;
              if (sample_counter >= sample_count) {
                sample_counter = 0;
                state = SWITCH_OFF;
                event = SWITCH_EVENT_OFF;
              }
            }
            else {
              sample_counter = 0;
            }
            break;
        }

      }

    }

};

class Relay
{

  private:

    unsigned long time_to_reduce_current;
    unsigned long time_to_reduction;
    uint8_t reduced_current;

    uint8_t relay_pin;
    uint8_t state = 0;

  public:

    Relay(uint8_t pin = RELAY, float reduction = 0.4, unsigned long millis_till_reduction = 500) {
      if ( reduction > 1.0 ) reduction = 1.0;
      if ( reduction < 0.0 ) reduction = 0.0;
      setReduction(reduction);
      time_to_reduction = millis_till_reduction;
      relay_pin = pin;
      pinMode ( relay_pin, OUTPUT );
      analogWriteFreq(20000);
    }

    void on() {
      switch (state) {
        case 0:
          analogWrite(relay_pin, PWMRANGE);
          time_to_reduce_current = millis() + time_to_reduction;
          state = 1;
          break;
      }
    }

    void off() {
      analogWrite(relay_pin, 0);
      state = 0;
    }

    bool isOn() {
      return state != 0;
    }

    void setReduction(float reduction) {
      reduced_current = (PWMRANGE * 1.0) * reduction;
    }

    void setMillisTillReduction(unsigned long millis_till_reduction) {
      time_to_reduction = millis_till_reduction;
    }

    void loop() {
      switch (state) {
        case 1:
          if (millis() > time_to_reduce_current) {
            analogWrite(relay_pin, reduced_current);
            state = 2;
          }
          break;
      }
    }

};
#endif /* Sonoff_h */

