#include <avr/io.h>
#include <util/delay.h>
#include "IRSensor.h"
#include "Wheel.h"
#include "Communication.h"
#include "can.h"

IRSensor::IRSensor(uint8_t pin) : pin_(pin) {}

uint8_t IRSensor::getMeasuredValue()
{
    return measuredValue_;
}

void IRSensor::measureValue()
{
    measuredValue_ = converter.lecture(pin_) >> 2;
};