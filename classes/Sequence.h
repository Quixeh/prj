#include <vector>

using namespace std;

class Sequence {
	private:
		double timing;
	public:
		vector<View> frames;
		Sequence();
		int getSize();
		void setSize(int);
		int getTiming();
		void updateTiming();
		void assignFrame(int, View);
		void addFrame(View);
		void insertFrame(int, View);
		bool removeFrame(int);
		void load();
		void loadSpecific(string);
		void play();
		void clear();
};

Sequence::Sequence (){ // Constructor
	updateTiming();
}

int Sequence::getSize(){
	return frames.size();
}

void Sequence::setSize(int sSize) {
	if (sSize > frames.size()){
		frames.reserve(sSize);
	} else {
		cout << "Sequence::setSize: Error - Unable to reduce size, or size is already that value." << endl;
	}
}

int Sequence::getTiming(){
	return int(timing);
}

void Sequence::updateTiming() {
	timing = int(pictureTime/1000);
}

void Sequence::assignFrame(int frame, View view){
	frames[frame] = view;
}

void Sequence::addFrame(View view){
	int viewSize = int(pxGrpSize);
	View temp(viewSize);
	for (int x = 0; x<1920; x++){
		for (int y = 0; y<1080; y++){
			temp.setPix(x,y, view.getPix(x,y));
		}
	}
	
	temp.checkValue();
	
	frames.push_back(temp);
}

void Sequence::insertFrame(int frame, View view){
//	frames.insert(frame, view);
}

bool Sequence::removeFrame(int frame){
	frames.erase(frames.begin()+frame);	
}

void Sequence::clear(){
	frames.clear();	
}

void Sequence::play(){
	for(int i=0; i<frames.size(); i++){
		frames[i].output();
		Sleep(DWORD(timing));
	}
}

void Sequence::load(){
	TCHAR path[MAX_PATH];
	string pathStr;
    	BROWSEINFO browseInfo = {0};
   	browseInfo.lpszTitle = ("Please located the saved sequence folder...");
	LPITEMIDLIST dataList = SHBrowseForFolder (&browseInfo);
	    	
	if (dataList != 0){
		SHGetPathFromIDList (dataList, path);
		pathStr = path;
		if (verbose) cout << "Sequence::load: Path Selected: " << pathStr << endl;
	} else {
		if (verbose) cout << "Sequence::load: Canceled Dialoge/Invalid Directory\n";
		return;
	}
	
	loadSpecific(pathStr);
}



void Sequence::loadSpecific(string pathStr){
	
// Using the reference: http://tomtech999.wordpress.com/2008/07/30/listing-files-in-a-directory-in-c/
	
	UINT count = 0;
	bool working = true;
	string buff, path, filePath;
	string fileName[1000];
	int viewSize = int(pxGrpSize);
	View loadView(viewSize);
	
	WIN32_FIND_DATA image;
	path = pathStr;
	pathStr.append("/*.bmp");
	if (verbose) cout << "Sequence::loadSpecific: Searching for: " << pathStr << endl;
	
	HANDLE handle = FindFirstFile(pathStr.c_str(),&image);
	
	if(handle != INVALID_HANDLE_VALUE){
		
	       buff = image.cFileName;
	       fileName[count] = buff;
	       if (verbose) cout << "Sequence::loadSpecific: Found " << buff << endl;
	       filePath = "";
	       filePath += path;
	       filePath += "\\";
	       filePath += buff;
	       	       
	       switch (imgMode){
			case 2:
				loadView.loadScaledBmpSpecific(filePath);
				break;
			case 1:
			default:
				loadView.loadBmpSpecific(filePath);
				break;
		}		
				
	       addFrame(loadView);
	
	       while(working){
			
			FindNextFile(handle, &image);
			
			if(image.cFileName != buff){
				
				buff = image.cFileName;
				++count;
				fileName[count] = buff;
				if (verbose) cout << "Sequence::loadSpecific: Found " << buff << endl;
				
				filePath = "";
				filePath += path;
				filePath += "\\";
				filePath += buff;
				
				switch (imgMode){
					case 2:
						loadView.loadScaledBmpSpecific(filePath);
						break;
					case 1:
					default:
						loadView.loadBmpSpecific(filePath);
						break;
				}		
				
				addFrame(loadView);
	              
		      } else {
				//end of files reached
				working = false;
	              }
	
	       }
	}
}
	

