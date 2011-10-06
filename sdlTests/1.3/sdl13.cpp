#include "SDL/SDL.h"
#include "windows.h"
#include "iostream"


using namespace std;

int main( int argc, char* args[] )
{
    // Start SDL
    SDL_Init( SDL_INIT_EVERYTHING );
    
    // Windows
    SDL_WindowID wCtrl, wDisp;
    wCtrl = SDL_CreateWindow("SLM Control Panel",100,100,230,600,SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    wDisp = SDL_CreateWindow("DMD Display",500,100,200,200,SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
    
    // Renderers
    SDL_Renderer *rCtrl, *rDisp;
    rCtrl = SDL_CreateRenderer(wCtrl, -1, 0);
    rDisp = SDL_CreateRenderer(wDisp, -1, 0);
    
    // Textures
    SDL_Texture *tCtrl, *tDisp;
    tCtrl = SDL_CreateTexture(rCtrl, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 50, 50);
    tDisp = SDL_CreateTexture(rDisp, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 50, 50);
    
    // Define Pixel Manipulation Vars
    Uint32 *pix; // Current Working Pixel
    void *pixels; // Pixels to work upon - comes from the texture lock
    int pitch; // Don't quite know what this does....

    SDL_LockTexture(tDisp, NULL, &pixels, &pitch);
    
    SDL_Color colour = {115,115,115};
    
    for (int x=0; x<50; x++){
        pix = (Uint32*)((Uint8*)pixels + x * pitch);
        for (int y=0; y<50; y++){
            *pix++ = (0xFF000000|(colour.r<<16)|(colour.g<<8)|colour.b);
        }
    }
    
    SDL_UnlockTexture(tDisp);
    
    // Update the display...
    SDL_RenderClear(rDisp);
    SDL_RenderCopy(rDisp, tDisp, NULL, NULL);
    SDL_RenderPresent(rDisp);
    
    
    bool key = false;
    
    SDL_Event event;

    while(!key){
                
                while(SDL_PollEvent(&event)) {      
                    switch (event.type){
                        case SDL_QUIT:
                             key = true;
                             break;
                        case SDL_KEYDOWN:
                             key = true;
                             break;
                    }
                }
                
                SDL_Delay(20);
    }
    //Quit SDL
    SDL_DestroyRenderer(rCtrl);
    SDL_DestroyRenderer(rDisp);
    SDL_DestroyWindow(wCtrl);
    SDL_DestroyWindow(wDisp);
    
    SDL_Quit();
    exit(0);
    return 0;    
}
