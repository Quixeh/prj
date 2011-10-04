#include "SDL/SDL.h"

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
    //Start SDL
    SDL_Init( SDL_INIT_EVERYTHING );
    
    int key = 0;
    
    SDL_Event event;
    SDL_Surface* disp;
    
    disp = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    
    while(!key){
                
                sPixel(disp, 0, 0, SDL_MapRGB(disp->format, 255, 255, 255));
                SDL_Flip(disp);
                
                
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
   // SDL_Quit();
    
    return 0;    
}
