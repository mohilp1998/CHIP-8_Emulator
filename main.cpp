#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "include/rom.h"
#include "include/memory.h"
#include "include/opcodeHandler.h"
#include "include/graphics.h"
#include "include/keyboard.h"
#include "include/timer.h"

// Method for disabling DEBUGGING FILE TO FILE Basis
// #define DISABLE_DEBUG_LOGS
#ifdef DISABLE_DEBUG_LOGS
    #define fprintf(myDebugFile, fmt, ...) (0)
#endif

const long long int CHIP8_TIMER_SPEED_MICROSECONDS = 16000;

int main(int argc, char *argv[])
{   
    // Creating a debug file which will be sent to everyone for transferring data
    std::FILE *myDebugFile = std::fopen("debug.log","w");
    if (myDebugFile == nullptr)
    {
        std::printf("[E] <main.cpp>::Error Opening debug file\n");
        std::printf("ERROR stopping code execution\n");
        std::exit(1);
    }

    // Initializing the components
    Rom myRom(myDebugFile);
    Memory myMemory(myDebugFile);
    OpcodeHandler myOpcodeHandler(myDebugFile);
    graphics myGraphics(myDebugFile);
    keyboard myKeyboard(myDebugFile);
    timer myTimer(myDebugFile);

    // Loading the Pong rom
    if (!myRom.LoadRom("roms/Pong.ch8"))
    {
        fprintf(myDebugFile,"[E] <main.cpp>::Error Loading Rom\n");
        fprintf(stderr, "ERROR stopping code execution\n");
        std::exit(1);
    }

    // Initializing Memory with pong Rom data
    if(!myMemory.init(&myRom))
    {
        fprintf(myDebugFile,"[E] <main.cpp>::Error initializing memory\n");
        fprintf(stderr, "ERROR stopping code execution\n");
        std::exit(1);
    }

    // Creating SDL variables for graphics
    if (!myGraphics.InitSDL())
    {
        fprintf(myDebugFile,"[E] <main.cpp>::Unable to Initialize SDL\n");
        fprintf(stderr, "ERROR stopping code execution\n");
        std::exit(1);
    }

    // Variable for closing the CHIP-8 Emulator
    bool quit = false;

    // Setting for timer calculation
    std::chrono::steady_clock::time_point beginTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
    long long int durationMicros = -1; // Initial setup

    // Reading instructions using opcode handlers
    while(!quit)
    {
        // Start Time
        if (durationMicros < 0)
        {
            beginTime = std::chrono::steady_clock::now();
        }

        // Reading Instructions
        if (!myOpcodeHandler.readNxtInstr(&myMemory))
        {
            fprintf(myDebugFile,"[E] <main.cpp>::Error reading nxt instruction\n");
            fprintf(stderr, "ERROR stopping code execution\n");
            std::exit(1);
        }

        // Emulating Instruction
        if (!myOpcodeHandler.emulateInstr(&myMemory, &myGraphics, &myKeyboard, &myTimer))
        {
            fprintf(myDebugFile,"[E] <main.cpp>::Error emulating instruction\n");
            fprintf(stderr, "ERROR stopping code execution\n");
            std::exit(1);
        }

        // Drawing the figure only if update Graphics is set
        if(myOpcodeHandler.getGraphicsFlag())
        {
            if(!myGraphics.draw())
            {
                fprintf(myDebugFile,"[E] <main.cpp>::Error Drawing Graphics\n");
                fprintf(stderr, "ERROR stopping code execution\n");
                std::exit(1);
            }
            else
            {
                fprintf(myDebugFile,"[I] <main.cpp>::Updated Screen with new Graphics\n");
            }
        }

        // Updating the quit and keyboard state
        if (!myKeyboard.updateKeyboard(quit))
        {
            fprintf(myDebugFile,"[E] <main.cpp>::Error Updating Keyboard State\n");
            fprintf(stderr, "ERROR stopping code execution\n");
            std::exit(1);
        }

        // End Time
        endTime = std::chrono::steady_clock::now();
        
        // Calculating DurationMicros
        durationMicros = CHIP8_TIMER_SPEED_MICROSECONDS - 
            ( std::chrono::duration_cast<std::chrono::microseconds>(endTime - beginTime).count() );
        // printf("Duration %lld\n", durationMicros);
        if (durationMicros < 0)
        {
            myTimer.updateTimer();
        }
    }

    std::fclose(myDebugFile);
    fprintf(stdout, "Code execution complete\n");
    return 0;
}