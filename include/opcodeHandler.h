#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include "memory.h"
#include "graphics.h"
#include "keyboard.h"
#include "timer.h"

const long long int EMULATION_SPEED_MICROS = 500;

class OpcodeHandler
{
private:
    /*File for debugging*/
    std::FILE *myDebugFile;
    
    /* Registers */
    // 16 registers: V0 to VF. With VF additionally working as carry flag (add),
    // no borrow flag (substract) and is set on pixel collision
    std::vector<unsigned char> reg {std::vector<unsigned char> (16, 0x00)};

    // I register
    unsigned short IReg {0x0000};

    // pc Counter
    unsigned short pc {0x0000};

    // opcode
    unsigned short opcode {0x0000};

    // stack ptr
    std::vector<unsigned short> stack {std::vector<unsigned short> (16, 0x0000)};
    unsigned short sptr {0x0000};

    // sprite addresses
    std::vector<unsigned short> spriteAddr {std::vector<unsigned short> (16, 0x0000)};

    // Graphics Update Flag
    bool m_updateGraphics {false};
public:

    // Functions
    OpcodeHandler(std::FILE *debugFile);
    ~OpcodeHandler();

    bool readNxtInstr(Memory *mem);
    bool emulateInstr(Memory *mem, graphics *myGraphics, keyboard *myKeyboard,
         timer *myTimer);
    bool getGraphicsFlag();
};