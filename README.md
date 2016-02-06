# Sonoff
Alternative firmware based on the Arduino port of the ESP8266

# Settings for the Arduino IDE

First I had a strange behavior:
* Serial working 
* GPIO working
* **and Wifi was not connecting**.

Thanks to Leonzio who posted here http://tech.scargill.net/itead-slampher-and-sonoff/#comment-13634 I figured out that I also had the wrong flash size set. 

Flash size to set for a generic module in the Arduino IDE is 512k for the V1.0 hardware. Even if the chip says 1M bit.

Also be aware that OTA is not support with that little flash. Tried it but it will now advertise itself and never shows up in the list of OTA devices.

Here the working setting for the Arduino ESP8622 2.0 IDE

# Helpfull Links
* http://www.esp8266.com/wiki/doku.php?id=esp8266_gpio_pin_allocations
* http://wiki.iteadstudio.com/images/6/6b/Sonoff_schmatic.pdf
