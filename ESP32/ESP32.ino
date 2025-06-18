#include <WiFi.h>
#include "esp_task_wdt.h"


// Pin assignments
const int SENSOR1_PIN = 14;
const int SENSOR2_PIN = 27;
const int SENSOR3_PIN = 26;
const int SENSOR4_PIN = 25;
const int SENSOR5_PIN = 33;
const int SENSOR6_PIN = 32;
const int SENSOR7_PIN = 35;
const int SENSOR8_PIN = 22;
const int SENSOR9_PIN = 23;
const int SENSOR10_PIN = 21;
const int SENSOR11_PIN = 19;


// MAC address string
char macStr[18];

// Timing control
unsigned long previousMillis = 0;
const unsigned long interval = 5000; // 2 seconds

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
  pinMode(SENSOR5_PIN, INPUT);
  pinMode(SENSOR6_PIN, INPUT);
  pinMode(SENSOR7_PIN, INPUT);
  pinMode(SENSOR8_PIN, INPUT);
  pinMode(SENSOR9_PIN, INPUT);
  pinMode(SENSOR10_PIN, INPUT);
  pinMode(SENSOR11_PIN, INPUT);
  

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
    int s5 = digitalRead(SENSOR5_PIN);
    int s6 = digitalRead(SENSOR6_PIN);
    int s7 = digitalRead(SENSOR7_PIN);
    int s8 = digitalRead(SENSOR8_PIN);    
    int s9 = digitalRead(SENSOR9_PIN);
    int s10 = digitalRead(SENSOR10_PIN);
    int s11 = digitalRead(SENSOR11_PIN);

    // Determine payloads
    String p1 = (s1 == HIGH) ? "Detected" : "Not Detected";
    String p2 = (s2 == HIGH) ? "Detected" : "Not Detected";
    String p3 = (s3 == LOW) ? "Detected" : "Not Detected";
    String p4 = (s4 == LOW) ? "Detected" : "Not Detected";
    String p5 = (s5 == LOW) ? "Detected" : "Not Detected";
    String p6 = (s6 == LOW) ? "Detected" : "Not Detected";
    String p7 = (s7 == LOW) ? "Detected" : "Not Detected";
    String p8 = (s8 == HIGH) ? "Detected" : "Not Detected";
    String p9 = (s9 == HIGH) ? "Detected" : "Not Detected";
    String p10 = (s10 == LOW) ? "Detected" : "Not Detected";
    String p11 = (s11 == LOW) ? "Detected" : "Not Detected";
 

    // Build and print topic/payload lines
    // Added a delay to help keep organization in order at the MQTT Server
    Serial.println("kenventory/" + String(macStr) + "/sensor/1 = " + p1);
    delay(100);
    Serial.println("kenventory/" + String(macStr) + "/sensor/2 = " + p2);
    delay(100);
    Serial.println("kenventory/" + String(macStr) + "/sensor/3 = " + p3);
    delay(100);
    Serial.println("kenventory/" + String(macStr) + "/sensor/4 = " + p4);
    delay(100);
    Serial.println("kenventory/" + String(macStr) + "/sensor/5 = " + p5);
    delay(100);
    Serial.println("kenventory/" + String(macStr) + "/sensor/6 = " + p6);
    delay(100);
    Serial.println("kenventory/" + String(macStr) + "/sensor/7 = " + p7);
    delay(100);
    Serial.println("kenventory/" + String(macStr) + "/sensor/8 = " + p8);
    delay(100);
    Serial.println("kenventory/" + String(macStr) + "/sensor/9 = " + p9);
    delay(100);
    Serial.println("kenventory/" + String(macStr) + "/sensor/10 = " + p10);
    delay(100);
    Serial.println("kenventory/" + String(macStr) + "/sensor/11 = " + p11);





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
