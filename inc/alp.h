//
//  *********************************************************************************
//  *                                                                               *
//  *   Project:      alp	  (ALP DLL)												*
//  *   Filename:     alp.h : Header File 											*
//  *                                                                               *
//  *********************************************************************************
//  *                                                                               *
//  *   © 2004-2008 ViALUX GmbH. All rights reserved.                               *
//  *                                                                               *
//  *********************************************************************************
//  *                                                                               *
//  *   Version:        5                                                           *
//  *                                                                               *
//  *********************************************************************************
//


#ifndef _ALP_H_INCLUDED
#define _ALP_H_INCLUDED


#ifndef ALP_API
#define ALP_ATTR
#define ALP_API extern "C" __declspec(dllimport)
#endif


/////////////////////////////////////////////////////////////////////////////

//		standard parameter

typedef unsigned long						ALP_ID;

#define ALP_DEFAULT							0L

#define ALP_CURRENT_SOFTWARE_VERSION		1.2


/////////////////////////////////////////////////////////////////////////////

//		return codes

#define ALP_OK				0x00000000L		//	successfull execution
#define ALP_NOT_ONLINE			1001L		//	The specified ALP has not been found or is not ready.
#define ALP_NOT_IDLE			1002L		//	The ALP is not in idle state.
#define ALP_NOT_AVAILABLE		1003L		//	The specified ALP identifier is not valid.
#define ALP_NOT_READY			1004L		//	The specified ALP is already allocated.
#define ALP_PARM_INVALID		1005L		//	One of the parameters is invalid.
#define ALP_ADDR_INVALID		1006L		//	Error accessing user data.
#define ALP_MEMORY_FULL			1007L		//	The requested memory is not available.
#define ALP_SEQ_IN_USE			1008L		//	The sequence specified is currently in use.
#define ALP_HALTED				1009L		//	The ALP has been stopped while image data transfer was active.
#define ALP_ERROR_INIT			1010L		//	Initialization error.
#define ALP_ERROR_COMM			1011L		//	Communication error.
#define ALP_DEVICE_REMOVED		1012L		//	The specified ALP has been removed.
#define ALP_NOT_CONFIGURED		1013L		//	The onboard FPGA is unconfigured.
#define ALP_LOADER_VERSION		1014L		//	The function is not supported by this version of the driver file VlxUsbLd.sys.
#define ALP_ERROR_DONGLE		1015L		//	Please connect the USB Dongle


// for ALP_DEV_STATE in AlpDevInquire

#define ALP_DEV_BUSY			1100L		//	the ALP is displaying a sequence or image data download is active
#define ALP_DEV_READY			1101L		//	the ALP is ready for further requests
#define ALP_DEV_IDLE			1102L		//	the ALP is in wait state


// for ALP_PROJ_STATE in AlpProjInquire

#define ALP_PROJ_ACTIVE			1200L		//	ALP projection active
#define ALP_PROJ_IDLE			1201L		//	no projection active


/////////////////////////////////////////////////////////////////////////////

//		parameter

// AlpDevInquire
//
#define ALP_DEVICE_NUMBER		2000L	//	Serial number of the ALP device
#define ALP_VERSION				2001L	//	Version number of the ALP device
#define ALP_DEV_STATE			2002L	//	current ALP status, see above
#define ALP_AVAIL_MEMORY		2003L	//	ALP on-board sequence memory available for further sequence
										//	allocation (AlpSeqAlloc); number of binary pictures

//	AlpDevControl - ControlTypes & ControlValues
#define ALP_TRIGGER_POLARITY	2004L	//  Select trigger output signal polarity
#define ALP_VD_EDGE				2005L	//  Select active VD input trigger edge (slave mode)
#define ALP_LEVEL_HIGH			2006L	//  Active high trigger output
#define ALP_LEVEL_LOW			2007L	//  Active low trigger output
#define ALP_EDGE_FALLING		2008L	//  High to low signal transition
#define ALP_EDGE_RISING 		2009L	//  Low to high signal transition

#define ALP_VD_TIME_OUT			2014L	//	VD trigger time-out (slave mode)
#define ALP_TIME_OUT_ENABLE 	   0L	//	Time-out enabled (default)
#define ALP_TIME_OUT_DISABLE 	   1L	//	Time-out disabled

#define ALP_USB_CONNECTION		2016L	//	Re-connect after a USB interruption

#define ALP_DEV_DMDTYPE			2021L	//	Select DMD type; only allowed for a new allocated ALP-3 high-speed device
#define ALP_DMDTYPE_XGA			   1L	//	1024*768 mirror pixels (0.7" Type A, D3000)
#define ALP_DMDTYPE_SXGA_PLUS	   2L	//	1400*1050 mirror pixels (0.95" Type A, D3000)
#define ALP_DMDTYPE_1080P_095A	   3L	//	1920*1080 mirror pixels (0.95" Type A, D4x00)
#define ALP_DMDTYPE_XGA_07A		   4L	//	1024*768 mirror pixels (0.7" Type A, D4x00)
#define ALP_DMDTYPE_XGA_055A	   5L	//	1024*768 mirror pixels (0.55" Type A, D4x00)
#define ALP_DMDTYPE_XGA_055X	   6L	//	1024*768 mirror pixels (0.55" Type X, D4x00)
#define ALP_DMDTYPE_DISCONNECT	 255L	//	behaves like 1080p (D4100)


// AlpSeqControl - ControlTypes
#define ALP_SEQ_REPEAT			2100L	//	Non-continuous display of a sequence (AlpProjStart) allows
										//	for configuring the number of sequence iterations.
#define ALP_SEQ_REPETE ALP_SEQ_REPEAT	//  According to the typo made in primary documentation (ALP API description)
#define ALP_FIRSTFRAME			2101L	//	First image of this sequence to be displayed.
#define ALP_LASTFRAME			2102L	//	Last image of this sequence to be displayed.

#define ALP_BITNUM				2103L	//	A sequence can be displayed with reduced bit depth for faster speed.
#define ALP_BIN_MODE			2104L	//	Binary mode: select from ALP_BIN_NORMAL and ALP_BIN_UNINTERRUPTED (AlpSeqControl)

#define ALP_BIN_NORMAL			2105L	//	Normal operation with progammable dark phase
#define ALP_BIN_UNINTERRUPTED	2106L	//	Operation without dark phase

#define ALP_DATA_FORMAT			2110L	//	Data format and alignment
#define ALP_DATA_MSB_ALIGN		   0L	//	Data is MSB aligned (default)
#define ALP_DATA_LSB_ALIGN		   1L	//	Data is LSB aligned
#define ALP_DATA_BINARY_TOPDOWN	   2L	//	Data is packed binary, top row first; bit7 of a byte = leftmost of 8 pixels
#define ALP_DATA_BINARY_BOTTOMUP   3L	//	Data is packed binary, bottom row first
		// XGA:   one pixel row occupies 128 byte of binary data.
		//        Byte0.Bit7 = top left pixel (TOPDOWN format)
		// SXGA+: one pixel row occupies 176 byte of binary data. First byte ignored.
		//        Byte1.Bit7 = top left pixel (TOPDOWN format)

#define ALP_FIRSTLINE			2111L	//	Start line position at the first image
#define ALP_LASTLINE			2112L	//	Stop line position at the last image
#define ALP_LINE_INC			2113L	//	Line shift value for the next frame

// AlpSeqInquire
#define ALP_BITPLANES			2200L	//	Bit depth of the pictures in the sequence
#define ALP_PICNUM				2201L	//	Number of pictures in the sequence
//      ALP_SEQ_REPEAT						Number of automatically repeated iterations of the sequence
#define ALP_PICTURE_TIME		2203L	//	Time between the start of consecutive pictures in the sequence in microseconds,
										//	the corresponding in frames per second is
										//	picture rate [fps] = 1 000 000 / ALP_PICTURE_TIME [µs]
#define ALP_ILLUMINATE_TIME		2204L	//	Duration of the display of one picture in microseconds
#define ALP_TRIGGER_DELAY		2205L	//	Delay of the start of picture display with respect
										//	to the trigger output (master mode) in microseconds
#define ALP_TRIGGER_PULSEWIDTH	2206L	//	Duration of the active trigger output pulse in microseconds
#define ALP_VD_DELAY			2207L	//	Delay of the start of picture display with respect to the
										//	active VD trigger input edge in microseconds
#define ALP_MAX_TRIGGER_DELAY	2209L	//	Maximal duration of trigger output to projection delay in microseconds
#define ALP_MAX_VD_DELAY		2210L	//	Maximal duration of trigger input to projection delay in microseconds

#define ALP_MIN_PICTURE_TIME	2211L	//	Minimum time between the start of consecutive pictures in microseconds
#define ALP_MIN_ILLUMINATE_TIME	2212L	//	Minimum duration of the display of one picture in microseconds
										//	depends on ALP_BITNUM and ALP_BIN_MODE
#define ALP_MAX_PICTURE_TIME	2213L	//	Maximum value of ALP_PICTURE_TIME

										//	ALP_PICTURE_TIME = ALP_ON_TIME + ALP_OFF_TIME
										//	ALP_ON_TIME may be smaller than ALP_ILLUMINATE_TIME
#define ALP_ON_TIME				2214L	//	Total active projection time
#define ALP_OFF_TIME			2215L	//	Total inactive projection time


// AlpProjControl - ControlTypes & ControlValues
#define	ALP_PROJ_MODE			2300L	//	Select from ALP_MASTER and ALP_SLAVE_VD mode
#define	ALP_MASTER				2301L	//	The ALP operation is controlled by internal
										//	timing, a trigger signal is sent out for any
										//	picture displayed
#define	ALP_SLAVE_VD			2302L	//	The ALP operation is controlled by external
										//	trigger, the next picture in a sequence is
										//	displayed after the detection of an external
										//	input trigger (VD or composite-video) signal.
#define	ALP_PROJ_SYNC			2303L	//	Select from ALP_SYNCHRONOUS and ALP_ASYNCHRONOUS mode
#define	ALP_SYNCHRONOUS			2304L	//	The calling program gets control back after completion
										//	of sequence display.
#define	ALP_ASYNCHRONOUS		2305L	//	The calling program gets control back immediatelly.

#define	ALP_PROJ_INVERSION		2306L	//  Reverse dark into bright
#define	ALP_PROJ_UPSIDE_DOWN	2307L	//  Turn the pictures upside down

// AlpProjInquire
//		ALP_PROJ_MODE
//		ALP_PROJ_SYNC
#define ALP_PROJ_STATE			2400L




//  *********************************************************************************
//
ALP_API long ALP_ATTR AlpDevAlloc( long DeviceNum, long InitFlag, ALP_ID* DeviceIdPtr);


/////////////////////////////////////////////////////////////////////////////
//
ALP_API long ALP_ATTR AlpDevControl( ALP_ID DeviceId, long ControlType, long ControlValue);


/////////////////////////////////////////////////////////////////////////////
//
ALP_API long ALP_ATTR AlpDevInquire( ALP_ID DeviceId, long InquireType, long *UserVarPtr);


/////////////////////////////////////////////////////////////////////////////
//
ALP_API long ALP_ATTR AlpDevHalt( ALP_ID DeviceId);


/////////////////////////////////////////////////////////////////////////////
//
ALP_API long ALP_ATTR AlpDevFree( ALP_ID DeviceId);


/////////////////////////////////////////////////////////////////////////////
//
ALP_API long ALP_ATTR AlpSeqAlloc( ALP_ID DeviceId, long BitPlanes, long PicNum,  ALP_ID *SequenceIdPtr);


/////////////////////////////////////////////////////////////////////////////
//
// 
ALP_API long ALP_ATTR AlpSeqControl( ALP_ID DeviceId, ALP_ID SequenceId,  long ControlType, long ControlValue);


/////////////////////////////////////////////////////////////////////////////
//
ALP_API long ALP_ATTR AlpSeqTiming( ALP_ID DeviceId, ALP_ID SequenceId,  long IlluminateTime, 
		long PictureTime, long TriggerDelay, long TriggerPulseWidth, long VdDelay);


/////////////////////////////////////////////////////////////////////////////
//
ALP_API long ALP_ATTR AlpSeqInquire( ALP_ID DeviceId, ALP_ID SequenceId,  long InquireType, 
		long *UserVarPtr);


/////////////////////////////////////////////////////////////////////////////
//
ALP_API long ALP_ATTR AlpSeqPut( ALP_ID DeviceId, ALP_ID SequenceId, long PicOffset, long PicLoad, 
	    void *UserArrayPtr);


/////////////////////////////////////////////////////////////////////////////
//
ALP_API long ALP_ATTR AlpSeqFree( ALP_ID DeviceId, ALP_ID SequenceId);


/////////////////////////////////////////////////////////////////////////////
//
ALP_API long ALP_ATTR AlpProjControl( ALP_ID DeviceId, long ControlType, long ControlValue);


/////////////////////////////////////////////////////////////////////////////
//
ALP_API long ALP_ATTR AlpProjInquire( ALP_ID DeviceId, long InquireType, long *UserVarPtr);


/////////////////////////////////////////////////////////////////////////////
//
ALP_API long ALP_ATTR AlpProjStart( ALP_ID DeviceId, ALP_ID SequenceId);


/////////////////////////////////////////////////////////////////////////////
//
ALP_API long ALP_ATTR AlpProjStartCont( ALP_ID DeviceId, ALP_ID SequenceId);


/////////////////////////////////////////////////////////////////////////////
//
ALP_API long ALP_ATTR AlpProjHalt( ALP_ID DeviceId);


/////////////////////////////////////////////////////////////////////////////
//
ALP_API long ALP_ATTR AlpProjWait( ALP_ID DeviceId);




/////////////////////////////////////////////////////////////////////////////
#endif  // _ALP_H_INCLUDED
