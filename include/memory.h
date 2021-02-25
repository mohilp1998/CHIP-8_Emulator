#pragma once
#include <vector>
#include <iostream>
#include "rom.h"

class Memory
{
private:
    /*File for debugging*/
    std::FILE *myDebugFile;
    
    // You cannot initialize variables of class outside class method. You must do
    // them in constructor or use initialize method. So we cannot use vector
    // constructor here.

    // Memory space of 4K bytes
    std::vector<unsigned char> mem {std::vector<unsigned char> (4096, 0x00)};
public:
    // Constructor and Destructor
    Memory(std::FILE *debugFile);
    ~Memory();

    bool init(Rom *rom);

    // Memory Data
    bool setMemData(unsigned char data, unsigned short addr);
    unsigned char getMemData(unsigned short addr);

    /////////////////////////////////////////////
    /* Moving the registers to opCode Handler */
    /////////////////////////////////////////////
    // // Register Data
    // bool setRegData(unsigned char data, size_t addr);
    // unsigned char getRegData(size_t addr);
};
