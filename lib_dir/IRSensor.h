#ifndef IRSENSOR_H
#define IRSENSOR_H
#include <avr/io.h>
#include <util/delay.h>
#include "Wheel.h"
#include "Communication.h"
#include "can.h"

class IRSensor {

    public:
        IRSensor(uint8_t pin);
        uint8_t getMeasuredValue();
        void measureValue();

    private:
        uint8_t pin_;
        uint8_t measuredValue_;
        //uint8_t compareValue_;
        
        //Communication comm;
        can converter;
        //Wheel wheels;
};

#endif