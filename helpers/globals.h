/** 
* Project Global Vars
*
**/

double pxGrpSize = 5;

int Xres = int(1920/pxGrpSize);
int Yres = int(1080/pxGrpSize);

SDL_WindowID wCtrl, wDisp;
SDL_Renderer *rCtrl, *rDisp;
SDL_Texture *tCtrl, *tDisp;
