#define F_CPU 8000000

#include <util/delay.h>
#include <avr/io.h> 

#include "LED.h"

const uint8_t LED::GREEN_DURATION_MS_ = 15;
const uint8_t LED::RED_DURATION_MS_ = 10;

LED::LED(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t pinGreen, uint8_t pinRed) : ddr_(ddr), port_(port), pinGreen_(pinGreen), pinRed_(pinRed) 
{
    *ddr_ |= (1 << pinGreen) | (1 << pinRed);
}

void LED::emitAmber()
{
    *port_ = (*port_ | (1 << pinGreen_)) & ~(1 << pinRed_);
    _delay_ms(GREEN_DURATION_MS_);
    *port_ = (*port_ & ~((1 << pinGreen_))) | (1 << pinRed_);
    _delay_ms(RED_DURATION_MS_);
    
}

void LED::inverseColor(uint16_t duration)
{
    for (uint16_t i = 0; i < duration; i++){
        uint16_t j = duration - i;
        for (uint16_t q = 0; q < j; q++) *port_ = (*port_ | (1 << pinGreen_)) & ~(1 << pinRed_);
        for (uint16_t a = 0; a < i; a++) *port_ = (*port_ & ~((1 << pinGreen_))) | (1 << pinRed_);;
    }
}


void LED::changeColor(Color color)
{
    switch(color){
        case Color::GREEN:
        *port_ = (*port_ | (1 << pinGreen_)) & ~(1 << pinRed_);
        break;

        case Color::RED:
        *port_ = (*port_ & ~((1 << pinGreen_))) | (1 << pinRed_);
        break;

        case Color::AMBER:
        while(true) {
            this->emitAmber();
        }
        break;

        case Color::OFF:
        *port_ &= ~((1 << pinGreen_) | (1 << pinRed_));
        break;
    }
}