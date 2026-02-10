// External
#include <Arduino.h>


// Local
#include <WaterSensor.h>
#include <TempAndHumiditySensor.h>
#include <ShutoffValve.h>
#include <MqttBroker.h>


// PROTOTYPES
void wifi_connect(const char* ssid, const char* passwd);
void handle_flood();


// PINOUT
const int SCL_PIN = D1;
const int SDA_PIN = D2;
const int VALVE1_PIN = D5;
const int VALVE2_PIN = D6;
const int WATER_SENSOR_PIN = D7;


// FLAGS
volatile bool Flooding_Detected = false;
volatile bool Flooding_Handled = false;


// GLOBAL VARIABLES
unsigned long Previous_Sensor_Read = 0;
unsigned long Previous_Broker_Reconnect = 0;
const long Sensor_Frequency = 5000;         // 5 seconds - frequency sensors are checked
const long Reconnect_Frequencty = 5000;     // 5 seconds - frequencty for MQTT reconnection attempts


// Objects
WaterSensor water_sensor(WATER_SENSOR_PIN);
TempAndHumiditySensor t_and_h_sensor;
ShutoffValve valve1(VALVE1_PIN);
ShutoffValve valve2(VALVE2_PIN);
MqttBroker broker(BROKER_ADDRESS, PORT);


// INTERRUPT HANDLERS
void IRAM_ATTR onWaterDetected() {
    Flooding_Detected = true;
}

void setup() {
    // Startup serial monitor for debugging
    Serial.begin(9600);
    delay(1000);

    // Setup built in light for error checking
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // Connect to wifi
    wifi_connect(WIFI_SSID, WIFI_PASS);

    // Initialize water sensor
    water_sensor.begin(onWaterDetected);
    delay(2000);

    // Initialize temperature and humidity sensor
    while( t_and_h_sensor.begin(SDA_PIN, SCL_PIN) != 0){
        Serial.printf("Unable to connect with temp/humidity sensor\n");
        digitalWrite(LED_BUILTIN, LOW);     // Turn LED on so error is visable without serial monitor
        delay(200);
    }

    Serial.printf("\nSystem ready: \n");
}

void loop() {
    // Get current uptime in milliseconds
    unsigned long current_uptime = millis();

    if(!broker.connection_status()){
        if(current_uptime - Previous_Broker_Reconnect >= Reconnect_Frequencty){
            broker.connect();
            Previous_Broker_Reconnect = current_uptime;
        }
    } else{
        // run broker loop
        broker.loop();
    }

    // Check flooding and handle issues if water detected
    if(Flooding_Detected){
        // If flood hasn't been handled, handle it
        if (!Flooding_Handled){
            handle_flood(); 
        }   
    } 
    // If no flooding, act on commands from broker
    else{
        if(broker.get_shutoff()){
            valve1.turn_off();
            valve2.turn_off();
        } 
        else{
            valve1.turn_on();
            valve2.turn_on();
        }
    }

    // When reset flag is set by broker, return all flags to default values
    if (broker.get_reset()){
        Flooding_Detected = false;
        Flooding_Handled = false;
        water_sensor.reset();

        broker.set_reset(false);
    }


    // Read Temp/Humidity Sensor
    if(current_uptime - Previous_Sensor_Read >= Sensor_Frequency){
        t_and_h_sensor.read_sensors();

        // print to serial monitor for troubleshooting
        Serial.printf("Temp: %.2f\n", t_and_h_sensor.get_temp());
        Serial.printf("Humidity: %.2f\n\n", t_and_h_sensor.get_humidity());

        // Check broker connection
        if(broker.connection_status()){
            // publish to mqtt broker
            broker.publish_humidity(t_and_h_sensor.get_humidity());
            broker.publish_temp(t_and_h_sensor.get_temp());
        }

        Previous_Sensor_Read = current_uptime;
    }
}

// Basic 2.4GHz WiFi connection
void wifi_connect(const char* ssid, const char* passwd) {
    Serial.printf("\nConnecting to %s\n", ssid);

    WiFi.begin(ssid, passwd);

    bool led_state = LOW;
    while (WiFi.status() != WL_CONNECTED){
        // flash led while connecting
        digitalWrite(LED_BUILTIN, led_state);
        led_state = !led_state;

        // send loading dots to serial monitor
        delay(500);
        Serial.printf(".");
    }

    digitalWrite(LED_BUILTIN, HIGH);        // Make sure LED is off
    Serial.printf("\nConnected! \nIP Address: ");
    Serial.println(WiFi.localIP());
}

// Handle water detection
void handle_flood(){
    Serial.println("!!! WATER DETECTED !!!");

    valve1.turn_off();
    valve2.turn_off();

    // check broker connection
    if (broker.connection_status()){
        broker.publish_flood(Flooding_Detected);
    }
    
    delay(20);
    water_sensor.reset();
    Flooding_Handled = true;
}

