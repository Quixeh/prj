/** SLM Control Program
*
* Created for the DLi Discovery 4000 with the .95' 1920x1080 DMD attached.
*
* The DMD is essentially an array of mirrors - so we have a digital system where
* a pixel can either be on or off. To introduce greyscale and intensity level
* control, we have grouped the pixels together, effectively reducing the 
* resolution.
*
* The image is handled by the class 'View' - and we have one 'View' per image.
* These can be grouped together to form sequences of images. Inside this View, 
* the pixels are grouped into 'PxGrp' classes. These deal with the individual 
* mirrors on the DMD. 
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*
*/




// Standard C++ Includes

/* TCHAR.h
* Includes the TCHAR variable type, used to build single-byte,
* MBCS, and Unicode applications from the same sources. 
*/
#include <TCHAR.h> 

/* windows.h
* Includes the windows API - useful for interfacing with 
* the operating system.
*/
#include <windows.h>

/* conio.h
* Command line user-interfaces. 
*/
#include <conio.h>

/* stdio
* Standard C++ input/output commands
*/
#include <stdio.h>

/* iostream
* Used to input and output to console.
*/
#include <iostream>

/* string
* Includes the String variable type.
*/
#include <string>

/* time.h
* Allows us to know what the system time is. 
*/
#include <time.h>




// Specific Includes

/* SDL.h
* Imports the SDL graphical library for using application windows. 
* Current version used is a 5300 snapshot of the 1.3 development build. 
*/
#include "SDL/SDL.h"

/* alp.h
* Imports the ALP device interface library - this is developed by TI and is 
* included with the product. Using the high-speed interface. 
*/
#include "inc/alp.h"

/* globals.h
* Globals variables and program settings.
*/
#include "helpers/globals.h"




// Custom Classes

/* PxGrp.h
* A class that handles all of the individual pixel values, and the
* distribution of the pixels with a group that has a greyscale applied.
*/
#include "classes/PxGrp.h"
#include "classes/View.h"
//#include "classes/AlpDMD.h"

using namespace std;

int main( int argc, char* args[] ){
     
	// Start SDL /////////////////
	SDL_Init( SDL_INIT_EVERYTHING );
	
	// Windows
//	wCtrl = SDL_CreateWindow("SLM Control Panel",100,100,230,600,SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
	if (showDisplay) wDisp = SDL_CreateWindow("DMD Display",100,100,Xres,Yres,SDL_WINDOW_SHOWN);
	if (showFullSize) wDispFull = SDL_CreateWindow("DMD Fullsize",200,30,1920,1080,SDL_WINDOW_SHOWN);
	
	cout << "Xres: " << Xres << " Yres: " << Yres << endl;
	
	// Renderers
//	rCtrl = SDL_CreateRenderer(wCtrl, -1, 0);
	if (showDisplay)rDisp = SDL_CreateRenderer(wDisp, -1, 0);
	if (showFullSize) rDispFull = SDL_CreateRenderer(wDispFull, -1, 0);
	
	// Textures
//	tCtrl = SDL_CreateTexture(rCtrl, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 50, 50);
	if (showDisplay) tDisp = SDL_CreateTexture(rDisp, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, 50, 50);
	if (showFullSize) tDispFull = SDL_CreateTexture(rDispFull, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, 50, 50);
	
	// Define Pixel Manipulation Vars
	Uint32 *pix; // Current Working Pixel
	void *pixels; // Pixels to work upon - comes from the texture lock
	int pitch; // Don't quite know what this does....
	
	if (showDisplay){
		SDL_LockTexture(tDisp, NULL, &pixels, &pitch);
		
		// Set the display to a default colour...	
		SDL_Color colourDefault = {115,115,115};
	    
		for (int x=0; x<50; x++){
			pix = (Uint32*)((Uint8*)pixels + x * pitch);
			for (int y=0; y<50; y++){
				*pix++ = (0xFF000000|(colourDefault.r<<16)|(colourDefault.g<<8)|colourDefault.b);
			}
		}
		
		SDL_UnlockTexture(tDisp);
		
		// Update the display...
		SDL_RenderClear(rDisp);
		SDL_RenderCopy(rDisp, tDisp, NULL, NULL);
		SDL_RenderPresent(rDisp);
	}
	
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
	if (showDisplay) SDL_DestroyRenderer(rDisp);
	if (showFullSize) SDL_DestroyRenderer(rDispFull);
//	SDL_DestroyWindow(wCtrl);
	if (showDisplay) SDL_DestroyWindow(wDisp);
	if (showFullSize) SDL_DestroyWindow(wDispFull);
	
	SDL_Quit();
	exit(0);
	return 0;    
}
