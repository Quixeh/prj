/** 
* Project Global Vars
*
**/

using namespace std;

double pxGrpSize = 5;
int pxFillMode = 1;

bool showFullSize = false;
bool showDisplay = false;
bool outputBmp = false;
bool outputFullBmp = false;

int Xres = int(1920/pxGrpSize);
int Yres = int(1080/pxGrpSize);

SDL_WindowID wCtrl, wDisp, wDispFull;
SDL_Renderer *rCtrl, *rDisp, *rDispFull;
SDL_Texture *tCtrl, *tDisp, *tDispFull;
