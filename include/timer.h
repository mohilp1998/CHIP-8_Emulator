#pragma once
#include <iostream>

class timer
{
private:
    /*File for debugging*/
    std::FILE *myDebugFile;

    // Delay Timer
    unsigned char delayTimer {0};

    // Sound Timer
    unsigned char soundTimer {0};

public:
    timer(std::FILE *debugFile);
    ~timer();

    // Delay timer getter-setter method
    unsigned char getDelayTimer();
    bool setDelayTimer(unsigned char val);

    // Sound timer setter method
    bool setSoundTimer(unsigned char val);

    // Update method to update timer values called at 60Hz rate in main
    bool updateTimer();
};
