/** View Class Methods
*
* Header defined in View.h
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*/

#include <cmath>
#include <fstream>
#include <string>


using namespace std;

View::View(int setPxSize){
	pxSize = setPxSize;
	Xres = 1920/pxSize;
	Yres = 1080/pxSize;
	groups = new PxGrp*[Xres];
	for (int i=0;i<Xres;i++){
		groups[i] = new PxGrp[Yres];
		for (int j=0;j<Yres;j++){
			groups[i][j].setPxSize(pxSize);
		}
	}
}

void View::resize(int setPxSize){
	
	delete [] groups; 
	pxGrpSize = setPxSize;
	pxSize = setPxSize;
	Xres = 1920/pxSize;
	Yres = 1080/pxSize;
	groups = new PxGrp*[Xres];
	for (int i=0;i<Xres;i++){
		groups[i] = new PxGrp[Yres];
		for (int j=0;j<Yres;j++){
			groups[i][j].setPxSize(pxSize);
			groups[i][j].setValue(0);
			
		}
	}
	checkValue();
}

void View::randomise(double chance){
		
	for (int x=0; x<Xres; x++){
		for (int y=0; y<Yres; y++){		
			int IO = rand() % int(1/chance);
			//cout << x << " " << y << " Ran: " << IO << endl;
			if (IO == 0){
				int val = rand() % int(pxGrpSize*pxGrpSize);
				val++;
				//cout << x << " " << y << " " << val << endl;
				groups[x][y].setValue(val);
				
			} else {
				groups[x][y].setValue(0);
			}
		}
	}
}

int View::getPix(int x, int y){
	int h = int(floor((x)/double(pxGrpSize)));
	int k = int(floor((y)/double(pxGrpSize)));
	int i = int(x - (h*pxGrpSize));
	int j = int(y - (k*pxGrpSize));
	groups[h][k].getData(i,j);
}

int View::setPix(int x, int y, int v){
	int h = int(floor((x)/double(pxGrpSize)));
	int k = int(floor((y)/double(pxGrpSize)));
	int i = int(x - (h*pxGrpSize));
	int j = int(y - (k*pxGrpSize));
	groups[h][k].setData(i,j,v);
}

void View::checkValue(){
	for (int x=0; x<Xres; x++){
		for (int y=0; y<Yres; y++){
			groups[x][y].checkValue();
		}
	}	
}

void View::applyXfn(int type){

     double xV = 0;

	for (int x=0; x<Xres; x++){
		switch (type){
			case 1:
				// Grey-Scale
				xV = (x/double(Xres))* (pxSize * pxSize); 
				break;
			case 2:
				// X^2 Centred at X/2. 
        			xV = ( ((x-(Xres/2.0))*(x-(Xres/2.0))/double((Xres/2.0) * (Xres/2.0))) * (pxSize * pxSize));
        			break;
        			
        		case 3: 
				// Sinx
				xV = abs(sin(x/double(10)))*(pxSize * pxSize);
				break;
				
		}
		
		for (int y=0; y<Yres; y++){
			groups[x][y].setValue(int(floor(xV+0.5))); 
		}
              //  cout << xV << " -> " << int(floor(xV+0.5)) << endl;
	}	
}

void View::applySinxfn(int dist){
	double xV = 0;
	if (pxSize > dist){
		cout << "Can't create a sinx with distance between peaks less than the PxGrp size." << endl;
	} else {
		for (int x=0; x<Xres; x++){
			for (int y=0; y<Yres; y++){
				xV = abs(sin(pi*x*pxSize/double(dist)))*(pxSize * pxSize);
				groups[x][y].setValue(int(floor(xV+0.5))); 
			}
		}
	}
}
void View::applyYfn(int type){

     double yV = 0;

	for (int y=0; y<Yres; y++){
		switch (type){
			case 1:
				// Grey-Scale
				yV = (y/double(Yres))* (pxSize * pxSize); 
				break;
			case 2:
				// Y^2 Centred at Y/2. 
        			yV = ( ((y-(Yres/2.0))*(y-(Yres/2.0))/double((Yres/2.0) * (Yres/2.0))) * (pxSize * pxSize));
        			break;
		}
		for (int x=0; x<Xres; x++){
		
			groups[x][y].setValue(int(floor(yV+0.5))); 
		}
              //  cout << yV << " -> " << int(floor(yV+0.5)) << endl;
	}	
}

void View::applyGrid(){
	
	int value = 0;	
	for (int y=0; y<Yres; y++){
		for (int x=0; x<Xres; x++){
			if (value == 0){
				groups[x][y].setValue(0);
				value = 1;
			} else {
				groups[x][y].setValue(pxSize*pxSize);
				value = 0;
			}
		}
		if (value == 0){
			value = 1;
		} else {
			value = 0;
		}
	}	
}

void View::applyXStripes(){
	
	int value = 0;	
	for (int y=0; y<Yres; y++){
		for (int x=0; x<Xres; x++){
			if (value == 0){
				groups[x][y].setValue(0);
				value = 1;
			} else {
				groups[x][y].setValue(pxSize*pxSize);
				value = 0;
			}
		}
	}	
}

void View::applyYStripes(){
	
	int value = 0;	
	for (int x=0; x<Xres; x++){
		for (int y=0; y<Yres; y++){
			if (value == 0){
				groups[x][y].setValue(0);
				value = 1;
			} else {
				groups[x][y].setValue(pxSize*pxSize);
				value = 0;
			}
		}
	}	
}

void View::invert(){
	for (int x=0; x<Xres; x++){
		for (int y=0; y<Yres; y++){
			groups[x][y].invert();
		}
	}
}

char View::output(){
	
	if (verbose) cout << "View::output: Commencing Output" << endl;
	
	if (showFullSize) outputToSdlFull();
	if (showDisplay) outputToSdl(); 
	if (outputFullBmp) outputToBmpFull();
	if (outputBmp) outputToBmp(); 
  
   
	if (verbose) cout << "View::output: Output Complete" << endl;
}

Uint32  View::gPixel( SDL_Surface *surface, int x, int y ){
    Uint32 *pixels = (Uint32 *)surface->pixels;
    return pixels[ ( y * surface->w ) + x ];
}

void View::sPixel( SDL_Surface *surface, int x, int y, Uint32 pixel ){
    Uint32 *pixels = (Uint32 *)surface->pixels;
    pixels[ ( y * surface->w ) + x ] = pixel;
}

bool View::outputToSdl(){
	if (verbose) cout << "View::outputToSdl: Commencing SDL Output" << endl;
	if (verbose) cout << "View::outputToSdl: Xres " << Xres << " Yres " << Yres << endl;  
	//if (verbose) cout << "View::outputToSdl: Locking Texture" << endl;   
	//SDL_LockTexture(tDisp, NULL, &pixels, &pitch);
	
	int min_value = 0;
	int max_value = pxSize * pxSize;
     	
	if (verbose) cout << "View::outputToSdl: Cycling..." << endl;
	
	SDL_Surface *surf;
	
	Uint32 rmask, gmask, bmask, amask;
    
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif

	surf = SDL_CreateRGBSurface(0, Xres, Yres, 32, rmask, gmask, bmask, amask);
	
	
	
	for (int x=0; x<Xres; x++){
        	for (int y=0; y<Yres; y++){
        		double val = 255 * ((groups[x][y].getValue() - min_value) / double(max_value));
			sPixel(surf, x, y, SDL_MapRGB(surf->format, int(val), int(val), int(val)));
          	}
      	} 
    
	SDL_Texture* newDisplay;
	
	newDisplay = SDL_CreateTextureFromSurface(rDisp, surf);

	if (verbose) cout << "View::outputToSdl: Unlocking Texture" << endl;
	//SDL_UnlockTexture(tDisp);

	SDL_RenderClear(rDisp);
	SDL_RenderCopy(rDisp, newDisplay, NULL, NULL);
	SDL_RenderPresent(rDisp);
    
	if (verbose) cout << "View::outputToSdl: SDL Output Complete" << endl;
}

bool View::outputToSdlFull(){
	if (verbose) cout << "View::outputToSdlFull: Commencing Full SDL Output" << endl;

	if (verbose) cout << "View::outputToSdlFull: Locking Texture" << endl;   
	//SDL_LockTexture(tDisp, NULL, &pixels, &pitch);
	
	int min_value = 0;
	int max_value = pxSize * pxSize;
     	
	SDL_Surface *surf;
	
	Uint32 rmask, gmask, bmask, amask;
    
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif

	surf = SDL_CreateRGBSurface(0, 1920, 1080, 32, rmask, gmask, bmask, amask);
	
	SDL_LockSurface(surf);
	
	if (verbose) cout << "View::outputToSdlFull: Cycling..." << endl;
	//cout << Xres << " " << Yres << endl;
	
        for (int y=0; y<1080; y++){
		for (int x=0; x<1920; x++){
		
			int val = 255 * getPix(x,y);
			
			sPixel(surf, int(x), int(y), SDL_MapRGB(surf->format, int(val), int(val), int(val)));
		
          	}
      	} 
    
    	SDL_UnlockSurface(surf);
    	
	SDL_Texture* newDisplay;
	
	newDisplay = SDL_CreateTextureFromSurface(rDispFull, surf);

	if (verbose) cout << "View::outputToSdlFull: Unlocking Texture" << endl;
	//SDL_UnlockTexture(tDisp);

	SDL_RenderClear(rDispFull);
	SDL_RenderCopy(rDispFull, newDisplay, NULL, NULL);
	SDL_RenderPresent(rDispFull);
    
	if (verbose) cout << "View::outputToSdlFull: Full SDL Output Complete" << endl;
}

bool View::outputToBmp(){
	int min_value = 0;
	int max_value = pxSize * pxSize;
     
	if (verbose) cout << "View::outputToBmp: Commencing BMP Output" << endl;
     
	// Open the output BMP file
	std::ofstream f( "output.bmp", std::ios::out | std::ios::trunc | std::ios::binary );
	if (!f) return false;

	// Some basic
	unsigned long headers_size    = 14  // sizeof( BITMAPFILEHEADER )
					+ 40; // sizeof( BITMAPINFOHEADER )
	unsigned long padding_size    = (4 - ((Yres * 3) % 4)) % 4;
	unsigned long pixel_data_size = Xres * ((Yres * 3) + padding_size);

    // Write the BITMAPFILEHEADER
    f.put( 'B' ).put( 'M' );                           // bfType
    f << lwrite( headers_size + pixel_data_size, 4 );  // bfSize
    f << lwrite( 0,                              2 );  // bfReserved1
    f << lwrite( 0,                              2 );  // bfReserved2
    f << lwrite( headers_size,                   4 );  // bfOffBits

    // Write the BITMAPINFOHEADER
    f << lwrite( 40,4 );  // biSize
    f << lwrite( Xres, 4 );  // biWidth
    f << lwrite( Yres,                           4 );  // biHeight
    f << lwrite( 1,                              2 );  // biPlanes
    f << lwrite( 24,                              2 );  // biBitCount
    f << lwrite( 0,                              4 );  // biCompression=BI_RGB
    f << lwrite( pixel_data_size,                4 );  // biSizeImage
    f << lwrite( 0,                              4 );  // biXPelsPerMeter
    f << lwrite( 0,                              4 );  // biYPelsPerMeter
    f << lwrite( 0,                              4 );  // biClrUsed
    f << lwrite( 0,                              4 );  // biClrImportant

    // Write the pixel data
   // std::cout << min_value << " " << max_value << std::endl;
    for (unsigned y = Yres; y; y--)           // bottom-to-top
      {
    //  std::cout << "Y Works" << std::endl;
      for (unsigned x = 0; x < Xres; x++)  // left-to-right
        {
    //    std::cout << "X Works" << std::endl;
        unsigned char red, green, blue;
        //
        // This is how we convert an integer value to a color:
        // by mapping it evenly along the CIECAM02 hue color domain.
        //
        // http://en.wikipedia.org/wiki/Hue
        // http://en.wikipedia.org/wiki/hsl_and_hsv#conversion_from_hsv_to_rgb
        //
        // The following algorithm takes a few shortcuts since
        // both 'value' and 'saturation' are always 1.0.
        //
        
        double val = 255 * ((groups[x][y-1].getValue() - min_value) / double(max_value));
        red = int(val);
        green = int(val);
        blue = int(val);
       // std::cout << y-1 << " " << x <<  " " << groups[ x][ y-1].getValue() << " " << int(val) << std::endl;

        f.put( static_cast <char> (blue)  )
         .put( static_cast <char> (green) )
         .put( static_cast <char> (red)   );
        }

      if (padding_size) f << lwrite( 0, padding_size );
      }

    // All done!
	if (verbose) cout << "View::outputToBmp: BMP Output Complete" << endl;
    return f.good();
    }    

bool View::outputToBmpFull(){
	int min_value = 0;
	int max_value = pxSize * pxSize;
     
	if (verbose) cout << "View::outputToBmpFull: Commencing Full BMP Output" << endl;
     
	// Open the output BMP file
	std::ofstream f( "outputFull.bmp", std::ios::out | std::ios::trunc | std::ios::binary );
	if (!f) return false;

	// Some basic
	unsigned long headers_size    = 14  // sizeof( BITMAPFILEHEADER )
					+ 40; // sizeof( BITMAPINFOHEADER )
	unsigned long padding_size    = (4 - ((1080 * 3) % 4)) % 4;
	unsigned long pixel_data_size = 1920 * ((1080 * 3) + padding_size);

    // Write the BITMAPFILEHEADER
    f.put( 'B' ).put( 'M' );                           // bfType
    f << lwrite( headers_size + pixel_data_size, 4 );  // bfSize
    f << lwrite( 0,                              2 );  // bfReserved1
    f << lwrite( 0,                              2 );  // bfReserved2
    f << lwrite( headers_size,                   4 );  // bfOffBits

    // Write the BITMAPINFOHEADER
    f << lwrite( 40,4 );  // biSize
    f << lwrite( 1920, 4 );  // biWidth
    f << lwrite( 1080,                           4 );  // biHeight
    f << lwrite( 1,                              2 );  // biPlanes
    f << lwrite( 24,                              2 );  // biBitCount
    f << lwrite( 0,                              4 );  // biCompression=BI_RGB
    f << lwrite( pixel_data_size,                4 );  // biSizeImage
    f << lwrite( 0,                              4 );  // biXPelsPerMeter
    f << lwrite( 0,                              4 );  // biYPelsPerMeter
    f << lwrite( 0,                              4 );  // biClrUsed
    f << lwrite( 0,                              4 );  // biClrImportant

     // Write the pixel data
   // std::cout << min_value << " " << max_value << std::endl;
    for (unsigned y = 1080; y; y--)           // bottom-to-top
      {
    //  std::cout << "Y Works" << std::endl;
      for (unsigned x = 0; x < 1920; x++)  // left-to-right
        {
    //    std::cout << "X Works" << std::endl;
        unsigned char red, green, blue;
        //
        // This is how we convert an integer value to a color:
        // by mapping it evenly along the CIECAM02 hue color domain.
        //
        // http://en.wikipedia.org/wiki/Hue
        // http://en.wikipedia.org/wiki/hsl_and_hsv#conversion_from_hsv_to_rgb
        //
        // The following algorithm takes a few shortcuts since
        // both 'value' and 'saturation' are always 1.0.
        //
        
        double val = 255 * getPix(x,y-1);
        red = int(val);
        green = int(val);
        blue = int(val);
	//std::cout << y-1 << " " << x <<  " " << groups[ x][ y-1].getValue() << " " << int(val) << std::endl;

        f.put( static_cast <char> (blue)  )
         .put( static_cast <char> (green) )
         .put( static_cast <char> (red)   );
        }

      if (padding_size) f << lwrite( 0, padding_size );
      }

    // All done!
	if (verbose) cout << "View::outputToBmp: BMP Output Complete" << endl;
    return f.good();
    }    
    
bool View::loadBmp() {

	// Using the reference http://www.winprog.org/tutorial/app_two.html

	OPENFILENAME openFileDialog;
	char fileName[MAX_PATH] = "";

	ZeroMemory(&openFileDialog, sizeof(openFileDialog));

	openFileDialog.lStructSize = sizeof(openFileDialog); 
	openFileDialog.lpstrFilter = "Bitmap Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
	openFileDialog.lpstrFile = fileName;
	openFileDialog.nMaxFile = MAX_PATH;
	openFileDialog.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	openFileDialog.lpstrDefExt = "bmp";
		
	if(!GetOpenFileName(&openFileDialog)){
		return 1;
	}

	// Using the reference http://www.dreamincode.net/forums/topic/26936-how-to-make-sense-of-the-bmp-format/
	
	HANDLE f; 
	f = CreateFile(fileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	RGBTRIPLE *inputImage;
	DWORD count;

	ReadFile(f, &bmpFileHeader, sizeof(bmpFileHeader), &count, NULL);
	ReadFile(f, &bmpInfoHeader, sizeof(bmpInfoHeader), &count, NULL);
	
	inputImage = new RGBTRIPLE[bmpInfoHeader.biWidth*bmpInfoHeader.biHeight];		
    	ReadFile(f, inputImage, sizeof(RGBTRIPLE)*bmpInfoHeader.biWidth*bmpInfoHeader.biHeight, &count,NULL);
    	
    	CloseHandle(f);
    	
    	for(int x=0; x<1920; x++){
		for(int y=0; y<1080; y++){
			setPix(x,y,int(int(((int) inputImage[(bmpInfoHeader.biHeight-1-y)*bmpInfoHeader.biWidth+x].rgbtBlue))/(float) 255));
			
		}
	}
	
	for (int x=0; x<Xres; x++){
		for (int y=0; y<Yres; y++){	
			groups[x][y].checkValue();
		}
	}
}

bool View::loadBmpSpecific(string fileNameStr) {
	// Using the reference http://www.winprog.org/tutorial/app_two.html
	char fileName[MAX_PATH] = "";
	
	strcpy(fileName, fileNameStr.c_str());
	if (verbose) cout << "View::loadBmpSpecific: Loading Specified File: \"" << fileName << "\"\n";

	// Using the reference http://www.dreamincode.net/forums/topic/26936-how-to-make-sense-of-the-bmp-format/
	
	HANDLE f; 
	f = CreateFile(fileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	RGBTRIPLE *inputImage;
	DWORD count;

	ReadFile(f, &bmpFileHeader, sizeof(bmpFileHeader), &count, NULL);
	ReadFile(f, &bmpInfoHeader, sizeof(bmpInfoHeader), &count, NULL);
	
	inputImage = new RGBTRIPLE[bmpInfoHeader.biWidth*bmpInfoHeader.biHeight];		
    	ReadFile(f, inputImage, sizeof(RGBTRIPLE)*bmpInfoHeader.biWidth*bmpInfoHeader.biHeight, &count,NULL);
    	
    	CloseHandle(f);
    	
    	for(int x=0; x<1920; x++){
		for(int y=0; y<1080; y++){
			setPix(x,y,int(int(((int) inputImage[(bmpInfoHeader.biHeight-1-y)*bmpInfoHeader.biWidth+x].rgbtBlue))/(float) 255));
			
		}
	}
	
	for (int x=0; x<Xres; x++){
		for (int y=0; y<Yres; y++){	
			groups[x][y].checkValue();
		}
	}
}

bool View::loadScaledBmp(){
	OPENFILENAME openFileDialog;
	char fileName[MAX_PATH] = "";

	ZeroMemory(&openFileDialog, sizeof(openFileDialog));

	openFileDialog.lStructSize = sizeof(openFileDialog); 
	openFileDialog.lpstrFilter = "Bitmap Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
	openFileDialog.lpstrFile = fileName;
	openFileDialog.nMaxFile = MAX_PATH;
	openFileDialog.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	openFileDialog.lpstrDefExt = "bmp";
		
	if(!GetOpenFileName(&openFileDialog)){
		return 1;
	}
	
	loadScaledBmpSpecific(fileName);
	
}

bool View::loadScaledBmpSpecific(string fileNameStr) {
	// Using the reference http://www.winprog.org/tutorial/app_two.html
	char fileName[MAX_PATH] = "";
	
	strcpy(fileName, fileNameStr.c_str());
	if (verbose) cout << "View::loadScaledBmpSpecific: Loading Specified File: \"" << fileName << "\"\n";

	// Using the reference http://www.dreamincode.net/forums/topic/26936-how-to-make-sense-of-the-bmp-format/
	
	HANDLE f; 
	f = CreateFile(fileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	RGBTRIPLE *inputImage;
	DWORD count;

	ReadFile(f, &bmpFileHeader, sizeof(bmpFileHeader), &count, NULL);
	ReadFile(f, &bmpInfoHeader, sizeof(bmpInfoHeader), &count, NULL);
	
	inputImage = new RGBTRIPLE[bmpInfoHeader.biWidth*bmpInfoHeader.biHeight];		
    	ReadFile(f, inputImage, sizeof(RGBTRIPLE)*bmpInfoHeader.biWidth*bmpInfoHeader.biHeight, &count,NULL);
    	
    	CloseHandle(f);
    	    	
    	pxGrpSize = floor(double(1920)/bmpInfoHeader.biWidth);
	if (verbose) cout << "View::loadScaledBmpSpecific: Setting PxGrpSize to: \"" << pxGrpSize << "\"\n";							
	::Xres = int(1920/pxGrpSize);
	::Yres = int(1080/pxGrpSize);
	resize(int(pxGrpSize));
	if (showDisplay){
		SDL_SetWindowSize(wDisp, Xres, Yres);
		SDL_DestroyRenderer(rDisp);
		rDisp = SDL_CreateRenderer(wDisp, -1, 0);
	}
	
	double greyscale = (pxGrpSize*pxGrpSize)/double(255);	
    	if (verbose) cout << "View::loadScaledBmpSpecific: Greyscale Scaler to: \"" << greyscale << "\"\n";	
    	for(int x=0; x<bmpInfoHeader.biWidth; x++){
		for(int y=0; y<bmpInfoHeader.biHeight; y++){
			groups[x][y].setValue((floor((inputImage[(bmpInfoHeader.biHeight-1-y)*bmpInfoHeader.biWidth+x].rgbtBlue * greyscale) + 0.5)));
		//	setPix(x,y,int(int(((int) inputImage[(bmpInfoHeader.biHeight-1-y)*bmpInfoHeader.biWidth+x].rgbtBlue))/(float) 255));
			
		}
	}
	
	for (int x=0; x<Xres; x++){
		for (int y=0; y<Yres; y++){	
			groups[x][y].checkValue();
		}
	}
}
