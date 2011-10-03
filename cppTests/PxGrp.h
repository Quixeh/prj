/** First Draft Pixel Grouping Class
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*
*/

class PxGrp {
	private:
		int value;
		int data[5][5];
	public:
		PxGrp ();
		int getValue();
		void setValue(int);	
};

PxGrp::PxGrp (){
	// Construct here.
	value = 0;
}

int PxGrp::getValue(){
	return value;
}

void PxGrp::setValue(int svalue){
	value = svalue;
}
