//
// Created by hakmouni mohamed on 16/11/2024.
//
#ifndef LINEFOLLOWER_HAL_ULTRASONIC_H
#define LINEFOLLOWER_HAL_ULTRASONIC_H

#include <NewPing.h>

class HAL_ULTRASONIC {
private:
    NewPing ultrason;      // Ultrasonic sensor object
    int consecutiveZeros;  // Counter for consecutive zero readings
    int lastValidReading;  // Last valid distance reading
public:

    float distance;
    // Constructor
    HAL_ULTRASONIC(int pin, int maxDistance);

    // Method to get the filtered distance
    float GetDistance();
};

#endif // LINEFOLLOWER_HAL_ULTRASONIC_H
