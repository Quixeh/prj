/** 
* Project Global Vars
*
**/

using namespace std;

double pxGrpSize = 20;
int pxFillMode = 1;

int Xres = int(1920/pxGrpSize);
int Yres = int(1080/pxGrpSize);

SDL_WindowID wCtrl, wDisp, wDispFull;
SDL_Renderer *rCtrl, *rDisp, *rDispFull;
SDL_Texture *tCtrl, *tDisp, *tDispFull;
