
#ifndef Sonoff_h
#define Sonoff_h

#include "relay.h"
#include "switch.h"

static const uint8_t RELAY = 12; // MTDI
static const uint8_t SWITCH = 0; // GPIO0
static const uint8_t LED = 13; // MTCK

class Sonoff
{

  private:

   

  public:
    Relay relay = Relay( RELAY, 0.4, 500, 25000);
    Switch sw =  Switch( SWITCH, 2, 5);

    Sonoff(void) {

    }

    void loop() {
      relay.loop();
      sw.loop();
    }

};
#endif /* Sonoff_h */

