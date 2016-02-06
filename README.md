# Sonoff
Alternative firmware based on the Arduino port of the ESP8266

# Settings for the Arduino IDE

First I had a strange behavior:
* Serial working 
* GPIO working
* **and Wifi was not connecting**.

Thanks to Leonzio who posted here [http://tech.scargill.net/itead-slampher-and-sonoff/#comment-13634](http://tech.scargill.net/itead-slampher-and-sonoff/#comment-13634) I figured out that I also had the wrong flash size set.
Flash size is 512k for the V1.0 hardware. 

Here the right setting for the Arduino ESP8622 2.0 IDE

# Helpfull Links
* http://www.esp8266.com/wiki/doku.php?id=esp8266_gpio_pin_allocations
