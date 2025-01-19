#ifndef _IR_SENDER_H_
#define _IR_SENDER_H_

#include "../interfaces/IArduinoBase.h"
#include "../interfaces/ITopicCallback.h"

namespace Interfaces
{
    class IMessenger;
}

namespace Control
{
    class IRSender : public Interfaces::IAduninoBase, public Interfaces::ITopicCallback
    {
        Interfaces::IMessenger* _messenger;

        public:
        IRSender(Interfaces::IMessenger* messenger);
        virtual void messageReceived(const String& topic, const String& payload);
        virtual void loop(unsigned long time);
        virtual void setup();        
    };    
}

#endif  //!_IR_SENDER_H_