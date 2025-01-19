#include <Arduino.h>
#include "./control/HomeAssistantMqtt.h"
#include "./control/Wifi.h"
#include "./control/IRSender.h"
#include "./control/Ota.h"
#include "constants.h"

Control::Wifi* wifi = new Control::Wifi();
Control::HomeAssistantMqtt* homeAssistant = new Control::HomeAssistantMqtt();
Control::IRSender* irSender = new Control::IRSender(homeAssistant);
Control::Ota* ota = new Control::Ota();

void setup()
{
    Serial.begin(SERIAL_BAUD_RATE);
    
    wifi->setup();
    homeAssistant->setup();
    irSender->setup();
    ota->setup();
}

void loop()
{
    unsigned long now = millis();

    wifi->loop(now);
    homeAssistant->loop(now);
    irSender->loop(now);
    ota->loop(now);
}