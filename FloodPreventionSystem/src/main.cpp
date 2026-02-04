#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WaterSensor.h>
#include <TempAndHumiditySensor.h>

// PROTOTYPES
void wifi_connect();


// PINOUT
const int SCL_PIN = D1;
const int SDA_PIN = D2;
const int WATER_SENSOR_PIN = D7;



// FLAGS
volatile bool waterAlert = false;


// Objects
WaterSensor water_sensor(WATER_SENSOR_PIN);
TempAndHumiditySensor t_and_h_sensor;


// INTERRUPT HANDLERS
void IRAM_ATTR onWaterDetected() {
    Serial.println("!!! WATER DETECTED !!!");

    // TODO: Shutoff valve

    delay(20);
    water_sensor.reset();
}


void setup() {
    Serial.begin(9600);
    delay(1000);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    wifi_connect();

    water_sensor.begin(onWaterDetected);
    delay(2000);

    while( t_and_h_sensor.begin(SDA_PIN, SCL_PIN) != 0){
        delay(200);
    }

    if(t_and_h_sensor.begin(SDA, SCL) != 0){
        Serial.printf("Reset and try agian\n");
        digitalWrite(LED_BUILTIN, LOW);
        while(1) delay(100);
    }

    Serial.printf("System ready: \n");

}

void loop() {

    t_and_h_sensor.read_sensors();
    Serial.printf("Temp: %.2f\nHumidity: %.2f\n", t_and_h_sensor.get_temp(), t_and_h_sensor.get_humidity());




    delay(1000);  //debounce delay

}


// Basic 2.4GHz WiFi connection
void wifi_connect() {
    Serial.printf("\nConnecting to %s\n", WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.printf(".");
    }

    Serial.printf("\nConnected! \nIP Address: ");
    Serial.println(WiFi.localIP());
}

