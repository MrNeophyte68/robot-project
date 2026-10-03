//
// Created by evenb on 10/28/2025.
//

#ifndef SOUNDMAKER_H
#define SOUNDMAKER_H
#include <avr/io.h>



struct Sound{

    uint8_t note;

};



class SoundMaker {
private:


public:
    SoundMaker();


    void MakeSound(const Sound& sound);
    uint16_t Givefrequency(const uint8_t& note);
    void stopSound();
};



#endif //SOUNDMAKER_H
