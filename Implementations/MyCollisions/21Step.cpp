#include "SHA2Util.h"
#include "inverse.h"

using namespace std;

#define HEIGHT 16
#define ROTRIGHT(word,bits) (((word) >> (bits)) | ((word) << (32-(bits))))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SSIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SSIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))

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


	vector<int> e7Vec = convertIntToVector(1839297739);
	for (int i = 0; i < 32; ++i) {
		if (e7Vec[i]==1) {
			eDifferential[7][i] = 'x';
		}
		else {
			eDifferential[7][i] = '-';
		}
	}


	vector<char> deltaSmallSig1E7 = calculateFromCross(SMALL_SIGMA_1, eDifferential[7]);
	vector<char> deltaCapSig0E7 = calculateFromCross(CAP_SIGMA_0, eDifferential[7]);
	vector<char> deltaCapSig1E7 = calculateFromCross(CAP_SIGMA_1, eDifferential[7]);
	wDifferential[10] = vector<char> (32, '?');
	wDifferential[8] = vector<char> (32, '?');
	wDifferential[7] = vector<char> (32, '?');

	// A 
	
	// 1
	aDifferential[7] = eDifferential[7];
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(eDifferential[7][i])) {
			conditions.setEqual("A", 7, i, "E", 7, i);
		}
	}


	// 2
	vector<int> majDiff;
	for (int i = 0; i < 32; ++i) {
		if (!isDifferent(deltaCapSig0E7[i]) && isDifferent(aDifferential[7][i])) {
			majDiff.push_back(i);
		}
	}
	vector<char> majVec(32, '-');
	for (int i = 0; i < majDiff.size(); ++i) {
		majVec[majDiff[i]]='d';
	}
	majVec[1]='-';
	majVec[11]='-';
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
		if (isDifferent(eDifferential[11][i])) {
			conditions.setEqual("A", 7, i, "E", 11, i);
		}
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
	// vector<char> ifDiffFor4(32, '-');
	// for (int i = 0; i < 32; ++i) {
	// 	if (isDifferent(eDifferential[8][i]) && !isDifferent(deltaCapSig1E7[i])) {
	// 		ifDiffFor4[i]='d';
	// 	}
	// }
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(eDifferential[8][i])) {
			conditions.setEqual("E", 9, i, "E", 10, i);
			eDifferential[9][i]='0';
			eDifferential[10][i]='0';
		}
	}


	
	// 5 
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(eDifferential[8][i])) {
			conditions.setDifferent("A", 7, i, "E", 8, i);
		}
	}

	// 6
	// automatically

	// 7
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(eDifferential[8][i])) {
			eDifferential[6][i]='1';
		}
	}
	
	// wDiff[7], wD[8] can be anything

	// cout << "w10 "; printSingleLine(wDifferential[10]);
	// cout << "s1: "; printSingleLine(deltaCapSig1E7);
	cout << "s0: "; printSingleLine(deltaCapSig0E7);
	// cout << "xor "; printSingleLine(xorCondition);
	cout << "e7: "; printSingleLine(eDifferential[7]);
	// cout << "e11 "; printSingleLine(eDifferential[11]);
	cout << "maj "; printSingleLine(majVec);
	// cout << "if: "; printSingleLine(ifDiffFor7);
	// cout << "ifz "; printSingleLine(if4VecZ);	
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



	vector<vector<vector<char> > > partialDifferentials = {aDifferential, eDifferential, wDifferential};

	vector<vector<vector<vector<char> > > > completeDifferentials = getCompleteDifferentials(partialDifferentials);
	// printDifferentials(printWDiff, printEDiff, printADiff, completeDifferentials[0][2], completeDifferentials[0][1], completeDifferentials[0][0]);

	if (completeDifferentials.size()>0) {
		cout << "Found proper :D " << endl << endl;
	}
	
	// vector<vector<int> > causingBits = getCausingBits("CAP_SIGMA_0");
	// for (int i = 0; i < 32; ++i) {
	// 	cout << i << ": " << causingBits[i][0] << " " << causingBits[i][1] << " " << causingBits[i][2] << endl;
	// }
	// vector<char> test(32, '-');
	// test[0]='x';
	// printSingleLine(calculateFromCross(CAP_SIGMA_0, test));
	// for (int i = 0; i < 32; ++i) {
	// 	cout << i%10;
	// }
	// cout << endl;
	return 0;
}