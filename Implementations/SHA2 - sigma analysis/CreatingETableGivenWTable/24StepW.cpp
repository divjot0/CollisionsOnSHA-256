#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include "tableUtil.h"

using namespace std;


//returns the assigment number
int nthAssignmentToxInWi(vector<char> &wDifferential, int n) {
	int xNum = 0;
	for (int i = 0; i < 32; ++i) {
		if (wDifferential[i]=='x') {
			xNum++;
		}
	}

	int totalPossibleAssignments = pow(2, xNum);

	n = n%totalPossibleAssignments;

	vector<int> nInVecForm(xNum, 0);
	int i=0;
	int nCopy = n;
	while(n>0) {
		nInVecForm[i] = n%2;
		n = n/2;
		i++;
	}
	n = nCopy;

	int ind = 0;
	for (int i = 0; i < 32; ++i) {
		if (wDifferential[i]=='x') {
			wDifferential[i] = 'u';
			if (nInVecForm[ind]==1) {
				wDifferential[i] = 'n';
			}
			ind++;
		}
	}
	return n;
}

//returns the random assignment numver
int randomAssignmentToxInWi(vector<char> &wDifferential) {
	int xNum = 0;
	for (int i = 0; i < 32; ++i) {
		if (wDifferential[i]=='x') {
			xNum++;
		}
	}

	int totalPossibleAssignments = pow(2, xNum);

	return nthAssignmentToxInWi(wDifferential, rand()%totalPossibleAssignments);
}


void fillFixedToBeAssigned(vector<char> &toBeAssigned, vector<char> &alreadyFixed, vector<vector<int> > &sigma) {
	// vector<int> fixedNum = convertToNum(alreadyFixed);
	vector<vector<int> > causedByCrosses(32, vector<int> ());
	vector<vector<int> > causedByAll(32, vector<int> ());

	for (int i = 0; i < 32; ++i) {
		if (alreadyFixed[i]!='-') {
			for (int j = 0; j < 32; ++j) {
				if (sigma[i][j]==1) {
					causedByCrosses[j].push_back(i);
				}
			}
		}
		for (int j = 0; j < 32; ++j) {
			if (sigma[i][j]==1) {
				causedByAll[j].push_back(i);
			}
		}
	}

	for (int i = 0; i < 32; ++i) {
		if (causedByCrosses[i].size()==causedByAll[i].size()) {
			if (causedByCrosses[i].size()%2==1) {
				int countU = 0;
				for (int j = 0; j < causedByCrosses[i].size(); ++j) {
					if (alreadyFixed[causedByCrosses[i][j]]=='u') {
						countU++;
					}
				}
				if (countU%2==1) {
					toBeAssigned[i]='n';
				}
				else {
					toBeAssigned[i]='u';
				}
			}
		}
	}
	return;
}

void fillAllFixed(vector<char> &toBeAssigned, vector<char> &alreadyFixed, vector<vector<int> > &sigma) {
	// vector<int> fixedNum = convertToNum(alreadyFixed);
	vector<vector<int> > causedByCrosses(32, vector<int> ());
	vector<vector<int> > causedByAll(32, vector<int> ());

	for (int i = 0; i < 32; ++i) {
		if (alreadyFixed[i]!='-') {
			for (int j = 0; j < 32; ++j) {
				if (sigma[i][j]==1) {
					causedByCrosses[j].push_back(i);
				}
			}
		}
		for (int j = 0; j < 32; ++j) {
			if (sigma[i][j]==1) {
				causedByAll[j].push_back(i);
			}
		}
	}

	for (int i = 0; i < 32; ++i) {
		if (causedByCrosses[i].size()==causedByAll[i].size()) {
			if (causedByCrosses[i].size()%2==1) {
				int countU = 0;
				for (int j = 0; j < causedByCrosses[i].size(); ++j) {
					if (alreadyFixed[causedByCrosses[i][j]]=='u') {
						countU++;
					}
				}
				if (countU%2==1) {
					toBeAssigned[i]='n';
				}
				else {
					toBeAssigned[i]='u';
				}
			}
		}
	}
	return;
}

void fill01Fixed(vector<char> &w15, vector<char> &w17, vector<vector<int> > &sigma) {
	// vector<int> fixedNum = convertToNum(alreadyFixed);
	vector<vector<int> > causedByCrosses(32, vector<int> ());
	vector<vector<int> > causedByAll(32, vector<int> ());
	vector<vector<int> > makeZero(32, vector<int> ());

	for (int i = 0; i < 32; ++i) {
		if (w17[i]=='u' || w17[i]=='n' || w17[i]=='x') {
			for (int j = 0; j < 32; ++j) {
				if (sigma[i][j]==1) {
					if (w15[j]=='u' || w15[j]=='n' || w15[j]=='x') {
						causedByCrosses[j].push_back(i);
					}
					else {
						makeZero[i].push_back(j);
					}
				}
			}
		}
	}

	vector<int> fixThese(32, -1);
	bool found = false;
	for (int i = 0; i < 32; ++i) {
		if (makeZero[i].size()==1) {
			fixThese[makeZero[i][0]] = 0;
			found = true;
		}
	}

	while(found) {
		found = false;
		for (int i = 0; i < 32; ++i) {
			if (makeZero[i].size()>1) {
				int numFree = 0;
				for (int j = 0; j < makeZero[i].size(); ++j) {
					if (fixThese[makeZero[i][j]]==-1) {
						numFree++;
					}
				}
				if (numFree==1) {
					for (int j = 0; j < makeZero[i].size(); ++j) {
						if (fixThese[makeZero[i][j]]==-1) {
							fixThese[makeZero[i][j]] = 0;
							found = true;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < 32; ++i) {
		if (fixThese[i]==0) {
			w15[i] = '0';
		}
	}

	return;
}

int count2;


void buildBack(vector<vector<char> > wDifferential, int buildStep, vector<vector<int> > &smallSigma0, vector<vector<int> > &smallSigma1) {
	if (buildStep==12) {
		fill01Fixed(wDifferential[17], wDifferential[12], smallSigma1);
		fill01Fixed(wDifferential[15], wDifferential[17], smallSigma1);
		fill01Fixed(wDifferential[7], wDifferential[15], smallSigma0);
		fill01Fixed(wDifferential[8], wDifferential[7], smallSigma0);
		count2++;
		cout << count2 << endl;
		printWDifferential(wDifferential);
	}
}


void dfsHelper(vector<vector<char> > wDifferential, vector<char> toBeAssigned, vector<char> alreadyFixed, vector<vector<int> > &sigma, int toBeAssignedNumber, vector<vector<int> > &smallSigma0, vector<vector<int> > &smallSigma1) {
	if (toBeAssignedNumber==7) {
		int i=0;
		fillFixedToBeAssigned(wDifferential[7], wDifferential[8], smallSigma0);
		vector<char> copy = wDifferential[7];
		while (true) {
			wDifferential[7] = copy;
			int r = nthAssignmentToxInWi(wDifferential[7], i);
			if (r<i) {
				break;
			}
			// for (int i = 0; i < 32; ++i) {
			// 	cout << wDifferential[7][i];
			// }
			// cout << endl;
			dfsHelper(wDifferential, wDifferential[15], wDifferential[7], smallSigma0, 15, smallSigma0, smallSigma1);
			i++;
		}
	}

	if (toBeAssignedNumber==15) {
		int i=0;
		fillFixedToBeAssigned(wDifferential[15], wDifferential[7], smallSigma0);
		vector<char> copy = wDifferential[15];
		while (true) {
			wDifferential[15] = copy;
			int r = nthAssignmentToxInWi(wDifferential[15], i);
			if (r<i) {
				break;
			}
			// for (int i = 0; i < 32; ++i) {
			// 	cout << wDifferential[7][i];
			// }
			// cout << endl;
			dfsHelper(wDifferential, wDifferential[17], wDifferential[15], smallSigma1, 17, smallSigma0, smallSigma1);
			i++;
		}
	}

	if (toBeAssignedNumber==17) {
		int i=0;
		fillFixedToBeAssigned(wDifferential[17], wDifferential[15], smallSigma1);
		vector<char> copy = wDifferential[17];
		while (true) {
			wDifferential[17] = copy;
			int r = nthAssignmentToxInWi(wDifferential[17], i);
			if (r<i) {
				break;
			}
			// for (int i = 0; i < 32; ++i) {
			// 	cout << wDifferential[7][i];
			// }
			// cout << endl;
			dfsHelper(wDifferential, wDifferential[12], wDifferential[17], smallSigma1, 12, smallSigma0, smallSigma1);
			i++;
		}
	}

	if (toBeAssignedNumber==12) {
		int i=0;
		fillFixedToBeAssigned(wDifferential[12], wDifferential[17], smallSigma1);
		vector<char> copy = wDifferential[12];
		while (true) {
			wDifferential[12] = copy;
			int r = nthAssignmentToxInWi(wDifferential[12], i);
			if (r<i) {
				break;
			}
			// for (int i = 0; i < 32; ++i) {
			// 	cout << wDifferential[7][i];
			// }
			// cout << endl;
			// dfsHelper(wDifferential, wDifferential[12], wDifferential[17], smallSigma1, 12, smallSigma0, smallSigma1);
			buildBack(wDifferential, 12, smallSigma0, smallSigma1);
			i++;
		}
	}

	// if (toBeAssignedNumber==17) {
	// 	int i=0;
	// 	vector<char> fixedW17 = wDifferential[17];
	// 	for (int i = 0; i < 32; ++i) {
	// 		if (wDifferential[17][i]!='-') {
	// 			fixedW17[i]='x';
	// 		}
	// 	}
		
	// 	bool works = true;
	// 	for (int i = 0; i < 32; ++i) {
	// 		if (fixedW17[i]!=wDifferential[17][i] && fixedW17[i]!='x') {
	// 			works = false;
	// 			break;
	// 		}
	// 	}
	// 	if (works) {
	// 		// cout << "works" << endl;
	// 		// printWDifferential(wDifferential);
	// 		fillAllFixed(wDifferential[12], wDifferential[17], smallSigma1);
	// 		buildBack(wDifferential, 15, smallSigma0, smallSigma1);
	// 	}
	// }

	
	return;
}

//
void dfs(vector<vector<char> > &wDifferential, vector<vector<int> > &smallSigma0, vector<vector<int> > &smallSigma1) {
	int i=0;
	vector<char> copy = wDifferential[8];
	while (true) {
		wDifferential[8] = copy;
		int r = nthAssignmentToxInWi(wDifferential[8], i);
		//set wDiff[17] opp of wDiff[8]
		if (r<i) {
			break;
		}
		dfsHelper(wDifferential, wDifferential[7], wDifferential[8], smallSigma0, 7, smallSigma0, smallSigma1);
		i=i+1;
	}
	return;
}


int main(int argc, char const *argv[]) {
	count2 = 0;
	vector<vector<int> > smallSigma0 = getSmallSig(7, 18, 3);
	vector<vector<int> > smallSigma1 = getSmallSig(17, 19, 10);

	// Loading deltaW values 
	// vector<int> crossPositionsInW7 = {0,2,3,4,5,7,9,10,12,13,15,17,20,21,23,24,25,27,29,30};
	// vector<int> crossPositionsInW8 = {2,3,4,6,7,8,10,12,13,16,17,18,20,21,23,24,27,28,29,30,31};
	// vector<int> crossPositionsInW12 = {5,9,10,11,15,16,17,19,21,22,26,27,28,29,30};
	// vector<int> crossPositionsInW15 = {1,2,4,6,8,11,13,14,17,18,19,25,27,28,30,31};
	// vector<int> crossPositionsInW17 = {2,3,4,6,7,8,10,12,13,16,17,18,20,21, 23,24,27,28,29,30,31};

	vector<int> crossPositionsInW7 = {3,20,31};
	vector<int> crossPositionsInW8 = {6};
	vector<int> crossPositionsInW12 = {0, 3, 4, 7, 18, 22, 26, 27, 28, 29, 30};
	vector<int> crossPositionsInW15 = {0, 2, 24};
	vector<int> crossPositionsInW17 = {5, 7, 13, 14, 17};

	// initializing differntial for W:
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

	vector<vector<char> > wDifferentialCrossOnly = wDifferential;

	// cout << nthAssignmentToxInWi(wDifferential[8], 400) << endl;

	// for (int i = 0; i < 32; ++i) {
	// 	cout << wDifferential[8][i];
	// }
	// cout << endl;

	dfs(wDifferential, smallSigma0, smallSigma1);

	return 0;
}