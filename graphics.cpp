#include "include\graphics.h"

graphics::graphics(/* args */)
{
}

graphics::~graphics()
{
}

bool InitSDL(SDL_Window* gWindow, SDL_Renderer* gRenderer)
{
    // Necessary step for SDL Dll linking properly
    SDL_SetMainReady();
    
    // Initialize SDL with only Video feature
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to initialize the SDL following error: %s\n", SDL_GetError() );
        return false;
    }
    
    // Creating Window
    gWindow = SDL_CreateWindow( "SDL List of Pixel Working", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if ( NULL == gWindow )
    {
        printf( "Failed creating Window: %s\n", SDL_GetError() );
        return false;
    }
    // Creating renderer
    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
    if ( NULL == gRenderer )
    {
        printf( "Unable to renderer: %s\n", SDL_GetError() );
        return false;
    }
    
    // Setting the window to empty
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( gRenderer );
    SDL_RenderPresent( gRenderer );

    return true;
}