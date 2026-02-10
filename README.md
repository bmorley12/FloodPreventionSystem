# FloodPreventionSystem
An IoT project for detecting and preventing floods caused by home appliances.

## Setup

Edit FloodPreventionSystem/private_secrets_template.ini with your wifi name and passwod, and rename the file to private_secrets.ini. The project should be easily flashed to any ESP8266 microcontroller using the Platform.io extension in VS Code.

## How it works

Once loaded to a ESP8266, the device connects to 2.4GHz WiFi and attempts to connect to an MQTT Broker as defined in the private_secrets.ini file. The system is designed to work without an MQTT connection, so if the microcontroller disconnects from the broker for whatever reason, the flood prevention functionality still works as intended. Once connected to the MQTT broker, the microcontroller will publish the temperature and humidity to the topics "[WIFI MAC ADDRESS]/temperature" and "[WIFI MAC ADDRESS]/humidity" respectively every 5 seconds. The microcontroller will also subscribe to "[WIFI MAC ADDRESS]/shutoff" and "[WIFI MAC ADDRESS]/reset" upon connection with the broker. The shutoff valves connected to the microcontroller can be actuated by sending a "0" or "1" to the shutoff topic when the microcontroller has not detected any flooding. Once flooding is detected, the shutoff valves are moved to the off position untill a "1" is sent to the reset topic, or the microcontroller is power cycled. 

## FoodPreventionSystem (subfolder)

This is a subfolder containing all current source code.

## ELTR2402_FinalDraft.pdf

This is the pdf of my final report for my capstone project at NAIT. It lays out the original design, testing, and implementation for this project. Unfortunately the original source code for this project was lost as it was made before I backed everything up to github. The source code in this repository was re-written after my computer science degree, and because of that, different design choices were made.


## 3D_Objects

A folder containing the 3D models that were 3D printed as a part of this project


## DataSheets

A folder containing the relevant data sheets for the hardware used.
