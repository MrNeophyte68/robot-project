//
// Created by evenb on 10/23/2025.
//

#include "timer0.h"


timer0::timer0(uint8_t duration, uint16_t prescaler) : duration_(duration), prescaler_(prescaler)
{
    switch(prescaler_)
    {
        case 1:
        TCCR0B |= (1 << CS00);
        break;
        case 8:
        TCCR0B |= (1 << CS01);
        break;
        case 64:
        TCCR0B |= (1 << CS01) | (1 << CS00);
        break;
        case 256:
        TCCR0B |= (1 << CS02);
        break;
    }

    
    TCCR0A = 0;
    TCNT0 = 0;
    TCCR0B |= (1 << WGM02);
    OCR0A = duration_;
}

timer0::~timer0() {}

void timer0::enable()
{
    TIMSK0 |= (1 << OCIE0A);
}

void timer0::disable()
{
    TIMSK0 &= ~(1 << OCIE0A);
}

void timer0::reset()
{
    TCNT0 = 0;
}