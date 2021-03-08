#include "include\graphics.h"

graphics::graphics(std::FILE *debugFile)
{
    // Debugging logs
    myDebugFile = debugFile;

    // Initializing pixels array
    std::vector<unsigned char> pixelRow ( (SCREEN_WIDTH / screenSizeModifier), 0x00);
    for (int i = 0; i < (SCREEN_HEIGHT / screenSizeModifier); i++)
    {
            pixels.push_back(pixelRow);
    }
    std::fprintf(myDebugFile, "[I] <graphics.cpp>::Initialized the pixels array "
        "to size %d x %d\n", pixels[0].size(), pixels.size());

}

graphics::~graphics()
{
    //Destroying the renderer and Window. Basically this will free all dynamic memory alloted
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

    // Closing SDL completely
    SDL_Quit();
}

bool graphics::InitSDL()
{
    // Necessary step for SDL Dll linking properly
    SDL_SetMainReady();
    
    // Initialize SDL with only Video feature
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        std::fprintf(myDebugFile, "[E] <graphics.cpp>::Unable to initialize the SDL following error: %s\n", SDL_GetError() );
        return false;
    }
    
    // Creating Window
    gWindow = SDL_CreateWindow( "CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if ( NULL == gWindow )
    {
        std::fprintf(myDebugFile, "[E] <graphics.cpp>::Failed creating Window: %s\n", SDL_GetError() );
        return false;
    }
    // Creating renderer
    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
    if ( NULL == gRenderer )
    {
        std::fprintf(myDebugFile, "[E] <graphics.cpp>::Unable to renderer: %s\n", SDL_GetError() );
        return false;
    }
    
    // Setting a black screen, applying it to renderer and presenting it.
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear( gRenderer ); 
    SDL_RenderPresent( gRenderer );

    return true;
}

bool graphics::draw()
{
    SDL_Rect rectangle;
    for (size_t y = 0; y < (SCREEN_HEIGHT / screenSizeModifier); y++)
    {
        for (size_t x = 0; x < (SCREEN_WIDTH / screenSizeModifier); x++)
        {
            SDL_SetRenderDrawColor(gRenderer, (pixels[y][x] * 0xFF), (pixels[y][x] * 0xFF), (pixels[y][x] * 0xFF), 0xFF);
            rectangle.x = x * screenSizeModifier;
            rectangle.y = y * screenSizeModifier;
            rectangle.w = screenSizeModifier;
            rectangle.h = screenSizeModifier;
            SDL_RenderFillRect(gRenderer, &rectangle);
        }
    }

    SDL_RenderPresent(gRenderer);
    return true;
}