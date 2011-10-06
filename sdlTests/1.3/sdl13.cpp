#include "SDL/SDL.h"
#include "windows.h"
#include "iostream"


using namespace std;

Uint32 gPixel( SDL_Surface *surface, int x, int y ){
    Uint32 *pixels = (Uint32 *)surface->pixels;
    return pixels[ ( y * surface->w ) + x ];
}

void sPixel( SDL_Surface *surface, int x, int y, Uint32 pixel ){
    Uint32 *pixels = (Uint32 *)surface->pixels;
    pixels[ ( y * surface->w ) + x ] = pixel;
}

int main( int argc, char* args[] )
{
    // Start SDL
    SDL_Init( SDL_INIT_EVERYTHING );
    
    // Windows
    SDL_WindowID wCtrl, wDisp;
    wCtrl = SDL_CreateWindow("SLM Control Panel",100,100,230,600,SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    wDisp = SDL_CreateWindow("DMD Display",500,100,200,200,SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    
    // Renderers
    SDL_Renderer *rCtrl, *rDisp;
    rCtrl = SDL_CreateRenderer(wCtrl, -1, 0);
    rDisp = SDL_CreateRenderer(wDisp, -1, 0);
    
    // Textures
    SDL_Texture *tCtrl, *tDisp;
    tCtrl = SDL_CreateTexture(rCtrl, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 50, 50);
    tDisp = SDL_CreateTexture(rDisp, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 50, 50);
    
    Uint32 *pix;
    void *pixels;
    int pitch;

    SDL_LockTexture(tDisp, NULL, &pixels, &pitch);
    
    SDL_Color colour = {115,115,115};
    
    for (int x=0; x<50; x++){
        pix = (Uint32*)((Uint8*)pixels + x * pitch);
        for (int y=0; y<50; y++){
            *pix++ = (0xFF000000|(colour.r<<16)|(colour.g<<8)|colour.b);
        }
    }
    
    SDL_UnlockTexture(tDisp);
    
    SDL_RenderClear(rDisp);
    SDL_RenderCopy(rDisp, tDisp, NULL, NULL);
    SDL_RenderPresent(rDisp);
    
    
    int key = 0;
    
    SDL_Event event;

    
    while(!key){
               
               Sleep(10);
                
                while(SDL_PollEvent(&event)) {      
                    switch (event.type){
                        case SDL_QUIT:
                             key = 1;
                             break;
                        case SDL_KEYDOWN:
                             key = 1;
                             break;
                    }
                }
    }
    //Quit SDL
    //SDL_Quit();
    
    return 0;    
}
