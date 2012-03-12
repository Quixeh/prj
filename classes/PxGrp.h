/** PxGrp Class
* Represents groups of pixels in the program.
*
* Methods defined in PxGrp.cpp
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*/

#include <string>

using namespace std;

class PxGrp {
	private: // Functions and vars usable only by the class
		int value;  // How many pixels in this group are on.
		int size;   // sqrt of many pixels this group contains.
		int** data; // Variable to hold each individual pixel's state.
		
	public:  // Functions and vars usable by any external object
		PxGrp();    // Constructor
		void invert(); // Swap on pixels for off pixels
		void setPxSize(int); // Resize the group
		int getValue(); // Retrieve the amount of on pixels in this group
		int getData(int, int); // Retrieve the state of a single pixel.
		void setData(int, int, int); // Set a single pixel manually.
		void setValue(int);	// Set the group's value
		string getLine(int);    // Retrieve a line of pixels. 
		void checkValue();      // Make sure that the amount of on pixels 
		                 	// corresponds to the group's value
};

// EOF
