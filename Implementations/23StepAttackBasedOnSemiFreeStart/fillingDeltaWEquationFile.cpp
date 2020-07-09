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
	map<unsigned int, unsigned int> deltaAndNumsW;

	while(true) {
		unsigned int delta = SSIG1(i+1)-SSIG1(i);
		if (deltaAndW.find(delta)==deltaAndW.end()) {
			deltaAndW.insert(pair<unsigned int, unsigned int> (delta, i));
			myFile << delta << " " << i << endl;
			deltaAndNumsW.insert(pair<unsigned int, unsigned int> (delta, 1));
		}
		else {
			deltaAndNumsW.find(delta)->second++;
		}
		cout << i << " " << deltaAndW.size() << endl;
		if (deltaAndW.size()==3000) {
			break;
		}
		i++;
	}
	myFile.close();

	myFile.open("deltaAndNumsW.txt");
	map<unsigned int, unsigned int>::iterator it;
	for (it = deltaAndNumsW.begin(); it != deltaAndNumsW.end(); ++it) {
		myFile << it->first << " " << it->second << endl;
	}
	return 0;
}