#include <Arduino.h>
#include "./Ota.h"
#include <ElegantOTA.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "constants.h"

using namespace Control;
ESP8266WebServer server(80);

void Ota::setup()
{
    server.on("/", []() { server.send(200, "text/plain", VERSION); });
 
    ElegantOTA.begin(&server);
    server.begin();
    Serial.println("HTTP server started");
}

void Ota::loop(unsigned long time)
{
    ElegantOTA.loop();
    server.handleClient();
}