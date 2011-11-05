using namespace std;

class AlpDMD {
	private:
		ALP_ID deviceID;
		long dmdType;
		long state;
		int init();
		
	public:
		AlpDMD();
		void quit();
		bool outputView(View);
};

