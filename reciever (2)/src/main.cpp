/**************************************************************************
 * TITLE: Actuator Node (Receiver)
 * DESCRIPTION: Receives commands via ESP-NOW and controls HW.
 **************************************************************************/

#include <WiFi.h>
#include <esp_now.h>
#include "FanController.h" // Your custom library
// Include other libraries if needed for Pins.h

// --- 1. CONFIGURATION ---
// Define pins strictly as used on your board
#define ESC_PIN 14     // Example, check your Pins.h
#define KLAXON_PIN 12  // Example
#define LED_PIN 13     // Example

// --- 2. GLOBAL OBJECTS ---
FanController fan(ESC_PIN);

// Structure to receive data (MUST match Sender)
typedef struct struct_message {
    bool fanCmd;
    bool klaxonCmd;
    bool ledCmd;
} struct_message;

struct_message incomingCommands;

// Logic variables
bool klaxonActive = false;
unsigned long lastKlaxonToggle = 0;

// --- 3. CALLBACK FUNCTION ---
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&incomingCommands, incomingData, sizeof(incomingCommands));

    // Debug
    // Serial.print("Bytes received: "); Serial.println(len);
    // Serial.printf("Fan: %d, Klaxon: %d, LED: %d\n", incomingCommands.fanCmd, incomingCommands.klaxonCmd, incomingCommands.ledCmd);
}

void setup() {
    Serial.begin(9600);

    // Init Pins
    pinMode(KLAXON_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    // Init Fan
    fan.initFan();
    delay(4000); // Wait for ESC arming
    fan.setFanSpeedPercentage(0);

    // Init ESP-NOW
    WiFi.mode(WIFI_STA); // Station mode for ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register Callback
    esp_now_register_recv_cb(OnDataRecv);
    Serial.println("ESP32 Receiver Ready.");
}

void loop() {
    // 1. Fan Control (Direct mapping)
    if (incomingCommands.fanCmd) {
        fan.setFanSpeedPercentage(20); // Or whatever speed you want
    } else {
        fan.setFanSpeedPercentage(0);
    }

    // 2. LED Control (Direct mapping)
    if (incomingCommands.ledCmd) {
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);
    }

    // 3. Klaxon Control (Non-blocking beep loop)
    if (incomingCommands.klaxonCmd) {
        if (millis() - lastKlaxonToggle > 100) { // Fast beep
            lastKlaxonToggle = millis();
            klaxonActive = !klaxonActive;
            digitalWrite(KLAXON_PIN, klaxonActive);
        }
    } else {
        digitalWrite(KLAXON_PIN, LOW);
        klaxonActive = false;
    }
}