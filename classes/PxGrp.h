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
		int getData(int, int);
		void setValue(int);	
		string getLine(int);
};

PxGrp::PxGrp (){
	// Construct here.
	value = 0;
}

void PxGrp::setPxSize(int setSize){
	//cout << setSize << " ";
	size = setSize;
	data = new int*[size];
	
	for (int i=0; i<size; i++){
		data[i] = new int[size];
	}
	
	for (int x=0; x<size; x++){
		for (int y=0; y<size; y++){
			data[x][y] = 0;
		}
	}
}

int PxGrp::getValue(){
	return value;
}

int PxGrp::getData(int x, int y){
	return data[x][y];
}

string PxGrp::getLine(int i){
	
	string rtn;
	rtn = "";
	
	for (int x=0; x<size; x++){
		 
		if (data[x][(i)] == 1){
			rtn += '1';
		} else {
			rtn += '0';
		}
		//cout << data[x][(i)] << " " << rtn << endl;
	}
	return rtn;
}

void PxGrp::setValue(int sValue){
	value = sValue;
	
	for (int y=0; y<size; y++){
		for (int x=0; x<size; x++){		
			data[x][y] = 0;
		}
	}
	
	if (value != 0){
		switch (pxFillMode){
			case 2: 
				
				for (int filled = 0; filled < sValue; NULL){
					int x = rand() % size;
					int y = rand() % size;
					
					if (data[x][y] != 1){
						data[x][y] = 1;
						filled++;
					}
				}
				
				break;
			case 1:
			default:
				int filled = 0;
				
				for (int y=0; y<size; y++){
					for (int x=0; x<size; x++){
						
						data[x][y] = 1;
						filled++;
						
						if (filled == sValue){
							return;
						}
					}
				}
		}
	}	
}
