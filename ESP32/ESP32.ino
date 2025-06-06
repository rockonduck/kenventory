#include <WiFi.h>
#include "esp_task_wdt.h"


// Pin assignments
const int SENSOR1_PIN = 27;
const int SENSOR2_PIN = 25;
const int SENSOR3_PIN = 32;
const int SENSOR4_PIN = 34;

// MAC address string
char macStr[18];

// Timing control
unsigned long previousMillis = 0;
const unsigned long interval = 2000; // 2 seconds

//Watchdog Function
esp_task_wdt_config_t wdt_config = {
  .timeout_ms = 30000,        // 30-second timeout
  .idle_core_mask = 1,        // Core 0 (main application core)
  .trigger_panic = false      // Clean soft reset, no crash dump
};


void setup() {
  Serial.begin(115200);

  // Initialize WiFi hardware (we don’t connect, just activate the radio)
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin();  // Not strictly necessary, but ensures hardware is ready


//Watchdog Function
  esp_task_wdt_init(&wdt_config);
esp_task_wdt_add(NULL);  // Watch the current task (loop)

  // Setup sensor pins
  pinMode(SENSOR1_PIN, INPUT);
  pinMode(SENSOR2_PIN, INPUT);
  pinMode(SENSOR3_PIN, INPUT);
  pinMode(SENSOR4_PIN, INPUT);

  // Get MAC and format for topic use
  uint8_t mac[6];
  WiFi.macAddress(mac);
  snprintf(macStr, sizeof(macStr), "%02X%02X%02X%02X%02X%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

WiFi.disconnect(true);   // Drop connection
WiFi.mode(WIFI_OFF);     // Shut off the WiFi stack


    // Boot-time presence + troubleshooting signal
    Serial.println("esp32/" + String(macStr) + " = Sensor data");
}

void loop() {

  //Watchdog function
esp_task_wdt_reset();  // Feed the watchdog

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Read sensors
    int s1 = digitalRead(SENSOR1_PIN);
    int s2 = digitalRead(SENSOR2_PIN);
    int s3 = digitalRead(SENSOR3_PIN);
    int s4 = digitalRead(SENSOR4_PIN);

    // Determine payloads
    String p1 = (s1 == LOW) ? "Detected" : "Not Detected";
    String p2 = (s2 == LOW) ? "Detected" : "Not Detected";
    String p3 = (s3 == LOW) ? "Detected" : "Not Detected";
    String p4 = (s4 == LOW) ? "Detected" : "Not Detected";

    // Build and print topic/payload lines
    Serial.println("kenventory/" + String(macStr) + "/sensor/1 = " + p1);
    Serial.println("kenventory/" + String(macStr) + "/sensor/2 = " + p2);
    Serial.println("kenventory/" + String(macStr) + "/sensor/3 = " + p3);
    Serial.println("kenventory/" + String(macStr) + "/sensor/4 = " + p4);


/*
//Watchdog freeze test
    static bool freezeTriggered = false;

    if (!freezeTriggered) {
        Serial.println("⚠️ FORCING FREEZE IN 5 SECONDS...");
        delay(5000);  // Let the message print
        Serial.println("🧊 Freezing now...");
      freezeTriggered = true;

//Simulate lockup (no watchdog reset)
      while (true) {
         delay(100);
  }
}
*/

  }
}
