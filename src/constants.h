#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#define SSID                "TheMadHouse24"
#define PASSWORD            "masonlewissuck!"
#define MQTT_SERVER         "192.168.1.120"
#define MQ_USER             "mqttuser"
#define MQ_PASSWORD         "mqttuserpassword"

#define DISCOVERY_TOPIC     "homeassistant/device/{ID}/config"
#define COMMAND_TOPIC       "home/{ID}/command"
#define STATE_TOPIC         "home/{ID}/state"
#define BASE_TOPIC          "home"
#define SERIAL_BAUD_RATE     115200
#define VERSION              "Relay Version 1.0.0"

#endif  //!_CONSTANT_H_