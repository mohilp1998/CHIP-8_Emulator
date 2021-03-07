#include "include/memory.h"

Memory::Memory(std::FILE *debugFile)
{
    // Debugging logs
    myDebugFile = debugFile;

    // Loading fonts 0-F sprites at locations 0x0050 to 0x00A0
    uint8_t fontset[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for(unsigned int i = 0 ; i < 80; i++)
    {
        mem[0x0050 + i] = fontset[i];
    }
}

Memory::~Memory()
{
    // Empty
}

bool Memory::init(Rom *rom)
{
    // Getting data from the Rom
    if (!rom->getRomData(mem))
    {
        std::fprintf(myDebugFile,"[E] <memory.cpp>::Error getting ROM data\n");
        return false;
    }

    std::fprintf(myDebugFile,"[I] <memory.cpp>::Memory initialized with size: %zu bytes\n", mem.size());
    return true;
}

bool Memory::setMemData(unsigned char data, unsigned short addr)
{
    // Updating memory address location
    mem[addr] = data;
    return true;
}

unsigned char Memory::getMemData(unsigned short addr)
{
    return mem[addr];
}

/////////////////////////////////////////////
/* Moving the registers to opCode Handler */
/////////////////////////////////////////////
// bool Memory::setRegData(unsigned char data, size_t addr)
// {
//     // Updating reg address location
//     reg[addr] = data;
//     return true;
// }

// unsigned char Memory::getRegData(size_t addr)
// {
//     return reg[addr];
// }