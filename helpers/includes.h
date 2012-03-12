/** 
* Project Includes List
* 
* Specifies all the includes we need to compile the project.
**/

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

/* iomanip
* Allows us to manipulate the output stream
*/
#include <iomanip>

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
#include "../inc/alp.h"

/* globals.h
* Globals variables and program settings.
*/
#include "globals.h"


// Custom Classes

/* PxGrp.h
* A class that handles all of the individual pixel values, and the
* distribution of the pixels with a group that has a greyscale applied.
*/
#include "../classes/PxGrp.h"

/* View.h
* Represents a single image on the DMD and handles all the PxGrp objects
* that are contained within. 
*/
#include "../classes/View.h"

/* Sequence.h
*/
#include "../classes/Sequence.h" 

/* AlpDMD.h
* Custom Handler for the Alp Device. If not in the lab, you shouldn't have 
* the useDMD var defined... see globals.h. Leaving it defined will cause a 
* CRYPTO-BOX error.  
*/
#ifdef useDMD
#include "../classes/AlpDMD.h"
#endif

// Member Functions 
#include "../classes/View.cpp" 
#include "../classes/PxGrp.cpp"

#ifdef useDMD
#include "../classes/AlpDMD.cpp"
#endif

// EOF
