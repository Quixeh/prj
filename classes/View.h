/** View Class
* Represents images in the program. Each view class consists of many 
* PxGrp classes
*
* Methods defined in View.cpp
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*/


#include <cmath>
#include <fstream>
#include <string>

using namespace std;

// Useful Structures for Binary files. 
// These were taken from the same source as the Bitmap Output Code.

struct lwrite {  // Declare the structure
	unsigned long value; // Internal Vars
	unsigned      size;
	lwrite( unsigned long value, unsigned size ):
	value( value ), size( size )
	{ }
};

// When using ostream (writing to files) we can redefine the << operator
// as follows to allow us to write Bitmaps. 

inline std::ostream& operator << ( std::ostream& outs, const lwrite& v ){ 
	unsigned long value = v.value;
	for (unsigned cntr = 0; cntr < v.size; cntr++, value >>= 8)
		outs.put( static_cast <char> (value & 0xFF) );
	return outs;
}

// Class Definition

class View { 
	private: // Functions and vars usable only by the class
		int pxSize, Xres, Yres; // Dimensions of the pixel groups and the view
		PxGrp** groups; // All the pxGrps are held within this var
		Uint32  gPixel(SDL_Surface*, int, int); // Get and Set for SDL operations
		void sPixel(SDL_Surface*, int, int, Uint32);
		
	public: // Functions and vars usable by any external object
		void applyXfn(int); // Apply a function to the view, in the x direction
		void applyYfn(int); // Apply a function to the view,in the y direction
		void applyGrid();   // Apply a grid to the view, size depends on pxSize. 
		void applySinxfn(int); // Apply a sine function, size given by pxSize
		void applySqxfn(int);  // Apply a square function, size given by pxSize
		void applyXStripes();  // Apply stripes to the view in the x direction, 
		void applyYStripes();  // As above, but for y. Sizes given by pxSize. 
		void invert();     // Interchange black pixels for white ones, etc. 
		View(int);         // Constructor
		void resize(int);  // Change the size of the view
		int getPix(int, int); // Get the value of the specified pixel
		int setPix(int, int, int); // Set the value of the specified pixel
		char output();     // Output control function.  
		void randomise(double); // Apply a random potential to the view
		bool outputToSdl();   // Show the view on the scaled Display window in SDL
		bool outputToBmp();   // Output a scaled bitmap representaion
		bool outputToSdlFull(); // Display the view on the fullsize SDL window
		bool outputToBmpFull(); // Output a fullsize bitmap representation
		bool loadBmp(); // Load a 1920x1080 24bit RGB bitmap
		bool loadBmpSpecific(string); // Load a specified bitmap
		bool loadScaledBmp(); // Load a 24bit RGB bitmap with dimensions less than 1920x1080
		bool loadScaledBmpSpecific(string); // As above, but a specified file. 
		void checkValue();  // Tell all the pxGrps to check how many on pixels they own. 
		
};
 // EOF

