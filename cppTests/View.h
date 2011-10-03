/** First Draft View Class
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*
*/

#include 'PxGrp.h'

class View {
	private:
		PxGrp groups[384][216];
	public:
		UCHAR output(); 
}

uchar View::output(){
	for (int y=0; y<216; y++){
		for (int line=1; line<5; line++){
			for (int x=0; x<384; x++){
				//groups[x][y]->output(line);
				cout << x << " " << y << " " << line << endl;
			}
		}
	}
}
