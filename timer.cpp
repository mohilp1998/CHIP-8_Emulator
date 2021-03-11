#include "include/timer.h"

// Method for disabling DEBUGGING FILE TO FILE Basis
// #define DISABLE_DEBUG_LOGS
#ifdef DISABLE_DEBUG_LOGS
    #define fprintf(myDebugFile, fmt, ...) (0)
#endif

timer::timer(std::FILE *debugFile)
{
    // Debugging logs
    myDebugFile = debugFile;
}

timer::~timer()
{
}

unsigned char timer::getDelayTimer()
{
    return delayTimer;
}

bool timer::setDelayTimer(unsigned char val)
{
    delayTimer = val;
    return true;
}

bool timer::setSoundTimer(unsigned char val)
{
    soundTimer = val;
    return true;
}

bool timer::updateTimer()
{
    if (delayTimer > 0)
    {
        delayTimer--;
    }

    if (soundTimer > 0)
    {
        // \todo add logic to do real BEEPS.
        soundTimer--;
        if ((soundTimer % 10) == 0)
        {
            printf("BEEP!\n");
        }
    }
    std::fprintf(myDebugFile,"[I] <timer.cpp>::Updated Timers - Delay Timer:%hhx & Sound Timer:%hhx\n",
        delayTimer, soundTimer);
    
    return true;
}