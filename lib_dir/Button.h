/* Identification:

Travail : TRAVAIL_PRATIQUE_7
Section # : 3
Équipe # : 6467
Correcteur : Paul Petibon

Description du programme:

Le programme ci-dessous permet de créer un objet bouton en choissiant falling edge, rising ou peu importe. On peut choisir les entrées.
*/


#ifndef BUTTON_H
#define BUTTON_H

enum class ButtonType{
  RisingEdge = 0,
  FallingEdge = 1,
  AnyEdge = 2
};

class Button {

    private:
    volatile uint8_t *ddr_;
    uint8_t pin_;
    ButtonType type_;

    public:

    Button(volatile uint8_t *ddr, uint8_t pin, const ButtonType &type);
    Button();
    //~Button();

    void setEdge(ButtonType edge);
    void toggleInterrupt(const bool& toggleValue);
    void resetInterrupt();
    bool isButtonPressed();
};



#endif //BUTTON_H
