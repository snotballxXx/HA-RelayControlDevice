#include <Arduino.h>
#include "./IRSender.h"
#include "../interfaces/IMessenger.h"
#include "../constants.h"
#define IR_SEND_PIN D2
#include <IRremote.hpp>

using namespace Control;

//https://developers.home-assistant.io/docs/core/entity/button
//https://www.home-assistant.io/integrations/button#device-class
/*
                 _____0x81____
                /    POWER    \
               /               \
              /                 \
         0x82 |FLAME        HEAT| 0xD4
              |                 |
         0x94 |TEMP-       TEMP+| 0x95
              |                 |
         0x83 |ECO         BOOST| 0x96
		      |                 |
         0xC0 |COLOR  BRIGHTNESS| 0x97
              |                 |
              |                 |
              \______TIMER______/
                      0x80
*/

IRSender::IRSender(Interfaces::IMessenger* messenger) :
_messenger(messenger)
{
}

void IRSender::setup()
{
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
    Serial.print(F("Send IR signals at pin "));
    Serial.println(IR_SEND_PIN);

    _messenger->subscribe(COMMAND_TOPIC, this);

    IrSender.begin();
    disableLEDFeedback();
}

void IRSender::messageReceived(const String &topic, const String &payload)
{
    Serial.print("Got message: ");
    Serial.println(payload);

    if (payload == "1") 
        IrSender.sendNEC(0x00, 0x81, 0);

    else if (payload == "2") 
        IrSender.sendNEC(0x00, 0xD4, 0);

    else if (payload == "3") 
        IrSender.sendNEC(0x00, 0x82, 0);

    else if (payload == "4") 
        IrSender.sendNEC(0x00, 0x95, 0);

    else if (payload == "5") 
        IrSender.sendNEC(0x00, 0x94, 0);

}

void IRSender::loop(unsigned long time)
{
}