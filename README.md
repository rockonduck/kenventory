# Kenventory Management System

Kenventory is an IoT-based inventory tracking system that uses sensors to share real-time 
inventory information from multiple locations on a single web interface.

---

## Table of Contents

1. [Overview](#overview)  
2. [Hardware & Subscription Requirements](#hardware--subscription-requirements)  
3. [Communication Architecture](#communication-architecture)  
4. [ESP32 Wiring & Sensor Connections](#esp32-wiring--sensor-connections)  
5. [Configuring Your Inventory & Locations](#configuring-your-inventory--locations)  
6. [Example Configuration Block](#example-configuration-block)  

---

## Overview

Kenventory allows you to:

- **Monitor multiple locations** using an ESP32 and KY-033 sensors.  
- **Publish sensor data** to an MQTT broker.  
- **Store data** in an AWS S3 bucket for archival and web-based viewing.  
- **View all locations** on a single dashboard hosted in the cloud.

Once everything is set up, plug the Kenventory Hub into the internet, connect each ESP32 to 
its sensors, and you’ll have real-time inventory updates across all your sites.

---

## Hardware & Subscription Requirements

### Sensor Tracking (ESP32 Unit)

- **ESP32-32S CH340C**  
- **30-pin ESP32 Expansion Board**  
- **Sensor**: KY-033 (Photoelectric Sensor)

### Kenventory Hub

- **Raspberry Pi 4B** (any RAM size)  
- **microSD Card** (16 GB or larger)

### MQTT Broker

- Any MQTT broker service (e.g., HiveMQ, Mosquitto).  
- **Tip:** We’re currently using HiveMQ (free tier).

### AWS Server (Optional)

- **S3 Bucket** to store and serve JSON data.  
- **Static Website Hosting** (e.g., AWS S3 + CloudFront) to display your inventory dashboard.

---

## Communication Architecture

Sensor → ESP32 → Raspberry Pi 4B (Kenventory Hub) → MQTT Broker → AWS S3 Bucket → Web Dashboard



1. **ESP32** reads each KY-033 sensor.  
2. ESP32 publishes sensor states (along with its MAC ID) to the **MQTT Broker**.  
3. **Kenventory Hub (Raspberry Pi 4B)** subscribes to the MQTT topics, formats the data, and uploads a JSON file to **AWS S3**.  
4. Your **Web Dashboard** fetches the JSON from S3 and displays all locations in real time.

---

## ESP32 Wiring & Sensor Connections

The ESP32 uses four digital inputs to read the KY-033 sensors. In `main.cpp` (or your preferred firmware), the code reads each pin as follows:

```cpp
int sensor1 = digitalRead(27);
int sensor2 = digitalRead(25);
int sensor3 = digitalRead(32);
int sensor4 = digitalRead(34);

Sensor	ESP32 GPIO Pin
sensor1	27
sensor2	25
sensor3	32
sensor4	34

Power the KY-033 sensors from the ESP32’s 3.3 V and GND pins.

Connect each sensor’s output to the GPIO pin listed above.

The ESP32 code concatenates these four sensor readings into a unique identifier (serial number)
 and publishes it to MQTT.

Configuring Your Inventory & Locations
Clone or download the Kenventory repository.

Navigate to the AWS configuration file:


kenventory/AWS/config.json
In config.json, each ESP32 device is identified by its MAC address. You’ll see blocks of 
JSON like this:


"78421C6C23945": {
  "name": "Team Biscuit – Pittsburgh Branch",
  "sensors": {
    "1": "Orbit Repair Kit",
    "2": "Grease (Auto Lube)",
    "3": "Battery (Auto Lube)",
    "4": "EP2 Grease (Mobile 1)"
  }
}
To add or update a location, copy the template block:


"[mac-id]": {
  "name": "[location name]",
  "sensors": {
    "1": "[inventory item for sensor1]",
    "2": "[inventory item for sensor2]",
    "3": "[inventory item for sensor3]",
    "4": "[inventory item for sensor4]"
  }
}
Replace [mac-id] with your ESP32’s MAC address.

Replace [location name] with a descriptive location (e.g., “Main Warehouse – Aisle 3”).

For each sensor index ("1", "2", …), set the corresponding inventory item name.

Finding Your ESP32’s MAC ID

When you power on your ESP32 and connect it to the network, Kenventory Hub automatically logs 
each device’s MAC address.

Check the Raspberry Pi’s screen for the mac id in green

ex

78421C6C23945

Copy that 12-character string (no colons) into your config.json.

Example Configuration Block
Below is a complete example for one location (“Team Biscuit – Pittsburgh Branch”). 
This block should appear inside config.json (alongside any other locations you add):

---

"78421C6C23945": {
  "name": "Team Biscuit – Pittsburgh Branch",
  "sensors": {
    "1": "Orbit Repair Kit",
    "2": "Grease (Auto Lube)",
    "3": "Battery (Auto Lube)",
    "4": "EP2 Grease (Mobile 1)"
  }
},

---

"78421C6C23945" is the ESP32’s MAC address.

"Team Biscuit – Pittsburgh Branch" is the human-readable location name.

Each numbered key ("1", "2", "3", "4") corresponds to one KY-033 sensor and its associated 
inventory item.


When completed with your edits save the config_text file as 

config.json

Upload

config.json

and

index.html

onto your S3 bucket on AWS

assign a static link to your S3 Bucket and use this to see your live sensor data.


but we are not done yet!!!!

Final Setup: Broker & Hub Configuration
Important: You must configure both the Kenventory Hub (Raspberry Pi) and AWS dashboard to 
communicate with your MQTT broker. Without these updates, you won’t see any data.

Update broker credentials in usb1_forwarder.py:


BROKER_ADDRESS = "your_broker_url"
BROKER_USER = "your_username"
BROKER_PASS = "your_password"
Update AWS dashboard in index.html (or your dashboard JS):


const MQTT_BROKER = "your_broker_url";
const MQTT_USER = "your_username";
const MQTT_PASS = "your_password";

Set up Raspberry Pi to run the forwarding script on boot, or configure kiosk mode so the 
dashboard loads automatically on a connected display.

Happy coding! If you run into issues, please open a GitHub Issue or submit a Pull Request.
"
}