#include <Arduino.h>
#include "./control/HomeAssistantMqtt.h"
#include "./control/Wifi.h"
#include "./control/Relay.h"
#include "./control/Ota.h"

//#include "constants.h

Control::Wifi* wifi = new Control::Wifi();
Control::HomeAssistantMqtt* homeAssistant = new Control::HomeAssistantMqtt();
Control::Relay* relay = new Control::Relay(homeAssistant);
Control::Ota* ota = new Control::Ota();

void setup()
{
    Serial.begin(115200);
    wifi->setup();
    homeAssistant->setup();
    relay->setup();
    ota->setup();
}

void loop()
{
    unsigned long now = millis();
    wifi->loop(now);
    homeAssistant->loop(now);
    relay->loop(now);
    ota->loop(now);
}