using namespace std;

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
		return 1;
	}
	
	// Inquire DMD type
	
	state = AlpDevInquire(deviceID, ALP_DEV_DMDTYPE, &dmdType );
	
	if (ALP_OK != state){
		cout << "AlpDMD::init: Error 2 - Device Returned non-OK value: " << state << endl; 
		return 1;
	}
	
	switch (dmdType) {
		case ALP_DMDTYPE_DISCONNECT: // no DMD attached
			cout << "AlpDMD::init: Error 3 - DMD Disconnected" << endl;
			return 1;
		case ALP_DMDTYPE_1080P_095A : // 1080p attached
			cout << "AlpDMD::init: Using 1080p DMD - Device ID: " << deviceID << endl;
			break; 
		default: // unsupported DMD type
			cout << "AlpDMD::init: Error 4 - Unsupported DMD - Exiting" << endl;
			return 1;
	}
}

void AlpDMD::quit(){
	cout << "AlpDMD::quit: Halting and Freeing Device..." << endl;
	AlpDevHalt(deviceID);
	AlpDevFree(deviceID);
	cout << "AlpDMD::quit: Completed" << endl;
}

bool AlpDMD::outputView(View view){
	ALP_ID sequenceID;
	UCHAR *transmitImages = NULL;
	transmitImages = new UCHAR[1920*1080];
	
	for (int x=0; x<1920; x++){
		for (int y=0; y<1920; y++){
			transmitImages[(y*1920) + x] = view.getPix(x,y);
		}
	}
	
	if (ALP_OK != AlpSeqAlloc(deviceID, 1, 1, &sequenceID)){
		return 1;
	}

	if (ALP_OK != AlpSeqPut(deviceID, sequenceID, 0, 1, transmitImages)){
		return 1;
	}
	free(transmitImages);
	
	if (ALP_OK != AlpSeqTiming(deviceID, sequenceID, ALP_DEFAULT, ALP_DEFAULT, ALP_DEFAULT, ALP_DEFAULT, ALP_DEFAULT)){
		return 1;
	}

	if (ALP_OK != AlpProjStartCont(deviceID, sequenceID)){
		return 1;
	}
} 
