/* Identification:

Travail : TRAVAIL_PRATIQUE_7
Section # : 3
Équipe # : 6467
Correcteur : Paul Petibon

Description du programme:

Le programme ci-dessous permet de créer un objet LED en choissant les sorties qui nous conviennent. Une fonction nous permet de changer la couleur.
*/

#ifndef LED_H
#define LED_H

#include <avr/io.h> 

enum class Color {

    GREEN,
    RED,
    AMBER,
    OFF

};

class LED
{
    private:
    volatile uint8_t* ddr_;
    volatile uint8_t* port_;
    const uint8_t pinGreen_;
    const uint8_t pinRed_;
    static const uint8_t GREEN_DURATION_MS_;
    static const uint8_t RED_DURATION_MS_;

    uint8_t inverseColorDuration = 50;
    const uint8_t INVERSE_COLOR_DURATION = 50;
    
    public:
    LED(volatile uint8_t* ddr_, volatile uint8_t* port_, uint8_t pinGreen_, uint8_t pinRed_);
    void changeColor(Color color);
    void emitAmber();
    void inverseColor(uint16_t duration);
    
};


#endif