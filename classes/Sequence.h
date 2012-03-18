/** Sequence Class
* Controls the Sequence Object in the program.
*
* Methods defined below.
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*/

#include <vector>

using namespace std;

// Class Definition 

class Sequence {
	private: // Functions and vars usable only by the class
		double timing;  // Intended refresh rate of this sequence.
	public:  // Functions and vars usable by any external object
		vector<View> frames; // Var to hold all the views. 
		Sequence();	     // Constructor
		int getSize();       // Returns the number of frames in the sequence
		void setSize(int);   // Allocate space for this many frames (unused)
		int getTiming();     // Return the indended timings
		void updateTiming(); // Update timings from settings
		void assignFrame(int, View); // Overwrite a frame
		void addFrame(View); // Add a new frame on the end
		bool removeFrame(int); // Remove a specified frame
		void load();         // Load a folder as a sequence
		void loadSpecific(string); // Load a specific folder.
		void play();         // Display the sequence on the SDL windows
		void clear();        // Delete the sequence
};

Sequence::Sequence (){ // Constructor
	updateTiming(); // Update the timings from program settings
}

int Sequence::getSize(){ // Returns the number of frames in the sequence
	return frames.size();
}

void Sequence::setSize(int sSize) { // Allocate space for this many frames (unused)
	if (sSize > frames.size()){
		frames.reserve(sSize);
	} else {
		cout << "Sequence::setSize: Error - Unable to reduce size, or size is already that value." << endl;
	}
}

int Sequence::getTiming(){  // Return the indended timings
	return int(timing);
}

void Sequence::updateTiming() {  // Update timings from settings
	timing = int(pictureTime/1000);
}

void Sequence::assignFrame(int frame, View view){ // Overwrite a frame
	frames[frame] = view;
}

void Sequence::addFrame(View view){ // Add a frame
	int viewSize = int(pxGrpSize); // Create a tempory view. 
	View temp(viewSize);
	for (int x = 0; x<1920; x++){ // Copy the new frame into the temp view
		for (int y = 0; y<1080; y++){
			temp.setPix(x,y, view.getPix(x,y));
		}
	}
	
	temp.checkValue(); // Check itself
	
	frames.push_back(temp); // Add to the frames data
}

bool Sequence::removeFrame(int frame){ // Remove a specified frame
	frames.erase(frames.begin()+frame);	
}

void Sequence::clear(){ // Delete the sequence
	frames.clear();	
}

void Sequence::play(){ // Display the sequence on the SDL windows
	for(int i=0; i<frames.size(); i++){
		frames[i].output(); // Output each view...
		Sleep(DWORD(timing)); // Wait until the next image should show
	}
}

void Sequence::load(){ // Load a sequence from a folder of images. 
	
	// Create the Browse folders window
	TCHAR path[MAX_PATH];
	string pathStr;
    	BROWSEINFO browseInfo = {0};
   	browseInfo.lpszTitle = ("Please locate the saved sequence folder...");
	LPITEMIDLIST dataList = SHBrowseForFolder (&browseInfo);
	    	
	// If there is something in the folder and all is ok...    	
	if (dataList != 0){
		SHGetPathFromIDList (dataList, path);
		pathStr = path;
		if (verbose) cout << "Sequence::load: Path Selected: " << pathStr << endl;
	} else { // Otherwise...
		if (verbose) cout << "Sequence::load: Canceled Dialoge/Invalid Directory\n";
		return;
	}
	
	loadSpecific(pathStr); // Load this specified folder. 
}



void Sequence::loadSpecific(string pathStr){ // Load a sequence from a specified folder of images. 
	
// Using the reference: http://tomtech999.wordpress.com/2008/07/30/listing-files-in-a-directory-in-c/
	
	UINT count = 0;
	bool working = true;
	string buff, path, filePath;
	string fileName[1000];
	int viewSize = int(pxGrpSize);
	View loadView(viewSize); // Create the tempory view to work with.
	
	WIN32_FIND_DATA image; // Find all the .bmp files in the folder
	path = pathStr;
	pathStr.append("/*.bmp");
	if (verbose) cout << "Sequence::loadSpecific: Searching for: " << pathStr << endl;
	
	// Find the first file...
	HANDLE handle = FindFirstFile(pathStr.c_str(),&image);
	
	if(handle != INVALID_HANDLE_VALUE){ // If we find a file.
		
	       buff = image.cFileName;
	       fileName[count] = buff; // Add the filename to the list of found files. 
	       if (verbose) cout << "Sequence::loadSpecific: Found " << buff << endl;
	       filePath = ""; // Prepare the full path to the file.
	       filePath += path;
	       filePath += "\\";
	       filePath += buff;
	       	       
	       switch (imgMode){ // Load These images using scaled or 1:1?
			case 2: // Scaled
				loadView.loadScaledBmpSpecific(filePath);
				break;
			case 1: // 1:1
			default:
				loadView.loadBmpSpecific(filePath);
				break;
		}		
				
	       addFrame(loadView); // Add this frame to the Sequence
	
	       while(working){ // Repeat for all the rest. 
			
			FindNextFile(handle, &image); // Find the next file
			
			if(image.cFileName != buff){
				
				buff = image.cFileName;
				++count;
				fileName[count] = buff; // Add the filename to the list of found files. 
				if (verbose) cout << "Sequence::loadSpecific: Found " << buff << endl;
				
				filePath = ""; // Prepare the full path to the file.
				filePath += path;
				filePath += "\\";
				filePath += buff;
				
				switch (imgMode){ // Load These images using scaled or 1:1?
					case 2: // Scaled
						loadView.loadScaledBmpSpecific(filePath);
						break;
					case 1: // 1:1
					default:
						loadView.loadBmpSpecific(filePath);
						break;
				}		
				
				addFrame(loadView); // Add this frame to the Sequence
	              
		      } else {
				//end of files reached
				working = false;
	              }
	
	       }
	}
}
	
// EOF
