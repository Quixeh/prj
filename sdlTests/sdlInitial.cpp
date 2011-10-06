

//
#include "SDL.h"

SDL_Surface* disp;

Uint32 gPixel( SDL_Surface *surface, int x, int y ){
    Uint32 *pixels = (Uint32 *)surface->pixels;
    return pixels[ ( y * surface->w ) + x ];
}

void sPixel( SDL_Surface *surface, int x, int y, Uint32 pixel ){
    Uint32 *pixels = (Uint32 *)surface->pixels;
    pixels[ ( y * surface->w ) + x ] = pixel;
}
#include <TCHAR.h>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "../inc/alp.h"
#include "View.h"
#include <time.h>





int SDLmain( int argc, char* args[] )
{
    
    double pxGrpSize = 5;
    
    
    int Xres = int(1920/pxGrpSize);
    int Yres = int(1080/pxGrpSize);
    
    cout << "Starting SDL." << endl;
    //Start SDL
    SDL_Init( SDL_INIT_EVERYTHING );
    disp = SDL_SetVideoMode(Xres, Yres, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    
    int key = 0;
    
    SDL_Event event;
    
    
    
    int intPxGrpSize = int(pxGrpSize);
    

    cout << "Creating View." << endl;
	View view(intPxGrpSize);
	
	cout << "Applying Fn to View." << endl;
	view.applyXfn();

	cout << "Creating Output." << endl;
	view.output();
    
    
    
    while(!key){
                
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
