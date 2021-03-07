#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>

const int screenSizeModifier = 10;
const int SCREEN_WIDTH = 64 * screenSizeModifier;
const int SCREEN_HEIGHT = 48 * screenSizeModifier;

class graphics
{
private:
    /* data */
public:
    graphics(/* args */);
    ~graphics();
};

bool InitSDL(SDL_Window* gWindow, SDL_Renderer* gRenderer);