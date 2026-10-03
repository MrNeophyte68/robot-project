//
// Created by evenb on 10/21/2025.
//
#include <avr/io.h>
#include "Button.h"
#include <util/delay.h>
#include "LED.h"

void Button::setEdge(ButtonType edge) {
    switch (edge) {
        case(ButtonType::RisingEdge):{

            resetInterrupt();

            EICRA = (1 << ISC00) | (1 << ISC01);

            break;
        }
        case(ButtonType::FallingEdge):{

            resetInterrupt();

            EICRA = (1 << ISC01);

            break;
        }
        case(ButtonType::AnyEdge):{

            resetInterrupt();

            EICRA = (1 << ISC00);

            break;
        }
    }
}

Button::Button(volatile uint8_t *ddr, uint8_t pin, const ButtonType &type):ddr_(ddr), pin_(pin), type_(type) {
    this->setEdge(type_);
    *ddr_ &= ~(1 << pin);
}

Button::Button()
{
    DDRD &= ~(1 << DDD2);
}

bool Button::isButtonPressed()
{
  if(PIND & (1 << PIND2)){
    _delay_ms(1);
    if(PIND & (1 << PIND2)){
      return true;
    }
  }
  return false;
}

void Button::resetInterrupt() {
    EICRA &= ~((1 << ISC00) | (1 << ISC01));
}

void Button::toggleInterrupt(const bool& toggleValue) {
    
    if (toggleValue) {
        EIMSK |= (1 << INT0);
    }
    else {
        EIMSK &= ~(1 << INT0);
    }
}

