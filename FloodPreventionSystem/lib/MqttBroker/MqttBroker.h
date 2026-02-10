#ifndef MQTT_BROKER_H
#define MQTT_BROKER_H

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

class MqttBroker {
    public:
        MqttBroker(const char* server, int port);
        bool connect();
        bool connection_status();
        void publish_temp(float temp);
        void publish_humidity(float humidity);
        void publish_flood(bool flooding);
        void subscribe(const char* topic);
        void loop();
        char* get_clientid();
        bool get_shutoff();
        bool get_reset();
        void set_reset(bool status);

    private:
        WiFiClient _espClient;
        PubSubClient _client;
        String _server;
        int _port;
        char _clientID[18];
        bool _shutoff;
        bool _reset;
        void _publish(const char* topic, const char* message);
        void _callback(char* topic, byte* payload, unsigned int length);
};


#endif