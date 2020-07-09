#include "SHA2Util.h"

using namespace std;

// vector<string> getSigmaDifference(vector<vector<int> > &sigma, vector<char> xDifference) {
// 	// vector<char> sigmaDifference = getSigmaDifferenceFormCross(sigma, xDifference);
// 	vector<vector<int> > influencesSigmaDifference = getInfluencesSigmaDifference(sigma, xDifference);
// 	vector<string> sigmaDifference(32, "-");
// 	for (int i = 0; i < 32; ++i) {
// 		if (influencesSigmaDifference[i].size()==1) {
// 			sigmaDifference[i] = "x";
// 		}
// 		else if (influencesSigmaDifference[i].size()==3) {
// 			string influencer0 = getVariableNameString("W", lineNumber, influencesSigmaDifference[i][0]);
// 			string influencer1 = getVariableNameString("W", lineNumber, influencesSigmaDifference[i][1]);
// 			string influencer2 = getVariableNameString("W", lineNumber, influencesSigmaDifference[i][2]);

// 			//if all are same then sigmaDiff is equal to their diff
// 			//if one is different then sigmaDiff is eqaul to majority
// 			//basically in case of majority of equality, the bit is fixed
// 			if (!conditions.canSetDifferent(influencer0, influencer1)) {
// 				sigmaDifference[i]=influencer0;
// 			}
// 			else if (!conditions.canSetDifferent(influencer1, influencer2)) {
// 				sigmaDifference[i]=influencer1;
// 			}
// 			else if (!conditions.canSetDifferent(influencer2, influencer0)) {
// 				sigmaDifference[i]=influencer2;
// 			}
// 			else {
// 				sigmaDifference[i] = "x";
// 			}
// 		}
// 		else {
// 			sigmaDifference[i] = "-";
// 		}
// 	}
// 	return sigmaDifference;
// }

// void dfsHelper(vector<vector<char> > table, Conditions conditions, int currentStep) {
// 	if (currentStep==8) {
// 		for (int i = 0; i < 32; ++i) {
// 			table[8][i] = '-';
// 		}
// 		table[8][26] = 'x';
// 		table[8][23] = 'x';
// 		table[8][19] = 'x';
// 		table[8][9] = 'x';
// 		table[8][8] = 'x';

// 		for (int i = 0; i < 32; ++i) {
// 			if (table[8][i]=='x') {
// 				table.addParticularVariable("W", 8, i);
// 			}
// 		}
// 		dfsHelper(table, conditions, 7);
// 	}
// 	else if (currentStep==7) {
// 		vector<string> bitConditions(32, "-");
		
// 		Conditions conditionsCopy = conditions;
// 		vector<string> bitConditionsCopy = bitConditions;

// 		vector<string> sigmaDifference = getSigmaDifference(SMALL_SIGMA_0, table[8]);
// 		unordered_set<int> triedSigmaDifference;

// 		int currentSigmaDifference = getFirstSigmaDifference(sigmaDifference, bitConditions);
// 		while(triedSigmaDifference.find(currentSigmaDifference)!=triedSigmaDifference.end()) {
// 			int currentStepDifference = getFirstStepDifference(currentSigmaDifference, conditions, bitConditions);
// 			unordered_set<int> triedStepDifference;
// 			while(triedStepDifference.find(currentStepDifference)!=triedStepDifference.end()) {
// 				dfsHelper(table, conditions, 12);
// 				triedStepDifference.insert(currentStepDifference);
// 				conditions = conditionsCopy;
// 				bitConditions = bitConditionsCopy;
// 				currentStepDifference = getNextStepDifference(currentStepDifference, currentSigmaDifference, conditions, bitConditions);
// 			}
// 			triedSigmaDifference.insert(currentSigmaDifference);
// 			conditions = conditionsCopy;
// 			bitConditions = bitConditionsCopy;
// 			currentSigmaDifference = getNextSigmaDifference(currentSigmaDifference, sigmaDifference, bitConditions);
// 		}
// 		return;
// 	}
// }


// void dfs(vector<vector<char> > &table, Conditions &conditions) {
// 	int currentStep = 8;
// 	dfsHelper(table, conditions, currentStep);
// }

bool isEqual(char c) {
	if (c=='-' || c=='0' || c=='1') {
		return true;
	}
	return false;
}

vector<char> fillIf(vector<vector<char> > &eDifferential, int step) {
	vector<char> res(32, '-');
	for (int i = 0; i < 32; ++i) {
		if (isEqual(eDifferential[step][i])) {
			if (isEqual(eDifferential[step-1][i]) && isEqual(eDifferential[step-2][i])) {
				continue;
			}
			else if (isEqual(eDifferential[step-1][i])) {
				eDifferential[step][i] = '1';
				continue;
			}
			else if (isEqual(eDifferential[step-2][i])) {
				eDifferential[step][i] = '0';
				continue;
			}
			else {
				res[i] = 'x';
			}
		}
		else {
			if (isEqual(eDifferential[step-1][i]) && isEqual(eDifferential[step-2][i])) {
				continue;
			}
			else if (isEqual(eDifferential[step-1][i])) {
				continue;
			}
			else if (isEqual(eDifferential[step-2][i])) {
				continue;
			}
			else {
				if (eDifferential[step-1][i]==eDifferential[step-2][i] && eDifferential[step-1][i]!='x') {
					res[i] = eDifferential[step-1][i];
				}
			}
		}
	}
	return res;
}

int main(int argc, char const *argv[]) {

	// Conditions wDifferentialConditions;

	// vector<vector<char> > wDifferential = vector<vector<char> > (27, vector<char>(32, '-'));
	// for (int i = 0; i < 27; ++i) {
	// 	if (i==7 || i==8 || i==12 || i==15 || i==17) {
	// 		for (int j = 0; j < 32; ++j) {
	// 			wDifferential[i][j] = '?';
	// 		}
	// 	}
	// }

	// dfs(wDifferentialTable, wDifferentialConditions);

	vector<int> crossPositionsInW7 = {3,20,31};
	vector<int> crossPositionsInW8 = {6};
	vector<int> crossPositionsInW12 = {0, 3, 4, 7, 18, 22, 26, 27, 28, 29, 30};
	vector<int> crossPositionsInW15 = {0, 2, 24};
	vector<int> crossPositionsInW17 = {5, 7, 13, 14, 17};

	// initializing differntial for W:
	vector<vector<char> > wDifferential(27, vector<char> (32, '-'));
	vector<vector<char> > eDifferential(27, vector<char> (32, '-'));
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

	Conditions eDifferentialConditions;
	vector<string> conditions;
	for (int i = 0; i < 27; ++i) {
		eDifferentialConditions.addFullLineVariables("E", i);
	}

	eDifferential[13] = wDifferential[17];

	wDifferential[17][31-18] = 'u';
	wDifferential[17][31-17] = 'n';

	eDifferential[13][31-17] = '-';

	eDifferential[12] = eDifferential[13];
	
	eDifferential[11] = eDifferential[12];
	for (int i = 0; i < 32; ++i) {
		if (wDifferential[15][i]=='x') {
			eDifferential[11][i] = 'x';
		}
	}

	// printSingleLine(wDifferential[17]);
	// printSingleLine(eDifferential[13]);

	eDifferential[10] = calculateFromCross(CAP_SIGMA_1, eDifferential[13]);

	eDifferential[9] = eDifferential[12];
	eDifferential[9][0] = 'x';

	// eDifferential[7] = wDifferential[7];
	// eDifferential[7][4] = 'x';
	// eDifferential[7][5] = 'x';
	// eDifferential[7][6] = 'x';
	// eDifferential[7][7] = 'x';
	// eDifferential[7][21] = 'x';
	// eDifferential[7][22] = 'x';
	// eDifferential[7][23] = 'x';
	// eDifferential[7][24] = 'x';
	vector<int> xPosE7 = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 20, 21, 22, 23, 24, 25, 26, 27, 31};
	for (int i = 0; i < xPosE7.size(); ++i) {
		eDifferential[7][xPosE7[i]] = 'u';
	}
	eDifferential[7][15] = 'n';
	eDifferential[7][27] = 'n';
	eDifferential[7][31] = 'n';
	eDifferential[5][15] = 'A';

	

	// eDifferential[8] = calculateFromCross(CAP_SIGMA_1, eDifferential[7]);
	// eDifferential[8][6] = '-';
	vector<int> eDiff8 = {0, 1, 4, 5, 6, 7, 10, 11, 14, 16, 17, 21, 23, 25, 26, 28, 29, 30, 31};
	for (int i = 0; i < eDiff8.size(); ++i) {
		eDifferential[8][eDiff8[i]] = 'x';
	}

	printSingleLine(eDifferential[8]);
	eDifferential[8][0] = 'u';

	eDifferential[8][1] = 'n';

	eDifferential[6][3] = '0';
	eDifferential[5][3] = '1';
	eDifferential[8][3] = '-';

	conditions.push_back("E[5][4]!=E[6][4]");
	eDifferential[8][4] = 'x';

	conditions.push_back("E[5][5]=E[6][5]");
	eDifferential[8][5] = 'u';
	
	// 6 can be u or n depending on value of W8
	conditions.push_back("E[5][6]=E[6][6]");
	eDifferential[8][6] = 'x';
	
	conditions.push_back("E[5][7]=E[6][7]");
	eDifferential[8][7] = 'u';

	eDifferential[6][8] = '0';
	eDifferential[5][8] = '1';
	eDifferential[8][8] = '-';
	
	conditions.push_back("E[5][9]=E[6][9]");
	eDifferential[8][9] = '-';

	conditions.push_back("E[5][10]!=E[6][10]");
	eDifferential[8][10] = 'x';

	conditions.push_back("E[5][11]!=E[6][11]");
	eDifferential[8][11] = 'x';

	conditions.push_back("E[5][12]=E[6][12]");
	eDifferential[8][12] = '-';

	conditions.push_back("E[5][13]=E[6][13]");
	eDifferential[8][13] = '-';

	conditions.push_back("E[5][14]=E[6][14]");
	eDifferential[8][14] = 'u';

	eDifferential[6][15] = '1';
	eDifferential[5][15] = '0';
	eDifferential[8][15] = '-';

	eDifferential[8][16] = 'n';

	eDifferential[8][17] = 'u';

	eDifferential[8][18] = '-';

	eDifferential[8][19] = '-';

	eDifferential[6][20] = '0';
	eDifferential[5][20] = '1';
	eDifferential[8][20] = '-';

	conditions.push_back("E[5][21]!=E[6][21]");
	eDifferential[8][21] = 'x';

	eDifferential[6][22] = '1';
	eDifferential[5][22] = '0';
	eDifferential[8][22] = '-';

	conditions.push_back("E[5][23]!=E[6][23]");
	eDifferential[8][23] = 'x';

	eDifferential[6][24] = '0';
	eDifferential[5][24] = '1';
	eDifferential[8][24] = '-';
	// eDifferential[8][24] = 'A';

	conditions.push_back("E[5][25]!=E[6][25]");
	eDifferential[8][25] = 'x';

	conditions.push_back("E[5][26]=E[6][26]");
	eDifferential[8][26] = 'u';

	conditions.push_back("E[5][27]=E[6][27]");
	eDifferential[8][27] = '-';

	eDifferential[8][28] = '-';
	eDifferential[8][29] = 'u';
	eDifferential[8][30] = 'u';
	eDifferential[8][31] = 'u';


	// for e9
	// conditions.push_back("E[6][0]=E[7][0]");
	// conditions.push_back("E[6][1]=E[7][1]");
	// eDifferential[8][3] = '1';


	vector<char> sig13 = calculateFromCross(CAP_SIGMA_1, eDifferential[13]);
	vector<char> sig9 = calculateFromCross(CAP_SIGMA_1, eDifferential[9]);
	vector<char> sig8 = calculateFromCross(CAP_SIGMA_1, eDifferential[8]);
	vector<char> sig7 = calculateFromCross(CAP_SIGMA_1, eDifferential[7]);
	cout << "sig13: "; printSingleLine(sig13);
	cout << "edd13: "; printSingleLine(eDifferential[13]);
	cout << "edd12: "; printSingleLine(eDifferential[12]);
	cout << "edd11: "; printSingleLine(eDifferential[11]);
	cout << "sig09: "; printSingleLine(sig9);
	cout << "edd10: "; printSingleLine(eDifferential[10]);
	cout << "edd09: "; printSingleLine(eDifferential[9]);
	cout << "edd07: "; printSingleLine(eDifferential[7]);
	cout << "sig07: "; printSingleLine(sig7);
	cout << "wdd08: "; printSingleLine(wDifferential[8]);
	cout << "edd08: "; printSingleLine(eDifferential[8]);
	cout << "edd09: "; printSingleLine(eDifferential[9]);
	cout << "sig08: "; printSingleLine(sig8);


	cout << endl << endl << endl;
	

	cout << "Now we want to create E9 such that diffreece follows the given" << endl << endl;
	vector<char> sig9Fixed = calculateFromFixed(CAP_SIGMA_1, eDifferential[9]);

	vector<char> resIf(32, '-');

	// s means extra 
	// n means needed
	resIf = fillIf(eDifferential, 9);
	// eDifferential[9][1] = '0';
	// eDifferential[9][3] = '1';
	// resIf[4] = 'x';
	// resIf[5] = 'u';
	// resIf[6] = 'x';
	// resIf[7] = 'u';
	// eDifferential[9][8] = '1';
	// eDifferential[9][9] = '1';
	// resIf[10] = 'x';
	// resIf[11] = 'x';

	cout << "edd07: "; printSingleLine(eDifferential[7]);
	cout << "edd08: "; printSingleLine(eDifferential[8]);
	cout << "edd09: "; printSingleLine(eDifferential[9]);
	cout << "sFI09: "; printSingleLine(sig9Fixed);
	cout << "edd10: "; printSingleLine(eDifferential[10]);
	cout << "resIf: "; printSingleLine(resIf);

	cout << endl;

	for (int i = 5; i < 17; ++i) {
		cout << "edd";
		if (i<10) {
			cout << "0";
		}
		cout << i << ": "; printSingleLine(eDifferential[i]);
	}

	// printForInverse(eDifferential[9]);
	// for (int i = 0; i < 32; ++i) {
	// 	if (sig7[i]=='x') {
	// 		cout << i << " ";
	// 	}
	// }
	// cout << endl;

	return 0;
}