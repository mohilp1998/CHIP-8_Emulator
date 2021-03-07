#include <iostream>
#include <vector>
#include "include/rom.h"
#include "include/memory.h"
#include "include/opcodeHandler.h"
#include "include/graphics.h"

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
    SDL_Window* gWindow = NULL; // Window for doing rendering
    SDL_Renderer* gRenderer = NULL; // rendering we will using to render
    if (!myGraphics.InitSDL(gWindow, gRenderer))
    {
        std::fprintf(myDebugFile,"[E] <main.cpp>::Unable to Initialize SDL\n");
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

    // Reading instructions using opcode handlers
    for (size_t i = 0; i < 27; i++)
    {
        if (!myOpcodeHandler.readNxtInstr(&myMemory))
        {
            std::fprintf(myDebugFile,"[E] <main.cpp>::Error reading nxt instruction\n");
            std::fprintf(stderr, "ERROR stopping code execution\n");
            std::exit(1);
        }
        if (!myOpcodeHandler.emulateInstr(&myMemory))
        {
            std::fprintf(myDebugFile,"[E] <main.cpp>::Error emulating instruction\n");
            std::fprintf(stderr, "ERROR stopping code execution\n");
            std::exit(1);
        }
    }

    // for (size_t i = 0x50; i < 0x50+80; ++i)
    // {
    //     std::printf("Instruction at position %llu: %hhx\n", i, myMemory.getMemData(i));
    // }

    std::fclose(myDebugFile);
    std::fprintf(stdout, "Code execution complete\n");
    return 0;
}