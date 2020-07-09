#include <iostream>
#include <vector>
#include "tableUtil.h"

using namespace std;

vector<vector<int> > capSigma0;
vector<vector<int> > capSigma1;
vector<vector<int> > smallSigma0;
vector<vector<int> > smallSigma1;


// void dfsForEHelper(vector<vector<int> > wDifferential, vector<vector<int> > eDifferential, int equationForStep, char diffSide) {
// 	if (equationForStep==7 && diffSide=='W') {
// 		int i=0;
// 		vector<char> copy = wDifferential[7];
// 		while (true) {
// 			wDifferential[7] = copy;
// 			int r = nthAssignmentToxInWi(wDifferential[7], i);
// 			dfsHelper(wDifferential, eDifferential, 'E');
// 			i++;
// 		}
// 		return;
// 	}
// 	if (equationForStep==7 && diffSide=='E') {
// 		int i=0;
// 		vector<char> copy = eDifferential[7];
// 		vector<char> sumToBe = 
// 		while (true) {
// 			eDifferential[7] = copy;
// 			int r = nthAssignmentToxInEi(eDifferential[7], );
// 			dfsHelper(wDifferential, eDifferential, 'E');
// 			i++;
// 		}
// 		return;
// 	}
// }


// void dfsForE(vector<vector<int> > wDifferential, vector<vector<int> > eDifferential) {
// 	dfsForEHelper(wDifferential, eDifferential, 7, 'W');
// }
vector<char> calculateFromCross(vector<vector<int> > &sigma, vector<char> &numCross) {
	vector<int> numVal = convertNumCrossToNumVal(numCross);
	vector<int> resVal = matMul(numVal, sigma);
	vector<char> resCross = convertNumValToNumCross(resVal);
	return resCross;
}

vector<char> chooseW8() {
	vector<char> res(32, '-');
	res[3] = 'x';
	return res;
}

int main(int argc, char const *argv[]) {
	// intializing sigmas
	capSigma0 = getCapSig(2, 13, 22);
	capSigma1 = getCapSig(6, 11, 25);
	smallSigma0 = getSmallSig(7, 18, 3);
	smallSigma1 = getSmallSig(17, 19, 10);

	// initializing and loading e,w

	vector<int> crossPositionsInW7 = {};
	vector<int> crossPositionsInW8 = {};
	vector<int> crossPositionsInW12 = {};
	vector<int> crossPositionsInW15 = {};
	vector<int> crossPositionsInW17 = {};

	for (int i = 0; i < 1; ++i) {
		vector<char> W8 = chooseW8();
		vector<char> W7 = calculateFromCross(smallSigma0, W8);
		vector<char> W15 = calculateFromCross(smallSigma0, W7);
		vector<char> W17 = calculateFromCross(smallSigma1, W15);
		vector<char> W12 = calculateFromCross(smallSigma1, W17);
		int numX = 0;
		for (int i = 0; i < 32; ++i) {
			if (W8[i]=='x') {
				numX++;
			}
			if (W7[i]=='x') {
				numX++;
			}
			if (W12[i]=='x') {
				numX++;
			}
			if (W15[i]=='x') {
				numX++;
			}
			if (W17[i]=='x') {
				numX++;
			}
		}
		cout << numX << endl;

		for (int i = 0; i < 32; ++i) {
			cout << W7[i];
			if (W7[i]=='x') {
				crossPositionsInW7.push_back(i);
			}
		}
		cout << endl;
		for (int i = 0; i < 32; ++i) {
			cout << W8[i];
			if (W8[i]=='x') {
				crossPositionsInW8.push_back(i);
			}
		}
		cout << endl;
		for (int i = 0; i < 32; ++i) {
			cout << W12[i];
			if (W12[i]=='x') {
				crossPositionsInW12.push_back(i);
			}
		}
		cout << endl;
		for (int i = 0; i < 32; ++i) {
			cout << W15[i];
			if (W15[i]=='x') {
				crossPositionsInW15.push_back(i);
			}
		}
		cout << endl;
		for (int i = 0; i < 32; ++i) {
			cout << W17[i];
			if (W17[i]=='x') {
				crossPositionsInW17.push_back(i);
			}
		}
		cout << endl;
	}

	
	for (int i = 0; i < crossPositionsInW7.size(); ++i) {
		cout << crossPositionsInW7[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < crossPositionsInW8.size(); ++i) {
		cout << crossPositionsInW8[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < crossPositionsInW12.size(); ++i) {
		cout << crossPositionsInW12[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < crossPositionsInW15.size(); ++i) {
		cout << crossPositionsInW15[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < crossPositionsInW17.size(); ++i) {
		cout << crossPositionsInW17[i] << " ";
	}
	cout << endl;


	// preparing structures for differentials of W, E, A and Conditions Matrix
	vector<vector<char> > aDifferential(27, vector<char> (32, '-'));
	vector<vector<char> > eDifferential(27, vector<char> (32, '-'));
	vector<vector<char> > wDifferential(27, vector<char> (32, '-'));

	for (int i = 0; i < 32; ++i) {
		if (find(crossPositionsInW7.begin(), crossPositionsInW7.end(), i) != crossPositionsInW7.end()) {
			wDifferential[7][i] = 'x';
		}
		if (find(crossPositionsInW8.begin(), crossPositionsInW8.end(), i) != crossPositionsInW8.end()) {
			wDifferential[8][i] = 'x';
		}
		if (find(crossPositionsInW12.begin(), crossPositionsInW12.end(), i) != crossPositionsInW12.end()) {
			wDifferential[12][i] = 'x';
		}
		if (find(crossPositionsInW15.begin(), crossPositionsInW15.end(), i) != crossPositionsInW15.end()) {
			wDifferential[15][i] = 'x';
		}
		if (find(crossPositionsInW17.begin(), crossPositionsInW17.end(), i) != crossPositionsInW17.end()) {
			wDifferential[17][i] = 'x';
		}
	}

	// dfsForE(wDifferential, eDifferential);

	return 0;
}