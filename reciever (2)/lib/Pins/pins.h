#ifndef PINS_H
#define PINS_H

// --- OUTPUT PINS ---
#define ESC_PIN 25        // GPIO pin for the Fan's ESC signal
#define KLAXON_PIN 2     // GPIO pin for the Klaxon/Buzzer
#define LED_PIN 27        // GPIO pin for the LED

// --- INPUT PINS ---
// Your HAL_ULTRASONIC library uses the same pin for Trig and Echo
#define ULTRASONIC_PIN 33

#endif
