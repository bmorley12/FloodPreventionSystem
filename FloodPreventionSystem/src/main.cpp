#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WaterSensor.h>
#include <TempAndHumiditySensor.h>
#include <ShutoffValve.h>

// PROTOTYPES
void wifi_connect();


// PINOUT
const int SCL_PIN = D1;
const int SDA_PIN = D2;
const int VALVE1_PIN = D5;
const int VALVE2_PIN = D6;
const int WATER_SENSOR_PIN = D7;


// FLAGS
volatile bool waterAlert = false;


// Objects
WaterSensor water_sensor(WATER_SENSOR_PIN);
TempAndHumiditySensor t_and_h_sensor;
ShutoffValve valve1(VALVE1_PIN);
ShutoffValve valve2(VALVE2_PIN);


// INTERRUPT HANDLERS
void IRAM_ATTR onWaterDetected() {
    Serial.println("!!! WATER DETECTED !!!");

    valve1.turn_off();
    valve2.turn_off();

    // TODO: Notify user

    delay(20);
    water_sensor.reset();
}


void setup() {
    Serial.begin(9600);
    delay(1000);

    // Setup built in light for error checking
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    wifi_connect();

    water_sensor.begin(onWaterDetected);
    delay(2000);

    // REQUIRES FURTHER TROUBLESHOOTING WITH WORKING SENSOR
    // while( t_and_h_sensor.begin(SDA_PIN, SCL_PIN) != 0){
    //     Serial.printf("Unable to connect with temp/humidity sensor\n");
    //     digitalWrite(LED_BUILTIN, LOW);
    //     delay(200);
    // }


    Serial.printf("System ready: \n");

}

void loop() {
    // REQUIRES FURTHER TROUBLESHOOTING WITH WORKING SENSOR
    // t_and_h_sensor.read_sensors();
    // Serial.printf("Temp: %.2f\n", t_and_h_sensor.get_temp());
    // Serial.printf("Humidity: %.2f\n", t_and_h_sensor.get_humidity());

    delay(1000);  //debounce delay

}


// Basic 2.4GHz WiFi connection
void wifi_connect() {
    Serial.printf("\nConnecting to %s\n", WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASS);

    bool led_state = LOW;
    while (WiFi.status() != WL_CONNECTED){
        // flash led while connecting
        digitalWrite(LED_BUILTIN, led_state);
        led_state = !led_state;

        // send loading dots to serial monitor
        delay(500);
        Serial.printf(".");
    }

    Serial.printf("\nConnected! \nIP Address: ");
    Serial.println(WiFi.localIP());
}

