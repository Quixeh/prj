/** 
* Project Global Vars
*
**/

using namespace std;

double pxGrpSize = 10;
int pxFillMode = 2;

vector<string> args;

long illuminateTime; // Change this at runtime, defaults to ALP_DEFAULT on ALP::init. 
long pictureTime = 200000; // In microseconds (us). Max: 10s, Min -> ALP_DEFAULT

bool showCtrl = false;
bool showFullSize = false;
bool showDisplay = true;
bool outputBmp = false;
bool outputFullBmp = false;
bool verbose =  false;

int Xres = int(1920/pxGrpSize);
int Yres = int(1080/pxGrpSize);

SDL_WindowID wCtrl, wDisp, wDispFull;
SDL_Renderer *rCtrl, *rDisp, *rDispFull;
SDL_Texture *tCtrl, *tDisp, *tDispFull;

bool close = false;

#define useDMD = TRUE
