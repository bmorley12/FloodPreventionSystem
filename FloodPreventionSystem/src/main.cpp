#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WaterSensor.h>

// PROTOTYPES
void wifi_connect();


// PINOUT
const int WATER_SENSOR_PIN = D7;


// FLAGS
volatile bool waterAlert = false;


// INTERRUPT HANDLERS
void IRAM_ATTR onWaterDetected() {
    waterAlert = true;
}

// Objects
WaterSensor water_sensor(WATER_SENSOR_PIN);


void setup() {
  Serial.begin(9600);
  delay(1000);

  wifi_connect();

  water_sensor.begin(onWaterDetected);

  Serial.printf("System ready: \n");

}

void loop() {
  if (waterAlert) {
    Serial.println("!!! WATER DETECTED !!!");
    
    // Add your emergency logic here
    
    // Reset the flag
    waterAlert = false;
  }

 
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

