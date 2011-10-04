/** First Draft View Class
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*
*/

#include "PxGrp.h"
#include "intarray2bmp.h"
#include <cmath>

using namespace std;

class View {
	private:
		int pxSize, Xres, Yres;
		PxGrp** groups;
	public:
		void applyXfn();
		View(int);
		char output(); 
		void randomise();
};

View::View(int setPxSize){
	pxSize = setPxSize;
	Xres = 1920/pxSize;
	Yres = 1080/pxSize;
	groups = new PxGrp*[Xres];
	for (int i=0;i<Xres;i++){
		groups[i] = new PxGrp[Yres];
		for (int j=0;j<Yres;j++){
			groups[i][j].setPxSize(5);
		}
	}
}

void View::randomise(){
	srand(time(NULL));	
	for (int x=0; x<Xres; x++){
		for (int y=0; y<Yres; y++){		
			int IO = rand() % 5;
			if (IO == 4){
				groups[x][y].setValue(rand() % 25 + 1);
				//cout << groups[x][y].getValue() << " ";
			}
		}
	}
}

void View::applyXfn(){
	double addj;
	addj = pow(double(Xres/2.0),2.0);
	for (int x=0; x<Xres; x++){
		double xV = addj*pow(double(x-(Xres/2.0)),2.0);
		for (int y=0; y<Yres; y++){
			groups[x][y].setValue(xV); 
		}
	}	
}

char View::output(){
	
	int** data;
	data = new int*[Xres*pxSize];
	for (int i=0; i<Xres*pxSize; i++){
		data[i] = new int[Yres*pxSize];
	}

	for (int x=0; x<Xres; x++){
		for (int y=0; y<Yres; y++){
			data[x][y] = groups[x][y].getValue();
		}
	}

	for (int y=0; y<Yres; y++){
		for (int line=1; line<5; line++){
			for (int x=0; x<Xres; x++){
				//groups[x][y]->output(line);
				//cout << x << " " << y << " " << line << endl;
			}
		}
	}

	intarray2bmp::intarray2bmp("output.bmp", &(data[0][0]), Yres, Xres, 0, 1); 

	cout << "done" << endl;
}
