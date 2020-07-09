#include "SHA2Util17.h"

using namespace std;

#define HEIGHT 16
#define ROTRIGHT(word,bits) (((word) >> (bits)) | ((word) << (32-(bits))))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SSIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SSIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
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
		// else if (step==6) {
		// 	// cout << "here" << endl;
		// 	state = state+1;
		// 	// cout << "tb: "; printSingleLine(eDifferential[step]);
		// 	unsigned int eNum = getVarForStateWithCheckAll(eDifferential[step], state);
		// 	currentMessageWord = getWForE(eNum, aVar, eVar, w, step);
		// }
		else if (step==7) {
			state = state+1;
			unsigned int aNum = 0;
			unsigned int aNum2=0;
			fillVarsUsingDifferentialWithCheckAll(aNum, aNum2, aDifferential[step], state);
			w[step] = getWForA(aNum, aVar, eVar, w, step);
			w2[step] = getWForA(aNum2, aVar2, eVar2, w2, step);
			return;
		}
		else if (step==6 || step==8 || step==9 || step==10 || step==11 || step==12 || step==13 || step==14) {
			state = state+1;
			unsigned int eNum = 0;
			unsigned int eNum2 = 0;
			// cout << "tb: "; printSingleLine(eDifferential[step]);
			fillVarsUsingDifferentialWithCheckAll(eNum, eNum2, eDifferential[step], state);
			w[step] = getWForE(eNum, aVar, eVar, w, step);
			w2[step] = getWForE(eNum2, aVar2, eVar2, w2, step);
			return;
		}
		else if (step==15) {
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
		else if (step==5 || step==7) {
			unsigned int aNum = rand();
			unsigned int aNum2=0;
			fillVarsUsingDifferential(aNum, aNum2, aDifferential[step]);
			w[step] = getWForA(aNum, aVar, eVar, w, step);
			w2[step] = getWForA(aNum2, aVar2, eVar2, w2, step);
			return;
		}
		else if (step==8 || step==11 || step==12 || step==13 || step==14) {
			unsigned int eNum = rand();
			unsigned int eNum2=0;
			fillVarsUsingDifferential(eNum, eNum2, eDifferential[step]);
			w[step] = getWForE(eNum, aVar, eVar, w, step);
			w2[step] = getWForE(eNum2, aVar2, eVar2, w2, step);
			return;
		}
		else if (step==15 || step==9) {
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

// bool conditionsStatisfied(AVarSHA2 aVar, EVarSHA2 eVar, vector<unsigned int> w, AVarSHA2 aVar2, EVarSHA2 eVar2, vector<unsigned int> w2, vector<vector<vector<char> > > &differentials, int step) {
// 	vector<int> aDiff = convertIntToVector(aVar.getIndex(step));
// 	vector<int> a2Diff = convertIntToVector(aVar2.getIndex(step));
// 	vector<char> aDifferential = differentials[0][step];
// 	bool check1 = true;

// 	// cout << step << " " << aVar.getIndex(step) << " " << aVar2.getIndex(step) << endl;
// 	// cout << step << endl;
// 	for (int i = 0; i < 32; ++i) {
// 		if (aDifferential[i]=='-') {
// 			if (aDiff[i]!=a2Diff[i]) {
// 				// cout << "here";
// 				check1 = false;
// 			}
// 		}
// 		if (aDifferential[i]=='x') {
// 			if (aDiff[i]==a2Diff[i]) {
// 				// cout << "here2";
// 				check1 = false;
// 			}
// 		}
// 		if (aDifferential[i]=='0') {
// 			if (aDiff[i]!=0 || a2Diff[i]!=0) {
// 				// cout << "here3";
// 				check1 = false;
// 			}
// 		}
// 		if (aDifferential[i]=='1') {
// 			if (aDiff[i]!=1 || a2Diff[i]!=1) {
// 				// cout << "here4";
// 				check1 = false;
// 			}
// 		}
// 		if (aDifferential[i]=='n') {
// 			if (aDiff[i]!=0 || a2Diff[i]!=1) {
// 				// cout << "here4";
// 				check1 = false;
// 			}
// 		}
// 		if (aDifferential[i]=='u') {
// 			if (aDiff[i]!=1 || a2Diff[i]!=0) {
// 				// cout << "here4";
// 				check1 = false;
// 			}
// 		}
// 	}

// 	vector<int> eDiff = convertIntToVector(eVar.getIndex(step));
// 	vector<int> e2Diff = convertIntToVector(eVar2.getIndex(step));
// 	vector<char> eDifferential = differentials[1][step];
// 	bool check2 = true;
// 	for (int i = 0; i < 32; ++i) {
// 		if (eDifferential[i]=='-') {
// 			if (eDiff[i]!=e2Diff[i]) {
// 				check2 = false;
// 			}
// 		}
// 		if (eDifferential[i]=='x') {
// 			if (eDiff[i]==e2Diff[i]) {
// 				check2 = false;
// 			}
// 		}
// 		if (eDifferential[i]=='0') {
// 			if (eDiff[i]!=0 || e2Diff[i]!=0) {
// 				check2 = false;
// 			}
// 		}
// 		if (eDifferential[i]=='1') {
// 			if (eDiff[i]!=1 || e2Diff[i]!=1) {
// 				check2 = false;
// 			}
// 		}
// 		if (eDifferential[i]=='n') {
// 			if (eDiff[i]!=0 || e2Diff[i]!=1) {
// 				// cout << "here4";
// 				check2 = false;
// 			}
// 		}
// 		if (eDifferential[i]=='u') {
// 			if (eDiff[i]!=1 || e2Diff[i]!=0) {
// 				// cout << "here4";
// 				check2 = false;
// 			}
// 		}
// 	}

// 	vector<int> wDiff = convertIntToVector(w[step]);
// 	vector<int> w2Diff = convertIntToVector(w2[step]);
// 	vector<char> wDifferential = differentials[2][step];
// 	bool check3 = true;
// 	for (int i = 0; i < 32; ++i) {
// 		if (wDifferential[i]=='?') {
// 			continue;
// 		}
// 		if (wDifferential[i]=='-') {
// 			if (wDiff[i]!=w2Diff[i]) {
// 				check3 = false;
// 			}
// 		}
// 		if (wDifferential[i]=='x') {
// 			if (wDiff[i]==w2Diff[i]) {
// 				check3 = false;
// 			}
// 		}
// 		if (wDifferential[i]=='0') {
// 			if (wDiff[i]!=0 || w2Diff[i]!=0) {
// 				check3 = false;
// 			}
// 		}
// 		if (wDifferential[i]=='1') {
// 			if (wDiff[i]!=1 || w2Diff[i]!=1) {
// 				check3 = false;
// 			}
// 		}
// 		if (wDifferential[i]=='n') {
// 			if (wDiff[i]!=0 || w2Diff[i]!=1) {
// 				// cout << "here4";
// 				check3 = false;
// 			}
// 		}
// 		if (wDifferential[i]=='u') {
// 			if (wDiff[i]!=1 || w2Diff[i]!=0) {
// 				// cout << "here4";
// 				check3 = false;
// 			}
// 		}
// 	}

// 	// if ((check1 || check2) && step>6) {
// 	// 	cout << step << check1 << check2 << endl;
// 	// }
// 	if (!check3) {
// 		cout << "check3 failed" << endl;
// 	}
// 	return check1 & check2 & check3;
// }

int prevPrinted = -1;
int eight = 0;
int six = 0;
int seven = 0;
int nine = 0;

void fillMessageAtStep(AVarSHA2 &aVar, EVarSHA2 &eVar, vector<unsigned int> &w, AVarSHA2 &aVar2, EVarSHA2 &eVar2, vector<unsigned int> &w2, vector<vector<vector<char> > > &differentials, Conditions &conditions, int step) {
	// if (step==7) {
	// 	seven++;
	// 	cout << "Step 7 : " << seven << endl;
	// 	// printMessages(w, w2);
	// }
	// if (step==6) {
	// 	six++;
	// 	cout << "Step 6 : " << six << endl;
	// 	// printMessages(w, w2);
	// }
	// cout << "step is " << step << endl;

	int state = -1;
	bool checkAll = false;
	if (step>6) {
		state = -1;
		checkAll = true;
	}
	if (step>8) {
		cout << "step is " << step << endl;
	}



	if (step==16) {
		done = true;
		return;
	}
	// if (step==7) {
	// 	cout << "step is " << step << endl;
	// }
	if (step==8) {
		eight++;
		cout << "Step 8 : " << eight << " " << nine << endl;
		// printMessages(w, w2);
	}
	if (step==9) {
		cout << "Step 9 : " << endl;
		nine++;
		// printMessages(w, w2);
		// checkAll = false;
		// return;
	}
	if (step==10) {
		cout << "Step 10 : " << endl;
		printMessages(w, w2);
		string a;
		cin >> a;
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
				// if (step==9) {
				// 	int i=step;
				// 	cout << "ds: "; printDifferentialForVars(EP1(eVar.getIndex(i-1)), EP1(eVar2.getIndex(i-1)));
				// 	cout << "di: "; printDifferentialForVars(CH(eVar.getIndex(i-1), eVar.getIndex(i-2), eVar.getIndex(i-3)), CH(eVar2.getIndex(i-1), eVar2.getIndex(i-2), eVar2.getIndex(i-3)));
				// 	cout << "e9: "; printDifferentialForVars(eVar.getIndex(i), eVar2.getIndex(i));	
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
				if (trials>10000000) {
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

void fillMessages(AVarSHA2 aVar, EVarSHA2 eVar, vector<unsigned int> &w, AVarSHA2 aVar2, EVarSHA2 eVar2, vector<unsigned int> &w2, vector<vector<vector<char> > > &differentials, Conditions conditions) {
	while(!done) {
		fillMessageAtStep(aVar, eVar, w, aVar2, eVar2, w2, differentials, conditions, 0);
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


	vector<int> e7Vec = convertIntToVector(1638182027);
	for (int i = 0; i < 32; ++i) {
		if (e7Vec[i]==1) {
			eDifferential[7][i] = 'x';
		}
		else {
			eDifferential[7][i] = '-';
		}
	}

	/*
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


	// bool printWDiff = true;
	// bool printEDiff = true;
	// bool printADiff = true;
	// printDifferentials(printWDiff, printEDiff, printADiff, wDifferential, eDifferential, aDifferential);
	if (conditions.areSatisfiable()) {
		cout << "YES YES YES !!!" << endl;
	}
	else {
		cout << "VERY BADDDD !!!" << endl;
	}

	*/

	vector<vector<vector<char> > > partialDifferentials = {aDifferential, eDifferential, wDifferential};

	vector<vector<vector<vector<char> > > > completeDifferentials = getCompleteDifferentials(partialDifferentials);

	while(completeDifferentials.size()==0) {
		cout << "not found!" << endl;
		completeDifferentials = getCompleteDifferentials(partialDifferentials);
	}

	vector<vector<vector<char> > > differentials = completeDifferentials[0];

	aDifferential = differentials[0];
	eDifferential = differentials[1];
	wDifferential = differentials[2];

	vector<int> change = {};
	// for (int i = 0; i < 32; ++i) {
	// 	change.push_back(i);
	// }
	for (int i = 0; i < 32; ++i) {
		if (find(change.begin(), change.end(), i)!=change.end()) {
			if (aDifferential[6][i]=='1') {
				aDifferential[6][i]='0';
			}
			else {
				aDifferential[6][i]='1';
			}
			if (aDifferential[7][i]=='1') {
				aDifferential[7][i]='0';
			}
			else {
				aDifferential[7][i]='1';
			}
			if (aDifferential[9][i]=='1') {
				aDifferential[9][i]='0';
			}
			else {
				aDifferential[9][i]='1';
			}
			if (aDifferential[10][i]=='1') {
				aDifferential[10][i]='0';
			}
			else {
				aDifferential[10][i]='1';
			}
		}
	}
	
	bool printWDiff = true;
	bool printEDiff = true;
	bool printADiff = true;
	
	printDifferentials(printWDiff, printEDiff, printADiff, differentials[2], differentials[1], differentials[0]);

	cout << "Found proper :D " << endl << endl;

	

	// Now starting to construct the message
	cout << endl << endl;

	AVarSHA2 aVar(HEIGHT);
	AVarSHA2 aVar2(HEIGHT);

	EVarSHA2 eVar(HEIGHT);
	EVarSHA2 eVar2(HEIGHT);

	vector<unsigned int> w(HEIGHT, 0);
	vector<unsigned int> w2(HEIGHT, 0);

	srand(time(0));
	cout << rand() << endl;
	// fillMessages(aVar, eVar, w, aVar2, eVar2, w2, differentials, conditions);
	printMessages(w, w2);

	writeMessagesToFile("22StepCollisionMessages.txt", w, w2);

	return 0;
}