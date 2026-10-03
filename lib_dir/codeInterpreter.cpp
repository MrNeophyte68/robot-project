#include "codeInterpreter.h"
#include "DelayMaker.h"
#include "Wheel.h"
#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>



void codeInterpreter::save() 
{   
    uint8_t byteInstruction;
    uint8_t byteOperand;
    uint16_t address = 0x0000;
    
    byteInstruction = com.receive();
    mem.ecriture(address, byteInstruction);
    nbAddress = (byteInstruction << 8);
    address++;
    byteOperand = com.receive();
    mem.ecriture(address, byteOperand);
    nbAddress |= byteOperand;
    address++;

    while(address < nbAddress)
    {
        uint8_t byteCode = com.receive();
        mem.ecriture(address, byteCode);
        address++;
    }
}

void codeInterpreter::test(){
    uint8_t byteCode;
    uint16_t address = 0x0000;
    char buffer[4];
    while (address < nbAddress)
    {
        mem.lecture(address, &byteCode);
        sprintf(buffer, "%02X\n", byteCode);
        com.transmitString(buffer);
        address++;
    }
    com.transmitString("\n");
}

void codeInterpreter::run()
{

    while(currentAddress < nbAddress)
    {
        mem.lecture(currentAddress, &ByteCodeInstruction);
        currentAddress++;
        mem.lecture(currentAddress, &ByteCodeOperand);
        currentAddress++;
        if (ByteCodeInstruction == DBT || isDBT)
        {
            interpret(ByteCodeInstruction, ByteCodeOperand);
        }
    }
}

void codeInterpreter::interpret(uint8_t& ByteCodeInstruction, uint8_t& ByteCodeOperand)
{

    switch(ByteCodeInstruction)
    {
        case DBT:
        isDBT = true;
        break;
            
        case ATT:
        for(uint16_t i = 0; i < ByteCodeOperand; i++) _delay_ms(25);
        break;

        case DAL:
        if (ByteCodeOperand == 0x01)
        {
            led.changeColor(Color::GREEN);
        }
        else if(ByteCodeOperand == 0x02)
        {
            led.changeColor(Color::RED);
        }
        break;

        case SGO:
        soundmaker.MakeSound({ByteCodeOperand});
        break;

        case SAR:
        soundmaker.stopSound();
        break;

        case DET:
        led.changeColor(Color::OFF);
        break;

        case MAR0:
        wheel.setCompareValue(0);
        break;

        case MAR1:
        wheel.setCompareValue(0);
        break;

        case MAV:
        wheel.setCompareValue(ByteCodeInstruction);
        wheel.changeDirection(0, WheelNumber::ONE);
        wheel.changeDirection(0, WheelNumber::TWO);
        break;

        case MRE:
        wheel.setCompareValue(ByteCodeInstruction);
        wheel.changeDirection(1, WheelNumber::ONE);
        wheel.changeDirection(1, WheelNumber::TWO);
        break;

        case TRD:
        wheel.setCompareValue(127);
        wheel.changeDirection(1, WheelNumber::ONE);
        wheel.changeDirection(0, WheelNumber::TWO);
        _delay_ms(2000);
        wheel.setCompareValue(0);
        break;

        case TRG:
        wheel.setCompareValue(127);
        wheel.changeDirection(0, WheelNumber::ONE);
        wheel.changeDirection(1, WheelNumber::TWO);
        _delay_ms(2000);
        wheel.setCompareValue(0);
        break;

        case DBC:
        savedAddress = currentAddress;
        boucleCount = ByteCodeOperand;
        break;

        case FBC:
        if (boucleCount > 0)
        {
            currentAddress = savedAddress;
            boucleCount--;
        }
        break;


        case FIN:
        isDBT = false;
        break;
    }

}
