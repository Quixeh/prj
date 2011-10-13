/** First Draft Pixel Grouping Class
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*
*/
#include <string>

using namespace std;

class PxGrp {
	private:
		int value, size;
		int** data;
		
	public:
		PxGrp ();
		void setPxSize(int);
		int getValue();
		void setValue(int);	
		string getLine(int);
};

PxGrp::PxGrp (){
	// Construct here.
	value = 0;
}

void PxGrp::setPxSize(int setSize){
	
	size = setSize;
	data = new int*[size];
	
	for (int i=0; i<size; i++){
		data[i] = new int[size];
	}
}

int PxGrp::getValue(){
	return value;
}

string PxGrp::getLine(int i){
	
	string rtn;
	
	for (int x=0; x<size; x++){
		if (data[x][(i-1)] == 0){
			rtn += '0';
		} else {
			rtn += '1';
		}
	}
	return rtn;
}

void PxGrp::setValue(int sValue){
	value = sValue;
	
	switch (pxFillMode){
		case 1:
		default:
			int filled = 0;
			for (int x=0; x<size; x++){
				for (int y=0; y<size; y++){
					data[x][y] = 1;
					filled++;
					
					if (filled == value){
						return;
					}
				}
			}
	}			
}
