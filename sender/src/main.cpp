/**************************************************************************
 * TITLE: Smart Controller (Sender + Logic + Firebase)
 * DESCRIPTION: Reads all sensors, decides actions, updates Firebase,
 * and sends commands via ESP-NOW to the receiver.
 **************************************************************************/

#include <WiFi.h>
#include <esp_now.h>
#include <Firebase_ESP_Client.h>
#include "DHT.h"

// --- 1. CONFIGURATION ---

// -- Wi-Fi & Firebase
const char* WIFI_SSID = "Ooredoo-M281-235CA9";
const char* WIFI_PASSWORD = "io56@&WF";

#define API_KEY "AIzaSyCQAPrt7xuqvhMJ1Aio7QuN0GVDFaxBaOE"
#define DATABASE_URL "https://tecweek-5bf5c-default-rtdb.firebaseio.com/"
#define USER_EMAIL "tecweek@gmail.com"
#define USER_PASSWORD "00000000"

uint8_t broadcastAddress[] = {};

// -- Pins
#define DHTPIN 32
#define DHTTYPE DHT11
#define GAS_PIN 34
#define LDR_PIN 35
#define TRIG_PIN 5  // Ultrasonic Trigger (Moved here for logic)
#define ECHO_PIN 18 // Ultrasonic Echo  (Moved here for logic)

// -- Thresholds
const float TEMP_THRESHOLD = 28.5;
const int GAS_THRESHOLD = 800;
const int LIGHT_THRESHOLD = 1500; // Value implies Darkness
const int DIST_THRESHOLD = 40;    // cm

// --- 2. GLOBAL OBJECTS ---
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
DHT dht(DHTPIN, DHTTYPE);

// Structure to send data via ESP-NOW
typedef struct struct_message {
    bool fanCmd;
    bool klaxonCmd;
    bool ledCmd;
} struct_message;

struct_message myCommands;
esp_now_peer_info_t peerInfo;

unsigned long lastTime = 0;
const int timerDelay = 1000;

// --- 3. HELPER FUNCTIONS ---
float getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    return duration * 0.034 / 2;
}

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    // Optional: Debug print
    // Serial.print("\r\nLast Packet Send Status:\t");
    // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
    Serial.begin(9600);

    // Init Sensors
    dht.begin();
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Init WiFi (Station Mode)
    WiFi.mode(WIFI_AP_STA); // Important for ESP-NOW + WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" Connected!");

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register Send Callback
    esp_now_register_send_cb(OnDataSent);

    // Register Peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = WiFi.channel(); // Must match WiFi channel
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    // Init Firebase
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}

void loop() {
    if (millis() - lastTime > timerDelay) {
        lastTime = millis();

        // 1. Read Sensors
        float t = dht.readTemperature();
        float h = dht.readHumidity();
        int gas = analogRead(GAS_PIN);
        int light = analogRead(LDR_PIN);
        float dist = getDistance();
        bool personDetected = (dist > 0 && dist < DIST_THRESHOLD);

        if (isnan(t) || isnan(h)) {
            Serial.println("DHT Read Failed");
            return;
        }

        // 2. Logic Verification (The "Brain")
        bool fanAction = (t > TEMP_THRESHOLD && personDetected);
        bool klaxonAction = (gas > GAS_THRESHOLD);
        bool ledAction = (light > LIGHT_THRESHOLD && personDetected); // LED ON if Dark + Person

        // 3. Prepare ESP-NOW Payload
        myCommands.fanCmd = fanAction;
        myCommands.klaxonCmd = klaxonAction;
        myCommands.ledCmd = ledAction;

        // 4. Send Order via ESP-NOW
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myCommands, sizeof(myCommands));

        Serial.println("-------------------------");
        Serial.printf("Temp: %.1f, Gas: %d, Light: %d, Dist: %.1f\n", t, gas, light, dist);
        Serial.printf("Orders -> Fan: %d, Klaxon: %d, LED: %d\n", fanAction, klaxonAction, ledAction);
        if (result == ESP_OK) Serial.println(">> Sent via ESP-NOW");
        else Serial.println(">> ESP-NOW Send Failed");

        // 5. Update Firebase
        if (Firebase.ready()) {
            // Upload Sensor Data
            Firebase.RTDB.setFloat(&fbdo, "/system/sensors/temperature", t);
            Firebase.RTDB.setFloat(&fbdo, "/system/sensors/humidity", h);
            Firebase.RTDB.setInt(&fbdo, "/system/sensors/gas_value", gas);
            Firebase.RTDB.setInt(&fbdo, "/system/sensors/light_value", light);
            Firebase.RTDB.setFloat(&fbdo, "/system/sensors/distance", dist);

            // Upload Command Status (Reporting what we ordered)
            Firebase.RTDB.setBool(&fbdo, "/system/commands/fan_status", fanAction);
            Firebase.RTDB.setBool(&fbdo, "/system/commands/klaxon_status", klaxonAction);
            Firebase.RTDB.setBool(&fbdo, "/system/commands/led_status", ledAction);
        }
    }
}