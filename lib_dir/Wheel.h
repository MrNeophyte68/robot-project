/* Identification:

Travail : TRAVAIL_PRATIQUE_7
Section # : 3
Équipe # : 6467
Correcteur : Paul Petibon

Description du programme:

Le programme ci-dessous permet de controller les roues du robot en modifiant les registres du timer1. On peut changer la direction des roues et augmenter la vitesse de celles-ci.
*/

#ifndef WHEEL_H
#define WHEEL_H

#include <avr/io.h>

enum class WheelNumber
{
    ONE,
    TWO
};

class Wheel
{
    private:
    WheelNumber wheelNumber_;
    volatile uint16_t* durationReg_;
    uint8_t direction_;
    


    public:
    Wheel();
    void setCompareValue(uint8_t duration1, uint8_t duration2);
    void changeDirection(uint8_t direction, const WheelNumber& wheelNumber);
};

#endif
