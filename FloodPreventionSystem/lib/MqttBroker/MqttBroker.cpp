#include "MqttBroker.h"

MqttBroker::MqttBroker(const char* server, int port){
    _server = server;
    _port = port;
    sniprintf(_clientID, sizeof(_clientID), "%s", WiFi.macAddress().c_str());
    _shutoff = false;
    _reset = false;

    _client.setClient(_espClient);
    _client.setServer(server, port);
    _client.setCallback([this](char* topic, byte* payload, unsigned int length) {
        this->_callback(topic, payload, length);
    });
}

bool MqttBroker::connect(){
    Serial.printf("\nAttempting MQTT connection...\n");

    // Attempt to connect
    if (_client.connect(_clientID)) {
        Serial.printf("client %s connnected\n", _clientID);

        subscribe("shutoff");
        subscribe("reset");

        Serial.printf("\n");
        return true;
    }
    Serial.printf("Failed to connect!\n\n");
    return false;
}

bool MqttBroker::connection_status(){
    return _client.connected();
}

void MqttBroker::_publish(const char* topic, const char* message) {
    // Length: 17 (MAC) + 1 (slash) + ~12 (max topic) + 1 (null)
    char fullTopic[64];

    // Format: CLIENT_ID/TOPIC
    snprintf(fullTopic, sizeof(fullTopic), "%s/%s", _clientID, topic);

    // Publish using the newly formatted string
    _client.publish(fullTopic, message);
}

void MqttBroker::publish_temp(float temp){
    // Set topic
    const char* topic = "temperature";
    
    // Set payload
    char message[10];
    snprintf(message, sizeof(message), "%.2f", temp);

    // Publish
    _publish(topic, message);
}

void MqttBroker::publish_humidity(float humidity){
    // Set topic
    const char* topic = "humidity";

    // Set payload
    char message[10];
    snprintf(message, sizeof(message), "%.2f", humidity);

    // Publish
    _publish(topic, message);
}

void MqttBroker::publish_flood(bool flooding){
    // Set topic
    const char* topic = "shutoff";

    // Sync shutoff control bool with state of microcontroller
    _shutoff = flooding;

    // Set payload
    char message[10];
    if(flooding){
        snprintf(message, sizeof(message), "1");
    } else{
        snprintf(message, sizeof(message), "0");
    }

    // Publish
    _publish(topic, message);
}

void MqttBroker::subscribe(const char* topic){
    char subscribe_topic[64];
    snprintf(subscribe_topic, sizeof(subscribe_topic), "%s/%s", _clientID, topic);


    if(_client.subscribe(subscribe_topic)){
        Serial.printf("Subscribed to: %s\n", subscribe_topic);
    } else{
        Serial.printf("Failed to subscribe to: %s\n", subscribe_topic);
    }
    
}

void MqttBroker::loop(){
    _client.loop();
}

char* MqttBroker::get_clientid(){
    return _clientID;
}

bool MqttBroker::get_shutoff(){
    return _shutoff;
}

void MqttBroker::_callback(char* topic, byte* payload, unsigned int length){
    // Create possible topic strings for comparison
    char shutoff_topic[30];
    snprintf(shutoff_topic, sizeof(shutoff_topic), "%s/shutoff", _clientID);

    char reset_topic[30];
    snprintf(reset_topic, sizeof(reset_topic), "%s/reset", _clientID);

    // Check topic against possibilities & handle each outcome
    if(strcmp(topic, shutoff_topic) == 0){
        if((char)payload[0] == '1'){
            _shutoff = true;
        } else{
            _shutoff = false;
        }
    }
    if(strcmp(topic, reset_topic) == 0){
        if((char)payload[0] == '1'){
            set_reset(true);
        }
    }
}

bool MqttBroker::get_reset(){
    return _reset;
}

void MqttBroker::set_reset(bool status){
    _reset = status;
}
