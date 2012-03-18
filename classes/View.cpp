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

/* View
* Constructor - Create an Image Object
*/
View::View(int setPxSize){
	pxSize = setPxSize; // Set the internal Var
	Xres = 1920/pxSize; // Calculate the scaled resolutions
	Yres = 1080/pxSize;
	groups = new PxGrp*[Xres]; // Create the PxGrp Objects
	for (int i=0;i<Xres;i++){ 
		groups[i] = new PxGrp[Yres]; 
		for (int j=0;j<Yres;j++){
			groups[i][j].setPxSize(pxSize); // Set their size
		}
	}
}

/* Resize
* Resize the PxGrps within the view. This involves removing them all
* and recreating them. 
*/
void View::resize(int setPxSize){
	
	delete [] groups;  // Remove the previous data
	pxGrpSize = setPxSize; // Reset the sizes (as above)
	pxSize = setPxSize;
	Xres = 1920/pxSize;
	Yres = 1080/pxSize;
	groups = new PxGrp*[Xres]; // Recreate the Pixel Groups
	for (int i=0;i<Xres;i++){
		groups[i] = new PxGrp[Yres];
		for (int j=0;j<Yres;j++){
			groups[i][j].setPxSize(pxSize);  // Set their size
			groups[i][j].setValue(0); // Initialise their values
			
		}
	}
	checkValue(); // Ensure their internal values are correct. 
}

/* Randomise
* Create a random intensity pattern as the image. 
*/
void View::randomise(double chance){ 
		
	for (int x=0; x<Xres; x++){ // For each PxGrp...
		for (int y=0; y<Yres; y++){		
			int IO = rand() % int(1/chance); // Chance of being on or off.
			if (IO == 0){ // If we're on...
				int val = rand() % int(pxGrpSize*pxGrpSize); // Randomise the value
				val++;
				groups[x][y].setValue(val); // Set this the value.
				
			} else { // Otherwise, set as 0.
				groups[x][y].setValue(0);
			}
		}
	}
}

/* getPix
* Return the Value of any single pixel within the View
*/
int View::getPix(int x, int y){
	int h = int(floor((x)/double(pxGrpSize))); // Find out which PxGrp this
	int k = int(floor((y)/double(pxGrpSize))); // pixel is in. 
	int i = int(x - (h*pxGrpSize)); // Find the relative co-ordinates
	int j = int(y - (k*pxGrpSize)); // within this group
	groups[h][k].getData(i,j);      // Ask the pixel group for the value.
}

/* setPix
* Set the Value of any single pixel within the View
*/
int View::setPix(int x, int y, int v){
	int h = int(floor((x)/double(pxGrpSize))); // Find out which PxGrp this
	int k = int(floor((y)/double(pxGrpSize))); // pixel is in. 
	int i = int(x - (h*pxGrpSize)); // Find the relative co-ordinates
	int j = int(y - (k*pxGrpSize)); // within this group
	groups[h][k].setData(i,j,v);    // Set the value.
}

/* checkValue
* Check the value of the pixels in the groups to ensure the number of on pixels
* relates to the 'value'.
*/
void View::checkValue(){
	for (int x=0; x<Xres; x++){ // For each group
		for (int y=0; y<Yres; y++){
			groups[x][y].checkValue(); // tell it to check itself.
		}
	}	
}

/* applyXfn
* Apply a mathematical function in the x-direction, be it greyscale
* harmonic or a sin function. 
*/
void View::applyXfn(int type){

     double xV = 0; // Individual pixel value

	for (int x=0; x<Xres; x++){ // for each group in the xdirection
		switch (type){ // Calculate the value....
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
		
		for (int y=0; y<Yres; y++){ // For each y pixel in this x band, set the value. 
			groups[x][y].setValue(int(floor(xV+0.5))); 
		}
        }	
}

/* applySinxfn
* Apply a more robust sin function with a given distance between peaks in the x direction. 
*/
void View::applySinxfn(int dist){
	double xV = 0;
	if (pxSize > dist){ // Check required spatial frequency. 
		cout << "Can't create a sinx with distance between peaks less than the PxGrp size." << endl;
	} else {
		for (int x=0; x<Xres; x++){
			for (int y=0; y<Yres; y++){ // Set value for each y group along x. 
				xV = abs(sin(pi*x*pxSize/double(dist)))*(pxSize * pxSize);
				groups[x][y].setValue(int(floor(xV+0.5))); 
			}
		}
	}
}

/* applySinxfn
* Apply stripes with a given distance between them. 
*/
void View::applySqxfn(int dist){
	double xV = 0;
	if (pxSize > dist){ // Check required spatial frequency. 
		cout << "Can't create a sqx with distance between peaks less than the PxGrp size." << endl;
	} else {
		for (int x=0; x<Xres; x++){ // Create a counter and a flipper. The flipper alternates value
			double count = x*pxSize; // dependent on the distance we are across the image.
			double flip = floor(count/double(dist*0.5)); 
			if ( int(flip) % 2== 0 ){ // If flip is even
				xV = (pxSize * pxSize);
			} else {
				xV = 0;           // If flip is odd
			}
			for (int y=0; y<Yres; y++){ // Set this value for all the Y groups
				groups[x][y].setValue(int(xV)); 
			}
		}
	}
}

/* applyYfn
* Apply a mathematical function in the y-direction, be it greyscale
* harmonic or a sin function. See applyXfn for details.
*/
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
	}	
}

/* applyGrid
* Apply a Grid to the image. This is a checker-style grid. 
*/
void View::applyGrid(){
	
	int value = 0;	
	for (int y=0; y<Yres; y++){ // For every Pxgrp
		for (int x=0; x<Xres; x++){ 
			if (value == 0){ // Alternate the value...
				groups[x][y].setValue(0);
				value = 1;
			} else {
				groups[x][y].setValue(pxSize*pxSize);
				value = 0;
			}
		} // At the end of each row, flip the value once more.
		if (value == 0){
			value = 1;
		} else {
			value = 0;
		}
	}	
}

/* applyXStrips
* Apply alternating stripes on the image, based on the PxGrp size. This performs
* the function in the x direction. 
*/
void View::applyXStripes(){
	
	int value = 0;	// Reset the value
	for (int y=0; y<Yres; y++){ // For each group...
		for (int x=0; x<Xres; x++){
			if (value == 0){ // Alternate Value... 
				groups[x][y].setValue(0);
				value = 1;
			} else {
				groups[x][y].setValue(pxSize*pxSize);
				value = 0;
			}
		}
	}	
}

/* applyYStrips
* Apply alternating stripes on the image, based on the PxGrp size. This performs
* the function in the Y direction. See above for comments.
*/
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

/* invert
* Swap black pixels for white pixels, and white pixels for black ones. 
*/
void View::invert(){
	for (int x=0; x<Xres; x++){ // For each group, 
		for (int y=0; y<Yres; y++){
			groups[x][y].invert(); // tell them to invert. 
		}
	}
}


/* Output
* Control the output of the View. Operates on the 'Display' Command.
*/
char View::output(){
	
	if (verbose) cout << "View::output: Commencing Output" << endl;
	
	if (showFullSize) outputToSdlFull(); // If we have the full SDL window..
	if (showDisplay) outputToSdl();      // If we have the small SDL window..
	if (outputFullBmp) outputToBmpFull();// If we have the full bitmap..
	if (outputBmp) outputToBmp();        // If we have the small bitmap..
  
   
	if (verbose) cout << "View::output: Output Complete" << endl;
}


/* gPixel & sPixel
* SDL Commands for getting and setting pixels. These use the SDL surface entities. 
*/
Uint32  View::gPixel( SDL_Surface *surface, int x, int y ){
    Uint32 *pixels = (Uint32 *)surface->pixels;
    return pixels[ ( y * surface->w ) + x ];
}

void View::sPixel( SDL_Surface *surface, int x, int y, Uint32 pixel ){
    Uint32 *pixels = (Uint32 *)surface->pixels;
    pixels[ ( y * surface->w ) + x ] = pixel;
}

/* Output to SDL
* Updates the Scaled SDL display. 
*/
bool View::outputToSdl(){
	if (verbose) cout << "View::outputToSdl: Commencing SDL Output" << endl;
	if (verbose) cout << "View::outputToSdl: Xres " << Xres << " Yres " << Yres << endl;  
	
	int min_value = 0; // Minimum PxGrp Value
	int max_value = pxSize * pxSize; // Maximum PxGrp Value
     	
	if (verbose) cout << "View::outputToSdl: Cycling..." << endl;
	
	SDL_Surface *surf; // Create the SDL Surface Var on which to place the image
	
	Uint32 rmask, gmask, bmask, amask; // Setup Byteorder masks for SDL
    
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
	
	// Create the SDL Surface
	surf = SDL_CreateRGBSurface(0, Xres, Yres, 32, rmask, gmask, bmask, amask); 
	
	// For each pixel group...	
	for (int x=0; x<Xres; x++){
        	for (int y=0; y<Yres; y++){ // Calculate value, set as the pixel on the surface.
        		double val = 255 * ((groups[x][y].getValue() - min_value) / double(max_value));
			sPixel(surf, x, y, SDL_MapRGB(surf->format, int(val), int(val), int(val)));
          	}
      	} 
    
    	// Create a new texture..
	SDL_Texture* newDisplay;
	
	// Convert Surface to Texture. 
	newDisplay = SDL_CreateTextureFromSurface(rDisp, surf);

	if (verbose) cout << "View::outputToSdl: Unlocking Texture" << endl;
	
	SDL_RenderClear(rDisp); // Clear Copy and Show the Texture Using the renderer. 
	SDL_RenderCopy(rDisp, newDisplay, NULL, NULL);
	SDL_RenderPresent(rDisp);
    
	if (verbose) cout << "View::outputToSdl: SDL Output Complete" << endl;
}

/* Output to SDL Full
* Updates the Full SDL display. See above for comments
* Potential for these two functions to be combined.  
*/
bool View::outputToSdlFull(){
	if (verbose) cout << "View::outputToSdlFull: Commencing Full SDL Output" << endl;
	if (verbose) cout << "View::outputToSdlFull: Locking Texture" << endl;   

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

/* Output to BMP
* Updates the Scaled BMP file. 
*/
bool View::outputToBmp(){
	
	// Using the reference: http://www.cplusplus.com/forum/beginner/23177/
	
	int min_value = 0; // As above, set max and min values. 
	int max_value = pxSize * pxSize;
     
	if (verbose) cout << "View::outputToBmp: Commencing BMP Output" << endl;
     
	// Open the output BMP file
	std::ofstream f( "output.bmp", std::ios::out | std::ios::trunc | std::ios::binary );
	if (!f) return false;

	// Deal with the headers... 
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
	f << lwrite( 24,                             2 );  // biBitCount
	f << lwrite( 0,                              4 );  // biCompression=BI_RGB
	f << lwrite( pixel_data_size,                4 );  // biSizeImage
	f << lwrite( 0,                              4 );  // biXPelsPerMeter
	f << lwrite( 0,                              4 );  // biYPelsPerMeter
	f << lwrite( 0,                              4 );  // biClrUsed
	f << lwrite( 0,                              4 );  // biClrImportant

	// Write the pixel data
	for (unsigned y = Yres; y; y--){           // bottom-to-top
		for (unsigned x = 0; x < Xres; x++){  // left-to-right
			// Create RGB Components.
			unsigned char red, green, blue;
		               
		        double val = 255 * ((groups[x][y-1].getValue() - min_value) / double(max_value));
		        red = int(val);
		        green = int(val);
		        blue = int(val); // Set all the same. 
		       
		        f.put( static_cast <char> (blue)  )
		         .put( static_cast <char> (green) )
		         .put( static_cast <char> (red)   );
		        }

		if (padding_size) f << lwrite( 0, padding_size ); // Write to file
	}

	// All done!
	if (verbose) cout << "View::outputToBmp: BMP Output Complete" << endl;
	return f.good(); // Close file.
}    

/* Output to BMP Full
* Updates the Full BMP file. See above for comments
* Potential for these two functions to be combined.  
*/
bool View::outputToBmpFull(){
	
	// Using the reference: http://www.cplusplus.com/forum/beginner/23177/
	
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
	for (unsigned y = 1080; y; y--){        // bottom-to-top
	
		for (unsigned x = 0; x < 1920; x++){  // left-to-right
		
			unsigned char red, green, blue;
			double val = 255 * getPix(x,y-1);
			
			red = int(val);
			green = int(val);
			blue = int(val);
			
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


/* loadBmp
* Deprecated by loadScaledBmp. Loads a 1:1 scale 24bit bitmap.
*/
bool View::loadBmp() {
	
	// Using the reference http://www.winprog.org/tutorial/app_two.html
	
	OPENFILENAME openFileDialog; // Create the open file dialogue box.
	char fileName[MAX_PATH] = "";

	ZeroMemory(&openFileDialog, sizeof(openFileDialog)); // Create zeroed vars. 
  
  	// Set options for the dialogue box.
  	
	openFileDialog.lStructSize = sizeof(openFileDialog); 
	
	// Allowed Filetypes.
	openFileDialog.lpstrFilter = "Bitmap Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0"; 
	openFileDialog.lpstrFile = fileName; // Var to place it in. 
	openFileDialog.nMaxFile = MAX_PATH;
	openFileDialog.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY; //Flags
	openFileDialog.lpstrDefExt = "bmp";
		
	if(!GetOpenFileName(&openFileDialog)){ // If we press cancel..
		return 1; /// End Function
	}
	
	// Otherwise, use load Specific. 
	loadBmpSpecific(fileName);
}


/* loadBmpSpecific
* Load a specified BMP
*/
bool View::loadBmpSpecific(string fileNameStr) {
	
	char fileName[MAX_PATH] = "";
	
	strcpy(fileName, fileNameStr.c_str());
	if (verbose) cout << "View::loadBmpSpecific: Loading Specified File: \"" << fileName << "\"\n";

	// Using the reference http://www.dreamincode.net/forums/topic/26936-how-to-make-sense-of-the-bmp-format/
	
	HANDLE f; // Create the file Handle with attribs
	f = CreateFile(fileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	
	// Variables to hold various parts of the file, such as the info and file headers.
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	RGBTRIPLE *inputImage; // RGB Structure to read the image into
	DWORD count; // Counter of pixels

	// Read the headers.
	ReadFile(f, &bmpFileHeader, sizeof(bmpFileHeader), &count, NULL);
	ReadFile(f, &bmpInfoHeader, sizeof(bmpInfoHeader), &count, NULL);
	
	// Read in the image
	inputImage = new RGBTRIPLE[bmpInfoHeader.biWidth*bmpInfoHeader.biHeight];		
    	ReadFile(f, inputImage, sizeof(RGBTRIPLE)*bmpInfoHeader.biWidth*bmpInfoHeader.biHeight, &count,NULL);
    	
    	// Close the file
    	CloseHandle(f);
    	
    	// For each pixel in the View
    	for(int x=0; x<1920; x++){
		for(int y=0; y<1080; y++){ // Set the value based on the green component (1:1 means that it should all be the same)
			setPix(x,y,int(int(((int) inputImage[(bmpInfoHeader.biHeight-1-y)*bmpInfoHeader.biWidth+x].rgbtGreen))/(float) 255));
		}
	}
	
	// Remove the input image from the memory
	free(inputImage);
	
	// For each pixel group, check the value. 
	for (int x=0; x<Xres; x++){
		for (int y=0; y<Yres; y++){	
			groups[x][y].checkValue();
		}
	}
}

/* loadScaledBmp
* See loadBmp for comments. 
*/
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

/* loadScaledBmpSpecific
* Loads a scaled Bitmap
*/
bool View::loadScaledBmpSpecific(string fileNameStr) {
	
	char fileName[MAX_PATH] = "";
	strcpy(fileName, fileNameStr.c_str());
	if (verbose) cout << "View::loadScaledBmpSpecific: Loading Specified File: \"" << fileName << "\"\n";

	// Using the reference http://www.dreamincode.net/forums/topic/26936-how-to-make-sense-of-the-bmp-format/
	
	HANDLE f; // Create the file Handle with attribs
	f = CreateFile(fileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	// Variables to hold various parts of the file, such as the info and file headers.
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	RGBTRIPLE *inputImage;
	DWORD count;
	
	// Read Headers
	ReadFile(f, &bmpFileHeader, sizeof(bmpFileHeader), &count, NULL);
	ReadFile(f, &bmpInfoHeader, sizeof(bmpInfoHeader), &count, NULL);
	
	// Read Image
	inputImage = new RGBTRIPLE[bmpInfoHeader.biWidth*bmpInfoHeader.biHeight];		
    	ReadFile(f, inputImage, sizeof(RGBTRIPLE)*bmpInfoHeader.biWidth*bmpInfoHeader.biHeight, &count,NULL);
    	
    	// Close File
    	CloseHandle(f);
    	    	
    	// Calculate the size of the pixel groups needed. 
    	if (floor(double(1920)/bmpInfoHeader.biWidth) != pxGrpSize){
	    	pxGrpSize = floor(double(1920)/bmpInfoHeader.biWidth); // PxGrp = SLM width/Image Width
	    	while((pxGrpSize*bmpInfoHeader.biHeight) > 1080){ // If the height doesn't match, 
			pxGrpSize--; // Reduce the size of the PxGrp
			if ((pxGrpSize*bmpInfoHeader.biHeight) > 1080){ // If this doesn't help, give up!
				cout << "View::loadScaledBmpSpecific: Error 1 - Image is unsuitable for scaled import." << endl;
				return 0;
			}
		}
		
		if (verbose) cout << "View::loadScaledBmpSpecific: Setting PxGrpSize to: \"" << pxGrpSize << "\"\n";							
		
		// Set the global scaled resolutions
		::Xres = int(1920/pxGrpSize);
		::Yres = int(1080/pxGrpSize);
		resize(int(pxGrpSize)); // Resize the View
		
		if (showDisplay){ // Update SDL if required.  
			SDL_SetWindowSize(wDisp, Xres, Yres);
			SDL_DestroyRenderer(rDisp);
			rDisp = SDL_CreateRenderer(wDisp, -1, 0);
		}
	}
	
	// Calculate the greyscale banding factor
	double greyscale = (pxGrpSize*pxGrpSize)/double(255);	
    	
	if (verbose) cout << "View::loadScaledBmpSpecific: Greyscale Scaler to: \"" << greyscale << "\"\n";	
    
    	for(int x=0; x<bmpInfoHeader.biWidth; x++){ // For each pixel in the image
		for(int y=0; y<bmpInfoHeader.biHeight; y++){
			
			// Calculate the luminosity in 3 steps (long var names!)
			double luminosity = 0.299 * inputImage[(bmpInfoHeader.biHeight-1-y)*bmpInfoHeader.biWidth+x].rgbtRed;
			luminosity = luminosity + (0.587 *inputImage[(bmpInfoHeader.biHeight-1-y)*bmpInfoHeader.biWidth+x].rgbtGreen);
			luminosity = luminosity + (0.114 *inputImage[(bmpInfoHeader.biHeight-1-y)*bmpInfoHeader.biWidth+x].rgbtBlue);
			groups[x][y].setValue((floor((luminosity * greyscale) + 0.5))); // Scale & Set as the PxGrp value.	
		}
	}
	
	// Free input image from the memory. 
	free(inputImage);
	
	// Check the values.
	for (int x=0; x<Xres; x++){
		for (int y=0; y<Yres; y++){	
			groups[x][y].checkValue();
		}
	}
}
