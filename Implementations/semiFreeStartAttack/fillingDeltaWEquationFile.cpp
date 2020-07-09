#include <iostream>
#include "semiFreeStartUtil.h"
#include <map>
#include <vector>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[]) {
	map<unsigned int, unsigned int> deltaAndW;
	int i=0;
	ofstream myFile;
	myFile.open("deltaAndWEquation.txt");

	while(true) {
		unsigned int delta = SSIG1(i+1)-SSIG1(i);
		if (deltaAndW.find(delta)==deltaAndW.end()) {
			deltaAndW.insert(pair<unsigned int, unsigned int> (delta, i));
			myFile << delta << " " << i << endl;
		}
		cout << i << " " << deltaAndW.size() << endl;
		if (i==0xffffffff) {
			break;
		}
		i++;
	}
	myFile.close();
	return 0;
}