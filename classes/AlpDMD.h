using namespace std;

class AlpDMD {
	private:
		ALP_ID deviceID;
		long dmdType;
		long state;
		int init();
		
	public:
		AlpDMD();
};

AlpDMD::AlpDMD (){ // Constructor
	init();
}

int AlpDMD::init(){
	cout << "AlpDMD::init: Initialising." << endl;	
	
	// Allocate the ALP high-speed device
	// We do this by testing if the device returns ok or not.

	state = AlpDevAlloc(ALP_DEFAULT, ALP_DEFAULT, &deviceID);
 	
	if (ALP_OK != state){
		cout << "AlpDMD::init: Error 1 - Device Returned non-OK value: " << state << endl;
	}
}
