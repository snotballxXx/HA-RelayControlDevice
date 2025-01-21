#include <Arduino.h>
#include "./Ota.h"
#include <ElegantOTA.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "constants.h"
#include "../utils/helpers.h"

using namespace Control;
ESP8266WebServer server(80);

void Ota::setup()
{
    Serial.println("Setup starting in Ota");
    String output = String(VERSION) + String(" ") + String(Utils::Helpers::composeClientID());
    server.on("/", [output]() { server.send(200, "text/plain", output); });
 
    ElegantOTA.begin(&server);
    server.begin();
    Serial.println("HTTP server started");
    Serial.println("Setup completed in Ota");
}

void Ota::loop(unsigned long time)
{
    ElegantOTA.loop();
    server.handleClient();
}