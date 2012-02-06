using namespace std;

AlpDMD::AlpDMD (){ // Constructor
	inUse = false;
	init();
}

int AlpDMD::init(){
	if (verbose) cout << "AlpDMD::init: Initialising.\n"; 
	
	if (inUse){
		cout << "AlpDMD::init: Error 1 - Device is currently connected to this program.\n";
		return 1;
	}
 	
	// Allocate the ALP high-speed device
	// We do this by testing if the device returns ok or not.
	
	state = AlpDevAlloc(ALP_DEFAULT, ALP_DEFAULT, &deviceID);
	
	if (ALP_OK != state){
		cout << "AlpDMD::init: Error 2 - Device Returned non-OK value: " << state << endl;
		return 1;
	}
	
	// Inquire DMD type
	
	state = AlpDevInquire(deviceID, ALP_DEV_DMDTYPE, &dmdType );
	
	if (ALP_OK != state){
		cout << "AlpDMD::init: Error 3 - Device Returned non-OK value: " << state << endl; 
		return 1;
	}
	
	switch (dmdType) {
		case ALP_DMDTYPE_DISCONNECT: // no DMD attached
			cout << "AlpDMD::init: Error 4 - DMD Disconnected" << endl;
			return 1;
		case ALP_DMDTYPE_1080P_095A : // 1080p attached
			cout << "AlpDMD Attached - Responding OK.\n";
			if (verbose) cout << "AlpDMD::init: Using 1080p DMD - Device ID: " << deviceID << endl;
			inUse = true;
			break; 
		default: // unsupported DMD type
			cout << "AlpDMD::init: Error 5 - Unsupported DMD - Exiting" << endl;
			return 1;
	}
	
	// Setup Default Timing variables.
	illuminateTime = ALP_DEFAULT;
	return 0;
}

void AlpDMD::quit(){
	
	if (verbose) cout << "AlpDMD::quit: Halting and Freeing Device..." << endl;
	AlpDevHalt(deviceID);
	AlpDevFree(deviceID);
	inUse = false;
	if (verbose) cout << "AlpDMD::quit: Completed" << endl;
}

bool AlpDMD::outputView(View view, int mode){
	if (verbose) cout << "AlpDMD::outputView: Placing a single frame on the device" << endl;
	ALP_ID sequenceID;
	UCHAR *transmitImages = NULL;
	transmitImages = new UCHAR[(1920*1080)];
	
	if (verbose) cout << "AlpDMD::outputView: Getting Pixels" << endl;
	for (int x=0; x<1920; x++){
		for (int y=0; y<1080; y++){
			if (view.getPix(x,y) == 1){
				FillMemory( transmitImages+x+(y*1920), 1, 0x00);
			} else {
				FillMemory( transmitImages+x+(y*1920), 1, 0x80);
			}
		}
	}
	
	if (verbose) cout << "AlpDMD::outputView: Allocating Sequence" << endl;
	if (ALP_OK != AlpSeqAlloc(deviceID, 1, 1, &sequenceID)){
		return 1;
	}
	
	if (verbose) cout << "AlpDMD::outputView: Sending to Device" << endl;
	if (ALP_OK != AlpSeqPut(deviceID, sequenceID, 0, 1, transmitImages)){
		return 1;
	}
	if (verbose) cout << "AlpDMD::outputView: Freeing Memory" << endl;
	free(transmitImages);
	
	if (verbose) cout << "AlpDMD::outputView: Setting up timings" << endl;
	if (ALP_OK != AlpSeqTiming(deviceID, sequenceID, illuminateTime, pictureTime, ALP_DEFAULT, ALP_DEFAULT, ALP_DEFAULT)){
		return 1;
	}
	
	switch(mode){
		case 2:
			cout << "Hardware Trigger Mode Unavailabe. Stopping.\n";
			break;
		case 1:
			cout << "Press any key to trigger projection...\n";
			do {_getch();} while (_kbhit());
			break;
		case 0:
		default:
			break;
	}
	
	if (verbose) cout << "AlpDMD::outputView: Starting Projection" << endl;
	if (ALP_OK != AlpProjStartCont(deviceID, sequenceID)){
		return 1;
	}
} 

bool AlpDMD::outputSeq(Sequence seq, int mode){
	if (verbose) cout << "AlpDMD::outputSeq: Placing a single frame on the device" << endl;
	ALP_ID sequenceID;
	UCHAR *transmitImages = NULL;
	transmitImages = new UCHAR[(1920*1080)*seq.getSize()];
		
	if (verbose) cout << "AlpDMD::outputSeq: Getting Pixels" << endl;
	
	for (int n=0; n<seq.getSize(); n++){
		for (int x=0; x<1920; x++){
			for (int y=0; y<1080; y++){
				if (seq.frames[n].getPix(x,y) == 1){
					FillMemory( transmitImages+x+(y*1920)+(n*(1920*1080)), 1, 0x00);
				} else {
					FillMemory( transmitImages+x+(y*1920)+(n*(1920*1080)), 1, 0x80);
				}
			}
		}
	}
	
	if (verbose) cout << "AlpDMD::outputSeq: Allocating Sequence" << endl;
	if (ALP_OK != AlpSeqAlloc(deviceID, 1, seq.getSize(), &sequenceID)){
		return 1;
	}
	
	if (verbose) cout << "AlpDMD::outputSeq: Sending to Device" << endl;
	if (ALP_OK != AlpSeqPut(deviceID, sequenceID, 0, seq.getSize(), transmitImages)){
		return 1;
	}
	if (verbose) cout << "AlpDMD::outputSeq: Freeing Memory" << endl;
	free(transmitImages);
	
	if (verbose) cout << "AlpDMD::outputSeq: Setting up timings" << endl;
	if (ALP_OK != AlpSeqTiming(deviceID, sequenceID, illuminateTime, pictureTime, ALP_DEFAULT, ALP_DEFAULT, ALP_DEFAULT)){
		return 1;
	}
	
	switch(mode){
		case 2:
			cout << "Hardware Trigger Mode Unavailabe. Stopping.\n";
			break;
		case 1:
			cout << "Press any key to trigger projection...\n";
			do {_getch();} while (_kbhit());
			break;
		case 0:
		default:
			break;
	}
	
	if (verbose) cout << "AlpDMD::outputSeq: Starting Projection" << endl;
	if (ALP_OK != AlpProjStartCont(deviceID, sequenceID)){
		return 1;
	}
} 


