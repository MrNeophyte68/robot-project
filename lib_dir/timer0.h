/* Identification:

Travail : TRAVAIL_PRATIQUE_7
Section # : 3
Équipe # : 6467
Correcteur : Paul Petibon

Description du programme:

Le programme ci-dessous permet de créer un objet timer0 utilant les registres pour le timer0 du ATMega-324pa.
*/

#ifndef TIMER0_H
#define TIMER0_H
#include <avr/io.h>

class timer0 {

private:

    uint8_t duration_;
    uint16_t prescaler_;



public:
    timer0(uint8_t duration, uint16_t prescaler);
    ~timer0();
    void enable();
    void disable();
    void reset();


};



#endif //TIMER_H
