#ifndef MAKERLIKE_H
#define MAKERLIKE_H
#include <avr/io.h>
#include "Wheel.h"
#include <util/delay.h>
#include "Communication.h"
#include "IRSensor.h"
#include "LED.h"
#include "SoundMaker.h"
#include "timer2.h"
#include "Button.h"

struct Symbol {
    bool found = false;
    uint8_t position = -1;
};

 enum Shape{

  NoShape,
  Piezo,
  Motor,
  Diode,
  Interrupteur,
 


}; 

class MakerLine {

    private:
        static const uint8_t ALL_ON         = 0b11111;
        static const uint8_t ALL_OFF        = 0b00000;
        static const uint8_t FAR_RIGHT      = 0b10000;
        static const uint8_t CLOSE_RIGHT    = 0b01000;
        static const uint8_t MIDDLE         = 0b00100;
        static const uint8_t CLOSE_LEFT     = 0b00010;
        static const uint8_t FAR_LEFT       = 0b00001;

        Wheel wheel;
        Communication comm;
        SoundMaker soundmaker;

        uint8_t pattern;
        uint16_t patternIndex = 0;
        uint8_t patternHistory[90] = {};
        const uint16_t PATTERN_SIZE = 90;
        uint16_t fakePatternIndex = 0;
        uint8_t fakePattern;
        uint16_t fflCounter = 0;
        uint8_t newPattern;

        uint8_t currentRobotPosition = 0;
        
        LED led = LED(&DDRC, &PORTC, PC4, PC5);
        IRSensor irsensor = IRSensor(PA6);
        Button button;

        

        bool detectingShape = false;

        bool isGoingLeft ;
       

        Shape lastShapeFound = NoShape ;

    public:
        MakerLine();
        uint8_t detectLine();
        void moveRobot(uint8_t speed1, uint8_t speed2 ,bool rightGoesForward, bool leftGoesForward);
        void moveOnLine();
       // void GuessShape(GuessState state = GuessState::Base);
        void initMakerLine();
        void followFakeLine(uint8_t counter = 130);

        void diagonalBackOnLine();
        void followFakeLineDiagonal(uint8_t counter = 100);

        void start();

        void EndingLights();
        
        void findExecuteType();
        void executeDiode(); //carre
        void executeMotor(); //cercle
        void executePiezo(); //diagonale
        void executeInterruptor(); //triangle

        //bool isButtonPressed();

        Symbol square;
        Symbol diagonal;
        Symbol triangle;
        Symbol circle;
        Symbol pillar1;
        Symbol pillar2;

         bool hasEnded = false;
        bool isCheckingFullLine = true;
        bool isEmptyLines = false;
        uint8_t fullLineCounter = 0;

};

#endif