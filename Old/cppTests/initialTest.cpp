/** Initial Controller Test Code
*
* @author Andy Blackmore <axb803@bham.ac.uk>
* Base code taken from the ALP High-Speed API Sample.
*
*/

#include <TCHAR.h>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "../inc/alp.h"

using namespace std;

int main(){
	
    ALP_ID nDevId, nSeqId;
	long nDmdType, nSizeX, nSizeY;
	long nReturn;
	const long nPictureTime = 20000;	// 20 ms, i.e. 50 Hz frame rate
	UCHAR *pImageData = NULL;
	int debugMode = 0;	

	// Allocate the ALP high-speed device
	// We do this by testing if the device returns ok or not.

	long alpState = AlpDevAlloc(ALP_DEFAULT, ALP_DEFAULT, &nDevId);
 	
	if (ALP_OK != alpState){
		cout << "Error 1: Device Returned non-OK value: " << alpState << endl;
		return 1;
	}

	// Inquire DMD type
	alpState = AlpDevInquire(nDevId, ALP_DEV_DMDTYPE, &nDmdType );
	if (ALP_OK != alpState){
		cout << "Error 2: Device Returned non-OK value: " << alpState << endl; 
		int x;
		cin >> x;
		return 1;
	}

	switch (nDmdType) {
		case ALP_DMDTYPE_DISCONNECT: // no DMD attached
			debugMode = 1;
			cout << "DMD Disconnected - Entering debugMode" << endl;
		case ALP_DMDTYPE_1080P_095A : // 1080p attached
			nSizeX = 1920; nSizeY = 1080;
			cout << "Using 1080p DMD - Device ID: " << nDevId << endl;
			break; 
		default: // unsupported DMD type
			cout << "Unsupported DMD - Exiting" << endl;
			return 1;
	}

	// Allocate a sequence of two binary frames
	if (ALP_OK != AlpSeqAlloc( nDevId, 1, 2, &nSeqId )) return 1;

	// Create images
	pImageData = (UCHAR*) malloc( 2*nSizeX*nSizeY ); // UCHAR array of 2 images' worth of pixels.
	if (NULL == pImageData) return 1;
	FillMemory( pImageData, nSizeX*nSizeY, 0x80 );				// white
	FillMemory( pImageData+nSizeX*nSizeY, nSizeX*nSizeY, 0x00 );		// black

	// Transmit images into ALP memory
	nReturn = AlpSeqPut( nDevId, nSeqId, 0, 2, pImageData );
	free( pImageData );
	if (ALP_OK != nReturn) return 1;

	// Set up image timing
	// For highest frame rate, first switch to binary uninterrupted
	// mode (ALP_BIN_UNINTERRUPTED) by using AlpDevControl. See also
	// the release notes for more details.
	if (ALP_OK != AlpSeqTiming( nDevId, nSeqId, ALP_DEFAULT, nPictureTime,
		ALP_DEFAULT, ALP_DEFAULT, ALP_DEFAULT ) ) return 1;

	// Start sequence
	if (ALP_OK != AlpProjStartCont( nDevId, nSeqId )) return 1;

	// Wait for key stroke
	_tprintf( _T("Press any key to stop ALP projection") );
	do { _getch(); } while (_kbhit()) ;
	

	// Done
	AlpDevHalt( nDevId );
	AlpDevFree( nDevId );

	return 0;
}
