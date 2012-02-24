/** AlpDMD Class
* Controls the ALP functions in the program.
*
* Methods defined in AlpDMD.cpp
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*/

using namespace std;

// Class Definition

class AlpDMD {
	private:  // Functions and vars usable only by the class
		ALP_ID deviceID;  // The device we are using - allows multiple DMDs.
		ALP_ID currentSeq;// Currently Projecting Sequence ID. 
		long dmdType;     // The type of DMD attached to this ALP. 
		long state;       // The last returned state of the Device. 
 		bool inUse;       // Internal var to define if we're currently connected. 
				
	public:  // Functions and vars usable by any external object
		AlpDMD(); 	               // Contrustor
		int init();                    // Connect the ALP
		void quit();                   // Disconnect the ALP 
		void stop();	               // Halt Current Projection
		void fullInquire();            // Display Device Settings
		long inquire(int);             // Inquire Device settings
		bool outputView(View, int);    // Output a single Image
		bool outputSeq(Sequence, int); // Output a sequence of images. 
		bool outputDynSeq(int);
		bool outputDynSeqSpecific(string, int);
};

