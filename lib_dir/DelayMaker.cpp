#define F_CPU 8000000UL
#include <util/delay.h>
#include "DelayMaker.h"


constexpr uint8_t DEBOUNCE_DELAY_MS_ = 50;
constexpr uint16_t DEBOUNCE_DELAY_US_ = 50000;
constexpr uint8_t minimumCustomDelay_ =10;

DelayMaker::DelayMaker(const Unit& unit) : unit_(unit) {}

void DelayMaker::customDelay(const uint16_t duration)
{

    if (duration % minimumCustomDelay_ != 0 ){
        return;
    }

    for (uint16_t i = 0; i < (duration / minimumCustomDelay_); i++) {
        switch(unit_){
            case Unit::MS:
                _delay_ms(minimumCustomDelay_);
            case Unit::US:
                _delay_us(minimumCustomDelay_);
        }
    }
}

void DelayMaker::delayDebounce()
{
    switch(unit_){
        case Unit::MS:
            _delay_ms(DEBOUNCE_DELAY_MS_);
        break;
        case Unit::US:
            _delay_us(DEBOUNCE_DELAY_US_);
        break;
    }
}