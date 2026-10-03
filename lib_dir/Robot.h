#ifndef ROBOT_H
#define ROBOT_H
#include "Communication.h"
#include "memoire_24.h"
#include "LED.h"
#include "codeInterpreter.h"
#include "DelayMaker.h"
#include "Wheel.h"
#include "SoundMaker.h"
#include "MakerLine.h"
#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "timer2.h"

enum class State{

  Start,
  Normal,
  Motor,
  End


};



/* struct Symbol {
    bool found = false;
    uint8_t position = 0;
}; */

class Robot{
    private:
        
        timer2 timer;
        
        Communication com;
        Memoire24CXXX mem;

        uint16_t currentAddress = 0x0000;
        uint16_t nAddress = 0x0000;
        uint16_t circuitAddress1 = 0x0000;
        uint16_t circuitAddress2 = 0x0000;
        uint16_t circuitAddress3 = 0x0000;

        char circuitChosen;
        const char* circuit1 = R"(
          _____               \__                      __
        ||     ||                \__              | __/  |                 
 A -----||     ||----- B -----      \----- C -----|{__   |----- D            
        ||_____||                                 |   \__|                 
    
      )";

        const char* circuit2 = R"(
          _____                __                    _____
         /     \              |  \__ |             ||     ||                 
 A -----(       )----- B -----|   __}|----- C -----||     ||----- D            
         \_____/              |__/   |             ||_____||                 
    
      )";

        const char* circuit3 = R"(
          _____                 _____                   __/
        ||     ||              /     \               __/                    
 A -----||     ||----- B -----(       )----- C -----/      ----- D            
        ||_____||              \_____/                                     
    
      )";
        const char* pillar = "(O)";
        const char* goingRight = ">>>";
        const char* goingLeft = "<<<";
        const char* line = "---";

        bool s;
        bool d;
        bool t;
        bool c; 

        bool circuit1Case1;
        bool circuit1Case2;
        bool circuit1Case3;

        bool circuit2Case1;
        bool circuit2Case2; 
        bool circuit2Case3; 

        bool circuit3Case1;
        bool circuit3Case2;
        bool circuit3Case3;

        char buf1[300];
        char buf2[300];
        
        void replaceLetter(const char* input, char* output, char letter, const char* replace);
        const char* choseCircuit();
        void updateValues();
        
        bool isCircuitInMemory();

        void showInfo(const char* robotName, const char* teamNumber);
        void showCircuit();

        
        

        bool isOneLine = false;

public:
    Robot();
    void showAll(const char* robotName, const char* teamNumber);
    void saveAndUpdateCircuit();
    char* modifyCircuit();

    void run();
    void reset();
    volatile State states = State::Start;
    SoundMaker soundmaker;
    MakerLine makerline;
};

#endif