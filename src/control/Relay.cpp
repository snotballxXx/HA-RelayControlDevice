#include <Arduino.h>
#include "./Relay.h"
#include "../interfaces/IMessenger.h"
#include "../constants.h"
#define RELAY 0
#define LED 2
using namespace Control;

Relay::Relay(Interfaces::IMessenger *messenger) : _messenger(messenger),
                                                  _relayState(false)
{
}

void Relay::setup()
{
    Serial.println("Setup starting in Relay");
    _messenger->subscribe(COMMAND_TOPIC, this);

    pinMode(RELAY, OUTPUT);
    digitalWrite(RELAY, HIGH);

    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);
    Serial.println("Setup completed in Relay");
}

void Relay::messageReceived(const String &topic, const String &payload)
{
    Serial.print("Got message: ");
    Serial.println(payload);

    _relayState = (payload == "ON");

    if (_relayState)  
    {
        Serial.println("RELAY=ON");
        digitalWrite(RELAY,LOW);
        digitalWrite(LED,LOW);
    }
    else 
    {
        Serial.println("RELAY=OFF");
        digitalWrite(RELAY,HIGH);
        digitalWrite(LED,HIGH);
    }

    String msg = "{ \"state\": \"{state}\" }";
    msg.replace("{state}", _relayState ? "ON" : "OFF");
    _messenger->sendMessage(STATE_TOPIC, msg);
}

void Relay::loop(unsigned long time)
{
}