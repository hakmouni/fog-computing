//
// Created by user on 02/07/2024.
//
#include "Arduino.h"
#include "Pins.h"
#include "HAL_TCRT5000.h"
#include "esp32-hal-gpio.h"
#include "driver/adc.h"  // Include this for direct ADC access

//getter setter for min max
//fonction calibrate capteur wehed

// Define sensor pins
#define LEFT_4          34
#define LEFT_3          13
#define LEFT_2          36 // vp
#define LEFT_1          35 // 32
#define MID             39 // 26
#define RIGHT_1         27
#define RIGHT_2         4
#define RIGHT_3         15 // 14
#define RIGHT_4         14 // 14

adc1_channel_t HAL_TCRT5000::mapPinToADC1(int pin) {
    switch (pin) {
        case 36:
            adc1Channel = ADC1_CHANNEL_0;
            isADC1 = true;
            break;
        case 39:
            adc1Channel = ADC1_CHANNEL_3;
            isADC1 = true;
            break;
        case 32:
            adc1Channel = ADC1_CHANNEL_4;
            isADC1 = true;
            break;
        case 33:
            adc1Channel = ADC1_CHANNEL_5;
            isADC1 = true;
            break;
        case 34:
            adc1Channel = ADC1_CHANNEL_6;
            isADC1 = true;
            break;
        case 35:
            adc1Channel = ADC1_CHANNEL_7;
            isADC1 = true;
            break;

        default:
            isADC1 = false;
            break; // Invalid pin
    }
    return adc1Channel;
}

adc2_channel_t HAL_TCRT5000::mapPinToADC2(int pin) {
    switch (pin) {

        case 13:
            adc2Channel = ADC2_CHANNEL_4;
            isADC1 = false;
            break;
        case 27:
            adc2Channel = ADC2_CHANNEL_7;
            isADC1 = false;
            break;
        case 4:
            adc2Channel = ADC2_CHANNEL_0;
            isADC1 = false;
            break;
        case 15:
            adc2Channel = ADC2_CHANNEL_3;
            isADC1 = false;
            break;
        case 14:
            adc2Channel = ADC2_CHANNEL_6;
            isADC1 = false;
            break;
        default:
            isADC1 = false;
            break; // Invalid pin
    }
    return adc2Channel;

}
HAL_TCRT5000::HAL_TCRT5000(int sensorPin)
        : sensorPin(sensorPin),
          minValue(4095),
          maxValue(0), sumMaxValue(0), index(0) ,isADC1(true){
}/*bool isADC1;
adc1_channel_t adc1Channel;  // ADC1 channel (if applicable)
adc2_channel_t adc2Channel; */ // ADC2 channel (if applicable)

// Read value
void HAL_TCRT5000::mapPinToADC(int pin) {
    switch (pin) {
        case 36: adc1Channel = ADC1_CHANNEL_0; isADC1 = true; break;
        case 39: adc1Channel = ADC1_CHANNEL_3; isADC1 = true; break;
        case 32: adc1Channel = ADC1_CHANNEL_4; isADC1 = true; break;
        case 33: adc1Channel = ADC1_CHANNEL_5; isADC1 = true; break;
        case 34: adc1Channel = ADC1_CHANNEL_6; isADC1 = true; break;
        case 35: adc1Channel = ADC1_CHANNEL_7; isADC1 = true; break;
        case 13: adc2Channel = ADC2_CHANNEL_4; isADC1 = false; break;
        case 27: adc2Channel = ADC2_CHANNEL_7; isADC1 = false; break;
        case 4:  adc2Channel = ADC2_CHANNEL_0; isADC1 = false; break;
        case 15: adc2Channel = ADC2_CHANNEL_3; isADC1 = false; break;
        case 14: adc2Channel = ADC2_CHANNEL_6; isADC1 = false; break;
        default: isADC1 = false; break; // Invalid pin
    }
}
/*adc1_channel_t mapPinToADC(int pin) {
    switch (pin) {
        case 36: adc1Channel = ADC1_CHANNEL_0; isADC1 = true; break;
        case 39: adc1Channel = ADC1_CHANNEL_3; isADC1 = true; break;
        case 32: adc1Channel = ADC1_CHANNEL_4; isADC1 = true; break;
        case 33: adc1Channel = ADC1_CHANNEL_5; isADC1 = true; break;
        case 34: adc1Channel = ADC1_CHANNEL_6; isADC1 = true; break;
        case 35: adc1Channel = ADC1_CHANNEL_7; isADC1 = true; break;
        default: isADC1 = false; break;
    }
    return adc1Channel;
}

adc2_channel_t mapPinToADC2(int pin) {
    switch (pin) {
        case 13: adc2Channel = ADC2_CHANNEL_4; isADC1 = false; break;
        case 27: adc2Channel = ADC2_CHANNEL_7; isADC1 = false; break;
        case 4:  adc2Channel = ADC2_CHANNEL_0; isADC1 = false; break;
        case 15: adc2Channel = ADC2_CHANNEL_3; isADC1 = false; break;
        case 14: adc2Channel = ADC2_CHANNEL_6; isADC1 = false; break;
        default: isADC1 = false; break;
    }
    return adc2Channel;
}*/

void HAL_TCRT5000::Init() {
    pinMode(sensorPin, INPUT);                // Initialize the sensor pin as input
    mapPinToADC(sensorPin);                   // Map the sensor pin to the appropriate ADC channel
}

uint16_t HAL_TCRT5000::GetValue() {
    unsigned long ct = micros();
    int rawValue = 0;
    int rawValue1 = 0;
    int rawValue2 = 0;
    esp_err_t result;

    adc1_get_raw(mapPinToADC1(LEFT_4));
}
uint16_t HAL_TCRT5000::GetValue1(int pin) {
    unsigned long ct = micros();
    int rawValue = 0;
    int rawValue1 = 0;
    int rawValue2 = 0;
    esp_err_t result;

    adc1_get_raw(mapPinToADC1(pin));
}

void HAL_TCRT5000::PrintValue() const {
  //  Serial.println(GetValue());
}

Color_e HAL_TCRT5000::Color() const {
    return (calibratedBlackValue >370 ? Black : White);
}

int HAL_TCRT5000::GetMinValue() const {
    return minValue;
}

int HAL_TCRT5000::GetMaxValue() const {
    return maxValue;
}

void HAL_TCRT5000::SetMinValue() {
    if (readValue < minValue) {
        minValue = readValue;
    }
}

void HAL_TCRT5000::SetMaxValue() {

    if (readValue<4095 && readValue > maxValue) {
        maxValue = readValue;
    }

}

void HAL_TCRT5000::UpdateCalibration() {
    SetMaxValue();
    SetMinValue();
}

uint16_t HAL_TCRT5000::GetCalibratedValue()  {
    double const tolerance = (maxValue - minValue) * 0;
    calibratedBlackValue=constrain(map(readValue, minValue + (uint16_t)tolerance, maxValue - (uint16_t)tolerance, 0, 1000), 0, 1000);
    return calibratedBlackValue;}

uint16_t HAL_TCRT5000::GetCalibratedValueWhite() {
    calibratedWhiteValue=constrain(map(GetValue(), minValue, maxValue, 0, 1000), 0, 1000);
    return calibratedWhiteValue;
}

void HAL_TCRT5000::PrintCalibratedValue() const {
    //Serial.print(" : ");
    Serial.print(readValue);

}
