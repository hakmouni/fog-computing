#include "FanController.h"
#include "Pins.h"

// Constructor: Initializes the ESC pin
FanController::FanController(int escPin) : escPin(escPin) {}

// Initialize the fan (MCPWM setup and ESC arming)
void FanController::initFan() {
    Serial.println("Initializing MCPWM for Fan...");

    // Configure the GPIO pin for MCPWM
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, escPin);

    // Configure MCPWM
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 50;                // 50Hz PWM frequency (20ms period)
    pwm_config.cmpr_a = 0;                    // Duty cycle of PWMxA = 0%
    pwm_config.cmpr_b = 0;                    // Duty cycle of PWMxB = 0% (not used)
    pwm_config.counter_mode = MCPWM_UP_COUNTER; // Up counting mode
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0; // Active high
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);
    //enterProgrammingMode();
    // Arm the ESC with a 1000µs signal
    Serial.println("Arming ESC...");
   setESCSignal(1000); // Send a 1ms pulse (1000µs)
    //delay(3000);        // Wait 3 seconds for ESC to arm

    // Start the motor at 60% speed
    /*setFanSpeedPercentage(15);
    delay(2000);*/

    // Wait 3 seconds for ESC to arm
}

// Set fan speed as a percentage (0 to 100)
void FanController::setFanSpeedPercentage(int percentage) {
    // Constrain the percentage to the range 0-100
    percentage = constrain(percentage, 0, 100);

    // Map the percentage to the ESC signal range (1000 µs to 2000 µs)
    int microseconds = map(percentage, 0, 100, 1000, 2000);

    // Set the ESC signal
    setESCSignal(microseconds);

}
void FanController::enterProgrammingMode() {
    // Send a full throttle signal (2000 µs)
    Serial.println("Entering programming mode...");
    setESCSignal(2000); // Send a 2ms pulse (2000µs)
    delay(3000);        // Wait for 3 seconds
}

// Set the ESC signal in microseconds
void FanController::setESCSignal(int microseconds) {
    // Convert microseconds to duty cycle percentage
    float duty_cycle = (float)microseconds / 20000.0 * 100.0; // 20ms period = 100%
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, duty_cycle);
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);

}
