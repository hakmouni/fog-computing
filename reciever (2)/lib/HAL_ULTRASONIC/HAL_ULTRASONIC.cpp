//
// Created by hakmouni mohamed on 16/11/2024.
//
#include "HAL_ULTRASONIC.h"

// Constructor: Initialize the ultrasonic object and variables
HAL_ULTRASONIC::HAL_ULTRASONIC(int pin, int maxDistance)
        : ultrason(pin, pin, maxDistance), consecutiveZeros(0), lastValidReading(-1) {}

float HAL_ULTRASONIC::GetDistance() {
    distance = ultrason.ping_cm();

    if (distance == 0) {
        consecutiveZeros++;
    } else {
        consecutiveZeros = 0;
        lastValidReading = distance;
    }

    if (consecutiveZeros >= 5) {
        distance = 0;
    } else {
        distance = lastValidReading;
    }

    return distance;
}
