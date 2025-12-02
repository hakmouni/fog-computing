//
// Created by hakmouni mohamed on 02/07/2024.
//

#ifndef LINEFOLLOWER_HAL_TCRT5000_H
#define LINEFOLLOWER_HAL_TCRT5000_H

#include <cstdint>
#include "Core_Global.h"

#include "driver/adc.h"


class HAL_TCRT5000 {
public:
    uint8_t sensorPin;
    uint8_t index;
    uint32_t sumMaxValue;
    uint16_t minValue;
    uint16_t maxValue;
    uint16_t calibratedWhiteValue;
    uint16_t calibratedBlackValue;
    uint16_t readValue;
    HAL_TCRT5000() : sensorPin(0) {}
    HAL_TCRT5000(int sensorPin);

    void Init() ;
    bool isADC1;

    uint16_t GetValue() ;
    uint16_t GetValue1(int pin) ;
    void mapPinToADC(int pin) ;
    adc1_channel_t mapPinToADC1(int pin) ;
    adc2_channel_t mapPinToADC2(int pin) ;
    adc1_channel_t adc1Channel;  // ADC1 channel (if applicable)
    adc2_channel_t adc2Channel;
    void PrintValue() const;

    Color_e Color() const;

    int GetMinValue() const;

    int GetMaxValue() const;

    void SetMinValue();

    void SetMaxValue();

    void UpdateCalibration();

    uint16_t GetCalibratedValue() ;

    void PrintCalibratedValue() const;
    uint16_t GetCalibratedValueWhite() ;

};


#endif //LINEFOLLOWER_HAL_TCRT5000_H

