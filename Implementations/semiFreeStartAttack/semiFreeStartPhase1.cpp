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

unsigned int findGammaAndEpsilon(unsigned int &gamma, unsigned int &epsilon, map<unsigned int, unsigned int> &deltaAndW) {
	gamma=0;
	epsilon=0;
	while(deltaAndW.find(EP1(epsilon-1)-EP1(epsilon)+CH(epsilon-1, 0, gamma+1)-CH(epsilon, -1, gamma+1))==deltaAndW.end()) {
		gamma=rand();
		epsilon=rand();
		while(epsilon%2!=1) {
			epsilon=rand();
		}
	}
	return deltaAndW.find(EP1(epsilon-1)-EP1(epsilon)+CH(epsilon-1, 0, gamma+1)-CH(epsilon, -1, gamma+1))->second;
}

unsigned int findAlphaBeta(unsigned int &alpha, unsigned int &beta, unsigned int &epsilon, map<unsigned int, unsigned int> &deltaAndW) {
	alpha=0;
	beta=alpha-EP0(alpha);
	while(deltaAndW.find(CH(beta, epsilon, -1) - CH(beta, epsilon-1, 0))==deltaAndW.end()) {
		alpha = rand();
		beta = alpha-EP0(alpha);
		while(beta%2!=0) {
			alpha = rand();
			beta = alpha-EP0(alpha);
		}
	}
	return deltaAndW.find(CH(beta, epsilon, -1)-CH(beta, epsilon-1, 0))->second+1;
}

void fillDeltaAndW(map<unsigned int, unsigned int> &deltaAndW) {
	ifstream myFile;
	string line;
	myFile.open("deltaAndWEquation.txt");
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
	fillDeltaAndW(deltaAndW);

	unsigned int W16 = findGammaAndEpsilon(gamma, epsilon, deltaAndW);
	unsigned int W17 = findAlphaBeta(alpha, beta, epsilon, deltaAndW);
	
	//check four equations
	cout << "Equation 1 evaluates to : " << SSIG1(W16+1)-SSIG1(W16)-EP1(epsilon-1)+EP1(epsilon)-CH(epsilon-1, 0, gamma+1)+CH(epsilon, -1, gamma+1) << endl;
	cout << "Equation 2 evaluates to : " << SSIG1(W17-1)-SSIG1(W17)-CH(beta, epsilon-1, 0)+CH(beta, epsilon, -1) << endl;
	cout << "Equation 3 evaluates to : " << beta-alpha+EP0(alpha) << endl;
	cout << "Equation 4 evaluates to : " << CH(beta, beta, epsilon-1)-CH(beta, beta, epsilon)+1 << endl;

	cout << "alpha = " << alpha << endl;
	cout << "beta = " << beta << endl;
	cout << "epsilon = " << epsilon << endl;
	cout << "gamma = " << gamma << endl;
	cout << "W16 = " << W16 << endl;
	cout << "W17 = " << W17 << endl;

	return 0;
}