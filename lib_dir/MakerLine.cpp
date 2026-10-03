


#include "MakerLine.h"
#include "Wheel.h"
#include "Communication.h"
#include "LED.h"
#include "Debug.h"
#include <util/delay.h>
#include "Button.h"

MakerLine::MakerLine()
{

};

void MakerLine::initMakerLine(){
    DDRA &= ~( (1 << DDA0) | (1 << DDA1) | (1 << DDA2) | (1 << DDA3) | (1 << DDA4) );
}

uint8_t MakerLine::detectLine()
{
    return PINA & 0x1F;
}

void MakerLine::moveRobot(uint8_t speed1, uint8_t speed2, bool rightGoesForward, bool leftGoesForward){ // speed1 right / speed 2 left

    wheel.changeDirection(rightGoesForward, WheelNumber::ONE);
    wheel.changeDirection(leftGoesForward, WheelNumber::TWO);
    wheel.setCompareValue(speed1, speed2);
}

void MakerLine::moveOnLine()
{   
   

    while(!hasEnded) {
        //comm.transmitString("Following a real line");
        // A MODIFIER
        // Enlever les nombres magiques (remplacer les binaires avec des variables privees)

        pattern = detectLine();
        irsensor.measureValue();
        uint8_t IRMeasuredValue = irsensor.getMeasuredValue();
        //soundmaker.stopSound();
        //comm.transmissionUART(IRMeasuredValue);
        if (IRMeasuredValue >= 43 && pillar1.found == false) { //detect at 10 inches from pole | 300 may not be 10 inches
            //comm.transmitString("trouver poto");
            findExecuteType();
            pillar1.found = true;
            
            pillar1.position = currentRobotPosition;
        }
        else if (IRMeasuredValue >= 43 && pillar1.found == true)
        {
            findExecuteType();
            pillar2.found = true;
            
            pillar2.position = currentRobotPosition;
        }//to be fixed

        //comm.transmissionUART(pattern); // Pour debuger
        
        switch (pattern){
        case 0b01110:
        case 0b00100:
        case 0b00110:
        case 0b01100: //straight
            moveRobot(120, 150, true, true);
            for (int i = 0; i < 20; i++) _delay_ms(5);
            moveRobot(50, 50, true, true);
            patternHistory[patternIndex] = 0b00100;
            patternIndex = (patternIndex + 1) % PATTERN_SIZE;
            for (int i = 0; i < 5; i++) _delay_ms(5);
            break;
        case 0b00111:
        case 0b00011:
        case 0b00001:
        case 0b00010://left
            isGoingLeft = true;
            //comm.transmitString("LEFT");
            moveRobot(110,0,true,true);
            for (int i = 0; i < 20; i++) _delay_ms(5);
            moveRobot(50, 50, true, true);
            patternHistory[patternIndex] = 0b00001;
            patternIndex = (patternIndex + 1) % PATTERN_SIZE;
            for (int i = 0; i < 5; i++) _delay_ms(5);
            break;
        case 0b11100:
        case 0b11000:
        case 0b10000:
        case 0b01000://right
            isGoingLeft = false;
            //comm.transmitString("RIGHT");
            moveRobot(0,110,true,true);
            for (int i = 0; i < 20; i++) _delay_ms(5);
            moveRobot(50, 50, true, true);
            patternHistory[patternIndex] = 0b10000;
            patternIndex = (patternIndex + 1) % PATTERN_SIZE;
            for (int i = 0; i < 5; i++) _delay_ms(5);
            break;
        case 0b00000:
            followFakeLineDiagonal(100);
            break;
                
        case 0b11111:
            //GuessShape();
              
            followFakeLine();
            break;
        default:
            moveRobot(0, 0, true, true);
            break;
        }
        //comm.transmissionUART(diagonal.position);
    } 
    }


void MakerLine::followFakeLine(uint8_t counter ) {

    uint16_t shapeTimer = 0;
    uint16_t isCircleCounter = 0;
    detectingShape = true;
    bool firstTimer = true;
    bool secondTimer = true;
    fullLineCounter = 0;

    //comm.transmitString("Following a fake line");

    while(true){

        //if (IRMeasuredValue >= 43) break;

        shapeTimer++;
        //comm.transmissionUART(shapeTimer);

        if (fflCounter >= counter) {
            fflCounter = 0;
            break;
        }
        
        //moveRobot(85, 100, true, true);

        if (fakePatternIndex <= 0){
            fakePatternIndex = PATTERN_SIZE - 1;
        }else{
            fakePatternIndex--;
        }

        fakePattern = patternHistory[fakePatternIndex];
        pattern = detectLine();
        //comm.transmissionUART(fakePattern);

        /*
        SHAPES DETECTION
        */
        comm.transmissionUART(fullLineCounter);
    


        if ((pattern == 0b11111) && detectingShape)
        {
            
            fullLineCounter++;

            
            if (fullLineCounter >= 25 && !square.found && detectingShape){
                lastShapeFound =Shape::Piezo;
                //comm.transmitString("carre");
                currentRobotPosition++;
                square.found = true;
                square.position = currentRobotPosition;
                detectingShape = false;
            }
        }
        else if (shapeTimer > 35 && firstTimer){
            
            if (pattern == 0b00000) {
                //comm.transmitString("timer at 35");
                isCircleCounter++;
                firstTimer = false;
            } 
        }
        else if (shapeTimer > 65 && secondTimer)
        {
            if (pattern == 0b0000)
            {
                //comm.transmitString("timer at 55");
                isCircleCounter++;
            }
                secondTimer = false;


            if (isCircleCounter >= 2 && !circle.found && detectingShape) 
                {
                lastShapeFound =Shape::Motor;
                //comm.transmitString("cercle");
                currentRobotPosition++;
                circle.found = true;
                circle.position = currentRobotPosition;
                detectingShape = false;
                }
            else if (isCircleCounter != 2 && !triangle.found && detectingShape)
                {
                lastShapeFound =Shape::Diode;
                //comm.transmitString("triangle");
                currentRobotPosition++;
                detectingShape = false;
                triangle.found = true;
                triangle.position = currentRobotPosition;
                }
        }

        
        
    

        switch (fakePattern){
            case 0b01110:
            case 0b00100:
            case 0b00110:
            case 0b01100: //straight
                moveRobot(120, 150, true, true);
                for (int i = 0; i < 20; i++) _delay_ms(5);
                moveRobot(50, 50, true, true);
                for (int i = 0; i < 5; i++) _delay_ms(5);
                break;
            case 0b00001:
            case 0b00011:
            case 0b00010://left-- steady
                moveRobot(110,0,true,true);
                for (int i = 0; i < 20; i++) _delay_ms(5);
                moveRobot(50, 50, true, true);
                for (int i = 0; i < 5; i++) _delay_ms(5);
                break;
            case 0b10000:
            case 0b11000:
            case 0b01000://right-- steady
                moveRobot(0,110,true,true);
                for (int i = 0; i < 20; i++) _delay_ms(5);
                moveRobot(50, 50, true, true);
                for (int i = 0; i < 5; i++) _delay_ms(5);
                break;
            default:
                moveRobot(130, 150, true, true);
                for (int i = 0; i < 20; i++) _delay_ms(5);
                moveRobot(50, 50, true, true);
                for (int i = 0; i < 5; i++) _delay_ms(5);
                break;
        }
        fflCounter += 1; 
    }
}



void MakerLine::diagonalBackOnLine(){
    
    do {


        pattern = detectLine();
        if (isGoingLeft){
            moveRobot(0,110,true,true);
        }
        else{
            moveRobot(110,0,true,true);
        }

    }
    while(pattern != 0b01100 && pattern !=  0b00110);

    do {
        pattern = detectLine();
        moveRobot(100, 130, true, true);
        

    }
    while(pattern != 0b00001 && pattern != 0b10000 );

    do {


        pattern = detectLine();
        if (!isGoingLeft){
            moveRobot(0,110,true,true);
            for (int i = 0; i < 20; i++) _delay_ms(5);
            moveRobot(50, 50, true, true);
        }
        else{
            moveRobot(110,0,true,true);
           
        }

    }
    while(pattern != 0b01100 && pattern !=  0b00110);

    do {
        pattern = detectLine();
        moveRobot(120, 150, false, false);
     
    }
    while(pattern != 0b00000);

    moveRobot(120, 150, true, true);
    for (int i = 0; i < 300; i++) _delay_ms(5);

}


void MakerLine::followFakeLineDiagonal(uint8_t counter ) {
   
    detectingShape = true;
    uint16_t emptyLineCounter = 0;
    while(true){

        if (fflCounter >= counter) {
            fflCounter = 0;
            break;
        }
        
        //moveRobot(85, 100, true, true);

        if (fakePatternIndex <= 0){
            fakePatternIndex = PATTERN_SIZE - 1;
        }else{
            fakePatternIndex--;
        }

        fakePattern = patternHistory[fakePatternIndex];
        pattern = detectLine();
        //comm.transmissionUART(fakePattern);

        /*
        SHAPES DETECTION
        */
        if ((pattern == 0b00000) && detectingShape)
        {
            emptyLineCounter++;
             //comm.transmissionUART(fullLineCounter);
            if (emptyLineCounter >= 65 && (currentRobotPosition==3 || currentRobotPosition==5) )
            {
                moveRobot(0,0,true,true);

                EndingLights();

                hasEnded = true;
                break;
            }
            else if (emptyLineCounter >= 65 && !diagonal.found && detectingShape){ //avant cetait 25 (who cares)
                
                lastShapeFound = Shape::Interrupteur;
                comm.transmitString("diagonale");
                currentRobotPosition++;
                //comm.transmissionUART(currentRobotPosition);
                diagonal.found = true;
                diagonal.position = currentRobotPosition;
                detectingShape = false;
                diagonalBackOnLine();
                //shapeTimer = 0;
                
            }
            
            
        }
        else if (detectingShape && emptyLineCounter < 65 ){




            currentRobotPosition++;

            if (pattern == 0b00001 || pattern == 0b10000 || pattern == 0b11000 || pattern == 0b00011){
                comm.transmitString("diagonale");
                lastShapeFound =Shape::Interrupteur;
                
                diagonal.found = true;
                diagonal.position = currentRobotPosition;
                
            }
            
            // gap found
            

            //comm.transmissionUART(currentRobotPosition);
            detectingShape = false;
        }

        
        //comm.transmissionUART(fullLineCounter);

        switch (fakePattern){
            case 0b01110:
            case 0b00100:
            case 0b00110:
            case 0b01100: //straight
                moveRobot(120, 150, true, true);
                for (int i = 0; i < 20; i++) _delay_ms(5);
                moveRobot(50, 50, true, true);
                for (int i = 0; i < 5; i++) _delay_ms(5);
                break;
            case 0b00001:
            case 0b00011:
            case 0b00010://left-- steady
                moveRobot(110,0,true,true);
                for (int i = 0; i < 20; i++) _delay_ms(5);
                moveRobot(50, 50, true, true);
                for (int i = 0; i < 5; i++) _delay_ms(5);
                break;
            case 0b10000:
            case 0b11000:
            case 0b01000://right-- steady
                moveRobot(0,110,true,true);
                for (int i = 0; i < 20; i++) _delay_ms(5);
                moveRobot(50, 50, true, true);
                for (int i = 0; i < 5; i++) _delay_ms(5);
                break;
            default:
                moveRobot(130, 150, true, true);
                for (int i = 0; i < 20; i++) _delay_ms(5);
                moveRobot(50, 50, true, true);
                for (int i = 0; i < 5; i++) _delay_ms(5);
                break;
        }
        fflCounter += 1; 

    }
}

void MakerLine::start(){
 /*    for (uint8_t i = 0; i < PATTERN_SIZE - 1; i++){
        patternHistory[i+1] = 0b00100;
        patternHistory[i-1] = 0b10000;
        patternHistory[i] = 0b00001;
    } */
    led.changeColor(Color::OFF);
    soundmaker.stopSound();
    moveRobot(0, 0, true, true);
    for (int i = 0; i < 3200; i++) _delay_ms(5);
    moveRobot(200, 200, true, true);
    for (int i = 0; i < 600; i++) _delay_ms(5);
}


void MakerLine::EndingLights(){

    

    while (!button.isButtonPressed())
    {
            led.changeColor(Color::RED);
            for (int i = 0; i <165  ; i++) _delay_ms(25);
            led.changeColor(Color::GREEN);
            for (int i = 0; i <165  ; i++) _delay_ms(25);
           
    }

    led.changeColor(Color::GREEN);




}


void MakerLine::findExecuteType()
{

    switch (lastShapeFound)
    {
    case Shape::Diode:
        comm.transmitString("execute diode");
        executeDiode();
        break;
    case Shape::Piezo:
        comm.transmitString("execute piezo");
        executePiezo();
        break;
    case Shape::Motor:
        comm.transmitString("execute moteur");
        executeMotor();
        break;
    case Shape::Interrupteur:
        comm.transmitString("execute interrupteur");
        executeInterruptor();
        break;
   
    }
  /*  if (currentRobotPosition == triangle.position)
    {
        comm.transmitString("execute diode");
        executeDiode();
        /* led.changeColor(Color::GREEN);
        moveRobot(0, 0, true, true);
        for (int i = 0; i < 400; i++) _delay_ms(5); //The robot must stop moving for 2 seconds, the value may be tweaked if duration is not precise
        executeDiode(); //note: executeDiode does not contain the entire behaviour for Diode since the part where it waits for 2sec may be tricky to implement inside a while loop 
    else if (currentRobotPosition == circle.position)
    {
        
    }
    else if (currentRobotPosition == square.position)
    {
        //comm.transmitString("execute piezo");
        //executePiezo();
    }
    else if (currentRobotPosition == diagonal.position)
    {
        //comm.transmitString("execute interruptor");
        //executeInterruptor();
    } */
    /* else
    {
        comm.transmitString("execute type not found");
    }  */
}

void MakerLine::executeDiode() //note: robot has to verify at least 4 times per sec if pole is still there when stopped
{
    
   bool waitToRemovePole = false;
   uint16_t duration = 570;
   uint16_t pwmA = 0;
   led.changeColor(Color::GREEN);
   start();
   while(true){
    pattern = detectLine();
    //if (pwmA > duration) break;
    uint16_t pwmB = duration - pwmA;
    irsensor.measureValue();
    uint8_t IRMeasuredValue = irsensor.getMeasuredValue();

    if (IRMeasuredValue >= 110){
        moveRobot(0, 0, true, true);
        led.changeColor(Color::RED);
        pwmB = 0;
        waitToRemovePole = true;
    }
    else{
        pwmA++;
    }

    if (waitToRemovePole && IRMeasuredValue < 50){
        led.changeColor(Color::OFF);
        start();
        break;
    }

    for (uint16_t i = 0; i < pwmB; i++) led.changeColor(Color::GREEN);
    for (uint16_t a = 0; a < pwmA; a++) led.changeColor(Color::RED);

    switch (pattern){
            case 0b01110:
            case 0b00100:
            case 0b00110:
            case 0b01100: //straight
                moveRobot(80, 100, true, true);
                /* for (int i = 0; i < 20; i++) _delay_ms(5);
                moveRobot(50, 50, true, true);
                patternHistory[patternIndex] = 0b00100;
                patternIndex = (patternIndex + 1) % PATTERN_SIZE;
                for (int i = 0; i < 5; i++) _delay_ms(5); */
                break;
            
            case 0b00111:
            case 0b00011:
            case 0b00001:
            case 0b00010://left
                isGoingLeft = true;
                //comm.transmitString("LEFT");
                moveRobot(120,0,true,true);
                break;
            case 0b11100:
            case 0b11000:
            case 0b10000:
            case 0b01000://right
                isGoingLeft = false;
                //comm.transmitString("RIGHT");
                moveRobot(0,120,true,true);
                break;
            default:
                moveRobot(0, 0, true, true);
                break;
        }
   }
}

void MakerLine::executeMotor()//robot will go around the pole
{
    currentRobotPosition++;
  

    do {
    pattern = detectLine();
    moveRobot(0,120,true,true);
    }while (pattern!= 0b00000);

     moveRobot(190, 210, true, true);
    for (int i = 0; i < 3550; i++) _delay_ms(5); 
    moveRobot(150,0,true,true);
    for (int i = 0; i < 1500; i++) _delay_ms(5); 

    do{
    pattern = detectLine();
    moveRobot(130, 150, true, true);
    }while (pattern != 0b10000);


    moveRobot(130, 150, true, true);
    for (int i = 0; i <800; i++) _delay_ms(5);

    moveRobot(0,0,true,true);
    for (int i = 0; i < 500; i++) _delay_ms(5); 
    
    do{
    pattern = detectLine();
    moveRobot(0, 200, true, true);
    }while (pattern != 0b00110  && pattern!= 0b00100 && pattern!= 0b01100 && pattern !=0b11100 && pattern != 0b11000 && pattern != 0b10000 );

    moveRobot(130, 150, false, false);
    for (int i = 0; i < 1500; i++) _delay_ms(5); 
}

void MakerLine::executePiezo()
{
    uint16_t IRMeasuredValue;
    irsensor.measureValue();
    IRMeasuredValue = irsensor.getMeasuredValue();

    do {
        irsensor.measureValue();
        IRMeasuredValue = irsensor.getMeasuredValue();
        moveRobot(96, 123, true, true);
        
    }  while (IRMeasuredValue <= 80);

    do {
        irsensor.measureValue();
        IRMeasuredValue = irsensor.getMeasuredValue();
        moveRobot(0, 0, true, true);
        for (int i = 0; i < 2; i++) _delay_ms(25);

        soundmaker.MakeSound({80});
        for (int i = 0; i < 300; i++) _delay_ms(25);
        soundmaker.stopSound();
        for (int i = 0; i < 2; i++) _delay_ms(25);

        soundmaker.MakeSound({75});
        for (int i = 0; i < 300; i++) _delay_ms(25);
        soundmaker.stopSound();
        for (int i = 0; i < 2; i++) _delay_ms(25);

        soundmaker.MakeSound({80});
        for (int i = 0; i < 300; i++) _delay_ms(25);
        soundmaker.stopSound();
        for (int i = 0; i < 2; i++) _delay_ms(25);
        //comm.transmissionUART(IRMeasuredValue);
        //for (int i = 0; i < 667; i++) _delay_ms(25); TWO SECONDS WAIT

        const int totalLoops = 667;  
        const int checks = 8;  
        const int loopsPerCheck = totalLoops / checks;

        for (int i = 0; i < checks; i++) {
            irsensor.measureValue();
            IRMeasuredValue = irsensor.getMeasuredValue();

            if (IRMeasuredValue < 60) {    
                break;                    
            }

            for (int j = 0; j < loopsPerCheck; j++) {
                _delay_ms(25);             
            }
        }

    } while (IRMeasuredValue > 60);

        soundmaker.MakeSound({60});
        for (int i = 0; i < 300; i++) _delay_ms(25);
        soundmaker.stopSound();
        for (int i = 0; i < 2; i++) _delay_ms(25);

        soundmaker.MakeSound({67});
        for (int i = 0; i < 300; i++) _delay_ms(25);
        soundmaker.stopSound();
        for (int i = 0; i < 2; i++) _delay_ms(25);

        soundmaker.MakeSound({60});
        for (int i = 0; i < 300; i++) _delay_ms(25);
        soundmaker.stopSound();
        for (int i = 0; i < 2; i++) _delay_ms(25); 
        start();

    /* while(true)
    {
        irsensor.measureValue();
        uint8_t IRMeasuredValue = irsensor.getMeasuredValue();
        if (IRMeasuredValue >= 90) //150 may not be 4 inches
        {
            moveRobot(0, 0, true, true);
            //for (int i = 0; i < 2; i++) _delay_ms(25);

            soundmaker.MakeSound({80});
            for (int i = 0; i < 10; i++) _delay_ms(25);
            soundmaker.stopSound();
            for (int i = 0; i < 2; i++) _delay_ms(25);

            soundmaker.MakeSound({75});
            for (int i = 0; i < 10; i++) _delay_ms(25);
            soundmaker.stopSound();
            for (int i = 0; i < 2; i++) _delay_ms(25);

            soundmaker.MakeSound({80});
            for (int i = 0; i < 10; i++) _delay_ms(25);
            soundmaker.stopSound();
            for (int i = 0; i < 2; i++) _delay_ms(25);

            for (int i = 0; i < 2000; i++);//wait 2sec
            {
                irsensor.measureValue();
                IRMeasuredValue = irsensor.getMeasuredValue();
                if (!(IRMeasuredValue >= 90)) //play song
                {
                    comm.transmitString("PLAYING SONG");
                    soundmaker.MakeSound({80});
                    for (int i = 0; i < 10; i++) _delay_ms(25);
                    soundmaker.stopSound();
                    for (int i = 0; i < 2; i++) _delay_ms(25);

                    soundmaker.MakeSound({80});
                    for (int i = 0; i < 10; i++) _delay_ms(25);
                    soundmaker.stopSound();
                    for (int i = 0; i < 2; i++) _delay_ms(25);

                    soundmaker.MakeSound({80});
                    for (int i = 0; i < 10; i++) _delay_ms(25);
                    soundmaker.stopSound();
                    for (int i = 0; i < 2; i++) _delay_ms(25);

                    return; //exit executePiezo
                } 
                _delay_ms(200);

            }
        }
        else
        {
            moveRobot(96, 123, true, true);//robot has to approach pole slowly until it is at 4inches from pole | note: if robot does not move either give boost or tweak moveRobot speed
        }

    }  */

}

void MakerLine::executeInterruptor()
{
    uint16_t IRMeasuredValue;
    irsensor.measureValue();
    IRMeasuredValue = irsensor.getMeasuredValue();

    do {
        irsensor.measureValue();
        IRMeasuredValue = irsensor.getMeasuredValue();
        switch (pattern){
            case 0b01110:
            case 0b00100:
            case 0b00110:
            case 0b01100: //straight
                moveRobot(80, 100, true, true);
                pattern = detectLine();
                break;
            case 0b00111:
            case 0b00011:
            case 0b00001:
            case 0b00010://left
                moveRobot(120,0,true,true);
                pattern = detectLine();
                break; 
            case 0b11100:
            case 0b11000:
            case 0b10000:
            case 0b01000://right
                moveRobot(0,120,true,true);
                pattern = detectLine();
                break;
            }
        
    }  while (IRMeasuredValue <= 110);

    pattern = detectLine();
    bool isStraight = false;

    moveRobot(0, 0, true, true);
    for (int i = 0; i < 250; i++) _delay_ms(25);
    moveRobot(130, 150, true, false); // ~15 degres antihoraire
    for (int i = 0; i < 150; i++) _delay_ms(25); 
    moveRobot(0, 0, true, true); // arrete un peu
    for (int i = 0; i < 200; i++) _delay_ms(25);
    moveRobot(130, 150, false, true); // ~30 degres horaire
    for (int i = 0; i < 330; i++) _delay_ms(25); 
    moveRobot(0, 0, true, true); // arrete un peu
    for (int i = 0; i < 200; i++) _delay_ms(25); 
    moveRobot(130, 150, true, false); // ~30 degres anti-horaire
    for (int i = 0; i < 340; i++) _delay_ms(25); //480
    moveRobot(0, 0, true, true); // arrete un peu
    for (int i = 0; i < 250; i++) _delay_ms(25);
    moveRobot(130, 150, false, true); // ~15 degres antihoraire
    for (int i = 0; i < 100; i++) _delay_ms(25); 
    moveRobot(0, 0, true, true); // arrete un peu

    while (!(button.isButtonPressed()))
    {
        moveRobot(0,0, true, true);
    }

    for (int i = 0; i < 667; i++) _delay_ms(25); // TWO SECONDS

    irsensor.measureValue();
    IRMeasuredValue = irsensor.getMeasuredValue();

    if (IRMeasuredValue >= 100) 
    {
        executeInterruptor();
    }

    isStraight = false;

    while (!isStraight) {
        pattern = detectLine();
        switch (pattern){
            case 0b01110:
            case 0b00100:
            case 0b00110:
            case 0b01100: //straight
                moveRobot(0, 0, true, true);
                isStraight = true;
                break;
            case 0b00111:
            case 0b00011:
            case 0b00001:
            case 0b00010://left
                moveRobot(120,0,true,true);
                pattern = detectLine();
                break; 
            case 0b11100:
            case 0b11000:
            case 0b10000:
            case 0b01000://right
                moveRobot(0,120,true,true);
                pattern = detectLine();
                break;
            }
    }
    start();
 
}

