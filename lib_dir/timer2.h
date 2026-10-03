/* Identification:

Travail : TRAVAIL_PRATIQUE_7
Section # : 3
Équipe # : 6467
Correcteur : Paul Petibon

Description du programme:

Le programme ci-dessous permet de créer un objet timer2 utilant les registres pour le timer2 du ATMega-324pa.
*/

#ifndef TIMER2_H
#define TIMER2_H
#include <avr/io.h>

class timer2 {

private:

    uint8_t duration_;
    uint16_t prescaler_;



public:
    timer2();
    timer2(uint8_t duration, uint16_t prescaler);
    ~timer2();
    void enable();
    void disable();
    void reset();


};



#endif //TIMER_H