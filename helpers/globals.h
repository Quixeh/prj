/** 
* Project Global Vars
*
**/

using namespace std;

bool debugMode = true;

double pxGrpSize = 5;
int pxFillMode = 2;

bool showCtrl = false;
bool showFullSize = true;
bool showDisplay = true;
bool outputBmp = false;
bool outputFullBmp = false;

int Xres = int(1920/pxGrpSize);
int Yres = int(1080/pxGrpSize);

SDL_WindowID wCtrl, wDisp, wDispFull;
SDL_Renderer *rCtrl, *rDisp, *rDispFull;
SDL_Texture *tCtrl, *tDisp, *tDispFull;
