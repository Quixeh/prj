/** SLM Control Program
*
* Created for the DLi Discovery 4100 with the .95' 1920x1080 DMD attached.
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
* @author Andy Blackmore <axb803@bham.ac.uk> <quixeh@gmail.com> 
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

/* sstream
* Allows us to use string streams 
*/
#include <sstream>

/* vector
* Dynamicly sized arrays
*/
#include <vector>

/* shlobj
*  Windows Folder Dialogues
*/
#include <shlobj.h>   

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

/* View.h
* Represents a single image on the DMD and handles all the PxGrp objects
* that are contained within. 
*/
#include "classes/View.h"

/* Sequence.h
*/
#include "classes/Sequence.h" 

/* AlpDMD.h
* Custom Handler for the Alp Device. If not in the lab, you shouldn't have 
* the useDMD var defined... see globals.h. Leaving it defined will cause a 
* CRYPTO-BOX error.  
*/
#ifdef useDMD
#include "classes/AlpDMD.h"
#endif

// Member Functions 
#include "classes/View.cpp" 
#include "classes/PxGrp.cpp"

#ifdef useDMD
#include "classes/AlpDMD.cpp"
#endif


  




// Namespace

using namespace std;

int viewSize = int(pxGrpSize);
View view(viewSize);
Sequence seq;


#ifdef useDMD
AlpDMD dmd;
#endif

// Last but not least, include the menu function. 
#include "helpers/menu.cpp"


// Main

int main(int argc, char* argv[]){ // Arguments are SDL Specific

	srand(0); // Seed random number generator.
	SetConsoleTitle("SLM Control Software");
	
// Command Line Arguments
	
	// Tell us what arguments we have
	if (verbose) cout << "Command Line Arguments: " << argc << endl; 
	
	// Display the arguments, and put them in a global so all functions can see them.
	for(int i=0; i < argc; i++){ 
		if (verbose) cout << "Argument " << i << " = " << argv[i] << endl;
		args.push_back(argv[i]);
	}
	
	
// Start SDL Graphical Library. 
	SDL_Init(SDL_INIT_EVERYTHING);
	 
// Create the windows to display
	/*
	* There are three possible windows. wDisp is the scale version of the 
	* DMD, where one pixel on screen represents a PxGrp, wCtrl is the planned
	* control panel for the application, and wDispFull is the fullscreen
	* 1-1 representation of the DMD. 
	*
	* Optimal Conditions for this program is set as having two monitors of
	* 1920x1080 resolution, where both monitors are level, and the left
	* monitor is the main window. Anything else may produce unexpected results.
	*
	* First we check to see if that window is enabled in the settings. Then we 
	* call SDL_CreateWindow(<title>,<xpos>,<ypos>,<xsize>,<ysize>,<flags>)
	* A position of 0,0 would be the top left of screen 1. 1920,0 is top left 
	* screen 2. Any window that is visible needs the flag SDL_WINDOW_SHOWN.
	* We've made the second window "borderless" which removes the usual windows
	* close minimise & outline - giving the effect of fullscreen without locking
	* input focus. 
	*/

	if (showCtrl) wCtrl = SDL_CreateWindow("SLM Control Panel",100,100,230,600,SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
	if (showDisplay) wDisp = SDL_CreateWindow("DMD Display",100,100,Xres,Yres,SDL_WINDOW_SHOWN);
//	if (showFullSize) wDispFull = SDL_CreateWindow("DMD Fullsize",1680,0,1920,1080,SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	if (showFullSize) wDispFull = SDL_CreateWindow("DMD Fullsize",100,100,1920,1080,SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	
	if (verbose) cout << "Scaled Xres: " << Xres << " Yres: " << Yres << endl;
	
// Renderers
	/*
	* These are SDL entities that enable rendering of graphics to the screen. 
	* For simplicity, we use one per window. 
	*/	
	
	if (showCtrl) rCtrl = SDL_CreateRenderer(wCtrl, -1, 0);
	if (showDisplay) rDisp = SDL_CreateRenderer(wDisp, -1, 0);
	if (showFullSize) rDispFull = SDL_CreateRenderer(wDispFull, -1, 0);
	 
// Textures
	/*
	* These are the textures that appear on the windows. Again, they need to 
	* be created for the window. We call the following command:
	* SDL_CreateTexture(<renderer>,<pixel format>,<texture mode>,<xsize>,<ysize>)
	*
	* For this project I'll be using two types of Pixel formats. 
	* ARGB (Alpha, Red, Green, Blue) 8888 (8 bits for each) - and RGB888 which
	* neglects the alpha channel. 
	*
	* Making the texture mode "Streaming" allows us to access it on a pixel level. 
	*/
	
	if (showCtrl) tCtrl = SDL_CreateTexture(rCtrl, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 50, 50);
	if (showDisplay) tDisp = SDL_CreateTexture(rDisp, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, 50, 50);
	if (showFullSize) tDispFull = SDL_CreateTexture(rDispFull, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, 50, 50);
	
// Default Display on wDisp

	// Define Pixel Manipulation Vars
	Uint32 *pix; // Current Working Pixel
	void *pixels; // Pixels to work upon - comes from the texture lock
	int pitch; // Don't quite know what this does.... But changing it messes up the pixel colours. 
	
	if (showDisplay){
		/*
		* First we lock the texture, which gives us the pixels it contains,
		* and allows us to work on them without any other functions
		* interrupting.
		*/
		SDL_LockTexture(tDisp, NULL, &pixels, &pitch); 
		
		// Set the display to a default colour...	
		SDL_Color colourDefault = {0,0,0};
	    
		for (int x=0; x<50; x++){ // Foreach column
			pix = (Uint32*)((Uint8*)pixels + x * pitch); // Grab those pixles
			for (int y=0; y<50; y++){ // Place them y times down the screen in the form A-R-G-B
				*pix++ = (0xFF000000|(colourDefault.r<<16)|(colourDefault.g<<8)|colourDefault.b);
			}
		}
		
		SDL_UnlockTexture(tDisp); // Unlock the texture to say we're done
		
		// Update the display...
		SDL_RenderClear(rDisp); // Clear the renderer
		SDL_RenderCopy(rDisp, tDisp, NULL, NULL); // Copy the texture to display
		SDL_RenderPresent(rDisp); // Present to user
	}
	
// Program Loop Setup
	/*
	* SDL Programs work by sticking themselves into an infinite loop. When the
	* loop ends, so does the program. We therefore initialize a variable named
	* 'close' which, when true, will end the program after the current loop. 
	*
	* Keypresses and mouse actions are captured by "SDL_Event"s - we need to 
	* create a way of looking at these captured actions, so we create the event
	* variable, and we will later check it in the loop.  
	*/
	
	
	SDL_Event event; // Event Catcher.
   	
// Console Menu Thread
   	int choice;
   	
   	SDL_Thread *menuThread;
        
	menuThread = SDL_CreateThread(menu, NULL); // Defines the function to run in the thread	
	if ( menuThread == NULL ) {
	        fprintf(stderr, "Unable to create thread: %s\n", SDL_GetError());
	}
   	
// Program Loop
   	while (!close){
		
		/*
		* Time to check if the user has pressed any keys while the SDL window
		* was in focus. To do this we use SDL_PollEvent.
		*
		*/
		
		while(SDL_PollEvent(&event)) {      
			switch (event.type){
				case SDL_WINDOWEVENT: // Window is moved/closed etc. 
					if (event.window.event == SDL_WINDOWEVENT_CLOSE){
						if (verbose) cout << "\n\nWindow Manager Close Requested." << endl;
						close = true;
					}
					if (event.window.event == SDL_WINDOWEVENT_RESTORED){
						if (showDisplay) view.outputToSdl();
					}
				     
				     break;
				case SDL_KEYDOWN: // Someone pressed a key while window in focus. 
					if (event.key.keysym.sym == SDLK_ESCAPE){
						close = true;
					}	
				     
				     break;
			}
		}
		
		
		
		SDL_Delay(20); // Give the processor back to other programs for a bit!
	}
   	
	
// End the program
	/*
	* Mummy always said that you need to clear up your toys after you play 
	* with them. SDL is no different! So, we close whichever of our windows
	* that we've opened up, then destroy the renderers for each. Then we use
	* the command SDL_Quit which will clean up everything on SDL's side of things.  
	*/
	
	if (showCtrl) { 
		SDL_DestroyRenderer(rCtrl);
		SDL_DestroyWindow(wCtrl);
	}
	
	if (showDisplay) { 
		SDL_DestroyRenderer(rDisp);
		SDL_DestroyWindow(wDisp);
	}
	
	if (showFullSize) {
		SDL_DestroyRenderer(rDispFull);
		SDL_DestroyWindow(wDispFull);
	}
	
	// Shutdown the DMD nicely if we've been using it. 
	#ifdef useDMD
	dmd.quit();
	#endif
	
	// Shutdown SDL... 
	SDL_Quit();
	exit(0);
	return 0;    
}
