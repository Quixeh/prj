/** Initial Grouping Test code
*
* @author Andy Blackmore <axb803@bham.ac.uk>
*
*/

//#include <TCHAR.h>
//#include <windows.h>
//#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <string>
//#include "../inc/alp.h"
#include "View.h"
#include <time.h>

using namespace std;

int main(){
	cout << "Creating View." << endl;
	View view(40);
	
	cout << "Applying Fn to View." << endl;
	view.applyXfn();

	cout << "Creating Output." << endl;
	view.output();
}
