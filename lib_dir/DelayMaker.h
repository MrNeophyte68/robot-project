/* Identification:

Travail : TRAVAIL_PRATIQUE_7
Section # : 3
Équipe # : 6467
Correcteur : Paul Petibon

Description du programme:

Le programme ci-dessous permet de créer délais modifiables selon le besoin d'un code en choissisant l'unité.
*/

#ifndef DELAYMAKER_H
#define DELAYMAKER_H

#include <util/delay.h>
#include <avr/io.h>


enum class Unit {

    MS,
    US

};

class DelayMaker
{
private:


    Unit unit_;

public:
    DelayMaker(const Unit& unit) ;
   
    void customDelay(const uint16_t duration);
    void delayDebounce();
};

#endif