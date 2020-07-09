#include "SHA2Util.h"

using namespace std;

#define HEIGHT 16
#define ROTRIGHT(word,bits) (((word) >> (bits)) | ((word) << (32-(bits))))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SSIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SSIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))

bool isDifferent(char c) {
	if (c=='x' || c=='u' || c=='n') {
		return true;
	}
	return false;
}

int main(int argc, char const *argv[]) {
	Conditions conditions;

	for (int i = 0; i < HEIGHT; ++i) {
		conditions.addFullLineVariables("W", i);
		conditions.addFullLineVariables("E", i);
		conditions.addFullLineVariables("A", i);
	}

	vector<vector<char> > wDifferential(HEIGHT, vector<char> (32, '-'));
	vector<vector<char> > eDifferential(HEIGHT, vector<char> (32, '-'));
	vector<vector<char> > aDifferential(HEIGHT, vector<char> (32, '-'));


	vector<vector<char> > sigA(HEIGHT, vector<char> (32, '-'));
	vector<vector<char> > sigE(HEIGHT, vector<char> (32, '-'));


	vector<int> e7Vec = convertIntToVector(1982388831);
	for (int i = 0; i < 32; ++i) {
		if (e7Vec[i]==1) {
			eDifferential[7][i] = 'u';
		}
		else {
			eDifferential[7][i] = '-';
		}
	}


	vector<char> deltaSmallSig1E7 = calculateFromCross(SMALL_SIGMA_1, eDifferential[7]);
	vector<char> deltaCapSig0E7 = calculateFromCross(CAP_SIGMA_0, eDifferential[7]);
	vector<char> deltaCapSig1E7 = calculateFromCross(CAP_SIGMA_1, eDifferential[7]);

	// A 
	
	// 1
	aDifferential[7] = eDifferential[7];
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(eDifferential[7][i])) {
			conditions.setEqual("A", 7, i, "E", 7, i);
		}
	}


	// 2
	vector<int> majDiff = {1, 2, 4, 6, 25};
	vector<char> majVec(32, '-');
	for (int i = 0; i < majDiff.size(); ++i) {
		majVec[majDiff[i]]='d';
	}
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(aDifferential[7][i])) {
			if (majVec[i]!='d') {
				conditions.setEqual("A", 5, i, "A", 6, i);
				aDifferential[5][i] = '0';
				aDifferential[6][i] = '0';
			}
			else {
				conditions.setDifferent("A", 5, i, "A", 6, i);
				aDifferential[5][i] = '1';
				aDifferential[6][i] = '0';
			}
		}
	}
	eDifferential[8] = eDifferential[7];

	// 3
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(aDifferential[7][i])) {
			conditions.setEqual("A", 6, i, "A", 8, i);
			aDifferential[8][i] = '0';
		}
	}

	// 4
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(aDifferential[7][i])) {
			conditions.setEqual("A", 8, i, "A", 9, i);
			aDifferential[9][i] = '0';
		}
	}

	// 5
	eDifferential[11] = aDifferential[7];
	for (int i = 0; i < 32; ++i) {
		conditions.setEqual("A", 7, i, "E", 11, i);
	}

	
	// E
	
	// 1
	wDifferential[15] = eDifferential[11];
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(eDifferential[11][i])) {
			conditions.setDifferent("E", 11, i, "W", 15, i);
		}
	}

	

	// 2
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(eDifferential[11][i])) {
			eDifferential[13][i]='1';
		}
	}

	// 3
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(eDifferential[11][i])) {
			eDifferential[12][i]='0';
		}
	}


	// 4
	vector<int> ifDiffFor4 = {1, 2, 4, 9, 12, 28};
	vector<char> if4Vec(32, '-');
	for (int i = 0; i < ifDiffFor4.size(); ++i) {
		if4Vec[ifDiffFor4[i]]='d';
	}
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(eDifferential[11][i])) {
			if (if4Vec[i]=='d') {
				eDifferential[9][i]='1';
				conditions.setDifferent("E", 9, i, "E", 10, i);
			}
			else {
				eDifferential[9][i]='0';
				conditions.setEqual("E", 9, i, "E", 10, i);
			}
		}
	}


	// 5
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(eDifferential[8][i])) {
			eDifferential[10][i]='0';
			conditions.setDifferent("A", 7, i, "E", 8, i);
		}
	}

	// 6 + lots of hope
	wDifferential[10] = deltaSmallSig1E7;

	// 7
	vector<int> ifDiff = {0, 3, 6, 11, 14, 15, 19, 21, 25, 29};
	vector<char> ifDiffVec(32, '-');
	for (int i = 0; i < ifDiff.size(); ++i) {
		ifDiffVec[ifDiff[i]]='d';
	}
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(eDifferential[8][i])) {
			if (ifDiffVec[i]=='d') {
				eDifferential[6][i]='1';
			}
			else {
				eDifferential[6][i]='0';
			}
		}
	}

	// 8
	for (int i = 0; i < 32; ++i) {
		if(isDifferent(eDifferential[7][i])) {
			if (ifDiffVec[i]=='d') {
				eDifferential[5][i]='0';
			}
			else {
				eDifferential[5][i]='0';
			}
		}
	}
	wDifferential[8] = eDifferential[8];

	// 9
	wDifferential[7] = eDifferential[7];



	/*
	// 3
	// vector<int> ifDiff = {0, 4, 7, 8, 9, 17, 21, 22, 24, 26};
	// vector<char> ifDiffVec(32, '-');
	// for (int i = 0; i < ifDiff.size(); ++i) {
	// 	ifDiffVec[ifDiff[i]]='x';
	// }
	// for (int i = 0; i < 32; ++i) {
	// 	if (isDifferent(eDifferential[11][i])) {
	// 		if (isDifferent(ifDiffVec[i])) {
	// 			conditions.setDifferent("E", 9, i, "E", 10, i);
	// 			eDifferential[9][i] = '1';
	// 			eDifferential[10][i] = '0';
	// 		}
	// 		else {
	// 			conditions.setEqual("E", 9, i, "E", 10, i);
	// 			eDifferential[9][i] = '0';
	// 			eDifferential[10][i] = '0';
	// 		}
	// 	}
	// }

	// 4

	// for (int i = 0; i < 32; ++i) {
	// 	if (isDifferent(deltaCapSig1E7[i]) && isDifferent(eDifferential[8][i])) {
	// 		// if must be zero
	// 		eDifferential[10][i]='1';
	// 	}
	// 	else if (isDifferent(eDifferential[11][i])) {
	// 		// if must be 1
	// 		eDifferential[10][i]='0';
	// 	}
	// }


	

	// 5
	wDifferential[10] = deltaSmallSig1E7;
	// for (int i = 0; i < 32; ++i) {
	// 	if (isDifferent(eDifferential[7][i])) {
	// 		conditions.setDifferent("A", 7, i, "E", 8, i);
	// 	}
	// }

	// 6
	// hope it works :D

	// 7
	// for (int i = 0; i < 32; ++i) {
	// 	if (isDifferent(deltaCapSig1E7[i]) && isDifferent(eDifferential[8][i])) {
	// 		// if must be zero
	// 		eDifferential[6][i]='0';
	// 	}
	// 	else if (isDifferent(eDifferential[8][i])) {
	// 		// if must be 1
	// 		eDifferential[6][i]='1';
	// 	}
	// }

	// for (int i = 0; i < 32; ++i) {
	// 	if (eDifferential[6][i]=='1') {
	// 		conditions.setEqual("E", 7, i, "E", 8, i);
	// 	}
	// }


	

	vector<char> xorCondition(32, '-');
	for (int i = 0; i < 32; ++i) {
		if ((deltaCapSig0E7[i]=='x' && deltaCapSig1E7[i]=='-') || (deltaCapSig0E7[i]=='-' && deltaCapSig1E7[i]=='x')) {
			xorCondition[i] = 'x';
		}
	}

	*/

	vector<int> deltaCapSig0E7Fixed = getNumbers(CAP_SIGMA_0, aDifferential[7]);
	cout << "    ";
	for (int i = 0; i < 32; ++i) {
		cout << deltaCapSig0E7Fixed[i];
	}
	cout << endl;


	// cout << "w10 "; printSingleLine(wDifferential[10]);
	// cout << "s1: "; printSingleLine(deltaCapSig1E7);
	cout << "s0: "; printSingleLine(deltaCapSig0E7);
	// cout << "xor "; printSingleLine(xorCondition);
	cout << "a5: "; printSingleLine(aDifferential[5]);
	cout << "a6: "; printSingleLine(aDifferential[6]);
	cout << "a7: "; printSingleLine(aDifferential[7]);
	// cout << "e11 "; printSingleLine(eDifferential[11]);
	// cout << "if4 "; printSingleLine(ifDiffFor4);
	// cout << "maj "; printSingleLine(majVec);
	// cout << "if: "; printSingleLine(if4Vec);	
	// cout << "e8: "; printSingleLine(deltaCapSig0E7);
	// cout << "e9: "; printSingleLine(eDifferential[9]);
	// cout << "e10 "; printSingleLine(eDifferential[10]);

	bool printWDiff = true;
	bool printEDiff = true;
	bool printADiff = true;
	printDifferentials(printWDiff, printEDiff, printADiff, wDifferential, eDifferential, aDifferential);
	if (conditions.areSatisfiable()) {
		cout << "YES YES YES !!!" << endl;
	}
	else {
		cout << "VERY BADDDD !!!" << endl;
	}
	return 0;
}