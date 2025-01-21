#ifndef _TMP_RELAY_H_
#define _TMP_RELAY_H_

#include "../interfaces/IRelay.h"
#include "../interfaces/ITopicCallback.h"

namespace Interfaces
{
    class IMessenger;
}

namespace Control
{
    class Relay : public Interfaces::IRelay, public Interfaces::ITopicCallback
    {
        Interfaces::IMessenger* _messenger;
        bool _relayState;

        public:
        Relay(Interfaces::IMessenger* messenger);
        virtual void messageReceived(const String& topic, const String& payload);
        virtual void loop(unsigned long time);
        virtual void setup();        
    };    
}

#endif  //!_TMP_RELAY_H_