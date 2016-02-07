/*
 * Copyright (c) 2016 ImUrlaub
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "Sonoff.h"

const char *ssid = "ssid";
const char *password = "pass";

ESP8266WebServer server ( 80 );

Sonoff sonoff;

const int led = LED;

void handleRoot() {
  
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;


  for ( uint8_t i = 0; i < server.args(); i++ ) {
    if(server.argName(i) == String("relay")){
      if(server.arg(i) == String("on")){
        sonoff.relay.on();
        digitalWrite ( led, 0 );  
      }
      else if(server.arg(i) == String("off")){
        sonoff.relay.off();
        digitalWrite ( led, 1 );
      }
    }
  }
  char *r;
  if(sonoff.relay.isOn()) r = "on"; 
  else r = "off"; 

  snprintf ( temp, 400,

"<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>Sonoff Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from Sonoff!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p>Relay: %s</p>\
    <a href=\"/?relay=on\">Relay on</a><br>\
    <a href=\"/?relay=off\">Relay off</a><br>\
  </body>\
</html>",

    hr, min % 60, sec % 60, r
  );
  server.send ( 200, "text/html", temp );
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
}

void setup ( void ) {
  pinMode ( led, OUTPUT );
  digitalWrite ( led, 1 );
  Serial.begin ( 115200 );
  WiFi.begin ( ssid, password );
  Serial.println ( "" );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  if ( MDNS.begin ( "Sonoff" ) ) {
    Serial.println ( "MDNS responder started" );
  }

  server.on ( "/", handleRoot );
  server.on ( "/on", []() {
    sonoff.relay.on();
    digitalWrite ( led, 0 );
    server.send ( 200, "text/plain", "on" );
  } );
  server.on ( "/off", []() {
    sonoff.relay.off();
    digitalWrite ( led, 1 );
    server.send ( 200, "text/plain", "off" );
  } );
  server.on ( "/state", []() {
    if(sonoff.relay.isOn()) server.send ( 200, "text/plain", "on" );
    else server.send ( 200, "text/plain", "off" );
  } );
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );
}

void loop ( void ) {
  server.handleClient();
  sonoff.loop();
  if(sonoff.sw.getEvent() == SWITCH_EVENT_ON){
    if(sonoff.relay.isOn()){
      sonoff.relay.off();
      digitalWrite ( led, 1 );
    }
    else {
      sonoff.relay.on();
      digitalWrite ( led, 0 );
    }
  }
}


