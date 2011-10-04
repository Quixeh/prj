/** First Draft Pixel Grouping Class
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*
*/

class PxGrp {
	private:
		int value, size;
		int** data;
		
	public:
		PxGrp ();
		void setPxSize(int);
		int getValue();
		void setValue(int);	
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

void PxGrp::setValue(int sValue){
	value = sValue;
}
