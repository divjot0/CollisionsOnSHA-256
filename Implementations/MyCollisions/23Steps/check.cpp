#include "SHA2Util23.h"

using namespace std;

#define HEIGHT 18
#define THRESHOLD0 100 // to be decided
#define THRESHOLD2 100
#define THRESHOLD1 100


bool done = false;

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

unsigned int getVarForState(vector<char> differential, int &state) {
	vector<int> stateInBinary = convertIntToVector(state);
	int posState = 0;
	vector<int> res(32, 0);
	for (int i = 0; i < 32; ++i) {
		if (differential[i]=='1') {
			res[i]=1;
		}
		else if (differential[i]=='0') {
			res[i]=0;
		}
		else if (differential[i]=='-') {
			res[i] = stateInBinary[posState];
			posState++;
		}
		else {
			cout << "SOMETHING WRONG!" << endl;
		}
	}
	// for (int i = posState; i < 32; ++i) {
	// 	if (stateInBinary[i]==1) {
	// 		state = -1;
	// 	}
	// }
	unsigned int r = convertVectorToInt(res);
	return r;
}

unsigned int getWForA(unsigned int aNum, AVarSHA2 aVar, EVarSHA2 eVar, vector<unsigned int> w, int step) {
	unsigned int res = aNum;
	res = res - EP0(aVar.getIndex(step-1));
	res = res - MAJ(aVar.getIndex(step-1), aVar.getIndex(step-2), aVar.getIndex(step-3));
	res = res - EP1(eVar.getIndex(step-1));
	res = res - CH(eVar.getIndex(step-1), eVar.getIndex(step-2), eVar.getIndex(step-3));
	res = res - eVar.getIndex(step-4);
	res = res - K[step];
	return res;
}

unsigned int getWForE(unsigned int eNum, AVarSHA2 aVar, EVarSHA2 eVar, vector<unsigned int> w, int step) {
	unsigned int res = eNum;
	res = res - aVar.getIndex(step-4);
	res = res - EP1(eVar.getIndex(step-1));
	res = res - CH(eVar.getIndex(step-1), eVar.getIndex(step-2), eVar.getIndex(step-3));
	res = res - eVar.getIndex(step-4);
	res = res - K[step];
	return res;
}

void fillVarsUsingDifferential(unsigned int &eNum, unsigned int &eNum2, vector<char> differential) {
	vector<int> numVec = convertIntToVector(eNum);
	vector<int> num2Vec(32, 0);
	for (int i = 0; i < 32; ++i) {
		if (differential[i]=='u') {
			numVec[i]=1;
			num2Vec[i]=0;
		}
		else if (differential[i]=='n') {
			numVec[i]=0;
			num2Vec[i]=1;
		}
		else if (differential[i]=='0') {
			num2Vec[i]=0;
			numVec[i]=0;
		}
		else if (differential[i]=='1') {
			numVec[i]=1;
			num2Vec[i]=1;
		}
		else {
			num2Vec[i]=numVec[i];
		}
	}
	eNum2 = convertVectorToInt(num2Vec);
	eNum = convertVectorToInt(numVec);
	return;
}

unsigned int getVarForStateWithCheckAll(vector<char> differential, int &state) {
	vector<int> stateInBinary = convertIntToVector(state);
	int posState = 0;
	vector<int> res(32, 0);
	for (int i = 0; i < 32; ++i) {
		if (differential[i]=='1') {
			res[i]=1;
		}
		else if (differential[i]=='0') {
			res[i]=0;
		}
		else if (differential[i]=='-') {
			res[i] = stateInBinary[posState];
			posState++;
		}
		else if (differential[i]=='u') {
			cout << "SOMETHING WRONG!" << endl;
		}
		else if (differential[i]=='n') {
			cout << "SOMETHING WRONG!" << endl;
		}
		else {
			cout << "SOMETHING WRONG!" << endl;
		}
	}
	for (int i = posState; i < 32; ++i) {
		if (stateInBinary[i]==1) {
			state = -1;
		}
	}
	return convertVectorToInt(res);
}

void fillVarsUsingDifferentialWithCheckAll(unsigned int &eNum, unsigned int &eNum2, vector<char> differential, int &state) {
	vector<int> numVec = convertIntToVector(eNum);
	vector<int> num2Vec(32, 0);
	vector<int> stateInBinary = convertIntToVector(state);
	int posState = 0;
	for (int i = 0; i < 32; ++i) {
		if (differential[i]=='u') {
			numVec[i]=1;
			num2Vec[i]=0;
		}
		else if (differential[i]=='n') {
			numVec[i]=0;
			num2Vec[i]=1;
		}
		else if (differential[i]=='0') {
			num2Vec[i]=0;
			numVec[i]=0;
		}
		else if (differential[i]=='1') {
			numVec[i]=1;
			num2Vec[i]=1;
		}
		else {
			num2Vec[i]=stateInBinary[posState];
			numVec[i]=stateInBinary[posState];
			posState++;
		}
	}
	for (int i = posState; i < 32; ++i) {
		if (stateInBinary[i]==1) {
			state = -1;
		}
	}
	eNum2 = convertVectorToInt(num2Vec);
	eNum = convertVectorToInt(numVec);
	return;
}

void fillMessageRandomlyAtStep(AVarSHA2 aVar, AVarSHA2 aVar2, EVarSHA2 eVar, EVarSHA2 eVar2, vector<vector<vector<char> > > &differentials, vector<unsigned int> &w, vector<unsigned int> &w2, int step, int &state, bool checkAll) {
	vector<vector<char> > wDifferential = differentials[2];
	vector<vector<char> > eDifferential = differentials[1];
	vector<vector<char> > aDifferential = differentials[0];
	unsigned int currentMessageWord;

	if (checkAll) {
		if (step==5) {
			state = state+1;
			unsigned int aNum = getVarForStateWithCheckAll(aDifferential[step], state);
			currentMessageWord = getWForA(aNum, aVar, eVar, w, step);
		}
		else if (step==6) {
			// cout << "here" << endl;
			state = state+1;
			// cout << "tb: "; printSingleLine(eDifferential[step]);
			unsigned int eNum = getVarForStateWithCheckAll(eDifferential[step], state);
			currentMessageWord = getWForE(eNum, aVar, eVar, w, step);
		}
		else if (step==7 || step==8) {
			state = state+1;
			unsigned int aNum = 0;
			unsigned int aNum2=0;
			fillVarsUsingDifferentialWithCheckAll(aNum, aNum2, aDifferential[step], state);
			w[step] = getWForA(aNum, aVar, eVar, w, step);
			w2[step] = getWForA(aNum2, aVar2, eVar2, w2, step);
			return;
		}
		else if (step==9 || step==10 || step==12 || step==13 || step==14) {
			state = state+1;
			unsigned int eNum = 0;
			unsigned int eNum2 = 0;
			// cout << "tb: "; printSingleLine(eDifferential[step]);
			fillVarsUsingDifferentialWithCheckAll(eNum, eNum2, eDifferential[step], state);
			w[step] = getWForE(eNum, aVar, eVar, w, step);
			w2[step] = getWForE(eNum2, aVar2, eVar2, w2, step);
			return;
		}
		else if (step==15 || step==11 || step==16) {
			state = state+1;
			unsigned int v = state;
			unsigned int v2 = 0;
			fillVarsUsingDifferentialWithCheckAll(v, v2, wDifferential[step], state);
			w[step] = v;
			w2[step] = v2;
			return;
		}
		else {
			currentMessageWord = state;
		}
	}
	else {
		if (step==5) {
			state = rand();
			unsigned int aNum = getVarForState(aDifferential[step], state);
			currentMessageWord = getWForA(aNum, aVar, eVar, w, step);
		}
		else if (step==10) {
			state = rand();
			unsigned int eNum = getVarForState(eDifferential[step], state);
			w[step] = getWForE(eNum, aVar, eVar, w, step);
			w2[step] = getWForE(eNum, aVar2, eVar2, w2, step);
			return;
		}
		else if (step==6) {
			// cout << "herer" << endl;
			state = rand();
			// cout << "tb: "; printSingleLine(eDifferential[step]);
			unsigned int eNum = getVarForState(eDifferential[step], state);
			currentMessageWord = getWForE(eNum, aVar, eVar, w, step);
		}
		else if (step==7) {
			unsigned int aNum = rand();
			unsigned int aNum2=0;
			fillVarsUsingDifferential(aNum, aNum2, aDifferential[step]);
			w[step] = getWForA(aNum, aVar, eVar, w, step);
			w2[step] = getWForA(aNum2, aVar2, eVar2, w2, step);
			return;
		}
		else if (step==8 || step==12 || step==13 || step==14) {
			unsigned int eNum = rand();
			unsigned int eNum2=0;
			fillVarsUsingDifferential(eNum, eNum2, eDifferential[step]);
			w[step] = getWForE(eNum, aVar, eVar, w, step);
			w2[step] = getWForE(eNum2, aVar2, eVar2, w2, step);
			return;
		}
		else if (step==11 || step==15 || step==9) {
			unsigned int v = rand();
			unsigned int v2 = 0;
			fillVarsUsingDifferential(v, v2, wDifferential[step]);
			w[step] = v;
			w2[step] = v2;
			return;
		}
		else {
			currentMessageWord = getRandomUnsignedInt();
		}
	}

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



int prevPrinted = -1;
int eight = 0;

void fillMessageAtStep(AVarSHA2 &aVar, EVarSHA2 &eVar, vector<unsigned int> &w, AVarSHA2 &aVar2, EVarSHA2 &eVar2, vector<unsigned int> &w2, vector<vector<vector<char> > > &differentials, Conditions &conditions, int step) {
	// if (step==7) {
	// 	cout << "Step 7 : " << endl;
	// 	printMessages(w, w2);
	// }
	// cout << "step is " << step << endl;

	int state = -1;
	bool checkAll = false;
	if (step>7) {
		state = -1;
		checkAll = true;
	}
	if (step>11) {
		cout << "step is " << step << endl;
		printMessages(w, w2);
		cout << "A is " << endl;
		aVar.printFullVariable();
		cout << "----" << endl;
		aVar2.printFullVariable();
		cout << "E is " << endl;
		eVar.printFullVariable();
		cout << "----" << endl;
		eVar2.printFullVariable();
		string s;
		cin >> s;
	}



	if (step==17) {
		done = true;
		return;
	}
	// if (step>8) {
	// 	cout << "step is " << step << endl;
	// }
	if (step==11) {
		eight++;
		cout << "Step 11 : " << eight << endl;
		checkAll=false;
		// printMessages(w, w2);
	}
	if (step==9) {
		cout << "Step 9 : " << endl;
		printMessages(w, w2);
		// checkAll = false;
		// return;
	}
	if (step==10) {
		// cout << "Step 10 : " << endl;
		// printMessages(w, w2);
		checkAll = true;
		// string a;
		// cin >> a;
		// done = true;
		// return;
	}
	// if (step==7) {
	// 	cout << "Step 7 : " << endl;
	// }
	// if (step==9) {
	// 	cout << "Step 8 : " << endl;
	// 	printMessages(w, w2);
	// }
	vector<vector<char> > wDifferential = differentials[2];
	vector<vector<char> > eDifferential = differentials[1];
	vector<vector<char> > aDifferential = differentials[0];
	// if (prevPrinted!=step) {	
	// 	cout << "Step is : " << step << endl;
	// 	prevPrinted = step;
	// }
	int trials = 0;
	int countNext=0;
	int thres = 1000;
	int thresBottom = 5;
	int thres5 = 100;
	int thresBottom5 = 2;
	while(!done) {
		if (checkAll) {
			trials++;
			fillMessageRandomlyAtStep(aVar, aVar2, eVar, eVar2, differentials, w, w2, step, state, checkAll);
			fillEVars(aVar, eVar, w, aVar2, eVar2, w2, step);
			fillAVars(aVar, eVar, w, aVar2, eVar2, w2, step);
			if (conditionsStatisfied(aVar, eVar, w, aVar2, eVar2, w2, differentials, step)) {
				fillMessageAtStep(aVar, eVar, w, aVar2, eVar2, w2, differentials, conditions, step+1);
			}
			else if (step>8) {
				// if (!hasDifferenceVec(aDifferential[8])) {
				// 	cout << "8 had no difference " << endl;
				// }
				if (step==9) {
					int i=step;
					cout << "ds: "; printDifferentialForVars(EP1(eVar.getIndex(i-1)), EP1(eVar2.getIndex(i-1)));
					cout << "di: "; printDifferentialForVars(CH(eVar.getIndex(i-1), eVar.getIndex(i-2), eVar.getIndex(i-3)), CH(eVar2.getIndex(i-1), eVar2.getIndex(i-2), eVar2.getIndex(i-3)));
					cout << "e9: "; printDifferentialForVars(eVar.getIndex(i), eVar2.getIndex(i));	
				}
				// if (step==11) {
				// 	int i=step;
				// 	cout << "ds: "; printDifferentialForVars(EP1(eVar.getIndex(i-1)), EP1(eVar2.getIndex(i-1)));
				// 	cout << "di: "; printDifferentialForVars(CH(eVar.getIndex(i-1), eVar.getIndex(i-2), eVar.getIndex(i-3)), CH(eVar2.getIndex(i-1), eVar2.getIndex(i-2), eVar2.getIndex(i-3)));
				// 	cout << "e1: "; printDifferentialForVars(eVar.getIndex(i), eVar2.getIndex(i));	
				// }
				if (step==8) {
					int i=step;
					cout << "ds: "; printDifferentialForVars(EP0(aVar.getIndex(i-1)), EP0(aVar2.getIndex(i-1)));
					cout << "dm: "; printDifferentialForVars(MAJ(aVar.getIndex(i-1), aVar.getIndex(i-2), aVar.getIndex(i-3)), MAJ(aVar2.getIndex(i-1), aVar2.getIndex(i-2), aVar2.getIndex(i-3)));
					cout << "a8: "; printDifferentialForVars(aVar.getIndex(i), aVar2.getIndex(i));	
				}
				// printTableDetails(aVar, eVar, w, aVar2, eVar2, w2, differentials, conditions, step);
				// string x;
				// cin >> x;
			}
			// else if (step>=8) {
			// 	printTableDetails(aVar, eVar, w, aVar2, eVar2, w2, differentials, conditions, step);
			// 	string x;
			// 	cin >> x;
			// }
			if (state==-1) {
				return;
			}
		}
		else {
			trials++;
			fillMessageRandomlyAtStep(aVar, aVar2, eVar, eVar2, differentials, w, w2, step, state, checkAll);
			fillEVars(aVar, eVar, w, aVar2, eVar2, w2, step);
			fillAVars(aVar, eVar, w, aVar2, eVar2, w2, step);
			if (conditionsStatisfied(aVar, eVar, w, aVar2, eVar2, w2, differentials, step)) {
				fillMessageAtStep(aVar, eVar, w, aVar2, eVar2, w2, differentials, conditions, step+1);
				countNext++;
			}
			// else if (step==9) {
			// 	printTableDetails(aVar, eVar, w, aVar2, eVar2, w2, differentials, conditions, step);
			// 	string x;
			// 	cin >> x;
			// }
			if (step==7 || step==8) {
				if (trials>20) {
					return;
				}
			}
			if (step<=5) {
				if (trials>10) {
					// cout << countNext << endl;
					return;
				}
			}
			if (step==6) {
				if (trials>thres) {
					if (countNext>thresBottom) {
						thresBottom = 3*thresBottom;
						thres = 10*thres;
						cout << "extending 6 to " << thres << endl;
					}
					else {
						return;
					}
					if (trials>1000000) {
						return;
					}
				}
			}
			if (step==9 || step==10 || step==11 || step==12 || step==13 || step==14 || step==15) {
				if (trials>20000) {
					return;
				}
			}
			// if (step==5) {
			// 	if (trials>thres5) {
			// 		if (countNext>thresBottom5) {
			// 			thresBottom5 = 3*thresBottom5;
			// 			thres5 = 10*thres5;
			// 			cout << "extending 5 to " << thres5 << endl;
			// 		}
			// 		else {
			// 			return;
			// 		}
			// 		if (trials>1000000) {
			// 			return;
			// 		}
			// 	}
			// }
		}
	}
}

void fillMessages(AVarSHA2 &aVar, EVarSHA2 &eVar, vector<unsigned int> &w, AVarSHA2 &aVar2, EVarSHA2 &eVar2, vector<unsigned int> &w2, vector<vector<vector<char> > > &differentials, Conditions conditions) {
	while(!done) {
		findFreeStartCollisionByFillingStep(aVar, eVar, w, aVar2, eVar2, w2, differentials, 7);
		fillMessageAtStep(aVar, eVar, w, aVar2, eVar2, w2, differentials, conditions, 11);
	}
	// for (int i = 0; i < w.size(); ++i) {
	// 	srand(time(0));
	// 	fillMessageAtStep(aVar, eVar, w, aVar2, eVar2, w2, differentials, conditions, i);
	// 	// cout << aVar.getIndex(i) << " ";
	// }
	// cout << endl;
	printMessages(w, w2);
	return;
}

int deterministicPoints(vector<char> v) {
	int res=0;
	for (int i = 0; i < 32; ++i) {
		if (v[i]!='-') {
			res++;
		}
	}
	return res;
}

bool compareInterval(vector<vector<vector<char> > > c1, vector<vector<vector<char> > > c2) { 
    int c1Weight = deterministicPoints(c1[1][8]);
    int c2Weight = deterministicPoints(c2[1][8]);
    return (c1Weight < c2Weight); 
}

void enterDifferential(vector<vector<char> > &aDifferential) {
	for (int i = 0; i < HEIGHT; ++i) {
		string s;
		cin >> s;
		for (int j = 0; j < 32; ++j) {
			aDifferential[i][j]=s[j+3];
		}
	}
}

int main(int argc, char const *argv[]) {
	
	vector<vector<char> > wDifferential(HEIGHT, vector<char> (32, '-'));
	vector<vector<char> > eDifferential(HEIGHT, vector<char> (32, '-'));
	vector<vector<char> > aDifferential(HEIGHT, vector<char> (32, '-'));

	cout << "Enter a differential " << endl;
	enterDifferential(aDifferential);

	cout << "Enter e differential " << endl;
	enterDifferential(eDifferential);

	cout << "Enter w differential " << endl;
	enterDifferential(wDifferential);

	vector<vector<vector<char> > > differentials = {aDifferential, eDifferential, wDifferential};
	printDifferentials(true, true, true, differentials[2], differentials[1], differentials[0]);
	// Now starting to construct the message
	// cout << "Found proper :D" << endl;
	cout << endl << endl;

	AVarSHA2 aVar(HEIGHT);
	AVarSHA2 aVar2(HEIGHT);

	EVarSHA2 eVar(HEIGHT);
	EVarSHA2 eVar2(HEIGHT);

	vector<unsigned int> w(HEIGHT, 0);
	vector<unsigned int> w2(HEIGHT, 0);

	Conditions conditions;

	srand(time(0));
	cout << rand() << endl;

	aVar.setIndex(6, 344328227);
	aVar.setIndex(7, 802668214);
	aVar.setIndex(8, 804267935);
	aVar.setIndex(9, 2183662664);
	aVar.setIndex(10, 7940161);
	aVar.setIndex(11, 1994306345);
	aVar.setIndex(12, 3225551980);
	aVar.setIndex(13, 119527133);
	aVar.setIndex(14, 4225808947);
	aVar.setIndex(15, 2101314977);
	aVar2.setIndex(6, 344328227);
	aVar2.setIndex(7, 802668214);
	aVar2.setIndex(8, 41334061);
	for (int i = 9; i < 16; ++i) {
		aVar2.setIndex(i, aVar.getIndex(i));
	}

	eVar.setIndex(6, 4257668022);
	eVar.setIndex(7, 1081674753);
	eVar.setIndex(8, 801719262);
	eVar.setIndex(9, 34593132);
	eVar.setIndex(10, 34591084);
	eVar.setIndex(11, 3179739070);
	eVar.setIndex(12, 797524958);
	eVar.setIndex(13, 763794098);
	eVar.setIndex(14, 763794098);
	eVar.setIndex(15, 2202821607);
	eVar2.setIndex(6, 4257668022);
	eVar2.setIndex(7, 1081674753);
	eVar2.setIndex(8, 38785388);
	eVar2.setIndex(9, 797527006);
	eVar2.setIndex(10, 797524958);
	eVar2.setIndex(11, 3179739070);
	eVar2.setIndex(12, 34591084);
	eVar2.setIndex(13, 763794098);
	eVar2.setIndex(14, 763794098);
	eVar2.setIndex(15, 2202821607);

	w[10] = 4004731369;
	w[11] = 771297928;
	w[12] = 2359766307;
	w[13] = 856615954;
	w[14] = 1944467250;
	w[15] = 0;
	w[16] = 274146082;
	w2[10] = 4004731369;
	w2[11] = 8364054;
	w2[12] = 2359766307;
	w2[13] = 856615954;
	w2[14] = 1944467250;
	w2[15] = 0;
	w2[16] = 1037079956;

	fillPrev(aVar, eVar, w, aVar2, eVar2, w2, differentials, 9);
	printMessages(w, w2);

	cout << endl << endl << "This is A: " << endl;
	aVar.printFullVariable();
	cout << "This is E: " << endl;
	eVar.printFullVariable();

	return 0;
}