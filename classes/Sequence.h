#include <vector>

using namespace std;

class Sequence {
	private:
		vector<View> frames;
		double timing;
	public:
		Sequence();
		int getSize();
		void setSize(int);
		int getTiming();
		void setTiming(int);
		void assignFrame(int, View);
		void addFrame(View);
		void insertFrame(int, View);
		bool removeFrame(int);
		void play();
};

Sequence::Sequence (){ // Constructor
	timing = 0;
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

void Sequence::setTiming(int sTime) {
	timing = sTime;
}

void Sequence::assignFrame(int frame, View view){
	frames[frame] = view;
}

void Sequence::addFrame(View view){
	View temp(intPxGrpSize);
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
//	frames.erase(frame);	
}

void Sequence::play(){
	for(int i=0; i<frames.size(); i++){
		frames[i].output();
		Sleep(timing);
	}
}


	

