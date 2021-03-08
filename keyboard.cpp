#include "include/keyboard.h"

keyboard::keyboard(std::FILE *debugFile)
{
    myDebugFile = debugFile;
}

keyboard::~keyboard()
{
}

bool keyboard::updateKeyboard(bool &quit)
{
    SDL_Event e;

    while( SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }
        else if (e.type ==  SDL_KEYDOWN)
        {
            // Updating keyState array
            switch (e.key.keysym.sym)
            {
            case SDLK_1: // 1 - 0x01
                keyState[0x01] = 0x01;
                break;

            case SDLK_2: // 2 - 0x02
                keyState[0x02] = 0x01;
                break;
            
            case SDLK_3: // 3 - 0x03
                keyState[0x03] = 0x01;
                break;
            
            case SDLK_q: // q - 0x04
                keyState[0x04] = 0x01;
                break;
            
            case SDLK_w: // w - 0x05
                keyState[0x05] = 0x01;
                break;
            
            case SDLK_e: // e - 0x06
                keyState[0x06] = 0x01;
                break;
            
            case SDLK_a: // a - 0x07
                keyState[0x07] = 0x01;
                break;
            
            case SDLK_s: // s - 0x08
                keyState[0x08] = 0x01;
                break;
            
            case SDLK_d: // d - 0x09
                keyState[0x09] = 0x01;
                break;
            
            case SDLK_z: // z - 0x0A
                keyState[0x0A] = 0x01;
                break;
            
            case SDLK_x: // x - 0x00
                keyState[0x00] = 0x01;
                break;
            
            case SDLK_c: // c - 0x0B
                keyState[0x0B] = 0x01;
                break;
            
            case SDLK_4: // 4 - 0x0C
                keyState[0x0C] = 0x01;
                break;
            
            case SDLK_r: // r - 0x0D
                keyState[0x0D] = 0x01;
                break;
            
            case SDLK_f: // f - 0x0E
                keyState[0x0E] = 0x01;
                break;
            
            case SDLK_v: // v - 0x0F
                keyState[0x0F] = 0x01;
                break;
            
            default:
                break;
            }
        }
        else if (e.type == SDL_KEYUP)
        {
            // Updating keyState array
            switch (e.key.keysym.sym)
            {
            case SDLK_1: // 1 - 0x01
                keyState[0x01] = 0x00;
                break;

            case SDLK_2: // 2 - 0x02
                keyState[0x02] = 0x00;
                break;
            
            case SDLK_3: // 3 - 0x03
                keyState[0x03] = 0x00;
                break;
            
            case SDLK_q: // q - 0x04
                keyState[0x04] = 0x00;
                break;
            
            case SDLK_w: // w - 0x05
                keyState[0x05] = 0x00;
                break;
            
            case SDLK_e: // e - 0x06
                keyState[0x06] = 0x00;
                break;
            
            case SDLK_a: // a - 0x07
                keyState[0x07] = 0x00;
                break;
            
            case SDLK_s: // s - 0x08
                keyState[0x08] = 0x00;
                break;
            
            case SDLK_d: // d - 0x09
                keyState[0x09] = 0x00;
                break;
            
            case SDLK_z: // z - 0x0A
                keyState[0x0A] = 0x00;
                break;
            
            case SDLK_x: // x - 0x00
                keyState[0x00] = 0x00;
                break;
            
            case SDLK_c: // c - 0x0B
                keyState[0x0B] = 0x00;
                break;
            
            case SDLK_4: // 4 - 0x0C
                keyState[0x0C] = 0x00;
                break;
            
            case SDLK_r: // r - 0x0D
                keyState[0x0D] = 0x00;
                break;
            
            case SDLK_f: // f - 0x0E
                keyState[0x0E] = 0x00;
                break;
            
            case SDLK_v: // v - 0x0F
                keyState[0x0F] = 0x00;
                break;
            
            default:
                break;
            }
        }
    }

    return true;
}