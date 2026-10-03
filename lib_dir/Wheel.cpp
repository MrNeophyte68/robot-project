#include <avr/io.h> 
#include "Wheel.h"

Wheel::Wheel()
{
    DDRD |= (1 << DDD7) | (1 << DDD6) | (1 << DDD5) | (1 << DDD4) ;

    

    TCCR1A |= (1 << COM1A1) | (0 << COM1A0) | (1 << COM1B1) | (0 << COM1B0) |(1 << WGM10);

    TCCR1B = (1 << CS11);

    TCCR1C = 0;
}



void Wheel::setCompareValue(uint8_t duration1, uint8_t duration2)
{   
    OCR1A = duration1;
    OCR1B = duration2;

}

void Wheel::changeDirection(uint8_t direction, const WheelNumber& wheelNumber)
{
    direction_ = direction;

    switch(wheelNumber){
        case WheelNumber::ONE:
        if(direction_ == 1){
            PORTD &= ~(1 << PD7);
        }
        else{
            PORTD |= (1 << PD7);
        }
        break;

        case WheelNumber::TWO:
        if (direction_ == 1){
            PORTD &= ~(1 << PD6);
        }
        else{
            PORTD |= (1 << PD6);
        }
        break;
    }
}