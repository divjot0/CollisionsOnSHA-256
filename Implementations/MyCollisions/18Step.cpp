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

string get0PaddedNum(int n) {
	string s = "";
	if (n<10) {
		s = "0";
	}
	s = s + to_string(n);
	return s;
}

void operateForE(vector<vector<char> > wDifferential, vector<vector<char> > eDifferential, vector<vector<char> > aDifferential, int step, vector<vector<char> > sigE) {
	cout << "edd" << get0PaddedNum(step) << ": "; printSingleLine(eDifferential[step]);
	cout << "edd" << get0PaddedNum(step-3) << ": "; printSingleLine(eDifferential[step-3]);
	cout << "edd" << get0PaddedNum(step-2) << ": "; printSingleLine(eDifferential[step-2]);
	cout << "edd" << get0PaddedNum(step-1) << ": "; printSingleLine(eDifferential[step-1]);
	cout << "sig" << get0PaddedNum(step-1) << ": "; printSingleLine(sigE[step-1]);
	cout << "add" << get0PaddedNum(step-4) << ": "; printSingleLine(aDifferential[step-4]);
	cout << "wdd" << get0PaddedNum(step) << ": "; printSingleLine(wDifferential[step]);
}

void operateForA(vector<vector<char> > wDifferential, vector<vector<char> > eDifferential, vector<vector<char> > aDifferential, int step, vector<vector<char> > sigA) {
	cout << "add" << get0PaddedNum(step-3) << ": "; printSingleLine(aDifferential[step-3]);
	cout << "add" << get0PaddedNum(step-2) << ": "; printSingleLine(aDifferential[step-2]);
	cout << "add" << get0PaddedNum(step-1) << ": "; printSingleLine(aDifferential[step-1]);
	cout << "sig" << get0PaddedNum(step-1) << ": "; printSingleLine(sigA[step-1]);
	cout << "edd" << get0PaddedNum(step) << ": "; printSingleLine(eDifferential[step]);
	cout << "add" << get0PaddedNum(step) << ": "; printSingleLine(aDifferential[step]);
}

unsigned int getRandomUnsignedInt() {
	unsigned int res = rand();
	if (res%2==1) {
		return res = RAND_MAX + rand();
	}
	return rand();
}

void fillMessageRandomlyAtStep(vector<vector<vector<char> > > &differentials, vector<unsigned int> &w, vector<unsigned int> &w2, int step) {
	vector<vector<char> > wDifferential = differentials[2];
	unsigned int currentMessageWord = getRandomUnsignedInt();
	vector<int> word = convertIntToVector(currentMessageWord);
	vector<int> word2(32, 0);
	for (int i = 0; i < 32; ++i) {
		if (wDifferential[step][i]=='x') {
			if (word[i]==1) {
				word2[i]=0;
			}
			else {
				word2[i]=1;
			}
		}
		else {
			word2[i]=word[i];
		}
	}
	w[step] = convertVectorToInt(word);
	w2[step] = convertVectorToInt(word2);
	return;
}

void fillEVars(AVarSHA2& aVar, EVarSHA2& eVar, vector<unsigned int> w, AVarSHA2& aVar2, EVarSHA2& eVar2, vector<unsigned int> w2, int step) {
	unsigned int res = aVar.getIndex(step-4);
	res = res + EP1(eVar.getIndex(step-1));
	res = res + CH(eVar.getIndex(step-1), eVar.getIndex(step-2), eVar.getIndex(step-3));
	res = res + eVar.getIndex(step-4);
	res = res + K[step] + w[step];
	eVar.setIndex(step, res);

	unsigned int res2 = aVar2.getIndex(step-4);
	res2 = res2 + EP1(eVar2.getIndex(step-1));
	res2 = res2 + CH(eVar2.getIndex(step-1), eVar2.getIndex(step-2), eVar2.getIndex(step-3));
	res2 = res2 + eVar2.getIndex(step-4);
	res2 = res2 + K[step] + w2[step];
	eVar2.setIndex(step, res2);
	return;
}

void fillAVars(AVarSHA2& aVar, EVarSHA2& eVar, vector<unsigned int> w, AVarSHA2& aVar2, EVarSHA2& eVar2, vector<unsigned int> w2, int step) {
	unsigned int res = EP0(aVar.getIndex(step-1));
	res = res + MAJ(aVar.getIndex(step-1), aVar.getIndex(step-2), aVar.getIndex(step-3));
	res = res + EP1(eVar.getIndex(step-1));
	res = res + CH(eVar.getIndex(step-1), eVar.getIndex(step-2), eVar.getIndex(step-3));
	res = res + eVar.getIndex(step-4);
	res = res + K[step] + w[step];
	aVar.setIndex(step, res);

	unsigned int res2 = EP0(aVar2.getIndex(step-1));
	res2 = res2 + MAJ(aVar2.getIndex(step-1), aVar2.getIndex(step-2), aVar2.getIndex(step-3));
	res2 = res2 + EP1(eVar2.getIndex(step-1));
	res2 = res2 + CH(eVar2.getIndex(step-1), eVar2.getIndex(step-2), eVar2.getIndex(step-3));
	res2 = res2 + eVar2.getIndex(step-4);
	res2 = res2 + K[step] + w2[step];
	aVar2.setIndex(step, res2);
	return;
}

bool conditionsStatisfied(AVarSHA2 aVar, EVarSHA2 eVar, vector<unsigned int> w, AVarSHA2 aVar2, EVarSHA2 eVar2, vector<unsigned int> w2, vector<vector<vector<char> > > &differentials, int step) {
	vector<int> aDiff = convertIntToVector(aVar.getIndex(step));
	vector<int> a2Diff = convertIntToVector(aVar2.getIndex(step));
	vector<char> aDifferential = differentials[0][step];
	bool check1 = true;

	// cout << step << " " << aVar.getIndex(step) << " " << aVar2.getIndex(step) << endl;
	// cout << step << endl;
	for (int i = 0; i < 32; ++i) {
		if (aDifferential[i]=='-') {
			if (aDiff[i]!=a2Diff[i]) {
				// cout << "here";
				check1 = false;
			}
		}
		if (aDifferential[i]=='x') {
			if (aDiff[i]==a2Diff[i]) {
				// cout << "here2";
				check1 = false;
			}
		}
		if (aDifferential[i]=='0') {
			if (aDiff[i]!=0 || a2Diff[i]!=0) {
				// cout << "here3";
				check1 = false;
			}
		}
		if (aDifferential[i]=='1') {
			if (aDiff[i]!=1 || a2Diff[i]!=1) {
				// cout << "here4";
				check1 = false;
			}
		}
	}

	vector<int> eDiff = convertIntToVector(eVar.getIndex(step));
	vector<int> e2Diff = convertIntToVector(eVar2.getIndex(step));
	vector<char> eDifferential = differentials[1][step];
	bool check2 = true;
	for (int i = 0; i < 32; ++i) {
		if (eDifferential[i]=='-') {
			if (eDiff[i]!=e2Diff[i]) {
				check2 = false;
			}
		}
		if (eDifferential[i]=='x') {
			if (eDiff[i]==e2Diff[i]) {
				check2 = false;
			}
		}
		if (eDifferential[i]=='0') {
			if (eDiff[i]!=0 || e2Diff[i]!=0) {
				check2 = false;
			}
		}
		if (eDifferential[i]=='1') {
			if (eDiff[i]!=1 || e2Diff[i]!=1) {
				check2 = false;
			}
		}
	}

	if (check1||check2) {
		cout << step << check1 << check2 << endl;
	}
	return check1 & check2;
}

void fillMessageAtStep(AVarSHA2 &aVar, EVarSHA2 &eVar, vector<unsigned int> &w, AVarSHA2 &aVar2, EVarSHA2 &eVar2, vector<unsigned int> &w2, vector<vector<vector<char> > > &differentials, Conditions &conditions, int step) {
	bool found = false;
	cout << "Step is : " << step << endl;
	while(!found) {
		fillMessageRandomlyAtStep(differentials, w, w2, step);
		fillEVars(aVar, eVar, w, aVar2, eVar2, w2, step);
		fillAVars(aVar, eVar, w, aVar2, eVar2, w2, step);
		if (conditionsStatisfied(aVar, eVar, w, aVar2, eVar2, w2, differentials, step)) {
			found = true;
			break;
		}
	}
}

void fillMessages(AVarSHA2 aVar, EVarSHA2 eVar, vector<unsigned int> w, AVarSHA2 aVar2, EVarSHA2 eVar2, vector<unsigned int> w2, vector<vector<vector<char> > > &differentials, Conditions conditions) {
	for (int i = 0; i < w.size(); ++i) {
		srand(time(0));
		fillMessageAtStep(aVar, eVar, w, aVar2, eVar2, w2, differentials, conditions, i);
		// cout << aVar.getIndex(i) << " ";
	}
	cout << endl;
	printMessages(w, w2);
	return;
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

	/* First Possibility 

	eDifferential[5][31] = 'x';
	conditions.setEqual("A", 5, 31, "E", 5, 31);
	aDifferential[5][31] = 'x'; sigA[5] = calculateFromCross(CAP_SIGMA_0, aDifferential[5]);
	eDifferential[6] = sigA[5];

	conditions.setDifferent("A", 3, 31, "A", 4, 31);
	aDifferential[6][31] = 'x'; sigA[6] = calculateFromCross(CAP_SIGMA_0, aDifferential[6]);
	// this needs to be satisfied
	conditions.setEqual("A", 5, 31, "A", 6, 31);
	eDifferential[7] = sigA[6];


	// this needs to be satisfied
	conditions.setEqual("A", 6, 31, "A", 7, 31);
	aDifferential[7][31] = 'x'; sigA[7] = calculateFromCross(CAP_SIGMA_0, aDifferential[7]);
	eDifferential[8] = sigA[7];
	conditions.setDifferent("A", 7, 31, "E", 8, 31);
	eDifferential[8][31] = 'x';

	// eDifferential[9], eDifferential[10] become zero by construction
	eDifferential[11][31] = 'x';
	conditions.setEqual("A", 7, 31, "E", 11, 31);


	*/
	

	/* Second One */

	eDifferential[5][31] = 'x';
	conditions.setEqual("A", 5, 31, "E", 5, 31);
	aDifferential[5][31] = 'x'; sigA[5] = calculateFromCross(CAP_SIGMA_0, aDifferential[5]);
	eDifferential[6] = sigA[5];

	conditions.setEqual("A", 3, 31, "A", 4, 31);
	// when we keep eD[6] = sigA[5], we get aD[6] = '-'

	conditions.setEqual("A", 4, 31, "A", 6, 31);
	// when we keep eD[7] = '-', we get aD[7] = '-'

	conditions.setEqual("A", 6, 31, "A", 7, 31);
	// now we have to keep E[8] = '-'

	conditions.setEqual("A", 5, 31, "E", 9, 31);
	eDifferential[9][31] = 'x';


	// operateForA(wDifferential, eDifferential, aDifferential, 8, sigA);


	// Start E now
	/* For 2nd A */

	// because delta E is fixed
	for (int i = 0; i < HEIGHT; ++i) {
		sigE[i] = calculateFromCross(CAP_SIGMA_1, eDifferential[i]);
	}

	//1
	conditions.setEqual("E", 5, 31, "W", 5, 31);
	wDifferential[5][31] = 'x';

	//2 (we used the fact that never are they both)
	conditions.setEqual("E", 3, 31, "E", 4, 31);
	for (int i = 0; i < 32; ++i) {
		if (eDifferential[6][i]=='x') {
			conditions.setEqual("E", 6, i, "W", 6, i);
			wDifferential[6][i] = 'x';
		}
		if (sigE[5][i]=='x') {
			wDifferential[6][i] = 'x';
		}
	}

	//3
	for (int i = 0; i < 32; ++i) {
		if (eDifferential[6][i]=='x') {
			conditions.setEqual("E", 4, i, "E", 5, i);
		}
	}
	eDifferential[6][31] = '0';
	for (int i = 0; i < 32; ++i) {
		if (sigE[6][i]=='x') {
			wDifferential[7][i]='x';
		}
	}

	//4
	for (int i = 0; i < 32; ++i) {
		if (eDifferential[6][i]=='x') {
			eDifferential[7][i] = '0';
		}
		if (eDifferential[5][i]=='x') {
			eDifferential[7][i] = '1';
		}
	}
	
	//5
	//becasue e9[31] = a5[31], they cancle
	wDifferential[9][31] = 'x';
	for (int i = 0; i < 32; ++i) {
		if (eDifferential[6][i]=='x') {
			eDifferential[8][i] = '1';
		}
	}

	//6
	conditions.setEqual("E", 7, 31, "E", 8, 31);
	eDifferential[8][31] = '1';
	for (int i = 0; i < 32; ++i) {
		if (sigE[9][i]=='x') {
			wDifferential[10][i] = 'x';
		}
		if (eDifferential[6][i]=='x') {
			wDifferential[10][i] = 'x';
		}
	}

	//7
	eDifferential[10][31] = '0';

	//8
	eDifferential[11][31] = '1';

	//9
	wDifferential[13][31] = 'x';

	//10

	//11


	// operateForE(wDifferential, eDifferential, aDifferential, 15, sigE);



	if (conditions.areSatisfiable()) {
		cout << "Good" << endl;
	}

	aDifferential[3][31] = '1';
	aDifferential[4][31] = '1';
	aDifferential[6][31] = '1';
	aDifferential[7][31] = '1';

	eDifferential[3][31] = '1';
	eDifferential[4][31] = '1';

	eDifferential[4][29] = '1';
	eDifferential[5][29] = '1';
	eDifferential[4][18] = '1';
	eDifferential[5][18] = '1';
	eDifferential[4][9] = '0';
	eDifferential[5][9] = '0';


	bool printWDiff = true;
	bool printEDiff = true;
	bool printADiff = true;
	printDifferentials(printWDiff, printEDiff, printADiff, wDifferential, eDifferential, aDifferential);

	cout << "Conditions: " << endl;
	conditions.printConditions();

	
	vector<vector<char> > newWDifferential(HEIGHT, vector<char> (32, '-'));
	vector<vector<char> > newEDifferential(HEIGHT, vector<char> (32, '-'));
	vector<vector<char> > newADifferential(HEIGHT, vector<char> (32, '-'));

	for (int i = 0; i < HEIGHT-2; ++i) {
		for (int j = 0; j < 32; ++j) {
			newADifferential[i][j] = aDifferential[i+2][j];
			newEDifferential[i][j] = eDifferential[i+2][j];
			newWDifferential[i][j] = wDifferential[i+2][j];
		}
	}

	vector<vector<vector<char> > > differentials = {newADifferential, newEDifferential, newWDifferential};


	// Now starting to construct the message
	cout << endl << endl;

	AVarSHA2 aVar(HEIGHT);
	AVarSHA2 aVar2(HEIGHT);

	EVarSHA2 eVar(HEIGHT);
	EVarSHA2 eVar2(HEIGHT);

	vector<unsigned int> w(HEIGHT, 0);
	vector<unsigned int> w2(HEIGHT, 0);

	
	fillMessages(aVar, eVar, w, aVar2, eVar2, w2, differentials, conditions);

	unsigned int r = -1;
	srand(time(0));
	cout << endl << r << " " << r+1 << " " << rand() << endl;

	return 0;
}