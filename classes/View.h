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
		void applyXfn(int);
		void applyYfn(int);
		void invert();
		View(int);
		void resize(int);
		int getPix(int, int);
		int setPix(int, int, int);
		char output(); 
		void randomise(double);
		bool outputToSdl();
		bool outputToBmp();
		bool outputToSdlFull();
		bool outputToBmpFull();
		bool loadBmp();
		void checkValue();
		
};


