#include "timer2.h"

timer2::timer2() {}

timer2::timer2(uint8_t duration, uint16_t prescaler) : duration_(duration), prescaler_(prescaler)
{
    switch(prescaler_)
    {
        case 1:
        TCCR2B |= (1 << CS20);
        break;
        case 8:
        TCCR2B |= (1 << CS21);
        break;
        case 64:
        TCCR2B |= (1 << CS21) | (1 << CS20);
        break;
        case 256:
        TCCR2B |= (1 << CS22);
        break;
    }

    TCCR2A = 0;
    TCNT2 = 0;
    TCCR2B |= (1 << WGM22);
    OCR2A = duration_;
}

timer2::~timer2() {}

void timer2::enable()
{
    TIMSK2 |= (1 << OCIE2A);
}

void timer2::disable()
{
    TIMSK2 &= ~(1 << OCIE2A);
}

void timer2::reset()
{
    TCNT2 = 0;
}