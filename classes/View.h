/** First Draft View Class
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*
*/


#include <cmath>
#include <fstream>
#include <string>

using namespace std;

  struct lwrite
    {
    unsigned long value;
    unsigned      size;
    lwrite( unsigned long value, unsigned size ):
      value( value ), size( size )
      { }
    };

  //--------------------------------------------------------------------------
  inline std::ostream& operator << ( std::ostream& outs, const lwrite& v )
    {
    unsigned long value = v.value;
    for (unsigned cntr = 0; cntr < v.size; cntr++, value >>= 8)
      outs.put( static_cast <char> (value & 0xFF) );
    return outs;
    }

class View {
	private:
		int pxSize, Xres, Yres;
		PxGrp** groups;
	public:
		void applyXfn();
		View(int);
		char output(); 
		void randomise();
		bool outputToSdl();
		bool outputToBmp();
};

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
        // xV = (x/double(Xres))* (pxSize * pxSize); // Grey-Scale
        xV = ( ((x-(Xres/2.0))*(x-(Xres/2.0))/double((Xres/2.0) * (Xres/2.0))) * (pxSize * pxSize));
		for (int y=0; y<Yres; y++){
			groups[x][y].setValue(int(xV)); 
		}
      //  cout << int(xV) << " " << itterations << endl;
	}	
}

char View::output(){

	int** data;
	data = new int*[Xres];
	for (int i=0; i<Xres; i++){
		data[i] = new int[Yres];
	}
    cout << "View::output: Output Data Created" << endl;

	for (int x=0; x<(Xres); x++){
		for (int y=0; y<Yres; y++){
           // cout << " " << y << endl;
           // data[x][y] = 0;
		  	data[x][y] = groups[x][y].getValue();
		  	
		  	//cout << "x: " << x << " y: " << y << " orig: " << groups[x][y].getValue() << " copy: " << data[x][y] << endl;
		}
	}
	cout << "View::output: Output Data Cloned" << endl;

	for (int y=0; y<Yres; y++){
		for (int line=1; line<5; line++){
			for (int x=0; x<Xres; x++){
				//groups[x][y]->output(line);
				//cout << x << " " << y << " " << line << endl;
			}
		}
	}
	
	cout << "View::output: Commencing Output" << endl;

   outputToBmp();
   outputToSdl(); 
    
/*    for (int y=0; y<Yres; y+=10){
        cout << groups[0][y].getValue() << " ";
    }
    
    cout << endl;

    for (int y=0; y<Yres; y+=10){
        cout << groups[Xres-1][y].getValue() << " ";
    }
    
    cout << endl;*/
	cout << "View::output: Output Complete" << endl;
}

bool View::outputToSdl(){
      cout << "View::outputToSdl: Commencing SDL Output" << endl;
      // Define Pixel Manipulation Vars
    Uint32 *pix; // Current Working Pixel/   
    void *pixels; // Pixels to work upon - comes from the texture lock
    int pitch; // Don't quite know what this does....
   int min_value = 0;
     int max_value = pxSize * pxSize;
     
  cout << "View::outputToSdl: Locking Texture" << endl;   
  SDL_LockTexture(tDisp, NULL, &pixels, &pitch);
    
    
    cout << "View::outputToSdl: Cycling..." << endl;
    cout << "Yres: " << Yres << " xRes: " << Xres << " Pitch: " << pitch << endl;
    for (int x=0; x<190; x++){
       cout << x << " ";
       pix = (Uint32*)((Uint8*)pixels + x *pitch);
       for (int y=0; y<50; y++){
           double val = 255 * ((groups[x][y].getValue() - min_value) / double(max_value));
          SDL_Color colour = {int(val),int(val),int(val)};
           *pix = ((colour.r<<16)|(colour.g<<8)|colour.b);
      }
   }
    
   cout << "View::outputToSdl: Unlocking Texture" << endl;
   SDL_UnlockTexture(tDisp);

    SDL_RenderClear(rDisp);
    SDL_RenderCopy(rDisp, tDisp, NULL, NULL);
    SDL_RenderPresent(rDisp);
    
      cout << "View::outputToSdl: SDL Output Complete" << endl;
}

bool View::outputToBmp(){
     int min_value = 0;
     int max_value = pxSize * pxSize;
     
     cout << "In BMP Output." << endl;
     
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
    return f.good();
    }    

