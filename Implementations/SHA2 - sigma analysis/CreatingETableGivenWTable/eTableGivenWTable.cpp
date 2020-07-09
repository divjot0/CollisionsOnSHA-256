#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;
// storing conditions in a matrix - first all values of E are written then all values of W
// the [i][i] element in the graph is 0 if the value has to be 0, 1 if it has to be 1 and -1 otherwise
// the [i][j] element in the graph is 0 if the values have to be same, 1 if they have to be different and -1 otherwise
// change to boolean for better

#define TOTAL_E_NODES 28*32
#define TOTAL_A_NODES 28*32
#define TOTAL_W_NODES 28*32

// Globaly used sigma functions
vector<vector<int> > capSigma0;
vector<vector<int> > capSigma1;
vector<vector<int> > smallSigma0;
vector<vector<int> > smallSigma1;
vector<string> conditionsInStringFormat;


// This function returns the matrix M, such that xM = v, where
// x = {x0, x1, x2.. x31} binary representation of x and same v is 
// the binary representation of the result of Sigma on x
vector<vector<int> > getCapSig(int leftRotate0, int leftRotate1, int leftRotate2) {
	vector<vector<int> > capSigmaFunction = vector<vector<int> > (32, vector<int> (32, 0));
	for (int i = 0; i < capSigmaFunction.size(); ++i) {
		capSigmaFunction[i][(32+i-leftRotate0)%32]=1;
		capSigmaFunction[i][(32+i-leftRotate1)%32]=1;
		capSigmaFunction[i][(32+i-leftRotate2)%32]=1;
	}
	return capSigmaFunction;
}

vector<vector<int> > getSmallSig(int leftRotate0, int leftRotate1, int leftShift0) {
	vector<vector<int> > smallSigmaFunction = vector<vector<int> > (32, vector<int> (32, 0));
	for (int i = 0; i < smallSigmaFunction.size(); ++i) {
		smallSigmaFunction[i][(32+i-leftRotate0)%32]=1;
		smallSigmaFunction[i][(32+i-leftRotate1)%32]=1;
		if (i-leftShift0>-1) {
			smallSigmaFunction[i][(i-leftShift0)]=1;
		}
	}
	return smallSigmaFunction;
}

vector<int> convertNumCrossToNumVal(vector<char> &numCross) {
	vector<int> numVal(32, 0);
	for (int i = 0; i < numCross.size(); ++i) {
		if (numCross[i]=='-') {
			numVal[i] = 0;
		}
		else {
			numVal[i] = 1;
		}
	}
	return numVal;
}

vector<char> convertNumValToNumCross(vector<int> &numVal) {
	vector<char> numCross(32, '-');
	for (int i = 0; i < numVal.size(); ++i) {
		if (numVal[i]==0) {
			numCross[i] = '-';
		}
		else {
			numCross[i] = 'x';
		}
	}
	return numCross;
}

vector<int> matMul(vector<int> numVal, vector<vector<int> > &sigma) {
	vector<int> resVal(sigma[0].size(), 0);
	for (int i = 0; i < numVal.size(); ++i) {
		if (numVal[i]==1) {
			for (int j = 0; j < sigma[0].size(); ++j) {
				resVal[j] = (resVal[j] + sigma[i][j])%2;
			}
		}
	}
	return resVal;
}

bool isEqual(char c) {
	if (c=='-' || c=='0' || c=='1' || c=='-') {
		return true;
	}
	return false;
}

vector<char> calculateFromCross(vector<vector<int> > &sigma, vector<char> &numCross) {
	vector<int> numVal = convertNumCrossToNumVal(numCross);
	vector<int> resVal = matMul(numVal, sigma);
	vector<char> resCross = convertNumValToNumCross(resVal);
	return resCross;
}

string getVar(char c0, int pos0, int bitPos0) {
	string s = string(1, c0);
	if (pos0<10) {
		s = s + "0";
	}
	s = s + to_string(pos0);
	s = s + "[" + to_string(bitPos0) + "]";
	return s;
}

// This function checks whether the loaded W values can be statisfy the conditions for W or not
bool isWDifferentialCorrect(vector<vector<char> > &wDifferential) {
	// order on conditions :
	// 1. 17, 12
	// 2. 7, 15
	// 3. 8, 7
	// 4. 15, 17
	// 5. 8, 17

	//1
	bool check1 = true;
	vector<char> s1w17Cross = calculateFromCross(smallSigma1, wDifferential[17]);
	for (int i = 0; i < s1w17Cross.size(); ++i) {
		if (s1w17Cross[i]!=wDifferential[12][i]) {
			check1 = false;
			cout << "Check 1 failed!";
			break;
		}
	}

	//2
	bool check2 = true;
	vector<char> s0w7Cross = calculateFromCross(smallSigma0, wDifferential[7]);
	for (int i = 0; i < s0w7Cross.size(); ++i) {
		if (s0w7Cross[i]!=wDifferential[15][i]) {
			check2 = false;
			cout << "Check 2 failed!";
			break;
		}
	}

	//3
	bool check3 = true;
	vector<char> s0w8Cross = calculateFromCross(smallSigma0, wDifferential[8]);
	for (int i = 0; i < s0w8Cross.size(); ++i) {
		if (s0w8Cross[i]!=wDifferential[7][i]) {
			check3 = false;
			cout << "Check 3 failed!";
			break;
		}
	}

	//4
	bool check4 = true;
	vector<char> s1w15Cross = calculateFromCross(smallSigma1, wDifferential[15]);
	for (int i = 0; i < s1w15Cross.size(); ++i) {
		if (s1w15Cross[i]!=wDifferential[17][i]) {
			check4 = false;
			cout << "Check 4 failed!";
			break;
		}
	}

	//5
	bool check5 = true;
	for (int i = 0; i < wDifferential[8].size(); ++i) {
		if (wDifferential[8][i]!=wDifferential[17][i]) {
			check5 = false;
			cout << "Check 5 failed!" << endl;
			break;
		}
	}

	return check1 & check2 & check3 & check4;// & check5;
}

void equalToCondition(char c0, int pos0, int bitPos0, int val, vector<vector<int> > &conditionsMatrix) {
	int matrixVar0 = 32*pos0 + bitPos0;
	
	// adding to stringconditions structure
	conditionsInStringFormat.push_back(getVar(c0, pos0, bitPos0) + "=" + to_string(val));

	if (c0=='W') {
		matrixVar0 = matrixVar0 + TOTAL_E_NODES;
	}

	conditionsMatrix[matrixVar0][matrixVar0] = val;
	return;
}

void sameDiffCondition(char c0, int pos0, int bitPos0, char sd, char c1, int pos1, int bitPos1, vector<vector<int> > &conditionsMatrix) {
	int matrixVar0 = 32*pos0 + bitPos0;
	if (c0=='W') {
		matrixVar0 = matrixVar0 + TOTAL_E_NODES;
	}

	int matrixVar1 = 32*pos1 + bitPos1;
	if (c1=='W') {
		matrixVar1 = matrixVar1 + TOTAL_E_NODES;
	}

	if (isEqual(sd)) {
		conditionsMatrix[matrixVar0][matrixVar1] = 0;
		conditionsMatrix[matrixVar1][matrixVar0] = 0;
		// adding to stringconditions structure
		conditionsInStringFormat.push_back(getVar(c0, pos0, bitPos0) + "=" + getVar(c1, pos1, bitPos1));
	}
	else {
		conditionsMatrix[matrixVar0][matrixVar1] = 1;
		conditionsMatrix[matrixVar1][matrixVar0] = 1;
		// adding to stringconditions structure
		conditionsInStringFormat.push_back(getVar(c0, pos0, bitPos0) + "!=" + getVar(c1, pos1, bitPos1));
	}
}



// sd represents same or different ('-' or 'x')
// takes differential, sd as input, startStep and conditionsMatrix
// makes changes to the conditionMatrix
int conditionToMakeIf(int bitPosition, char sd, vector<vector<char> > &eDifferential, int startStep, vector<vector<int> > &conditionsMatrix) {
	// done is used to add condition after this condition
	int done = 2;
	if (isEqual(sd)) {
		if (isEqual(eDifferential[startStep][bitPosition])) {
			if (isEqual(eDifferential[startStep-1][bitPosition]) && isEqual(eDifferential[startStep-2][bitPosition])) {
				// do nothing
			}
			else if (isEqual(eDifferential[startStep-1][bitPosition])) {
				equalToCondition('E', startStep, bitPosition, 1, conditionsMatrix);
				// sameDiffCondition('E', 6, i, '-', 'E', 5, i, conditionsMatrix);
			}
			else if (isEqual(eDifferential[startStep-2][bitPosition])) {
				equalToCondition('E', startStep, bitPosition, 0, conditionsMatrix);
			}
			else {
				cout << "SOMETHING WORNG!" << endl;
				done = false;
			}
		}
		else {
			if (isEqual(eDifferential[startStep-1][bitPosition]) && isEqual(eDifferential[startStep-2][bitPosition])) {
				sameDiffCondition('E', startStep-1, bitPosition, '-', 'E', startStep-2, bitPosition, conditionsMatrix);
			}
			else if (isEqual(eDifferential[startStep-1][bitPosition])) {
				sameDiffCondition('E', startStep-1, bitPosition, 'x', 'E', startStep-2, bitPosition, conditionsMatrix);
			}
			else if (isEqual(eDifferential[startStep-2][bitPosition])) {
				sameDiffCondition('E', startStep-1, bitPosition, 'x', 'E', startStep-2, bitPosition, conditionsMatrix);
			}
			else {
				sameDiffCondition('E', startStep-1, bitPosition, 'x', 'E', startStep-2, bitPosition, conditionsMatrix);
			}
		}
	}
	else {
		if (isEqual(eDifferential[startStep][bitPosition])) {
			if (!isEqual(eDifferential[startStep-1][bitPosition]) && !isEqual(eDifferential[startStep-2][bitPosition])) {
				// do nothing or condition here
			}
			else if (!isEqual(eDifferential[startStep-1][bitPosition])) {
				equalToCondition('E', startStep, bitPosition, 1, conditionsMatrix);
				done = 1;
				// sameDiffCondition('E', 6, i, '-', 'E', 5, i, conditionsMatrix);
			}
			else if (!isEqual(eDifferential[startStep-2][bitPosition])) {
				equalToCondition('E', startStep, bitPosition, 0, conditionsMatrix);
				done = 0;
			}
			else {
				cout << "SOMETHING WORNG!" << endl;
				done = -1;
			}
		}
		else {
			if (isEqual(eDifferential[startStep-1][bitPosition]) && isEqual(eDifferential[startStep-2][bitPosition])) {
				sameDiffCondition('E', startStep-1, bitPosition, 'x', 'E', startStep-2, bitPosition, conditionsMatrix);
			}
			else if (isEqual(eDifferential[startStep-1][bitPosition])) {
				sameDiffCondition('E', startStep-1, bitPosition, '-', 'E', startStep-2, bitPosition, conditionsMatrix);
			}
			else if (isEqual(eDifferential[startStep-2][bitPosition])) {
				sameDiffCondition('E', startStep-1, bitPosition, '-', 'E', startStep-2, bitPosition, conditionsMatrix);
			}
			else {
				sameDiffCondition('E', startStep-1, bitPosition, '-', 'E', startStep-2, bitPosition, conditionsMatrix);
			}
		}
	}
	return done;
}

// Adding 2 bit conditions pm denotes whether the condition 
// is c0[pos0] + c1[pos1] = 0 or c0[pos0] - c1[pos1] = 0
// right now returns whether it is possible just based on same position bits
bool conditionsForSumEqualToZero(char c0, int pos0, char pm, char c1, int pos1,  vector<vector<int> > &conditionsMatrix, vector<vector<char> > &eDifferential, vector<vector<char> > &wDifferential) {
	int matrixStartVar0=0, matrixStartVar1=0;
	vector<char> differentialForVar0 = eDifferential[pos0];
	vector<char> differentialForVar1 = eDifferential[pos1];

	if (c0=='W') {
		matrixStartVar0 = TOTAL_E_NODES;
		differentialForVar0 = wDifferential[pos0];
	}
	matrixStartVar0 = matrixStartVar0 + pos0*32;

	if (c1=='W') {
		matrixStartVar1 = TOTAL_E_NODES;
		differentialForVar1 = wDifferential[pos1];
	}
	matrixStartVar1 = matrixStartVar1 + pos1*32;

	// Checking whether it is possible to do this or not
	bool possible = true;

	for (int i = 0; i < eDifferential[0].size(); ++i) {
		if (differentialForVar0[i]!=differentialForVar1[i]) {
			possible = false;
			break;
		}
	}

	int conditionValue = 0;
	if (pm=='+') {
		conditionValue = 1;
	}

	// add conditions
	if (possible) {
		int matrixCurrentVar0 = matrixStartVar0;
		int matrixCurrentVar1 = matrixStartVar1;

		for (int i = 0; i < differentialForVar1.size(); ++i) {
			if (differentialForVar1[i]=='x') {
				conditionsMatrix[matrixStartVar0+i][matrixStartVar1+i]=conditionValue;
				conditionsMatrix[matrixStartVar1+i][matrixStartVar0+i]=conditionValue;
			}
		}
	}

	return possible;
}

// making sum equal to zero
// first argument can take any value (is free right now)
void crossForSumEqualToZeroOn(vector<char> &x, vector<char> &y) {
	for (int i = 0; i < y.size(); ++i) {
		x[i] = y[i];
	}
	return;
}

vector<char> fillPossibilitiesOfIf(vector<vector<char> > &eDifferential, int startStep) {
	vector<char> possibilities(32, '?');
	for (int i = 0; i < 32; ++i) {
		if (eDifferential[startStep][i] == '-') {
			if (eDifferential[startStep-1][i]=='-' && eDifferential[startStep-2][i]=='-'){
				possibilities[i] = '-';
			}
			else if (eDifferential[startStep-1][i]=='x' && eDifferential[startStep-2][i]=='x') {
				possibilities[i] = 'x';
			}
		}
	}
	return possibilities;
}



// used to set cross for E7 and add all conditions
void setCrossForE7(vector<vector<char> >& eDifferential,vector<vector<char> >& wDifferential,vector<vector<int> >& conditionsMatrix) {
	crossForSumEqualToZeroOn(eDifferential[7], wDifferential[7]);
	if(!conditionsForSumEqualToZero('E', 7, '-', 'W', 7, conditionsMatrix, eDifferential, wDifferential)){
		cout << "Not possible!" << endl;
	}
	return;
}

// used to set cross for E8 and add all conditions
void setCrossForE8(vector<vector<char> >& eDifferential,vector<vector<char> >& wDifferential,vector<vector<int> >& conditionsMatrix) {
	vector<char> S1e7Cross = calculateFromCross(capSigma1, eDifferential[7]);
	vector<char> ifWillBe = fillPossibilitiesOfIf(eDifferential, 8);
	cout << "cing out S1e7Cross:" << endl;
	for (int i = 0; i < 32; ++i) {
		cout << S1e7Cross[i];
	}
	cout << endl;
	cout << "cing out w8:" << endl;
	for (int i = 0; i < 32; ++i) {
		cout << wDifferential[8][i];
	}
	cout << endl;
	cout << "cing out e7:" << endl;
	for (int i = 0; i < 32; ++i) {
		cout << eDifferential[7][i];
	}
	cout << endl;
	cout << "cing out ifWillBe:" << endl;
	for (int i = 0; i < 32; ++i) {
		cout << ifWillBe[i];
	}
	cout << endl;
	for (int i = 0; i < 32; ++i) {
		if (!isEqual(S1e7Cross[i]) && !isEqual(wDifferential[8][i]) && ifWillBe[i]=='?') {
			// do nothing
			sameDiffCondition('E', 6, i, '-', 'E', 5, i, conditionsMatrix);
		}
		else if (!isEqual(S1e7Cross[i]) && isEqual(wDifferential[8][i]) && ifWillBe[i]=='?') {
			sameDiffCondition('E', 6, i, '-', 'E', 5, i, conditionsMatrix);
			eDifferential[8][i]='x';
			// if (!isEqual(wDifferential[8][i])) {
			// 	sameDiffCondition('E', 8, i, '-', 'W', 8, i, conditionsMatrix);
			// }
		}
		else if (isEqual(S1e7Cross[i]) && !isEqual(wDifferential[8][i]) && ifWillBe[i]=='?') {
			sameDiffCondition('E', 6, i, '-', 'E', 5, i, conditionsMatrix);
			eDifferential[8][i]='x';
			// if (!isEqual(wDifferential[8][i])) {
			// 	sameDiffCondition('E', 8, i, '-', 'W', 8, i, conditionsMatrix);
			// }
		}
		else if (isEqual(S1e7Cross[i]) && isEqual(wDifferential[8][i]) && ifWillBe[i]=='?'){
			sameDiffCondition('E', 6, i, '-', 'E', 5, i, conditionsMatrix);
		}
		else {
			if (!isEqual(S1e7Cross[i]) && !isEqual(wDifferential[8][i])) {
				//do nothing
			}
			else if (!isEqual(S1e7Cross[i]) && isEqual(wDifferential[8][i])) {
				eDifferential[8][i] = 'x';
			}
			else if (isEqual(S1e7Cross[i]) && !isEqual(wDifferential[8][i])) {
				eDifferential[8][i] = 'x';
			}
		}
	}
	return;
}

// used to set cross for E9 and all conditions
void setCrossForE9(vector<vector<char> >& eDifferential,vector<vector<char> >& wDifferential,vector<vector<int> >& conditionsMatrix) {
	vector<char> S1e8Cross = calculateFromCross(capSigma1, eDifferential[8]);
	vector<char> ifWillBe = fillPossibilitiesOfIf(eDifferential, 8);
	for (int i = 0; i < 32; ++i) {
		if (S1e8Cross[i]=='x' && ifWillBe[i]=='x') {
			// do nothing
		}
		else if (S1e8Cross[i]=='x' && ifWillBe[i]=='?') {
			conditionToMakeIf(i, 'x', eDifferential, 8, conditionsMatrix);
		}
		else if (S1e8Cross[i]=='x' && ifWillBe[i]=='-') {
			eDifferential[9][i] = 'x';
		}
		else if (S1e8Cross[i]=='-' && ifWillBe[i]=='-') {
			// do nothing
		}
		else if (S1e8Cross[i]=='-' && ifWillBe[i]=='?') {
			eDifferential[9][i] = 'x';
			conditionToMakeIf(i, 'x', eDifferential, 8, conditionsMatrix);
		}
		else if (S1e8Cross[i]=='-' && ifWillBe[i]=='x'){
			eDifferential[9][i] = 'x';
			// to add or condition
			// ENCOUNTERED DIFFERENT TYPE OF PROBLEM it has to be eqyal to W[9] or W[8]
			//addConditionOfEquale9and
		}
	}
	return;
}


// used to set cross for E10
void setCrossForE10(vector<vector<char> >& eDifferential, vector<vector<char> >& wDifferential, vector<vector<int> >& conditionsMatrix) {
	vector<char> S1e9Cross = calculateFromCross(capSigma1, eDifferential[9]);
	vector<char> ifWillBe = fillPossibilitiesOfIf(eDifferential, 9);
	for (int i = 0; i < 32; ++i) {
		// if (i==28) {
		// 	eDifferential[10][i] = 'x';
		// 	int res = conditionToMakeIf(i, 'x', eDifferential, 9, conditionsMatrix);
		// 	if (res==0 || res==1) {
		// 		sameDiffCondition('E', 10, i, '-', 'E', 9-2+res, i, conditionsMatrix);
		// 	}
		// }
		if (S1e9Cross[i]=='x' && ifWillBe[i]=='x') {
			// do nothing
		}
		else if (S1e9Cross[i]=='x' && ifWillBe[i]=='?') {
			eDifferential[10][i] = 'x';
			conditionToMakeIf(i, '-', eDifferential, 9, conditionsMatrix);
		}
		else if (S1e9Cross[i]=='x' && ifWillBe[i]=='-') {
			eDifferential[10][i] = 'x';
		}
		else if (S1e9Cross[i]=='-' && ifWillBe[i]=='-') {
			// do nothing
		}
		else if (S1e9Cross[i]=='-' && ifWillBe[i]=='?') {
			eDifferential[10][i] = 'x';
			conditionToMakeIf(i, 'x', eDifferential, 9, conditionsMatrix);
		}
		else if (S1e9Cross[i]=='-' && ifWillBe[i]=='x'){
			eDifferential[10][i] = 'x';
			// ENCOUNTERED DIFFERENT TYPE OF PROBLEM it has to be eqyal to W[9] or W[8]
			//addConditionOfEquale9and
			// add or condition
		}
	}
	return;
}

// 1 equation below and conditions
void setCrossForE13ByConditionsOnE17(vector<vector<char> >& eDifferential,vector<vector<char> >& wDifferential,vector<vector<int> >& conditionsMatrix) {
	crossForSumEqualToZeroOn(eDifferential[13], wDifferential[17]);
	if(!conditionsForSumEqualToZero('E', 13, '+', 'W', 17, conditionsMatrix, eDifferential, wDifferential)){
		cout << "Not possible!" << endl;
	}
	return;
}

void printSingleLine(vector<char> &v) {
	for (int i = 0; i < v.size(); ++i) {
		cout << v[i];
	}
	cout << endl;
	return;
}

// This just prints the Matrix (function)
void printSigmaFunction(vector<vector<int> > &sigmaFunction) {
	cout << "Printing the function matrix" << endl;
	for (int i = 0; i < sigmaFunction.size(); ++i) {
		for (int j = 0; j < sigmaFunction[0].size(); ++j) {
			cout << sigmaFunction[i][j] << " ";
		}
		cout << endl;
	}
	return;
}

// This prints the differentials in the order a, e, w
void printDifferentials(vector<vector<char> > &aDifferential, vector<vector<char> > &eDifferential, vector<vector<char> > &wDifferential) {
	cout << "Printing A Differential Table:" << endl;
	for (int i = 0; i < aDifferential.size(); ++i) {
		if (i<10) {
			cout << "0";
		}
		cout << i << " ";
		for (int j = 0; j < aDifferential[0].size(); ++j) {
			cout << aDifferential[i][j];
		}
		cout << endl;
	}
	cout << endl;

	cout << "Printing E Differential Table:" << endl;
	for (int i = 0; i < eDifferential.size(); ++i) {
		if (i<10) {
			cout << "0";
		}
		cout << i << " ";
		for (int j = 0; j < eDifferential[0].size(); ++j) {
			cout << eDifferential[i][j];
			// if (eDifferential[i][j]=='-') {
			// 	cout << "\\Sm";
			// }
			// else {
			// 	cout << "\\Sz";
			// }
		}
		cout << endl;
	}
	cout << endl;

	cout << "Printing W Differential Table:" << endl;
	for (int i = 0; i < wDifferential.size(); ++i) {
		if (i<10) {
			cout << "0";
		}
		cout << i << " ";
		for (int j = 0; j < wDifferential[0].size(); ++j) {
			cout << wDifferential[i][j];
			// if (wDifferential[i][j]=='-') {
			// 	cout << "\\Sm";
			// }
			// else {
			// 	cout << "\\Sz";
			// }
		}
		cout << endl;
	}
	cout << endl;
}

void printConditions() {
	cout << "These are the conditions:" << endl;
	for (int i = 0; i < conditionsInStringFormat.size(); ++i) {
		cout << conditionsInStringFormat[i] << ", ";
	}
	cout << endl;
}

// add differential .... /// to be added ":: LD and conditions
// we can make changes
// this diff and conditions are based e12 = - everywhere
void testsForE11E12(vector<vector<char> >& eDifferential,vector<vector<char> >& wDifferential,vector<vector<int> >& conditionsMatrix) {
	vector<char> S1e13Cross = calculateFromCross(capSigma1, eDifferential[13]);
	cout << "This is S1e13Cross: " << endl;
	printSingleLine(S1e13Cross);

	// cout << "This is w15Cross: " << endl;
	// printSingleLine(wDifferential[15]);

	vector<char> e11Diff (32, '-');

	for (int i = 0; i < 32; ++i) {
		if (i==8 || i==4 || i==22 || i==28 || i==25 || i==26) {
			// it is being created smartly here at these locations
			continue;
		}
		if (eDifferential[13][i]=='x' && wDifferential[15][i]=='x') {
			//addCondition that thesea re different
			sameDiffCondition('E', 13, i, 'x', 'W', 15, i, conditionsMatrix);
			equalToCondition('E', 14, i, 1, conditionsMatrix);
			// conditionToMakeIf(int bitPosition, '-', eDifferential, 14, conditionsMatrix)
		}
		else if (eDifferential[13][i]=='x' && wDifferential[15][i]=='-') {
			e11Diff[i] = 'x';
			// add condition tahat e11 and e13 are same
			// make changes e11 = '-' can be done herer
			sameDiffCondition('E', 13, i, '-', 'E', 11, i, conditionsMatrix);
			equalToCondition('E', 14, i, 1, conditionsMatrix);
		}
		else if (eDifferential[13][i]=='-' && wDifferential[15][i]=='-') {
			// do nothing
		}
		else {
			e11Diff[i] = 'x';
			// add condition that w15 != e11
			sameDiffCondition('E', 11, i, 'x', 'W', 15, i, conditionsMatrix);
			// ENCOUNTERED DIFFERENT TYPE OF PROBLEM it has to be eqyal to W[9] or W[8]
			//addConditionOfEquale9and
		}
	}

	e11Diff[9] = 'x';
	sameDiffCondition('E', 13, 8, '-', 'W', 15, 8, conditionsMatrix);
	equalToCondition('E', 14, 8, 1, conditionsMatrix);
	e11Diff[5] = 'x';
	sameDiffCondition('E', 13, 4, '-', 'W', 15, 4, conditionsMatrix);
	equalToCondition('E', 14, 4, 1, conditionsMatrix);
	e11Diff[22] = 'x';
	e11Diff[23] = 'x';
	sameDiffCondition('E', 13, 22, 'x', 'E', 11, 22, conditionsMatrix);
	equalToCondition('E', 14, 22, 1, conditionsMatrix);
	sameDiffCondition('E', 13, 22, 'x', 'E', 11, 23, conditionsMatrix);
	e11Diff[28] = 'x';
	equalToCondition('E', 14, 22, 1, conditionsMatrix);
	sameDiffCondition('E', 13, 4, 'x', 'W', 15, 4, conditionsMatrix);
	e11Diff[26] = 'x';
	sameDiffCondition('E', 11, 25, '-', 'W', 15, 25, conditionsMatrix);
	sameDiffCondition('E', 11, 25, 'x', 'E', 11, 26, conditionsMatrix);
	// cout << "This is e11Cross: " << endl;
	// printSingleLine(e11Diff);

	for (int i = 0; i < eDifferential[0].size(); ++i) {
		eDifferential[11][i] = e11Diff[i];
	}

	// cout << "This is S1e13Cross: " << endl;
	// printSingleLine(S1e13Cross);
	// cout << "This is e10Cross: " << endl;
	// printSingleLine(eDifferential[10]);
	vector<char> ifToBe (32, '-');
	for (int i = 0; i < 32; ++i) {
		if (S1e13Cross[i]=='x' && eDifferential[10][i]=='x') {
			// do nothing
		}
		else if (S1e13Cross[i]=='x' && eDifferential[10][i]=='-') {
			ifToBe[i] = 'x';
		}
		else if (S1e13Cross[i]=='-' && eDifferential[10][i]=='-') {
			// do nothing
		}
		else {
			ifToBe[i] = 'x';
			// ENCOUNTERED DIFFERENT TYPE OF PROBLEM it has to be eqyal to W[9] or W[8]
			//addConditionOfEquale9and
		}
	}
	cout << "This is ifToBe: " << endl;
	printSingleLine(ifToBe);

	// conditions for if to be:-
	for (int i = 0; i < 32; ++i) {
		if (ifToBe[i]=='x') {
			conditionToMakeIf(i, 'x', eDifferential, 13, conditionsMatrix);
		}
		else {
			conditionToMakeIf(i, '-', eDifferential, 13, conditionsMatrix);
			// ENCOUNTERED DIFFERENT TYPE OF PROBLEM it has to be eqyal to W[9] or W[8]
			//addConditionOfEquale9and
		}
	}

	vector<char> potentialE12 (32, '-');
	// eDifferential[12][18]='x';
	// eDifferential[12][26]='x';
	// eDifferential[12][27]='x';
	// eDifferential[12][15]='x';
	// eDifferential[12][4] ='x';
	// eDifferential[12][30] ='x';
	vector<char> S1e12Cross = calculateFromCross(capSigma1, eDifferential[12]);
	cout << "This is s1e12Cross: " << endl;
	printSingleLine(S1e12Cross);
	cout << "This is e9: " << endl;
	printSingleLine(eDifferential[9]);

	cout << "This is e10: " << endl;
	printSingleLine(eDifferential[10]);
	cout << "This is e11: " << endl;
	printSingleLine(eDifferential[11]);
	cout << "This is e12: " << endl;
	printSingleLine(eDifferential[12]);
	cout << "This is e13: " << endl;
	printSingleLine(eDifferential[13]);
	cout << "This is w15: " << endl;
	printSingleLine(wDifferential[15]);
	return;

}

void dfsHelper(vector<vector<vector<char> > > fullDifferential, int EStep) {
	if (EStep == 7) {
		// vector<int> deltaE = nthPossibleDelta();
	}
}

void dfs(vector<vector<vector<char> > > fullDifferential) {
	dfsHelper(fullDifferential, 7);
}

void printWDifferential(vector<vector<char> > wDiff) {
	for (int i = 0; i < wDiff.size(); ++i) {
		for (int j = 0; j < wDiff[0].size(); ++j) {
			cout << wDiff[i][j];
		}
		cout << endl;
	}
	return;
}


int main(int argc, char const *argv[]) {
	// Intializing global variables
	conditionsInStringFormat = vector<string> (0);
	// Intitalizing sigma functions
	capSigma0 = getCapSig(2, 13, 22);
	capSigma1 = getCapSig(6, 11, 25);

	smallSigma0 = getSmallSig(7, 18, 3);
	smallSigma1 = getSmallSig(17, 19, 10);

	// Loading deltaW values 
	// vector<int> crossPositionsInW7 = {0,2,3,4,5,7,9,10,12,13,15,17,20,21,23,24,25,27,29,30};
	// vector<int> crossPositionsInW8 = {2,3,4,6,7,8,10,12,13,16,17,18,20,21,23,24,27,28,29,30,31};
	// vector<int> crossPositionsInW12 = {5,9,10,11,15,16,17,19,21,22,26,27,28,29,30};
	// vector<int> crossPositionsInW15 = {1,2,4,6,8,11,13,14,17,18,19,25,27,28,30,31};
	// vector<int> crossPositionsInW17 = {2,3,4,6,7,8,10,12,13,16,17,18,20,21, 23,24,27,28,29,30,31};

	vector<int> crossPositionsInW7 = {0, 17, 28};
	vector<int> crossPositionsInW8 = {3};
	vector<int> crossPositionsInW12 = {1, 11, 15, 19, 24, 25, 26, 29};
	vector<int> crossPositionsInW15 = {21, 31};
	vector<int> crossPositionsInW17 = {2, 4, 11, 12, 14, 21};



	// preparing structures for differentials of W, E, A and Conditions Matrix
	vector<vector<char> > aDifferential(TOTAL_A_NODES/32, vector<char> (32, '-'));
	vector<vector<char> > eDifferential(TOTAL_E_NODES/32, vector<char> (32, '-'));
	vector<vector<char> > wDifferential(TOTAL_W_NODES/32, vector<char> (32, '-'));
	vector<vector<int> > conditionsMatrix(TOTAL_E_NODES+TOTAL_W_NODES, vector<int> (TOTAL_E_NODES+TOTAL_W_NODES, -1));

	// filling in values of w and setting the starting point of e, a differential tables
		// W First
			// Adding x to differential first and changing position to 31-x
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
			// Adding conditions now for deltaW8 + deltaW17 = 0
	// conditionsForSumEqualToZero('W', 17, '+', 'W', 8, conditionsMatrix, eDifferential, wDifferential);

		// E next (we start will all '-' and change as and when we statisfy difference equations)

		// A same as in E

	if (isWDifferentialCorrect(wDifferential)) {
		cout << "W Differential is correct! :-)" << endl;
	}

	vector<vector<vector<char> > > fullDifferential(3, vector<vector<char> > ());

	fullDifferential[0] = aDifferential;
	fullDifferential[1] = eDifferential;
	fullDifferential[2] = wDifferential;

	// dfs(fullDifferential);

	// Finding E Now :')

	// order of equations:

	// 11
	setCrossForE7(eDifferential, wDifferential, conditionsMatrix);

	// 10
	setCrossForE8(eDifferential, wDifferential, conditionsMatrix);
	eDifferential[8][4] = 'x';
	// eDifferential[8][3] = 'x';
	eDifferential[8][0] = 'x';
	eDifferential[8][1] = 'x';
	eDifferential[8][19] = 'x';
	eDifferential[8][18] = 'x';


	// 9
	setCrossForE9(eDifferential, wDifferential, conditionsMatrix);
	
	for (int i = 0; i < 32; ++i) {
		eDifferential[9][i] = '-';
	}
	// eDifferential[9][4] = 'x';
	eDifferential[9][31] = 'x';
	eDifferential[9][30] = 'x';
	eDifferential[9][21] = 'x';
	eDifferential[9][25] = 'x';
	eDifferential[9][11] = 'x';
	// eDifferential[9][14] = 'x';
	// eDifferential[9][17] = 'x';
	// eDifferential[9][16] = 'x';


	// 8
	setCrossForE10(eDifferential, wDifferential, conditionsMatrix);

	eDifferential[11][21] = 'x';
	eDifferential[11][31] = 'x';

	for (int i = 0; i < 32; ++i) {
		eDifferential[10][i] = '-';
	}
	eDifferential[10][1] = 'x';
	eDifferential[10][3] = 'x';
	eDifferential[10][6] = 'x';
	eDifferential[10][10] = 'x';
	eDifferential[10][11] = 'x';
	eDifferential[10][15] = 'x';
	eDifferential[10][19] = 'x';
	// eDifferential[10][18] = 'x';
	// eDifferential[10][17] = 'x';
	eDifferential[10][21] = 'x';
	eDifferential[10][22] = 'x';
	eDifferential[10][23] = 'x';
	eDifferential[10][25] = 'x';
	eDifferential[10][30] = 'x';

	// 1
	setCrossForE13ByConditionsOnE17(eDifferential, wDifferential, conditionsMatrix);

	cout << "cing cout w15:" << endl;
	printSingleLine(wDifferential[15]);


	eDifferential[13][11] = '-';
	cout << "cing cout e13:" << endl;
	printSingleLine(eDifferential[13]);

	cout << "cing cout Sig1E13: "<< endl;
	vector<char> f = calculateFromCross(capSigma1, eDifferential[13]);
	printSingleLine(f);


	// cout << "cing cout Sig1E8: "<< endl;
	// vector<char> c = calculateFromCross(capSigma1, eDifferential[8]);
	// printSingleLine(c);

	cout << "cing cout E9: "<< endl;
	// vector<char> b = calculateFromCross(capSigma1, eDifferential[9]);
	printSingleLine(eDifferential[9]);

	cout << "cing cout sig1E9: "<< endl;
	vector<char> aa = calculateFromCross(capSigma1, eDifferential[9]);
	printSingleLine(aa);

	cout << "cing cout E8: "<< endl;
	// vector<char> b = calculateFromCross(capSigma1, eDifferential[9]);
	printSingleLine(eDifferential[8]);

	cout << "cing cout E7: "<< endl;
	// vector<char> b = calculateFromCross(capSigma1, eDifferential[9]);
	printSingleLine(eDifferential[7]);

	cout << "cing cout E6: "<< endl;
	// vector<char> b = calculateFromCross(capSigma1, eDifferential[9]);
	printSingleLine(eDifferential[6]);

	// for (int i = 21; i < 32; ++i) {
	// 	eDifferential[12][i] = 'u';
	// }
	// eDifferential[12][31] = 'n';

	cout << "cing cout e12:" << endl;
	printSingleLine(eDifferential[12]);
	cout << "cing cout e10:" << endl;
	printSingleLine(eDifferential[10]);

	cout << "cing cout Sig1E13: "<< endl;
	vector<char> a = calculateFromCross(capSigma1, eDifferential[13]);
	printSingleLine(a);

	cout << "cing cout Sig1E8: "<< endl;
	vector<char> bb = calculateFromCross(capSigma1, eDifferential[8]);
	printSingleLine(bb);



	cout << "cing cout e10:" << endl;
	printSingleLine(eDifferential[10]);

	cout << "cing cout Sig1E7: "<< endl;
	vector<char> d = calculateFromCross(capSigma1, eDifferential[7]);
	printSingleLine(d);
	cout << "cing cout e7: "<< endl;
	// vector<char> a = calculateFromCross(capSigma1, eDifferential[13]);
	printSingleLine(eDifferential[7]);


	cout << "cing cout Sig1E12: "<< endl;
	vector<char> e = calculateFromCross(capSigma1, eDifferential[12]);
	printSingleLine(e);
	cout << endl << endl;

	// testsForE11E12(eDifferential, wDifferential, conditionsMatrix);


	for (int i = 0; i < 32; ++i) {
		if (eDifferential[13][i]=='x') {
			eDifferential[15][i] = '1';
		}
	}

	for (int i = 0; i < 32; ++i) {
		if (eDifferential[11][i]=='x') {

		}
	}
	eDifferential[14][21] = '1';
	eDifferential[11][21] = 'u';
	eDifferential[13][21] = 'n';
	eDifferential[14][31] = '-';
	wDifferential[15][31] = 'u';
	eDifferential[11][31] = 'n';



	eDifferential[10][21] = 'u';
	eDifferential[10][22] = 'n';



	eDifferential[5][0] = '1';
	eDifferential[6][0] = '0';
	eDifferential[7][0] = 'u';
	eDifferential[8][0] = 'n';
	eDifferential[8][1] = 'u';
	wDifferential[8][3] = 'u';
	// eDifferential[8][3] = 'u';
	eDifferential[8][4] = 'n';
	eDifferential[8][17] = 'u';
	eDifferential[8][18] = 'u';
	eDifferential[8][19] = 'n';






	// addConditions
	// This differential works if we only consider the equations by E's table
	printDifferentials(aDifferential, eDifferential, wDifferential);
	// printWDifferential(eDifferential);
	// printConditions();


	cout << "cing cout E6: "<< endl;
	// vector<char> ad = calculateFromCross(capSigma1, eDifferential[13]);
	printSingleLine(eDifferential[6]);
	cout << "cing cout E7: "<< endl;
	// vector<char> ad = calculateFromCross(capSigma1, eDifferential[13]);
	printSingleLine(eDifferential[7]);
	cout << "cing cout E8: "<< endl;
	// vector<char> ad = calculateFromCross(capSigma1, eDifferential[13]);
	printSingleLine(eDifferential[8]);


	cout << "cing cout Sig1E8: "<< endl;
	vector<char> ad = calculateFromCross(capSigma1, eDifferential[8]);
	printSingleLine(ad);

	// cout << "cing cout W8: "<< endl;
	// // vector<char> ad = calculateFromCross(capSigma1, eDifferential[13]);
	// printSingleLine(wDifferential[8]);

	cout << "cing cout E9: "<< endl;
	// vector<char> ad = calculateFromCross(capSigma1, eDifferential[13]);
	printSingleLine(eDifferential[9]);



	return 0;
}