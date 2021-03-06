#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>
#include <vector>

const int screenSizeModifier = 10;
const int SCREEN_WIDTH = 64 * screenSizeModifier;
const int SCREEN_HEIGHT = 32 * screenSizeModifier;

class graphics
{
private:
    /*File for debugging*/
    std::FILE *myDebugFile;

    // SDL Variables
    SDL_Window* gWindow = NULL; // Window for doing rendering
    SDL_Renderer* gRenderer = NULL; // rendering we will using to render

public:
    // List of pixels which will be used for drawing
    std::vector<std::vector<unsigned char>> pixels;
    
    graphics(std::FILE *debugFile);
    ~graphics();

    // Basic SDL initialization, this must happen before keyboard init
    bool InitSDL();

    // Drawing the pixel array to the screen
    bool draw();
};