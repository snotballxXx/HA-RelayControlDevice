#include "HomeAssistantMqtt.h"
#include <StreamString.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "interfaces/IWifi.h"
#include "interfaces/ITopicCallback.h"
#include "../constants.h"
#include "../utils/composeClientId.h"

using namespace Control;

String discoveryMessage = "{    \
  \"device\": {    \
    \"identifiers\": [    \
      \"{ID}\"    \
    ],    \
    \"name\": \"Fire\",    \
    \"model\": \"FIRE-200\",    \
    \"manufacturer\": \"DeviceCo\",    \
    \"sw_version\": \"2.0\"    \
  },    \
  \"origin\": {    \
    \"name\": \"FireCtrl Device {ID}\"    \
  },    \
  \"components\": {    \
     \"power\": {          \
       \"platform\": \"button\",          \
       \"device_class\": \"restart\",          \
       \"name\": \"Power\",    \
	   \"icon\": \"mdi:power\",    \
       \"payload_press\": \"1\",	       \
       \"command_topic\": \"home/{ID}/command\",  	             \
       \"unique_id\": \"{ID}_power\"          \
     },    \
     \"heat_on_off\": {          \
       \"platform\": \"button\",          \
       \"device_class\": \"restart\",          \
       \"name\": \"Heater\",    \
	   \"icon\": \"mdi:heat-wave\",    \
       \"payload_press\": \"2\",	       \
       \"command_topic\": \"home/{ID}/command\",  	             \
       \"unique_id\": \"{ID}_heat_on_off\"          \
     },    \
     \"flame_on_off\": {          \
       \"platform\": \"button\",          \
       \"device_class\": \"restart\",          \
       \"name\": \"Flame\",    \
	   \"icon\": \"mdi:fire-circle\",    \
       \"payload_press\": \"3\",	       \
       \"command_topic\": \"home/{ID}/command\",  	             \
       \"unique_id\": \"{ID}_flame_on_off\"          \
     },    \
     \"temp_up\": {          \
       \"platform\": \"button\",          \
       \"device_class\": \"restart\",          \
       \"name\": \"Temp Up\",    \
	   \"icon\": \"mdi:thermometer-chevron-up\",    \
       \"payload_press\": \"4\",	       \
       \"command_topic\": \"home/{ID}/command\",  	             \
       \"unique_id\": \"{ID}_temp_up\"          \
     },    \
     \"temp_down\": {          \
       \"platform\": \"button\",          \
       \"device_class\": \"restart\",          \
       \"name\": \"Temp Down\",    \
	   \"icon\": \"mdi:thermometer-chevron-down\",    \
       \"payload_press\": \"5\",	       \
       \"command_topic\": \"home/{ID}/command\",  	             \
       \"unique_id\": \"{ID}_temp_down\"          \
     }    \
  }    \
}";

void callback(char *topic, byte *payload, unsigned int length, void *p)
{
  String content;
  content.reserve(length);

  for (unsigned int i = 0; i < length; i++)
    content += (char)payload[i];

  ((HomeAssistantMqtt *)p)->invokeCallbacks(String(topic), content);
}

HomeAssistantMqtt::HomeAssistantMqtt()
{
}

void HomeAssistantMqtt::setup()
{
  _espClient = new WiFiClient;
  _client = new PubSubClient(*_espClient);

  _client->setBufferSize(3000);
  _client->setServer(MQTT_SERVER, 1883);
  _client->setCallback(callback, (void *)this);

  _clientId = Utils::composeClientID();
  _clientId.replace(':', '-');
}

void HomeAssistantMqtt::loop(unsigned long time)
{
  // confirm still connected to mqtt server
  if (!_client->connected())
    reconnect();

  _client->loop();
}

void HomeAssistantMqtt::sendMessage(const String &topic, const String &msg)
{
  if (_client->connected())
  {
    String tmp = topic;
    tmp.replace("{ID}", _clientId);
    Serial.println("Request to send on " + tmp);
    Serial.println("Publish on " + tmp + " with message " + msg);
    _client->publish(tmp.c_str(), msg.c_str(), true);
  }
}

void HomeAssistantMqtt::subscribe(const String &topic, Interfaces::ITopicCallback *callback)
{
  String tmp(topic);
  tmp.replace("{ID}", _clientId);
  Serial.println("Subscribed to " + tmp);
  _callbacks.insert(std::pair<String, Interfaces::ITopicCallback *>(tmp, callback));
}

void HomeAssistantMqtt::invokeCallbacks(const String &topic, const String &payload)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.print(": ");
  Serial.println(payload);

  auto range = _callbacks.equal_range(topic);

  for (auto it = range.first; it != range.second; ++it)
    it->second->messageReceived(topic, payload);
}

void HomeAssistantMqtt::reconnect()
{
  // Loop until we're reconnected
  while (!_client->connected())
  {
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    if (_client->connect(_clientId.c_str(), MQ_USER, MQ_PASSWORD))
    {
      Serial.println("connected");
      String topic = DISCOVERY_TOPIC;
      topic.replace("{ID}", _clientId);

      String msg = discoveryMessage;
      msg.replace("{ID}", _clientId);

      Serial.println("Message published to topic (2k) '" + topic + "' :" + msg);
      // Once connected, publish a discovery message
      _client->publish(topic.c_str(), msg.c_str(), true);

      for (auto it = _callbacks.begin(); it != _callbacks.end(); ++it)
      {
        _client->unsubscribe(it->first.c_str());
        _client->subscribe(it->first.c_str());
      }
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(_client->state());
      Serial.print(" wifi=");
      Serial.print(WiFi.status());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}