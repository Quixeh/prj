/** First Draft View Class
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*
*/

#include "PxGrp.h"
#include "intarray2bmp.h"

using namespace std;

class View {
	private:
		PxGrp groups[384][216];
	public:
		char output(); 
		void randomise();
};


void View::randomise(){
	srand(time(NULL));	
	cout << "Randomising" << endl;
	for (int x=0; x<384; x++){
		for (int y=0; y<216; y++){		
			int IO = rand() % 5;
			if (IO == 4){
				groups[x][y].setValue(rand() % 25 + 1);
				cout << groups[x][y].getValue() << " ";
			}
		}
	}
}



char View::output(){

	int data[384][216];

	for (int x=0; x<384; x++){
		for (int y=0; y<216; y++){
			data[x][y] = groups[x][y].getValue();
		}
	}

	for (int y=0; y<216; y++){
		for (int line=1; line<5; line++){
			for (int x=0; x<384; x++){
				//groups[x][y]->output(line);
				//cout << x << " " << y << " " << line << endl;
			}
		}
	}

	intarray2bmp::intarray2bmp("output.bmp", &(data[0][0]), 216, 384, 0, 1); 

	cout << "done" << endl;
}
