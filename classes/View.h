/** First Draft View Class
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*
*/


#include <cmath>
#include <fstream>
#include <string>

using namespace std;

// Useful Structures

struct lwrite {
	unsigned long value;
	unsigned      size;
	lwrite( unsigned long value, unsigned size ):
	value( value ), size( size )
	{ }
};

inline std::ostream& operator << ( std::ostream& outs, const lwrite& v ){
	unsigned long value = v.value;
	for (unsigned cntr = 0; cntr < v.size; cntr++, value >>= 8)
		outs.put( static_cast <char> (value & 0xFF) );
	return outs;
}

// Class Definition

class View {
	private:
		int pxSize, Xres, Yres;
		PxGrp** groups;
		Uint32  gPixel(SDL_Surface*, int, int);
		void sPixel(SDL_Surface*, int, int, Uint32);
		
	public:
		void applyXfn();
		View(int);
		char output(); 
		void randomise();
		bool outputToSdl();
		bool outputToBmp();
};

// Member Functions

View::View(int setPxSize){
	pxSize = setPxSize;
	Xres = 1920/pxSize;
	Yres = 1080/pxSize;
	groups = new PxGrp*[Xres];
	for (int i=0;i<Xres;i++){
		groups[i] = new PxGrp[Yres];
		for (int j=0;j<Yres;j++){
			groups[i][j].setPxSize(5);
		}
	}
}

void View::randomise(){
	srand(time(NULL));	
	for (int x=0; x<Xres; x++){
		for (int y=0; y<Yres; y++){		
			int IO = rand() % 5;
			if (IO == 4){
				groups[x][y].setValue(rand() % 25 + 1);
				//cout << groups[x][y].getValue() << " ";
			}
		}
	}
}

void View::applyXfn(){

     double xV = 0;

	for (int x=0; x<Xres; x++){
        xV = (x/double(Xres))* (pxSize * pxSize); // Grey-Scale
        //xV = ( ((x-(Xres/2.0))*(x-(Xres/2.0))/double((Xres/2.0) * (Xres/2.0))) * (pxSize * pxSize));
		for (int y=0; y<Yres; y++){
			groups[x][y].setValue(int(floor(xV+0.5))); 
		}
                cout << xV << " -> " << int(floor(xV+0.5)) << endl;
	}	
}

char View::output(){
	
	cout << "View::output: Commencing Output" << endl;

   outputToBmp();
   outputToSdl(); 
    
	cout << "View::output: Output Complete" << endl;
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
	cout << "View::outputToSdl: Commencing SDL Output" << endl;

	cout << "View::outputToSdl: Locking Texture" << endl;   
	//SDL_LockTexture(tDisp, NULL, &pixels, &pitch);
	
	int min_value = 0;
	int max_value = pxSize * pxSize;
     	
	cout << "View::outputToSdl: Cycling..." << endl;
	
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

	cout << "View::outputToSdl: Unlocking Texture" << endl;
	//SDL_UnlockTexture(tDisp);

	SDL_RenderClear(rDisp);
	SDL_RenderCopy(rDisp, newDisplay, NULL, NULL);
	SDL_RenderPresent(rDisp);
    
	cout << "View::outputToSdl: SDL Output Complete" << endl;
}

bool View::outputToBmp(){
	int min_value = 0;
	int max_value = pxSize * pxSize;
     
	cout << "View::outputToBmp: Commencing BMP Output" << endl;
     
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
    	cout << "View::outputToBmp: BMP Output Complete" << endl;
    return f.good();
    }    

