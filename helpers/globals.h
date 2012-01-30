/** 
* Project Global Vars
*
**/

using namespace std;

bool debugMode = true;

double pxGrpSize = 10;
int pxFillMode = 2;

int intPxGrpSize = int(pxGrpSize); 

bool showCtrl = false;
bool showFullSize = false;
bool showDisplay = true;
bool outputBmp = true;
bool outputFullBmp = false;

int Xres = int(1920/pxGrpSize);
int Yres = int(1080/pxGrpSize);

SDL_WindowID wCtrl, wDisp, wDispFull;
SDL_Renderer *rCtrl, *rDisp, *rDispFull;
SDL_Texture *tCtrl, *tDisp, *tDispFull;

bool close = false;

#define useDMD = TRUE
