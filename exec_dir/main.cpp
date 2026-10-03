#define F_CPU 8000000UL

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "Debug.h"
#include "Button.h"
#include "LED.h"
#include "Communication.h"
#include "can.h"
#include "Wheel.h"
#include "timer0.h"
#include "timer2.h"
#include "DelayMaker.h"
#include "MakerLine.h"
#include "can.h"
#include "Robot.h"

Robot jarvis;
Button b;

int main()
{
    jarvis.makerline.initMakerLine();
    jarvis.makerline.start();
    jarvis.makerline.moveOnLine();
    while (!jarvis.makerline.hasEnded){}
    jarvis.saveAndUpdateCircuit();
    jarvis.showAll("Jarvis", "6467");
    while(jarvis.makerline.hasEnded){
        if (b.isButtonPressed()) jarvis.makerline.hasEnded = false;
    }
    jarvis.makerline.start();
    jarvis.makerline.moveOnLine();
    while (!jarvis.makerline.hasEnded){}
    jarvis.saveAndUpdateCircuit();
    jarvis.showAll("Jarvis", "6467");
    while(jarvis.makerline.hasEnded){
        if (b.isButtonPressed()) jarvis.makerline.hasEnded = false;
    }
    jarvis.makerline.start();
    jarvis.makerline.moveOnLine();
    while (!jarvis.makerline.hasEnded){}
    jarvis.saveAndUpdateCircuit();
    jarvis.showAll("Jarvis", "6467");
}
