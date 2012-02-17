/** AlpDMD Class Methods
*
* Header defined in AlpDMD.h
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*/

using namespace std;


/* AlpDMD
* Constructor - Call the device initialisation
*/
AlpDMD::AlpDMD (){ 
	inUse = false; // Internal Bool so we don't try to initialise twice. 
	init();        // Run the init function. 
}


/* init
* Device initialisation. Grab a device, check its OK, see what DMD is attached
* and generally prepare it for use. 
*/
int AlpDMD::init(){ 
	if (verbose) cout << "AlpDMD::init: Initialising.\n"; 
	
	if (inUse){  // If we're already using the Device don't initialise twice. 
		cout << "AlpDMD::init: Error 1 - Device is currently connected to this program.\n";
		return 1;
	}
 	
	// Allocate the ALP high-speed device
	// We do this by testing if the device returns ok or not.
	// The state variable should always contain the value returned by the device.
	
	state = AlpDevAlloc(ALP_DEFAULT, ALP_DEFAULT, &deviceID);
	
	if (ALP_OK != state){  // On error
		cout << "AlpDMD::init: Error 2 - Device Returned non-OK value: " << state << endl;
		return 1;
	}
	
	// Inquire DMD type attached to the control board. 
	
	state = AlpDevInquire(deviceID, ALP_DEV_DMDTYPE, &dmdType );
	
	if (ALP_OK != state){ // On error
		cout << "AlpDMD::init: Error 3 - Device Returned non-OK value: " << state << endl; 
		return 1;
	}
	
	// If further support for different size DMDs is required, it needs to be placed
	// into this switch for the DMD to initialise. 
	
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

/* Stop
* Halt the current projection. 
*/
void AlpDMD::stop(){
	
	if (verbose) cout << "AlpDMD::stop: Halting Projection..." << endl;
	AlpDevHalt(deviceID);
}


/* quit
* Halt the current projection, and free the device. 
*/
void AlpDMD::quit(){
	
	if (verbose) cout << "AlpDMD::quit: Halting and Freeing Device..." << endl;
	AlpDevHalt(deviceID); // Stop projection.
	AlpDevFree(deviceID); // Free mirrors & disconnect device. 
	inUse = false;	      // Allow reinitialisation.
	if (verbose) cout << "AlpDMD::quit: Completed" << endl;
}

/* outputView(View <name>, int <mode>)
*
* This function sends a single image to the device, and begins a continuous projection 
* with the settings defined by mode, and the timings set in illuminate time and picture time. 
*/
bool AlpDMD::outputView(View view, int mode){
	
	if (verbose) cout << "AlpDMD::outputView: Placing a single frame on the device" << endl;
	
	ALP_ID sequenceID;  // Variable to hold this current Sequences' ID. 
	UCHAR *transmitImages = NULL; // Variables to hold the data to transmit. 
	transmitImages = new UCHAR[(1920*1080)];
	
	if (verbose) cout << "AlpDMD::outputView: Getting Pixels" << endl;
	
	for (int x=0; x<1920; x++){ // For each pixel in the image, copy it to the transmit format. 
		for (int y=0; y<1080; y++){
			if (view.getPix(x,y) == 1){
				FillMemory( transmitImages+x+(y*1920), 1, 0x00); // White
			} else {
				FillMemory( transmitImages+x+(y*1920), 1, 0x80); // Black
			}
		}
	}
	
	if (verbose) cout << "AlpDMD::outputView: Allocating Sequence" << endl;
	
	// Ask for a sequence of 1 image, with 1 bitplane. Record the state returned. 
	state = AlpSeqAlloc(deviceID, 1, 1, &sequenceID);  
	
	if (ALP_OK != state){ // On error
		cout << "AlpDMD::outputView: Error 1 - Unable to Allocate Sequence. " << state << endl;
		return 1;
	}
	
	if (verbose) cout << "AlpDMD::outputView: Sending to Device" << endl;
	
	// Transmit the sequence to the device, starting at frame 0 and consisting of 1 frame. 
	state = AlpSeqPut(deviceID, sequenceID, 0, 1, transmitImages);
	
	if (ALP_OK != state){ // On error
		cout << "AlpDMD::outputView: Error 2 - Unable to Transmit Sequence. " << state << endl;
		return 1;
	}
	
	if (verbose) cout << "AlpDMD::outputView: Freeing Memory" << endl;
	free(transmitImages); // Delete our copy of the memory. 
	
	if (verbose) cout << "AlpDMD::outputView: Changing Device to Uninterrupted Mode" << endl;
	
	// Operate the device without blank time. ALP_DARK_TIME = 0. 
	state = AlpSeqControl(deviceID, sequenceID, ALP_BIN_MODE, ALP_BIN_UNINTERRUPTED);
	
	if (ALP_OK != state){ // On error
		cout << "AlpDMD::outputView: Error 3 - Unable to Request Uninterrupted mode. " << state << endl;
		return 1;
	}
	
	if (verbose) cout << "AlpDMD::outputView: Setting up timings" << endl;
	
	// Send timing settings to the device. In BIN_UNIN.. illuminate time is ignored.  
	state = AlpSeqTiming(deviceID, sequenceID, illuminateTime, pictureTime, ALP_DEFAULT, ALP_DEFAULT, ALP_DEFAULT);
	
	if (ALP_OK != state){ // On error
		cout << "AlpDMD::outputView: Error 4 - Unable to Set to the desired timing settings." 
		<< "See \"help timings\" for details. " << state << endl;
		return 1;
	}
	
	// Work out which mode we've been asked to project under:
	switch(mode){
		case 2: // Hardware Trigger Mode
		
			if (verbose) cout << "AlpDMD::outputView: Requesting Trigger Mode." << endl;
			
			// Set the device to trigger on the leading edge. 
			state = AlpDevControl(deviceID, ALP_VD_EDGE, ALP_DEFAULT);
			
			if (ALP_OK != state){
				cout << "AlpDMD::outputView: Error 5 - Device Rejected Trigger mode change. " << state << endl;
				return 1;
			}
			
			// Set the projection to accept a trigger input, rather than outputting a trigger.
			state = AlpProjControl(deviceID, ALP_PROJ_MODE, ALP_SLAVE_VD);
			
			if (ALP_OK != state){
				cout << "AlpDMD::outputView: Error 6 - Projection Rejected Trigger mode change. " << state << endl;
				return 1;
			}
			
			if (verbose) cout << "AlpDMD::outputView: Starting Projection" << endl;
			
			// Start the projection (Makes device ready to accept a trigger pulse)
			state = AlpProjStartCont(deviceID, sequenceID);
			
			if (ALP_OK != state){
				cout << "AlpDMD::outputView: Error 7 - Unable to begin projection. " << state << endl;
				return 1;
			}
			
			cout << "Hardware Trigger Mode. Press any key when finished.\n";
			do {_getch();} while (_kbhit()); // Capture a users' key input.
			AlpDevHalt(deviceID);            // Finish the projection. 
			
			if (verbose) cout << "AlpDMD::outputView: Defaulting Trigger Mode." << endl;
			
			// Reset the Device trigger settings
			state = AlpDevControl(deviceID, ALP_TRIGGER_POLARITY, ALP_DEFAULT);
			
			if (ALP_OK != state){
				cout << "AlpDMD::outputView: Error 8 - Device Rejected Trigger mode reversion. " << state << endl;
				return 1;
			}
			
			// Reset the Projection trigger settings
			state = AlpProjControl(deviceID, ALP_PROJ_MODE, ALP_DEFAULT);
			
			if (ALP_OK != state){
				cout << "AlpDMD::outputView: Error 9 - Projection Rejected Trigger mode reversion. " << state << endl;
				return 1;
			}
			
			// End the function.
			return 0;
			break;
			
		case 1: // Software Trigger Mode
		
			cout << "Press any key to trigger projection...\n";
			do {_getch();} while (_kbhit()); // Capture a users' key input.
			break;
			
		case 0: // Project as soon as ready... 
		default:
			break;
	}
	
	if (verbose) cout << "AlpDMD::outputView: Starting Projection" << endl;
	
	// Start Projection
	state = AlpProjStartCont(deviceID, sequenceID);
	
	if (ALP_OK != state){
		cout << "AlpDMD::outputView: Error 10 - Unable to begin projection." << endl;
		return 1;
	}
	
	return 0;
} 


/* outputSeq(Seq <name>, int <mode>)
*
* This function sends a sequence of images to the device, and begins a continuous projection 
* with the settings defined by mode, and the timings set in illuminate time and picture time. 
* As this function is essentially identical to outputView, See that function for detailed 
* Comments. Comments will be restricted to significant differences between the two functions. 
*/

bool AlpDMD::outputSeq(Sequence seq, int mode){
	if (verbose) cout << "AlpDMD::outputSeq: Placing a single frame on the device" << endl;
	ALP_ID sequenceID;
	
	// This variable needs to be bigger as we have more than one image. 
	UCHAR *transmitImages = NULL; 
	transmitImages = new UCHAR[(1920*1080)*seq.getSize()];
		
	if (verbose) cout << "AlpDMD::outputSeq: Getting Pixels" << endl;
	
	for (int n=0; n<seq.getSize(); n++){ // For each image
		for (int x=0; x<1920; x++){
			for (int y=0; y<1080; y++){ // Use n to place the data in the correct place.
				if (seq.frames[n].getPix(x,y) == 1){  
					FillMemory( transmitImages+x+(y*1920)+(n*(1920*1080)), 1, 0x00);
				} else {
					FillMemory( transmitImages+x+(y*1920)+(n*(1920*1080)), 1, 0x80);
				}
			}
		}
	}
	
	if (verbose) cout << "AlpDMD::outputSeq: Allocating Sequence" << endl;
	
	// Sequence consists of more than one image, remember to allocate the correct size. 
	state = AlpSeqAlloc(deviceID, 1, seq.getSize(), &sequenceID);
	
	if (ALP_OK != state){
		cout << "AlpDMD::outputSeq: Error 1 - Unable to Allocate Sequence. " << state << endl;
		return 1;
	}
	
	if (verbose) cout << "AlpDMD::outputSeq: Sending to Device" << endl;
	
	state = AlpSeqPut(deviceID, sequenceID, 0, seq.getSize(), transmitImages); // As above
	
	if (ALP_OK != state){
		cout << "AlpDMD::outputSeq: Error 2 - Unable to Transmit Sequence. " << state << endl;
		return 1;
	}
	
	if (verbose) cout << "AlpDMD::outputSeq: Freeing Memory" << endl;
	free(transmitImages);
	
	if (verbose) cout << "AlpDMD::outputSeq: Changing Device to Uninterrupted Mode" << endl;
	
	state = AlpSeqControl(deviceID, sequenceID, ALP_BIN_MODE, ALP_BIN_UNINTERRUPTED); 
	
	if (ALP_OK != state){
		cout << "AlpDMD::outputSeq: Error 3 - Unable to Request Uninterrupted mode. " << state << endl;
		return 1;
	}
	
	if (verbose) cout << "AlpDMD::outputSeq: Setting up timings" << endl;
	
	state = AlpSeqTiming(deviceID, sequenceID, ALP_DEFAULT, pictureTime, ALP_DEFAULT, ALP_DEFAULT, ALP_DEFAULT);
	
	if (ALP_OK != state){
		cout << "AlpDMD::outputSeq: Error 4 - Unable to Set to the desired timing settings." 
		<< "See \"help timings\" for details. " << state << endl;
		return 1;
	}
	
	switch(mode){
		case 2: // Hardware Trigger Mode
		
			if (verbose) cout << "AlpDMD::outputSeq: Requesting Trigger Mode." << endl;

			state = AlpDevControl(deviceID, ALP_VD_EDGE, ALP_DEFAULT);
			
			if (ALP_OK != state){
				cout << "AlpDMD::outputSeq: Error 5 - Device Rejected Trigger mode change. " << state << endl;
				return 1;
			}
			
			state = AlpProjControl(deviceID, ALP_PROJ_MODE, ALP_SLAVE_VD);
			
			if (ALP_OK != state){
				cout << "AlpDMD::outputSeq: Error 6 - Projection Rejected Trigger mode change. " << state << endl;
				return 1;
			}
			
			if (verbose) cout << "AlpDMD::outputSeq: Starting Projection" << endl;

			state = AlpProjStartCont(deviceID, sequenceID);
			
			if (ALP_OK != state){
				cout << "AlpDMD::outputSeq: Error 7 - Unable to begin projection. " << state << endl;
				return 1;
			}
			
			cout << "Hardware Trigger Mode. Press any key when finished.\n";
			do {_getch();} while (_kbhit());
			AlpDevHalt(deviceID);
			
			if (verbose) cout << "AlpDMD::outputSeq: Defaulting Trigger Mode." << endl;
			
			state = AlpDevControl(deviceID, ALP_TRIGGER_POLARITY, ALP_DEFAULT);
			
			if (ALP_OK != state){
				cout << "AlpDMD::outputSeq: Error 8 - Device Rejected Trigger mode reversion. " << state << endl;
				return 1;
			}
			
			state = AlpProjControl(deviceID, ALP_PROJ_MODE, ALP_DEFAULT);
			
			if (ALP_OK != state){
				cout << "AlpDMD::outputSeq: Error 9 - Projection Rejected Trigger mode reversion. " << state << endl;
				return 1;
			}
			
			return 0;
			break;
			
		case 1: // Software Trigger
			cout << "Press any key to trigger projection...\n";
			do {_getch();} while (_kbhit());
			break;
			
		case 0: // Project as soon as ready... 
		default:
			break;
	}
	
	if (verbose) cout << "AlpDMD::outputSeq: Starting Projection" << endl;

	state = AlpProjStartCont(deviceID, sequenceID);
	
	if (ALP_OK != state){
		cout << "AlpDMD::outputSeq: Error 10 - Unable to begin projection." << endl;
		return 1;
	}
	
	return 0;
} 


