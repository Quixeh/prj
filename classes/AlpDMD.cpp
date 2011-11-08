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
	cout << "AlpDMD::outputView: Placing a single frame on the device" << endl;
	ALP_ID sequenceID;
	UCHAR *transmitImages = NULL;
	transmitImages = new UCHAR[(1920*1080)*2];
	
	cout << "AlpDMD::outputView: Getting Pixels" << endl;
	for (int x=0; x<1920; x++){
		
		for (int y=0; y<1080; y++){
			//cout << "AlpDMD::outputView: x = " << x << " y = " << y << endl;
			if (view.getPix(x,y) == 1){
				FillMemory( transmitImages+x+(y*1920), 1, 0x80 );
				FillMemory( transmitImages+x+(y*1920)+(1920*1080), 1, 0x80 );			
			} else {
				FillMemory( transmitImages+x+(y*1920), 1, 0x00 );
				FillMemory( transmitImages+x+(y*1920)+(1920*1080), 1, 0x00 );
			}
		}
	}
	
	cout << "AlpDMD::outputView: Allocating Sequence" << endl;
	if (ALP_OK != AlpSeqAlloc(deviceID, 1, 2, &sequenceID)){
		return 1;
	}
	
	cout << "AlpDMD::outputView: Sending to Device" << endl;
	if (ALP_OK != AlpSeqPut(deviceID, sequenceID, 0, 2, transmitImages)){
		return 1;
	}
	cout << "AlpDMD::outputView: Freeing Memory" << endl;
	free(transmitImages);
	
	cout << "AlpDMD::outputView: Setting up timings" << endl;
	if (ALP_OK != AlpSeqTiming(deviceID, sequenceID, ALP_DEFAULT, 200000, ALP_DEFAULT, ALP_DEFAULT, ALP_DEFAULT)){
		return 1;
	}
	
	cout << "AlpDMD::outputView: Starting Projection" << endl;
	if (ALP_OK != AlpProjStartCont(deviceID, sequenceID)){
		return 1;
	}
} 
