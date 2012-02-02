using namespace std;

class AlpDMD {
	private:
		ALP_ID deviceID;
		long dmdType;
		long state;
		bool inUse;
				
	public:
		AlpDMD();
		int init();
		void quit();
		bool outputView(View, int);
};

