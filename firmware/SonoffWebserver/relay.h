#ifndef relay_h
#define relay_h

class Relay
{
  private:

    unsigned long time_to_reduce_current;
    unsigned long millis_till_reduction;
    uint16_t reduced_current;

    uint8_t pin;

    static const uint8_t RELAY_STATE_OFF = 0;
    static const uint8_t RELAY_STATE_ON_WAIT_TO_REDUCE = 1;
    static const uint8_t RELAY_STATE_ON_REDUCED = 1;
    uint8_t state = RELAY_STATE_OFF;

  public:

    Relay(uint8_t pin = 0, float reduction = 0.4, unsigned long millis_till_reduction = 500, unsigned long analog_write_frequency = 10000) {
      setReduction(reduction);
      
      setMillisTillReduction(millis_till_reduction);

      analogWriteFreq(analog_write_frequency);
      
      this->pin = pin;
      pinMode ( pin, OUTPUT );
    }

    void on() {
      switch (state) {
        case RELAY_STATE_OFF:
          analogWrite(pin, PWMRANGE);
          time_to_reduce_current = millis() + millis_till_reduction;
          state = RELAY_STATE_ON_WAIT_TO_REDUCE;
          break;
      }
    }

    void off() {
      analogWrite(pin, 0);
      state = RELAY_STATE_OFF;
    }

    bool isOn() {
      return state != RELAY_STATE_OFF;
    }

    void setReduction(float reduction) {
      if ( reduction > 1.0 ) reduction = 1.0;
      else if ( reduction < 0.0 ) reduction = 0.0;
      reduced_current = (PWMRANGE * 1.0) * reduction;
    }

    void setMillisTillReduction(unsigned long millis_till_reduction) {
       this->millis_till_reduction = millis_till_reduction;
    }

    void loop() {
      switch (state) {
        case RELAY_STATE_ON_WAIT_TO_REDUCE:
          if (millis() > time_to_reduce_current) {
            analogWrite(pin, reduced_current);
            state = RELAY_STATE_ON_REDUCED;
          }
          break;
      }
    }

};
#endif /* relay_h */

