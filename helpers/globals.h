/** 
* Project Global Vars
* 
* These are the default settings for the program, each of which
* can be changed at runtime. 
**/

using namespace std;

// Pixel Group Settings

double pxGrpSize = 10; // Default size of each pixel group
int pxFillMode = 2;    // Default pxGrp filling mode. 
double pi = atan(1)*4; // Pi!

// SLM Settings
 
int Xres = int(1920/pxGrpSize);  // Initial DMD sizes. 
int Yres = int(1080/pxGrpSize); 

   // Picture time -> Time between the start of each image. 
   // Illuminate time -> Time each image is shown for. 
   // Therefore, Picture time = illuminate time + time taken to change image. 
   // When operating in BIN_UNINTERRUPTED (all of this program to date), illuminate
   // time is ignored by the device. If Picture time is set to ALP_DEFAULT, we recieve
   // a 30Hz framerate (approx 33,333us?). 
   
long pictureTime = 200000; // In microseconds (us). Max: 10s, Min depends on output mode. 
long illuminateTime; // Change this at runtime, defaults to ALP_DEFAULT on ALP::init. 

// Program Preferences

bool showCtrl = false; // Show the Control Window (Unused)
bool showFullSize = false; // Show the Fullsize 1:1 GUI (requires large Screen Area)
bool showDisplay = true;   // Show the Scaled GUI
bool outputBmp = false;    // Output a scaled Bitmap on 'display' command. 
bool outputFullBmp = false;// Output the fullsize bitmap on display. 
bool verbose =  false;     // Debug and fully verbose discription of all program activities. 
int imgMode = 2;

vector<string> args; // Variable for Command Line Args - Not editable. 

SDL_WindowID wCtrl, wDisp, wDispFull; // SDL Variables - Not editable. 
SDL_Renderer *rCtrl, *rDisp, *rDispFull;
SDL_Texture *tCtrl, *tDisp, *tDispFull;

bool close = false; // Menu Variable - Not editable. 
 
// Comment out the next line to compile a version of the program that does not
// control the DMD. This is useful for testing on a PC that does not have the
// Cryptobox software installed. 
//#define useDMD = TRUE 
