#include <iostream>
#include <sstream>
#include <bitset>
#include <vector>
#include <iomanip>
#include <cstring>
#include "semiFreeStartUtil.h"
#include <fstream>
#include <map>

unsigned int uIntHere(string str, int s, int &e) {
	string uIntString = "";
	for (int i = s; i < str.length(); ++i) {
		if (str[i]==' ' || str[i]=='\n') {
			e=i-1;
			break;
		}
		uIntString = uIntString + str[i];
	}
	return stoul(uIntString);
}

unsigned int findWWithDelta(unsigned int deltaNeeded) {
	ifstream myFile;
	string line;
	myFile.open("deltaAndWEquation.txt");
	while(getline(myFile, line)) {
		int firstWordEnd=-1;
		unsigned int delta = uIntHere(line, 0, firstWordEnd);
		if (delta==deltaNeeded) {
			return uIntHere(line, firstWordEnd+2, firstWordEnd);
		}
	}
	myFile.close();
	cout << "CODE 101 : Could not find deltaNeeded : val was " << deltaNeeded << endl;
	return -1;
}

bool isBad(map<unsigned int, unsigned int> &deltaAndNumsW, unsigned int delta) {
	unsigned int threshold = 100000;
	if (deltaAndNumsW.find(delta)->second<threshold) {
		return true;
	}
	return false;
}

unsigned int findGoodGammaAndEpsilon(unsigned int &gamma, unsigned int &epsilon, map<unsigned int, unsigned int> &deltaAndW, map<unsigned int, unsigned int> &deltaAndNumsW) {
	cout << "Started Finding gamma and epsilon" << endl;
	gamma=0;
	epsilon=0;
	int i=0;
	while((deltaAndW.find(EP1(epsilon-1)-EP1(epsilon)+CH(epsilon-1, 0, gamma+1)-CH(epsilon, -1, gamma+1))==deltaAndW.end()) || isBad(deltaAndNumsW, EP1(epsilon-1)-EP1(epsilon)+CH(epsilon-1, 0, gamma+1)-CH(epsilon, -1, gamma+1))) {
		i++;
		gamma=rand();
		epsilon=rand();
		while(epsilon%2!=1) {
			epsilon=rand();
		}
	}
	cout << "Found gamma and epsilon in " << i << " trials" << endl;
	return deltaAndW.find(EP1(epsilon-1)-EP1(epsilon)+CH(epsilon-1, 0, gamma+1)-CH(epsilon, -1, gamma+1))->second;
}

unsigned int findGoodAlphaBeta(unsigned int &alpha, unsigned int &beta, unsigned int &epsilon, map<unsigned int, unsigned int> &deltaAndW, map<unsigned int, unsigned int> &deltaAndNumsW) {
	cout << "Started Finding alpha and beta" << endl;
	alpha=0;
	beta=alpha-EP0(alpha);
	int i=0;
	while((deltaAndW.find(CH(beta, epsilon, -1) - CH(beta, epsilon-1, 0))==deltaAndW.end()) && isBad(deltaAndNumsW, CH(beta, epsilon, -1) - CH(beta, epsilon-1, 0))) {
		i++;
		alpha = rand();
		beta = alpha-EP0(alpha);
		while(beta%2!=0) {
			alpha = rand();
			beta = alpha-EP0(alpha);
		}
	}
	cout << "Found alpha and beta in " << i << " trials" << endl;
	return deltaAndW.find(CH(beta, epsilon, -1) - CH(beta, epsilon-1, 0))->second+1;
}

void fillDeltaAndW(map<unsigned int, unsigned int> &deltaAndW, string fileName) {
	ifstream myFile;
	string line;
	myFile.open(fileName);
	while(getline(myFile, line)) {
		int firstWordEnd=-1;
		unsigned int delta = uIntHere(line, 0, firstWordEnd);
		unsigned int W = uIntHere(line, firstWordEnd+2, firstWordEnd);
		deltaAndW.insert(pair<unsigned int, unsigned int> (delta, W));
	}
	myFile.close();
	cout << "Loaded Map " << deltaAndW.size() << endl;
	return;
}

int main(int argc, char* argv[]) {
	unsigned int alpha;
	unsigned int beta;
	unsigned int epsilon;
	unsigned int gamma;

	map<unsigned int, unsigned int> deltaAndW;
	map<unsigned int, unsigned int> deltaAndNumsW;
	fillDeltaAndW(deltaAndW, "deltaAndWEquation.txt");
	fillDeltaAndW(deltaAndNumsW, "deltaAndNumsW.txt");

	unsigned int W16 = findGoodGammaAndEpsilon(gamma, epsilon, deltaAndW, deltaAndNumsW);
	unsigned int W17 = findGoodAlphaBeta(alpha, beta, epsilon, deltaAndW, deltaAndNumsW);
	
	//check four equations
	cout << "Equation 1 evaluates to : " << SSIG1(W16+1)-SSIG1(W16)-EP1(epsilon-1)+EP1(epsilon)-CH(epsilon-1, 0, gamma+1)+CH(epsilon, -1, gamma+1) << endl;
	cout << "Equation 2 evaluates to : " << SSIG1(W17-1)-SSIG1(W17)-CH(beta, epsilon-1, 0)+CH(beta, epsilon, -1) << endl;
	cout << "Equation 3 evaluates to : " << beta-alpha+EP0(alpha) << endl;
	cout << "Equation 4 evaluates to : " << CH(beta, beta, epsilon-1)-CH(beta, beta, epsilon)+1 << endl;

	cout << "alpha = " << alpha << endl;
	cout << "beta = " << beta << endl;
	cout << "epsilon = " << epsilon << endl;
	cout << "gamma = " << gamma << endl;

	return 0;
}