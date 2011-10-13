// Standard Includes
#include <TCHAR.h>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <time.h>

// Specific Includes
#include "SDL/SDL.h"
#include "inc/alp.h"
#include "helpers/globals.h"

// Classes
#include "classes/PxGrp.h"
#include "classes/View.h"
//#include "classes/AlpDMD.h"

using namespace std;

int main( int argc, char* args[] ){
     
	// Start SDL /////////
	SDL_Init( SDL_INIT_EVERYTHING );
	
	// Windows
//	wCtrl = SDL_CreateWindow("SLM Control Panel",100,100,230,600,SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
	wDisp = SDL_CreateWindow("DMD Display",100,100,Xres,Yres,SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
	wDispFull = SDL_CreateWindow("DMD Fullsize",200,30,1980,1080,SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
	
	// Renderers
//	rCtrl = SDL_CreateRenderer(wCtrl, -1, 0);
	rDisp = SDL_CreateRenderer(wDisp, -1, 0);
	rDispFull = SDL_CreateRenderer(wDispFull, -1, 0);
	
	// Textures
//	tCtrl = SDL_CreateTexture(rCtrl, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 50, 50);
	tDisp = SDL_CreateTexture(rDisp, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, 50, 50);
	tDispFull = SDL_CreateTexture(rDispFull, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, 50, 50);
	
	// Define Pixel Manipulation Vars
	Uint32 *pix; // Current Working Pixel
	void *pixels; // Pixels to work upon - comes from the texture lock
	int pitch; // Don't quite know what this does....
	
	SDL_LockTexture(tDisp, NULL, &pixels, &pitch);
//	SDL_LockTexture(tFullDisp, NULL, &pixels, &pitch);
	
	// Set the display to a default colour...	
	SDL_Color colourDefault = {115,115,115};
    
	for (int x=0; x<50; x++){
		pix = (Uint32*)((Uint8*)pixels + x * pitch);
		for (int y=0; y<50; y++){
			*pix++ = (0xFF000000|(colourDefault.r<<16)|(colourDefault.g<<8)|colourDefault.b);
		}
	}
	
	SDL_UnlockTexture(tDisp);
//	SDL_UnlockTexture(tFullDisp);
	
	// Update the display...
	SDL_RenderClear(rDisp);
	SDL_RenderCopy(rDisp, tDisp, NULL, NULL);
	SDL_RenderPresent(rDisp);
	
	bool key = false;
	
	SDL_Event event;
	int intPxGrpSize = int(pxGrpSize);
	
	cout << "Main: Creating View." << endl;
	View view(intPxGrpSize);
	
	cout << "Main: Applying Fn to View." << endl;
	view.applyXfn();
	
	cout << "Main: Creating Output." << endl;
	view.output();
   
	while(!key){ // SDL Event Loop
	        
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
//	SDL_DestroyRenderer(rCtrl);
	SDL_DestroyRenderer(rDisp);
	SDL_DestroyRenderer(rDispFull);
//	SDL_DestroyWindow(wCtrl);
	SDL_DestroyWindow(wDisp);
	SDL_DestroyWindow(wDispFull);
	
	SDL_Quit();
	exit(0);
	return 0;    
}
