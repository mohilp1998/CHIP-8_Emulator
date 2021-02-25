#pragma once
#include <iostream>
#include "memory.h"

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
public:
    OpcodeHandler(std::FILE *debugFile);
    ~OpcodeHandler();

    bool readNxtInstr(Memory *mem);
    bool emulateInstr(Memory *mem); // \todo add graphicsHandler, Timer and Inputs
    // \todo in graphicsHandler, Timer and Inputs add flags for next iteration update logic.
    // \todo emulateInstr will update those flags only.
};