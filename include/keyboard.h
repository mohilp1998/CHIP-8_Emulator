#pragma once

#include <SDL.h>
#include <iostream>
#include <vector>

class keyboard
{
private:
    /*File for debugging*/
    std::FILE *myDebugFile;

public:
    // Keyboard Press array
    std::vector<unsigned char> keyState {std::vector<unsigned char> (16, 0x00)};

    keyboard(std::FILE *debugFile);
    ~keyboard();

    bool updateKeyboard(bool &quit);
};
