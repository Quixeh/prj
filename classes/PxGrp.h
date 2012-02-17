/** PxGrp Class
* Represents p in the program. Each view class consists of many 
* PxGrp classes
*
* Methods defined in PxGrp.cpp
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*/

#include <string>

using namespace std;

class PxGrp {
	private:
		int value, size;
		int** data;
		
	public:
		PxGrp();
		void invert();
		void setPxSize(int);
		int getValue();
		int getData(int, int);
		void setData(int, int, int);
		void setValue(int);	
		string getLine(int);
		void checkValue();
};


