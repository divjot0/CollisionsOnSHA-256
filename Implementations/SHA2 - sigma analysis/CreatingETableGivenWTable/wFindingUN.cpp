#include <iostream>
#include <vector>
#include "tableUtil.h"

using namespace std;

vector<vector<int> > capSigma0;
vector<vector<int> > capSigma1;
vector<vector<int> > smallSigma0;
vector<vector<int> > smallSigma1;

void addOneToVector(vector<int> &v) {
	int c = 1;
	for (int i = v.size()-1; i >= 0; --i) {
		if (v[i]==0) {
			v[i]=1;
			break;
		}
		v[i]=0;
	}
	return;
}

void dfs(vector<vector<int> > xBitsOfW8ThatAffectW17, vector<vector<int> > &allBitsOfW8ThatAffectW17, vector<int> answer, int start, vector<vector<int> > &result) {
	if (start>31) {
		result.push_back(answer);
		for (int i = 0; i < 32; ++i) {
			cout << answer[i] << " ";
		}
		cout << endl;
		return;
	}
	if (xBitsOfW8ThatAffectW17[start].size()%2==0) {
		dfs(xBitsOfW8ThatAffectW17, allBitsOfW8ThatAffectW17, answer, start+1, result);
		return;
	}

	// cout << "Start is : " << start << endl;
	// for (int i = 0; i < 32; ++i) {
	// 	cout << answer[i] << " ";
	// }
	// cout << endl;
	

	// vector<int> varsHere = vector<int> ();
	// varsHere.push_back(start);

	int numFreeVar = 0;
	if (answer[start]==-1) {
		numFreeVar++;
	}
	for (int i = 0; i < xBitsOfW8ThatAffectW17[start].size(); ++i) {
		if (answer[xBitsOfW8ThatAffectW17[start][i]]==-1) {
			numFreeVar++;
		}
	}

	numFreeVar--;
	// cout << numFreeVar << endl;

	if (numFreeVar==-1) {
		int sumMod2 = answer[start];
		for (int i = 0; i < xBitsOfW8ThatAffectW17[start].size(); ++i) {
			sumMod2 = (sumMod2 + answer[xBitsOfW8ThatAffectW17[start][i]])%2;
		}
		if (sumMod2==1) {
			dfs(xBitsOfW8ThatAffectW17, allBitsOfW8ThatAffectW17, answer, start+1, result);
		}
		else {
			for (int i = 0; i < allBitsOfW8ThatAffectW17[start].size(); ++i) {
				if (answer[allBitsOfW8ThatAffectW17[start][i]]==-1) {
					answer[allBitsOfW8ThatAffectW17[start][i]] = 1;
					dfs(xBitsOfW8ThatAffectW17, allBitsOfW8ThatAffectW17, answer, start+1, result);
					return;
				}
			}
		}
		return;
	}
	if (numFreeVar<0) {
		return;
	}
	if (numFreeVar==0) {
		if (answer[start]==-1) {
			int sumMod2 = 0;
			for (int i = 0; i < xBitsOfW8ThatAffectW17[start].size(); ++i) {
				sumMod2 = (sumMod2 + answer[xBitsOfW8ThatAffectW17[start][i]])%2;
			}
			answer[start] = (sumMod2+1)%2;
		}
		else {
			int sumMod2 = answer[start];
			for (int i = 0; i < xBitsOfW8ThatAffectW17[start].size(); ++i) {
				if (answer[xBitsOfW8ThatAffectW17[start][i]]!=-1) {
					sumMod2 = (sumMod2 + answer[xBitsOfW8ThatAffectW17[start][i]])%2;
				}
			}
			for (int i = 0; i < xBitsOfW8ThatAffectW17[start].size(); ++i) {
				if (answer[xBitsOfW8ThatAffectW17[start][i]]==-1) {
					answer[xBitsOfW8ThatAffectW17[start][i]] = (sumMod2+1)%2;
				}
			}
		}
		dfs(xBitsOfW8ThatAffectW17, allBitsOfW8ThatAffectW17, answer, start+1, result);
		return;
	}
	// cout << numFreeVar << endl;

	vector<int> cur(numFreeVar, 0);
	vector<int> allZeros(numFreeVar, 0);
	vector<int> answerCopy = answer;
	do {
		answer = answerCopy;
		int ind=0;
		int sumMod2 = 0;
		if (answer[start]==-1) {
			answer[start] = cur[ind];
			ind++;
		}
		for (int i = 0; i < xBitsOfW8ThatAffectW17[start].size(); ++i) {
			if (answer[xBitsOfW8ThatAffectW17[start][i]]==-1) {
				answer[xBitsOfW8ThatAffectW17[start][i]] = cur[ind];
				ind++;
			}
			if (ind==cur.size()) {
				break;
			}
		}
		sumMod2 = answer[start];
		for (int i = 0; i < xBitsOfW8ThatAffectW17[start].size(); ++i) {
			if (answer[xBitsOfW8ThatAffectW17[start][i]]!=-1) {
				sumMod2 = (sumMod2 + answer[xBitsOfW8ThatAffectW17[start][i]])%2;
			}
		}
		for (int i = 0; i < xBitsOfW8ThatAffectW17[start].size(); ++i) {
			if (answer[xBitsOfW8ThatAffectW17[start][i]]==-1) {
				answer[xBitsOfW8ThatAffectW17[start][i]] = (sumMod2 +1)%2;
			}
		}
		dfs(xBitsOfW8ThatAffectW17, allBitsOfW8ThatAffectW17, answer, start+1, result);
		addOneToVector(cur);
	} while (cur!=allZeros);
	return;

}

int main(int argc, char const *argv[]) {
	// Intitalizing sigma functions
	capSigma0 = getCapSig(2, 13, 22);
	capSigma1 = getCapSig(6, 11, 25);

	smallSigma0 = getSmallSig(7, 18, 3);
	smallSigma1 = getSmallSig(17, 19, 10);

	// Loading deltaW values 
	vector<int> crossPositionsInW7 = {0,2,3,4,5,7,9,10,12,13,15,17,20,21,23,24,25,27,29,30};
	vector<int> crossPositionsInW8 = {2,3,4,6,7,8,10,12,13,16,17,18,20,21,23,24,27,28,29,30,31};
	vector<int> crossPositionsInW12 = {5,9,10,11,15,16,17,19,21,22,26,27,28,29,30};
	vector<int> crossPositionsInW15 = {1,2,4,6,8,11,13,14,17,18,19,25,27,28,30,31};
	vector<int> crossPositionsInW17 = {2,3,4,6,7,8,10,12,13,16,17,18,20,21,23,24,27,28,29,30,31};

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

	// printWDifferential(wDifferential);
	vector<vector<int> > smallSig100 = sigMul(sigMul(smallSigma0, smallSigma0), smallSigma1);

	vector<int> W8 = convertNumCrossToNumVal(wDifferential[8]);
	vector<int> W15 = convertNumCrossToNumVal(wDifferential[15]);
	vector<int> allOnes (32, 1);
	vector<vector<int> > sig0AffectedBy = hasContribution(allOnes, smallSigma0);
	vector<vector<int> > sig1AffectedBy = hasContribution(allOnes, smallSigma1);
	
	vector<vector<int> > xBitsOfW8ThatAffectW17 = hasContribution(W8, smallSig100);
	vector<vector<int> > allBitsOfW8ThatAffectW17 = hasContribution(allOnes, smallSig100);

	for (int i = 0; i < 32; ++i) {
		if (xBitsOfW8ThatAffectW17[i].size()%2==1) {
			cout << "i = " << i << endl;
			for (int j = 0; j < xBitsOfW8ThatAffectW17[i].size(); ++j) {
				cout << xBitsOfW8ThatAffectW17[i][j] << " ";
			}
			cout << endl;
			for (int j = 0; j < allBitsOfW8ThatAffectW17[i].size(); ++j) {
				cout << allBitsOfW8ThatAffectW17[i][j] << " ";
			}
			cout << endl;
			cout << endl;
		}
	}

	vector<int> W7Fixed = getsFixedGivenX(smallSigma0, W8, wDifferential[7]);
	cout << "These are fixed W7: " << endl;
	for (int i = 0; i < W7Fixed.size(); ++i) {
		cout << W7Fixed[i] << " ";
	}
	cout << endl;


	vector<int> W7FixedInNum(32, 0);
	for (int i = 0; i < W7Fixed.size(); ++i) {
		W7FixedInNum[W7Fixed[i]] = 1;
	}
	vector<int> W15Fixed = getsFixedGivenX(smallSigma0, W7FixedInNum, wDifferential[15]);
	cout << "These are fixed W15: " << endl;
	for (int i = 0; i < W15Fixed.size(); ++i) {
		cout << W15Fixed[i] << " ";
	}
	cout << endl;
	// vector<vector<int> > W17Fixed = hasContribution(W7FixedInNum, smallSigma0);
	// vector<vector<int> > W17AllOne = hasContribution(allOnes, smallSigma0);
	// cout << "These are fixed W17: " << endl;
	// for (int i = 0; i < W17Fixed.size(); ++i) {
	// 	cout << W17Fixed[i].size() << " ";
	// }
	// cout << endl;
	// for (int i = 0; i < W17Fixed.size(); ++i) {
	// 	cout << W17AllOne[i].size() << " ";
	// }
	// cout << endl;

	// cout << " This is fixed set: " << endl;
	// for (int i = 0; i < 32; ++i) {
	// 	cout << "(" << i << ")" << " ";
	// 	for (int j = 0; j < W17Fixed[i].size(); ++j) {
	// 		cout << W17Fixed[i][j] << " ";
	// 	}
	// 	cout << endl;
	// }

	// cout << " This is all set: " << endl;
	// for (int i = 0; i < 32; ++i) {
	// 	cout << "(" << i << ")" << " ";
	// 	for (int j = 0; j < W17AllOne[i].size(); ++j) {
	// 		cout << W17AllOne[i][j] << " ";
	// 	}
	// 	cout << endl;
	// }






	vector<int> W15FixedInNum(32, 0);
	for (int i = 0; i < W15Fixed.size(); ++i) {
		W15FixedInNum[W15Fixed[i]] = 1;
	}
	vector<vector<int> > W17Fixed = hasContribution(W15FixedInNum, smallSigma1);
	vector<vector<int> > W17AllOne = hasContribution(allOnes, smallSigma1);
	cout << "These are fixed W17: " << endl;
	for (int i = 0; i < W17Fixed.size(); ++i) {
		cout << W17Fixed[i].size() << " ";
	}
	cout << endl;
	for (int i = 0; i < W17AllOne.size(); ++i) {
		cout << W17AllOne[i].size() << " ";
	}
	cout << endl;

	cout << " This is fixed set: " << endl;
	for (int i = 0; i < 32; ++i) {
		if (wDifferential[17][i]=='x') {
			cout << "(" << i << ")" << " ";
			for (int j = 0; j < W17Fixed[i].size(); ++j) {
				cout << W17Fixed[i][j] << " ";
			}
			cout << endl;
		}
	}

	cout << " This is all set: " << endl;
	for (int i = 0; i < 32; ++i) {
		if (wDifferential[17][i]=='x') {
			cout << "(" << i << ")" << " ";
			for (int j = 0; j < W17AllOne[i].size(); ++j) {
				cout << W17AllOne[i][j] << " ";
			}
			cout << endl;
		}
	}



	// vector<int> W7FixedInNum(32, 0);
	for (int i = 0; i < W7Fixed.size(); ++i) {
		W7FixedInNum[W7Fixed[i]] = 1;
	}
	vector<vector<int> > W15Fixed2 = hasContribution(W7FixedInNum, smallSigma0);
	vector<vector<int> > W15AllOne2 = hasContribution(allOnes, smallSigma0);
	cout << "These are fixed W15: " << endl;
	for (int i = 0; i < W15Fixed2.size(); ++i) {
		cout << W15Fixed2[i].size() << " ";
	}
	cout << endl;
	for (int i = 0; i < W15AllOne2.size(); ++i) {
		cout << W15AllOne2[i].size() << " ";
	}
	cout << endl;

	cout << " This is fixed set: " << endl;
	for (int i = 0; i < 32; ++i) {
		if (wDifferential[15][i]=='x') {
			cout << "(" << i << ")" << " ";
			for (int j = 0; j < W15Fixed2[i].size(); ++j) {
				cout << W15Fixed2[i][j] << " ";
			}
			cout << endl;
		}
	}

	cout << " This is all set: " << endl;
	for (int i = 0; i < 32; ++i) {
		if (wDifferential[15][i]=='x') {
			cout << "(" << i << ")" << " ";
			for (int j = 0; j < W15AllOne2[i].size(); ++j) {
				cout << W15AllOne2[i][j] << " ";
			}
			cout << endl;
		}
	}





	///

	vector<int> W8Fixed(32, 0);
	for (int i = 0; i < 32; ++i) {
		if (wDifferential[8][i]=='x') {
			W8Fixed.push_back(i);
		}
	}


	vector<int> W8FixedInNum(32, 0);
	for (int i = 0; i < W8Fixed.size(); ++i) {
		W8FixedInNum[W8Fixed[i]] = 1;
	}
	vector<vector<int> > W7Fixed2 = hasContribution(W8FixedInNum, smallSigma0);
	vector<vector<int> > W7AllOne2 = hasContribution(allOnes, smallSigma0);
	cout << "These are fixed W7: " << endl;
	for (int i = 0; i < W7Fixed2.size(); ++i) {
		cout << W7Fixed2[i].size() << " ";
	}
	cout << endl;
	for (int i = 0; i < W7AllOne2.size(); ++i) {
		cout << W7AllOne2[i].size() << " ";
	}
	cout << endl;

	cout << " This is fixed set: " << endl;
	for (int i = 0; i < 32; ++i) {
		if (wDifferential[7][i]=='x') {
			cout << "(" << i << ")" << " ";
			for (int j = 0; j < W7Fixed2[i].size(); ++j) {
				cout << W7Fixed2[i][j] << " ";
			}
			cout << endl;
		}
	}

	cout << " This is all set: " << endl;
	for (int i = 0; i < 32; ++i) {
		if (wDifferential[7][i]=='x') {
			cout << "(" << i << ")" << " ";
			for (int j = 0; j < W7AllOne2[i].size(); ++j) {
				cout << W7AllOne2[i][j] << " ";
			}
			cout << endl;
		}
	}



	//finding non trivial solutions
	// vector<vector<int> > results(0);
	// vector<int> answer (32, -1);
	// // dfs(xBitsOfW8ThatAffectW17, allBitsOfW8ThatAffectW17, answer, 0, results);
	// // cout << results.size() << endl;


	// // Ex = r
	// vector<int> r(32, 0);
	// vector<vector<int> > equations(32, vector<int> (32, 0));
	// for (int i = 0; i < 32; ++i) {
	// 	if (xBitsOfW8ThatAffectW17[i].size()%2==1) {
	// 		for (int j = 0; j < allBitsOfW8ThatAffectW17[i].size(); ++j) {
	// 			equations[i][allBitsOfW8ThatAffectW17[i][j]]=1;
	// 		}
	// 		equations[i][i] = (equations[i][i]+1)%2;
	// 		r[i] = 1;
	// 	}
	// }

	// cout << "Equations before:" << endl;
	// for (int i = 0; i < 32; ++i) {
	// 	for (int j = 0; j < 32; ++j) {
	// 		cout << equations[i][j] << " ";
	// 	}
	// 	cout << endl;
	// }

	// //find row echelon form
	// vector<int> newRow(32, 0);
	// for (int i = 0; i < 32; ++i) {
	// 	newRow[i] = i;
	// }

	// for (int i = 0; i < 32; ++i) {
	// 	if (equations[i][i]==1) {
	// 		for (int j = i+1; j < 32; ++j) {
	// 			if (equations[j][i]==1) {
	// 				for (int k = 0; k < 32; ++k) {
	// 					equations[j][k] = (equations[j][k] + equations[i][k])%2;
	// 				}
	// 			}
	// 		}
	// 	}
	// 	else {
	// 		bool dealt = false;
	// 		for (int j = i+1; j < 32; ++j) {
	// 			if (equations[j][i]==1) {
	// 				//swap i and j row
	// 				vector<int> temp(32, 0);
	// 				for (int k = 0; k < 32; ++k) {
	// 					temp[k] = equations[j][k];
	// 				}
	// 				for (int k = 0; k < 32; ++k) {
	// 					equations[j][k] = equations[i][k];
	// 				}
	// 				for (int k = 0; k < 32; ++k) {
	// 					equations[i][k] = temp[k];
	// 				}
	// 				for (int k = i+1; k < 32; ++k) {
	// 					if (equations[k][i]==1) {
	// 						for (int q = 0; q < 32; ++q) {
	// 							equations[k][q] = (equations[k][q] + equations[i][q])%2;
	// 						}
	// 					}
	// 				}
	// 				dealt = true;
	// 				break;
	// 			}
	// 		}
	// 		// if (!dealt) {
				
	// 		// }
	// 	}
	// }

	// cout << "Equations afterwards:" << endl;
	// for (int i = 0; i < 32; ++i) {
	// 	for (int j = 0; j < 32; ++j) {
	// 		cout << equations[i][j] << " ";
	// 	}
	// 	cout << endl;
	// }

	// for (int i = 0; i < 32; ++i) {
	// 	if (W17.size()%2==1) {
	// 		cout << "i = " << i << endl;
	// 		vector<int> iAffectedBy (32, 0);
	// 		for (int j = 0; j < W17.size(); ++j) {
	// 			for (int k = 0; k < sig0AffectedBy[W17[j]].size(); ++k)
	// 			{
	// 				/* code */
	// 			}
	// 		}
	// 	}
	// }

	// vector<int> allOnes (32, 1);
	// cout << "all bits that affect W17 in W15" << endl;
	// W17 = matMulNoMod(allOnes, smallSigma1);

	// for (int i = 0; i < 32; ++i) {
	// 	cout << W8[i];
	// }
	// cout << endl;
	// for (int i = 0; i < 32; ++i) {
	// 	cout << W17[i];
	// }
	// cout << endl;



	return 0;
}