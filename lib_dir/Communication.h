/* Identification:

Travail : TRAVAIL_PRATIQUE_7
Section # : 3
Équipe # : 6467
Correcteur : Paul Petibon

Description du programme:

Le programme ci-dessous permet de créer un objet permettant à la communication par RS232.
*/

#include <stdint.h>
#include <avr/io.h>
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

class Communication
{
    public:
    Communication();
    ~Communication();
    void transmissionUART(uint8_t data);
    void resetRegisters();
    void transmitString(const char *data);
    uint8_t receive();
};



#endif