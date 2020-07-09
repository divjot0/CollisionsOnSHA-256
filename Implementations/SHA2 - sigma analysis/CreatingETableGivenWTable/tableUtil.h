#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;


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

long long convertNumValToNum(vector<int> &numVal) {
	long long ans=0;
	long long p2 = 1;
	for (int i = 0; i < numVal.size(); ++i) {
		if (numVal[i]==1) {
			ans = ans + p2;
		}
		p2 = p2*2;
	}
	return ans;
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

vector<vector<int> > hasContribution(vector<int> numVal, vector<vector<int> > &sigma) {
	vector<vector<int> > hasContribution(32, vector<int> ());
	vector<int> resVal(sigma[0].size(), 0);
	for (int i = 0; i < numVal.size(); ++i) {
		if (numVal[i]==1) {
			for (int j = 0; j < sigma[0].size(); ++j) {
				resVal[j] = (resVal[j] + sigma[i][j]);
				if (sigma[i][j]==1) {
					hasContribution[j].push_back(i);
				}
			}
		}
	}
	return hasContribution;
	// for (int i = 0; i < hasContribution.size(); ++i) {
	// 	for (int j = 0; j < hasContribution[i].size(); ++j) {
	// 		cout << hasContribution[i][j] << " ";
	// 	}
	// 	cout << endl;
	// }


	// vector<vector<int> > hasContributionSig1 = vector<vector<int> > (32, vector<int> ());
	// vector<int> allOnes (32, 1);
	// cout << "all bits that affect W17 in W15" << endl;
	// numVal = allOnes;
	// resVal = vector<int> (sigma[0].size(), 0);
	// for (int i = 0; i < numVal.size(); ++i) {
	// 	if (numVal[i]==1) {
	// 		for (int j = 0; j < sigma[0].size(); ++j) {
	// 			resVal[j] = (resVal[j] + sigma[i][j]);
	// 			if (sigma[i][j]==1) {
	// 				hasContributionSig1[j].push_back(i);
	// 			}
	// 		}
	// 	}
	// }
	// for (int i = 0; i < hasContributionSig1.size(); ++i) {
	// 	if (hasContribution[i].size()%2==1) {
	// 		cout << "(" << i << ")" << endl;
	// 		for (int j = 0; j < hasContribution[i].size(); ++j) {
	// 			cout << hasContribution[i][j] << " ";
	// 		}
	// 		cout << endl;
	// 		for (int j = 0; j < hasContribution1[i].size(); ++j) {
	// 			cout << hasContribution1[i][j] << " ";
	// 		}
	// 		cout << endl;
	// 		cout << endl;
	// 	}
	// }

	// vector<vector<int> > hasContributionSig0 = vector<vector<int> > (32, vector<int> ());
	// allOnes = vector<int> (32, 1);
	// // cout << "all bits that affect W17 in W15" << endl;
	// numVal = allOnes;
	// resVal = vector<int> (sigma[0].size(), 0);
	// for (int i = 0; i < numVal.size(); ++i) {
	// 	if (numVal[i]==1) {
	// 		for (int j = 0; j < sigma[0].size(); ++j) {
	// 			resVal[j] = (resVal[j] + sigma[i][j]);
	// 			if (sigma[i][j]==1) {
	// 				hasContributionSig0[j].push_back(i);
	// 			}
	// 		}
	// 	}
	// }
	// for (int i = 0; i < hasContributionSig0.size(); ++i) {
	// 	if (hasContribution[i].size()%2==1) {
	// 		cout << "(" << i << ")" << endl;
	// 		for (int j = 0; j < hasContribution[i].size(); ++j) {
	// 			cout << hasContribution[i][j] << " ";
	// 		}
	// 		cout << endl;
	// 		for (int j = 0; j < hasContribution1[i].size(); ++j) {
	// 			cout << hasContribution1[i][j] << " ";
	// 		}
	// 		cout << endl;
	// 		cout << endl;
	// 	}
	// }

	// return resVal;
}

vector<vector<int> > sigMul(vector<vector<int> > a, vector<vector<int> > b) {
	vector<vector<int> > res(32, vector<int> (32, 0));
	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
			for (int k = 0; k < 32; ++k) {
				res[i][j] = (res[i][j] + a[i][k]*b[k][j])%2;
			}
		}
	}
	return res;
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

vector<int> getsFixedGivenX(vector<vector<int> > sigma, vector<int> w8, vector<char> wDiff7) {
	vector<int> res(0);
	vector<vector<int> > hasContribution(32, vector<int> (0));
	vector<vector<int> > hasContribution2(32, vector<int> (0));
	for (int i = 0; i < 32; ++i) {
		if (w8[i]==1) {
			for (int j = 0; j < 32; ++j) {
				if (sigma[i][j]==1) {
					hasContribution[j].push_back(i);
				}
			}
		}
		for (int j = 0; j < 32; ++j) {
			if (sigma[i][j]==1) {
				hasContribution2[j].push_back(i);
			}
		}
	}

	for (int i = 0; i < 32; ++i) {
		if (hasContribution[i].size()==hasContribution2[i].size()) {
			if (wDiff7[i]=='x') {
				res.push_back(i);
			}
		}
	}
	return res;
}