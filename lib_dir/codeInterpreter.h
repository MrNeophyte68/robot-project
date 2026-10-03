#ifndef CODEINTERPRETER_H
#define CODEINTERPRETER_H
#include "Communication.h"
#include "memoire_24.h"
#include "LED.h"
#include "codeInterpreter.h"
#include "DelayMaker.h"
#include "Wheel.h"
#include "SoundMaker.h"

class codeInterpreter
{
    public:
    static const uint8_t DBT = 0x01;
    static const uint8_t ATT = 0x02;
    static const uint8_t DAL = 0x44;
    static const uint8_t DET = 0x45;
    static const uint8_t SGO = 0x48;
    static const uint8_t SAR = 0x09;
    static const uint8_t MAR0 = 0x60;
    static const uint8_t MAR1 = 0x61;
    static const uint8_t MAV = 0x62;
    static const uint8_t MRE = 0x63;
    static const uint8_t TRD = 0x64;
    static const uint8_t TRG = 0x65;
    static const uint8_t DBC = 0xC0;
    static const uint8_t FBC = 0xC1;
    static const uint8_t FIN = 0xFF;
    uint16_t currentAddress = 0x0000;
    Communication com;
    Memoire24CXXX mem;
    uint16_t nbAddress = 0x0000;
    bool isDBT = false;
    LED led = LED(&DDRA, &PORTA, PA0, PA1);
    uint8_t ByteCodeInstruction;
    uint8_t ByteCodeOperand;
    Wheel wheel;
    uint16_t savedAddress;
    uint8_t boucleCount;
    SoundMaker soundmaker;

    
    void save();
    void run();
    void interpret(uint8_t& ByteCodeInstruction, uint8_t& ByteCodeOperand);
    void test();

};

extern codeInterpreter globalInterpreter;

#endif