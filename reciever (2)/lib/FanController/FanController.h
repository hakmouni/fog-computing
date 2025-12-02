#ifndef FAN_CONTROLLER_H
#define FAN_CONTROLLER_H

#include <Arduino.h>
#include "driver/mcpwm.h"

class FanController {
public:
    // Constructor
    FanController(int escPin);

    // Initialize the fan (sets up MCPWM and arms the ESC)
    void initFan();

    // Set fan speed as a percentage (0 to 100)
    void setFanSpeedPercentage(int percentage);
    void armESC() ;
    void enterProgrammingMode() ;

    int escPin; // GPIO pin for the ESC signal wire

    // Helper function to set the ESC signal in microseconds
    void setESCSignal(int microseconds);
};

#endif // FAN_CONTROLLER_H