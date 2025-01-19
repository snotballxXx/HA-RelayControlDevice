#ifndef _COMPOSE_CLIENT_ID_H_
#define _COMPOSE_CLIENT_ID_H_

#include <ESP8266WiFi.h>
#include "macConverter.h"

namespace Utils
{
    String composeClientID() 
    {
        uint8_t mac[6];
        WiFi.macAddress(mac);
        String clientId;
        clientId += "esp-";
        clientId += macToStr(mac);
        return clientId;
    }
}

#endif //_COMPOSE_CLIENT_ID_H_