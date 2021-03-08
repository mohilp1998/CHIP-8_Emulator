#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "include/rom.h"
#include "include/memory.h"
#include "include/opcodeHandler.h"
#include "include/graphics.h"

const long long int CHIP8_TIMER_SPEED_MICROSECONDS = 16600;

int main(int argc, char *argv[])
{   
    // Creating a debug file which will be sent to everyone for transferring data
    std::FILE *myDebugFile = std::fopen("debug.log","w");
    if (myDebugFile == nullptr)
    {
        std::printf("Error Opening debug file\n");
        std::exit(1);
    }

    // Initializing the components
    Rom myRom(myDebugFile);
    Memory myMemory(myDebugFile);
    OpcodeHandler myOpcodeHandler(myDebugFile);
    graphics myGraphics(myDebugFile);

    // Creating SDL variables for graphics
    if (!myGraphics.InitSDL())
    {
        std::fprintf(myDebugFile,"[E] <main.cpp>::Unable to Initialize SDL\n");
        std::fprintf(stderr, "ERROR stopping code execution\n");
        std::exit(1);
    }

    // Loading the Pong rom
    if (!myRom.LoadRom("roms/Testing-ROM.ch8"))
    {
        std::fprintf(myDebugFile,"[E] <main.cpp>::Error Loading Rom\n");
        std::fprintf(stderr, "ERROR stopping code execution\n");
        std::exit(1);
    }

    // Initializing Memory with pong Rom data
    if(!myMemory.init(&myRom))
    {
        std::fprintf(myDebugFile,"[E] <main.cpp>::Error initializing memory\n");
        std::fprintf(stderr, "ERROR stopping code execution\n");
        std::exit(1);
    }

    // Setting for proper timing
    std::chrono::steady_clock::time_point beginTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
    long long int durationMicros = -1; // Initially negative as we want the code to calculate time
    // Reading instructions using opcode handlers
    for (size_t i = 0; i < 27; i++)
    {
        // Start Time
        if (durationMicros < 0)
        {
            beginTime = std::chrono::steady_clock::now();
        }

        // Reading Instructions
        if (!myOpcodeHandler.readNxtInstr(&myMemory))
        {
            std::fprintf(myDebugFile,"[E] <main.cpp>::Error reading nxt instruction\n");
            std::fprintf(stderr, "ERROR stopping code execution\n");
            std::exit(1);
        }

        // Emulating Instruction
        if (!myOpcodeHandler.emulateInstr(&myMemory))
        {
            std::fprintf(myDebugFile,"[E] <main.cpp>::Error emulating instruction\n");
            std::fprintf(stderr, "ERROR stopping code execution\n");
            std::exit(1);
        }

        // Drawing the figure
        if(myOpcodeHandler.updateGraphics)
        {
            if(!myGraphics.draw())
            {
                std::fprintf(myDebugFile,"[E] <main.cpp>::Error Drawing Graphics\n");
                std::fprintf(stderr, "ERROR stopping code execution\n");
                std::exit(1);
            }
        }

        // End Time
        endTime = std::chrono::steady_clock::now();
        
        // Calculating DurationMicros
        durationMicros = CHIP8_TIMER_SPEED_MICROSECONDS - 
            ( std::chrono::duration_cast<std::chrono::microseconds>(endTime - beginTime).count() );
        if (durationMicros < 0)
        {
            //\todo Update Time values here
        }
    }

    std::fclose(myDebugFile);
    std::fprintf(stdout, "Code execution complete\n");
    return 0;
}