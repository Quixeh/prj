/** PxGrp Class Methods
*
* Header defined in PxGrp.h
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*/

#include <string>
#include <cmath>

using namespace std;

/* PxGrp
* Constructor - Initialise the pxGrp. 
*/
PxGrp::PxGrp (){
	// Construct here.
	value = 0; // Ensure the value is reset.
}

/* setPxSize
* Initialise all the data structures at the correct
* size and reset their values. Usually this is only
* used on the creation of a PxGrp entity.  
*/
void PxGrp::setPxSize(int setSize){
	size = setSize; // Set the internal Vars
	data = new int*[size]; // Allocate memory
	
	for (int i=0; i<size; i++){ // Create the second dimension
		data[i] = new int[size];
	}
	
	for (int x=0; x<size; x++){ // Reset the values
		for (int y=0; y<size; y++){
			data[x][y] = 0;
		}
	}
}

/* invert
* invert swaps the on pixels into off pixels, and the 
* off pixels into on pixels. 
*/
void PxGrp::invert(){
	value = abs(int(value - (size*size))); // Reset the value
	
	for (int x=0; x<size; x++){ // For each pixel
		for (int y=0; y<size; y++){
			if (data[x][y] == 1){ // If we're on
				data[x][y] = 0; // set off
			} else {
				data[x][y] = 1; // else set on
			}
		}
	}	
}

/* getValue
* accessor function to return the value of the pixel group. 
*/
int PxGrp::getValue(){
	return value;
}

/* getData
* accessor function to return the value of a single given pixel
*/
int PxGrp::getData(int x, int y){
	return data[x][y];
}

/* setData
* accessor function to set the value of a single given pixel
*/
void PxGrp::setData(int x, int y, int v){
	data[x][y] = v;
}

/* getLine
* Returns a line of pixels from the pixel group. This is useful
* when the parent view needs to be read out in a sequential fashion, 
* which occurs in all output methods. 
*/
string PxGrp::getLine(int i){ 
	
	string rtn; // declare return string
	rtn = "";   // reset it's value. 
	
	for (int x=0; x<size; x++){ // for each pixel in the line
		 if (data[x][(i)] == 1){ // set the value as a string
			rtn += '1';
		} else {
			rtn += '0';
		}
	}
	return rtn; // return this string
}

/* checkValue
* Loops over the group and counts the number of on pixels
* to ensure the integrity of the 'value' internal var. 
*/
void PxGrp::checkValue() {
	int count = 0; // initialise our count. 
	for (int y=0; y<size; y++){ // for every pixel 
		for (int x=0; x<size; x++){		
			if (data[x][y] == 1){ // if its on.
				count++; // increment count
			}
		}
	}
	value = count; // set this as our internal value. 
}

/* setValue
* Sets the value of the group when asked, and also
* handles the filling mechanism of the pixel group. 
*/
void PxGrp::setValue(int sValue){ 
	value = sValue; // set our internal var. 	
	for (int y=0; y<size; y++){ // for each pixel
		for (int x=0; x<size; x++){		
			data[x][y] = 0; // reset to off.
		}
	}
	
	if (value != 0){ // If our value is not 0, ie no pixels on.
		switch (pxFillMode){ // Consult program settings to determine
		                     // the method to use to populate the group
			case 2:  // Random Filling
			
			// Loop over the group, until we have filled the desired
			// number of pixels.
				for (int filled = 0; filled < sValue; NULL){
					int x = rand() % size; // create random co-ords
					int y = rand() % size;
					
					if (data[x][y] != 1){ // If not already used...
						data[x][y] = 1; // turn on
						filled++;  // Increment count
					}
				}
				
				break; // Leave the switch
				
			case 1: // Sequential Filling
			default:
				int filled = 0; // Internal counter
				
				for (int y=0; y<size; y++){ // for each pixel in the group
					for (int x=0; x<size; x++){
						
						data[x][y] = 1; // set on
						filled++; // increment count
						
						if (filled == sValue){ // if we're done filling
							return; // leave the function.
						}
					}
				}
		}
	}	
}

// EOF
