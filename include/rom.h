#pragma once
#include <vector>
#include <iostream>
#include <cstdlib>

class Rom
{
private:
    /*File for debugging*/
    std::FILE *myDebugFile;

    /* data */
    unsigned char *dataBuffer {nullptr};
    long int fileSize;
public:
    Rom(std::FILE *debugFile);
    // Free the dataBuffer if there is something in it
    ~Rom();

    // Load dataBuffer from the ROM data after deleting any existing dataBuffer.
    bool LoadRom(const char *romPath);
    // Copy the data buffer to memory
    bool getRomData(std::vector<unsigned char>& memory);
};
