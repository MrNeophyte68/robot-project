#include "Robot.h"
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
#include <avr/interrupt.h>

Robot::Robot() {
    // code
}

void Robot::updateValues()
{
    s = makerline.square.found;
    d = makerline.diagonal.found;
    t = makerline.triangle.found;
    c = makerline.circle.found; 

    circuit1Case1 = (s && d && t && !c);
    circuit1Case2 = (!s && d && t && !c);
    circuit1Case3 = (s && d && !t && !c);

    circuit2Case1 = (s && !d && t && c);
    circuit2Case2 = (s && !d && t && !c);
    circuit2Case3 = (!s && !d && t && c);

    circuit3Case1 = (s && d && !t && c);
    circuit3Case2 = (s && !d && !t && c);
    circuit3Case3 = (!s && d && !t && c);
}

void Robot::showInfo(const char* robotName, const char* teamNumber)
{
    com.transmitString("\n");
    com.transmitString("robot   : ");
    com.transmitString(robotName);
    com.transmitString("\n");
    com.transmitString("equipe  : ");
    com.transmitString(teamNumber);
    com.transmitString("\n");
}

 // Function to replace a letter in a string with another string
void Robot::replaceLetter(const char* input, char* output, char letter, const char* replace)
{   
    uint16_t maxSize = 300;
    uint16_t j = 0;
    /* for (uint16_t i = 0; input[i] != '\0' && j + 1 < maxSize; i++)
    {
        if (input[i] == letter)
        {

            for (uint16_t k = 0; replace[k] != '\0' && j + 1 < maxSize; k++)
            {
                output[j++] = replace[k];
            }
        }
        else
        {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';  */
    for (uint16_t i = 0; input[i] != '\0' && j + 1 < maxSize; i++)
    {
        output[j++] = input[i];
    }
    output[j] = '\0';
    for (int q = 0; q < maxSize; q++){
        if (output[q] == letter)
        {
            if (replace == ">>>")
            {
                output[q-1] = '>';
                output[q] = '>';
                output[q+1] = '>';
            }
            else if (replace == "<<<")
            {
                output[q-1] = '<';
                output[q] = '<';
                output[q+1] = '<';
            }
            else if (replace == "(O)")
            {
                output[q-1] = '(';
                output[q] = 'O';
                output[q+1] = ')';
            }
            else if (replace == "---")
            {
                output[q-1] = '-';
                output[q] = '-';
                output[q+1] = '-';
            }
        }
    }
}

// Function that chooses the circuit based on the symbols detected
const char* Robot::choseCircuit() 
{   
    updateValues();
    if (circuit1Case1 || circuit1Case2 || circuit1Case3){
        circuitChosen = '1';
        return circuit1;
    }
    if (circuit2Case1 || circuit2Case2 || circuit2Case3){ 
        circuitChosen = '2';
        return circuit2;
    }
    if (circuit3Case1 || circuit3Case2 || circuit3Case3){
        circuitChosen = '3';
        return circuit3;
    }
    return "No circuit matching the symbols detected";
}

// Function that modifies the circuit chosen based on the position of each symbols
char* Robot::modifyCircuit()
{
    const char* circuit = choseCircuit();
    
    if (circuit == circuit1) {
    
        if (circuit1Case2) {
            if (makerline.pillar1.found) {
                replaceLetter(circuit, buf1, 'C', pillar);
                replaceLetter(buf1, buf2, 'B', goingRight);
                replaceLetter(buf2, buf1, 'A', line);
                replaceLetter(buf1, buf2, 'D', line);
                return buf2;
            } 
            else {
                replaceLetter(circuit, buf1, 'C', line);
                replaceLetter(buf1, buf2, 'B', goingRight);
                replaceLetter(buf2, buf1, 'A', line);
                replaceLetter(buf1, buf2, 'D', line);
                return buf2;
            }
        }
        
        else if(circuit1Case3) {
          if (makerline.pillar1.found) {
                replaceLetter(circuit, buf1, 'B', pillar);
                replaceLetter(buf1, buf2, 'C', goingLeft);
                replaceLetter(buf2, buf1, 'A', line);
                replaceLetter(buf1, buf2, 'D', line);
                return buf2;
            } 
            else {
                replaceLetter(circuit, buf1, 'B', line);
                replaceLetter(buf1, buf2, 'C', goingLeft);
                replaceLetter(buf2, buf1, 'A', line);
                replaceLetter(buf1, buf2, 'D', line);
                return buf2;
            }  
        }
    
    
        else if (circuit1Case1) {
            if (makerline.square.position < makerline.triangle.position) {
                if (!(makerline.pillar1.found) && !(makerline.pillar2.found)) {
                    replaceLetter(circuit, buf1, 'A', goingRight);
                    replaceLetter(buf1, buf2, 'B', line);
                    replaceLetter(buf2, buf1, 'C', line);
                    replaceLetter(buf1, buf2, 'D', line);
                    return buf2;
                }
                
                if (makerline.pillar1.found && !(makerline.pillar2.found)) {
                    if (makerline.pillar1.position == 2) {
                        replaceLetter(circuit, buf1, 'A', goingRight);
                        replaceLetter(buf1, buf2, 'B', pillar);
                        replaceLetter(buf2, buf1, 'C', line);
                        replaceLetter(buf1, buf2, 'D', line);
                        return buf2;
                    }
                    else {
                        replaceLetter(circuit, buf1, 'A', goingRight);
                        replaceLetter(buf1, buf2, 'B', line);
                        replaceLetter(buf2, buf1, 'C', pillar);
                        replaceLetter(buf1, buf2, 'D', line);
                        return buf2;
                     }
                 }
             
                 if (makerline.pillar1.found && makerline.pillar2.found) {
                    replaceLetter(circuit, buf1, 'A', goingRight);
                    replaceLetter(buf1, buf2, 'B', pillar);
                    replaceLetter(buf2, buf1, 'C', pillar);
                    replaceLetter(buf1, buf2, 'D', line);
                    return buf2;
                 }
             }
         
         
             else if (makerline.square.position > makerline.triangle.position) {
                    if (!(makerline.pillar1.found) && !(makerline.pillar2.found)) {
                    replaceLetter(circuit, buf1, 'A', line);
                    replaceLetter(buf1, buf2, 'B', line);
                    replaceLetter(buf2, buf1, 'C', line);
                    replaceLetter(buf1, buf2, 'D', goingLeft);
                    return buf2;
                 }
                 
                 if (makerline.pillar1.found && !(makerline.pillar2.found)) {
                    if (makerline.pillar1.position == 2) {
                        replaceLetter(circuit, buf1, 'A', line);
                        replaceLetter(buf1, buf2, 'B', line);
                        replaceLetter(buf2, buf1, 'C', pillar);
                        replaceLetter(buf1, buf2, 'D', goingLeft);
                        return buf2;
                     }
                     else {
                        replaceLetter(circuit, buf1, 'A', line);
                        replaceLetter(buf1, buf2, 'B', pillar);
                        replaceLetter(buf2, buf1, 'C', line);
                        replaceLetter(buf1, buf2, 'D', goingLeft);
                        return buf2;
                     }
                        }
                    
                        if (makerline.pillar1.found && makerline.pillar2.found) {
                            replaceLetter(circuit, buf1, 'A', line);
                            replaceLetter(buf1, buf2, 'B', pillar);
                            replaceLetter(buf2, buf1, 'C', pillar);
                            replaceLetter(buf1, buf2, 'D', goingLeft);
                            return buf2;
                        }
                    }
                }
            
            }
        
            else if (circuit == circuit2) {
                
                if (circuit2Case2) {
                    if (makerline.pillar1.found) {
                        replaceLetter(circuit, buf1, 'C', pillar);
                        replaceLetter(buf1, buf2, 'B', goingRight);
                        replaceLetter(buf2, buf1, 'A', line);
                        replaceLetter(buf1, buf2, 'D', line);
                        return buf2;
                    } 
                    else {
                        replaceLetter(circuit, buf1, 'C', line);
                        replaceLetter(buf1, buf2, 'B', goingRight);
                        replaceLetter(buf2, buf1, 'A', line);
                        replaceLetter(buf1, buf2, 'D', line);
                        return buf2;
                    }
                }
                
                else if(circuit2Case3) {
                  if (makerline.pillar1.found) {
                        replaceLetter(circuit, buf1, 'B', pillar);
                        replaceLetter(buf1, buf2, 'C', goingLeft);
                        replaceLetter(buf2, buf1, 'A', line);
                        replaceLetter(buf1, buf2, 'D', line);
                        return buf2;
                    } 
                    else {
                        replaceLetter(circuit, buf1, 'B', line);
                        replaceLetter(buf1, buf2, 'C', goingLeft);
                        replaceLetter(buf2, buf1, 'A', line);
                        replaceLetter(buf1, buf2, 'D', line);
                        return buf2;
                    }  
                }
            
                else if (circuit2Case1) {
                    if (makerline.circle.position < makerline.square.position) {
                        if (!(makerline.pillar1.found) && !(makerline.pillar2.found)) {
                            replaceLetter(circuit, buf1, 'A', goingRight);
                            replaceLetter(buf1, buf2, 'B', line);
                            replaceLetter(buf2, buf1, 'C', line);
                            replaceLetter(buf1, buf2, 'D', line);
                            return buf2;
                        }
                        
                        if (makerline.pillar1.found && !(makerline.pillar2.found)) {
                            if (makerline.pillar1.position == 2) {
                                replaceLetter(circuit, buf1, 'A', goingRight);
                                replaceLetter(buf1, buf2, 'B', pillar);
                                replaceLetter(buf2, buf1, 'C', line);
                                replaceLetter(buf1, buf2, 'D', line);
                                return buf2;
                            }
                            else{
                                replaceLetter(circuit, buf1, 'A', goingRight);
                                replaceLetter(buf1, buf2, 'B', line);
                                replaceLetter(buf2, buf1, 'C', pillar);
                                replaceLetter(buf1, buf2, 'D', line);
                                return buf2;
                            }
                        }
                    
                        if (makerline.pillar1.found && makerline.pillar2.found) {
                            replaceLetter(circuit, buf1, 'A', goingRight);
                            replaceLetter(buf1, buf2, 'B', pillar);
                            replaceLetter(buf2, buf1, 'C', pillar);
                            replaceLetter(buf1, buf2, 'D', line);
                            return buf2;
                        }
                    }
                
                
                    else if (makerline.circle.position > makerline.square.position) {
                            if (!(makerline.pillar1.found) && !(makerline.pillar2.found)) {
                            replaceLetter(circuit, buf1, 'A', line);
                            replaceLetter(buf1, buf2, 'B', line);
                            replaceLetter(buf2, buf1, 'C', line);
                            replaceLetter(buf1, buf2, 'D', goingLeft);
                            return buf2;
                        }
                        
                        if (makerline.pillar1.found && !(makerline.pillar2.found)) {
                            if (makerline.pillar1.position == 2) {
                                replaceLetter(circuit, buf1, 'A', line);
                                replaceLetter(buf1, buf2, 'B', line);
                                replaceLetter(buf2, buf1, 'C', pillar);
                                replaceLetter(buf1, buf2, 'D', goingLeft);
                                return buf2;
                            }
                            else{
                                replaceLetter(circuit, buf1, 'A', line);
                                replaceLetter(buf1, buf2, 'B', pillar);
                                replaceLetter(buf2, buf1, 'C', line);
                                replaceLetter(buf1, buf2, 'D', goingLeft);
                                return buf2;
                            }
                            
                    }
                
                        if (makerline.pillar1.found && makerline.pillar2.found) {
                            replaceLetter(circuit, buf1, 'A', line);
                            replaceLetter(buf1, buf2, 'B', pillar);
                            replaceLetter(buf2, buf1, 'C', pillar);
                            replaceLetter(buf1, buf2, 'D', goingLeft);
                            return buf2;
                        
                        }
                    }
                }
            }
        
            else if (circuit == circuit3) {
                
                if (circuit3Case3) {
                    if (makerline.pillar1.found) {
                        replaceLetter(circuit, buf1, 'C', pillar);
                        replaceLetter(buf1, buf2, 'B', goingRight);
                        replaceLetter(buf2, buf1, 'A', line);
                        replaceLetter(buf1, buf2, 'D', line);
                        return buf2;
                    } 
                    else {
                        replaceLetter(circuit, buf1, 'C', line);
                        replaceLetter(buf1, buf2, 'B', goingRight);
                        replaceLetter(buf2, buf1, 'A', line);
                        replaceLetter(buf1, buf2, 'D', line);
                        return buf2;
                    }
                }
                
                else if(circuit3Case2) {
                  if (makerline.pillar1.found) {
                        replaceLetter(circuit, buf1, 'B', pillar);
                        replaceLetter(buf1, buf2, 'C', goingLeft);
                        replaceLetter(buf2, buf1, 'A', line);
                        replaceLetter(buf1, buf2, 'D', line);
                        
                        return buf2;
                    } 
                    else {
                        replaceLetter(circuit, buf1, 'B', line);
                        replaceLetter(buf1, buf2, 'C', goingLeft);
                        replaceLetter(buf2, buf1, 'A', line);
                        replaceLetter(buf1, buf2, 'D', line);
                        return buf2;
                    }  
                }
            
                else if (circuit3Case1) {
                    if (makerline.square.position < makerline.diagonal.position) {
                        if (!(makerline.pillar1.found) && !(makerline.pillar2.found)) {
                            replaceLetter(circuit, buf1, 'A', goingRight);
                            replaceLetter(buf1, buf2, 'B', line);
                            replaceLetter(buf2, buf1, 'C', line);
                            replaceLetter(buf1, buf2, 'D', line);
                            return buf2;
                        }
                        
                        else if (makerline.pillar1.found && !(makerline.pillar2.found)) {
                            if (makerline.pillar1.position == 2) {
                                replaceLetter(circuit, buf1, 'A', goingRight);
                                replaceLetter(buf1, buf2, 'B', pillar);
                                replaceLetter(buf2, buf1, 'C', line);
                                replaceLetter(buf1, buf2, 'D', line);
                                return buf2;
                            }
                            else{
                                replaceLetter(circuit, buf1, 'A', goingRight);
                                replaceLetter(buf1, buf2, 'B', line);
                                replaceLetter(buf2, buf1, 'C', pillar);
                                replaceLetter(buf1, buf2, 'D', line);
                                return buf2;
                            }
                        }
                    
                        else if (makerline.pillar1.found && makerline.pillar2.found) {
                            replaceLetter(circuit, buf1, 'A', goingRight);
                            replaceLetter(buf1, buf2, 'B', pillar);
                            replaceLetter(buf2, buf1, 'C', pillar);
                            replaceLetter(buf1, buf2, 'D', line);
                            return buf2;
                        }
                    }
                
                
                
                    else if (makerline.square.position > makerline.diagonal.position) {
                            if (!(makerline.pillar1.found) && !(makerline.pillar2.found)) {
                            replaceLetter(circuit, buf1, 'A', line);
                            replaceLetter(buf1, buf2, 'B', line);
                            replaceLetter(buf2, buf1, 'C', line);
                            replaceLetter(buf1, buf2, 'D', goingLeft);
                            return buf2;
                        }
                        
                            if (makerline.pillar1.found && !(makerline.pillar2.found)) {
                                if (makerline.pillar1.position == 2) {
                                    replaceLetter(circuit, buf1, 'A', line);
                                    replaceLetter(buf1, buf2, 'B', line);
                                    replaceLetter(buf2, buf1, 'C', pillar);
                                    replaceLetter(buf1, buf2, 'D', goingLeft);
                                    return buf2;
                                    }
                                else{
                                    replaceLetter(circuit, buf1, 'A', line);
                                    replaceLetter(buf1, buf2, 'B', pillar);
                                    replaceLetter(buf2, buf1, 'C', line);
                                    replaceLetter(buf1, buf2, 'D', goingLeft);
                                    return buf2;
                                    }
                                }  
                    
                            if (makerline.pillar1.found && makerline.pillar2.found) {
                                replaceLetter(circuit, buf1, 'A', line);
                                replaceLetter(buf1, buf2, 'B', pillar);
                                replaceLetter(buf2, buf1, 'C', pillar);
                                replaceLetter(buf1, buf2, 'D', goingLeft);
                                return buf2;
                            
                            }
                    }
                }
            }
            return "No circuit matching the symbols detected";
        }

bool Robot::isCircuitInMemory()
{
    uint16_t address = 0x0000;
    uint8_t data;
    while(address < nAddress){
        mem.lecture(address, &data);
        address++;
        if ((data == circuitChosen) && (circuitChosen == '1')){
            currentAddress = circuitAddress1;
            currentAddress++;
            return true;
        }
        else if ((data == circuitChosen) && (circuitChosen == '2')){
            currentAddress = circuitAddress2;
            currentAddress++;
            return true;
        }
        else if ((data == circuitChosen) && (circuitChosen == '3')){
            currentAddress = circuitAddress3;
            currentAddress++;
            return true;
        }
    }
    return false;
}

void Robot::saveAndUpdateCircuit()
{
    char* c;
    c = modifyCircuit();
    uint16_t size = strlen(c);

    
    uint16_t index = 0;

    if (!(isCircuitInMemory())) { 
        mem.ecriture(currentAddress, circuitChosen);
        if (circuitChosen == '1'){
            circuitAddress1 = currentAddress;
        }
        else if (circuitChosen == '2'){
            circuitAddress2 = currentAddress;
        }
        else {
            circuitAddress3 = currentAddress;
        }
        currentAddress++;
        nAddress++;

        while (index < size){
            mem.ecriture(currentAddress, c[index]);
            currentAddress++;
            nAddress++;
            index++;
        }

        mem.ecriture(currentAddress, 'F');
        currentAddress++;
        nAddress++;

    }

    else {
        while (index < size){
            mem.ecriture(currentAddress, c[index]);
            currentAddress++;
            index++;
        }
        mem.ecriture(currentAddress, 'F');
        currentAddress = nAddress;
    }

}

void Robot::showCircuit()
{
    uint16_t address = 0x0000;
    uint8_t data;
    while (address < nAddress) {
        mem.lecture(address, &data);
        if (!(data == '1' || data == '2' || data == '3' || data == 'F')) com.transmissionUART(data);
        else if (data == 'F') com.transmitString("\n");
        address++;
    }
}

void Robot::showAll(const char* robotName, const char* teamNumber)
{
    showInfo(robotName, teamNumber);
    showCircuit();
}

void Robot::reset() 
{
    makerline.fullLineCounter = 0;
}



void Robot::run()
{
    timer = timer2(249, 8);
    timer.enable();
    sei();
    while(true)
    {
        switch(states)
        {
            case State::Start:
                makerline.initMakerLine();
                makerline.isCheckingFullLine = true;
                makerline.start();
                makerline.moveOnLine();


                if (makerline.fullLineCounter >= 25 && !makerline.square.found) {
                    makerline.square.found = true;
                }
                else if (makerline.fullLineCounter == 0 && !makerline.diagonal.found) {
                    makerline.diagonal.found = true;
                }
                else if (makerline.fullLineCounter < 20) {
                    isOneLine = true;
                }
                break;
            
            case State::Normal:
                soundmaker.MakeSound({67});
                //timer(20, 1);
                reset();
                makerline.moveOnLine();
                makerline.isCheckingFullLine = false;
                if (!makerline.circle.found && !makerline.square.found)
                {
                    if (makerline.isEmptyLines && isOneLine) {
                        makerline.circle.found = true;
                    }
                    else {
                        makerline.triangle.found = true;
                    }
                }
                break;
        }   
    }
}