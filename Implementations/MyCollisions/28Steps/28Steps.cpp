#include "SHA2Util28.h"

using namespace std;

// send a x diff and get back a complete diff with same x diff
void setRandomComplete(vector<char> &v) {
	for (int i = 0; i < v.size(); ++i) {
		if (v[i]=='x') {
			if (rand()%2==0) {
				v[i]='u';
			}
			else {
				v[i]='n';
			}
		}
	}
	return;
}

// send goodDiff for better results
vector<char> getRandomCrossCanceler(vector<char> goodV) {
	vector<char> res(32, '-');
	for (int i = 0; i < 32; ++i) {
		if (!isDifferent(goodV[i])) {
			continue;
		}
		else {
			int lengthLimit = 1;
			for (int j = i+1; j < 32; ++j) {
				if (!isDifferent(goodV[j])) {
					lengthLimit++;
				}
				else {
					break;
				}
			}
			int lengthOfResCross = rand()%lengthLimit+1;
			for (int j = 0; j < lengthOfResCross; ++j) {
				res[i+j] = 'x';
			}
		}
	}
	return res;
}

// send in complete differential (true/partially-true does not matter)
// returns \nabla a such that a + diff = 0
vector<char> getRandomCompleteCanceller(vector<char> diff) {
	vector<char> goodDiff = getGoodDiff(diff);
	vector<char> resStructure = getRandomCrossCanceler(goodDiff);

	vector<vector<int> > lList = getLListForCut(resStructure, goodDiff);

	vector<char> cancelWith = goodDiff;
	vector<char> finalRes(32, '-');

	for (int i = 0; i < lList.size(); ++i) {
		char current;
		if (cancelWith[lList[i][0]]=='n') {
			current = 'u';
		}
		else {
			current = 'n';
		}
		for (int j = lList[i][0]; j < lList[i][1]; ++j) {
			if (isDifferent(resStructure[j])) {
				finalRes[j] = cancelWith[lList[i][0]];
			}
			else {
				if (cancelWith[j]!=cancelWith[lList[i][0]]) {
					cout << "SOMETHING WRONG!" << endl;
					return finalRes;
				}
			}
		}
		if (isDifferent(resStructure[lList[i][1]])) {
			finalRes[lList[i][1]] = current;
		}
		else {
			if (cancelWith[lList[i][1]]!=current) {
				cout << "SOMETHING WRONG!" << endl;
				return finalRes;
			}
		}
	}
	if (isDifferent(finalRes[31])) {
		if (rand()%2==0) {
			finalRes[31]='u';
		}
		else {
			finalRes[31]='n';
		}
	}
	return finalRes;
}

vector<char> replaceXWithFixedRandomly(vector<char> differential) {
	unsigned int state = getRandomUnsignedInt();
	vector<int> stateInBinary = convertIntToVector(state);
	int posState = 0;
	vector<char> res(32, '9');
	for (int i = 0; i < 32; ++i) {
		if (differential[i]=='-') {
			res[i]='-';
		}
		else if (differential[i]=='1') {
			res[i]='1';
		}
		else if (differential[i]=='0') {
			res[i]='0';
		}
		else if (differential[i]=='u') {
			res[i]='u';
		}
		else if (differential[i]=='n') {
			res[i]='n';
		}
		else if (differential[i]=='x') {
			if (stateInBinary[posState]==1) {
				res[i]='u';
				posState++;
			}
			else {
				res[i]='n';
				posState++;
			}
		}
		else {
			cout << "SOMETHING WRONG!" << endl;
		}
	}
	return res;
}

// send true e7
vector<char> getRandomSigmaResultWithAllDash(vector<vector<int> > SIGMA_FUNCTION, vector<char>& e7, string SIGMA_FUNCTION_STRING) {

	vector<char> res(32, '-');

	int countIterations = 0;

	bool found = false;

	while(!found) {
		vector<char> a8Differential = e7;
		vector<char> sigA8Differential = calculateFromCross(SIGMA_FUNCTION, e7);
		vector<char> randVec = replaceXWithFixedRandomly(sigA8Differential);
		vector<char> e9Differential = randVec;
		vector<vector<int> > lList = getLListForCut(sigA8Differential, e9Differential);
		vector<char> pureSigDifferential(32, '-');
		for (int i = 0; i < lList.size(); ++i) {
			char current;
			if (e9Differential[lList[i][0]]=='n') {
				current = 'u';
			}
			else {
				current = 'n';
			}
			for (int j = lList[i][0]; j < lList[i][1]; ++j) {
				if (isDifferent(sigA8Differential[j])) {
					pureSigDifferential[j] = e9Differential[lList[i][0]];
				}
				else {
					// if (e9Differential[j]!=e9Differential[lList[i][0]]) {
					// 	continue;
					// }
					if (e9Differential[lList[i][0]]=='u') {
						e9Differential[j] = 'n';
					}
					else if (e9Differential[lList[i][0]]=='n') {
						e9Differential[j] = 'u';
					}
				}
			}
			if (isDifferent(sigA8Differential[lList[i][1]])) {
				pureSigDifferential[lList[i][1]] = current;
			}
			else {
				// if (e9Differential[lList[i][1]]!=current) {
				// 	continue;
				// }
				e9Differential[lList[i][1]]=current;
			}
		}
		
		// get A8 differential which gets pureSigDifferential
		vector<vector<int> > sig0CausingBits = getCausingBits(SIGMA_FUNCTION_STRING);

		Conditions conditions;
		for (int i = 0; i < 32; ++i) {
			if (!isDifferent(a8Differential[i])) {
				conditions.addParticularVariable(to_string(i), i, i);
			}
		}

		for (int i = 0; i < 32; ++i) {
			if (isDifferent(pureSigDifferential[i])) {
				int count = 0;
				int countU = 0;
				int countV = 0;
				char diffType;
				vector<int> relationVariables;
				for (int j = 0; j < sig0CausingBits[i].size(); ++j) {
					if (isDifferent(a8Differential[sig0CausingBits[i][j]])) {
						count++;
						diffType = a8Differential[sig0CausingBits[i][j]];
					}
					else {
						relationVariables.push_back(sig0CausingBits[i][j]);
					}
				}
				if (count==1) {
					if (diffType == pureSigDifferential[i]) {
						conditions.setEqual(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
					}
					else {
						conditions.setDifferent(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
					}
				}
				if (count==3) {
					if (a8Differential[sig0CausingBits[i][0]]=='u') {
						countU++;
					}
					if (a8Differential[sig0CausingBits[i][1]]=='u') {
						countU++;
					}
					if (a8Differential[sig0CausingBits[i][2]]=='u') {
						countU++;
					}
					if (countU==1 || countU==3) {
						if (pureSigDifferential[i]!='u') {
							countIterations++;
							continue;
						}
						// pureSigDifferential[i]='u';
					}
					else {
						if (pureSigDifferential[i]!='n') {
							countIterations++;
							continue;
						}
						// pureSigDifferential[i]='n';
					}
				}
			}
		}

		if (!conditions.areSatisfiable()) {
			countIterations++;
			// cout << "here" << endl;
			continue;
		}

		setValidAssignment(a8Differential, conditions);
		found = true;
		res = pureSigDifferential;
		e7 = a8Differential;
	}

	cout << "countIterations = " << countIterations << endl;

	return res;
}

// y and z should not have x bits
vector<char> getRandomIfResult(vector<char>& x, vector<char>& y, vector<char>& z) {
	vector<char> res(32, '-');

	for (int i = 0; i < 32; ++i) {
		if (!isDifferent(x[i])) {
			if (x[i]=='0') {
				res[i] = z[i];
			}
			else if (x[i]=='1') {
				res[i] = y[i];
			}
			else if (x[i]=='-') {
				if (isDifferent(y[i]) && isDifferent(z[i])) {
					if (y[i]==z[i]) {
						res[i]=y[i];
					}
					else {
						int temp = rand()%2;
						if (temp==0) {
							x[i] = '0';
							res[i] = z[i];
						}
						else {
							x[i] = '1';
							res[i] = y[i];
						}
					}
				}
				else if (!isDifferent(y[i]) && !isDifferent(z[i])) {
					res[i] = '-';
				}
				else {
					int temp = rand()%2;
					if (temp==0) {
						x[i] = '0';
						if (isDifferent(z[i])) {
							res[i] = z[i];
						}
						else {
							res[i] = '-';
						}
					}
					else {
						x[i] = '1';
						if (isDifferent(y[i])) {
							res[i] = y[i];
						}
						else {
							res[i] = '-';
						}
					}
				}
			}
		}
		else {
			// cout << "hi" << endl;
			char* first;
			char* second;
			if (x[i]=='u') {
				first = &y[i];
				second = &z[i];
			}
			else if (x[i]=='n') {
				first = &z[i];
				second = &y[i];
			}
			if (!isDifferent(*first) && !isDifferent(*second)) {
				if (!isFullyDetermined(*second)) {
					if (rand()%2==0) {
						*second = '0';
					}
					else {
						*second = '1';
					}
				}
				if (!isFullyDetermined(*first)) {
					if (rand()%2==0) {
						*first = '0';
					}
					else {
						*first = '1';
					}
				}
				if (*first=='0' && *second=='1') {
					res[i]='n';
				}
				else if (*first=='1' && *second=='0') {
					res[i]='u';
				}
				else if (*first==*second) {
					res[i]='-';
				}
			}
			else if (isDifferent(*first) && isDifferent(*second)) {
				if (*first==*second) {
					res[i] = *first;
				}
				else if (*first!=*second) {
					res[i] = '-';
				}
			}
			else if (isDifferent(*first) && !isDifferent(*second)) {
				if (!isFullyDetermined(*second)) {
					if (rand()%2==0) {
						*second = '0';
					}
					else {
						*second = '1';
					}
				}
				if (*first=='u' && *second=='1') {
					res[i]='-';
				}
				else if (*first=='u' && *second=='0') {
					res[i]='u';
				}
				else if (*first=='n' && *second=='1') {
					res[i]='n';
				}
				else if (*first=='n' && *second=='0') {
					res[i]='-';
				}
			}
			else if (!isDifferent(*first) && isDifferent(*second)) {
				if (!isFullyDetermined(*first)) {
					if (rand()%2==0) {
						*first = '0';
					}
					else {
						*first = '1';
					}
				}
				if (*first=='0' && *second=='u') {
					res[i]='-';
				}
				else if (*first=='1' && *second=='u') {
					res[i]='u';
				}
				else if (*first=='0' && *second=='n') {
					res[i]='n';
				}
				else if (*first=='1' && *second=='n') {
					res[i]='-';
				}
			}
		}
	}
	
	return res;
}

// input is proper differentials
// returns good differential
vector<char> addDifferentials(vector<char> diff_0, vector<char> diff_1) {
	vector<char> res(32, '-');
	// get the sum of the two differentials

	char carry = '-';
	for (int i = 0; i < 32; ++i) {
		if (!isDifferent(diff_0[i]) && !isDifferent(diff_1[i])) {
			if (isDifferent(carry)) {
				res[i] = carry;
				carry = '-';
			}
			else {
				res[i] = '-';
				carry = '-';
			}
		}
		else if (isDifferent(diff_0[i]) && !isDifferent(diff_1[i])) {
			if (isDifferent(carry)) {
				if (carry==diff_0[i]) {
					res[i] = '-';
					carry = diff_0[i];
				}
				else {
					res[i] = '-';
					carry = '-';
				}
			}
			else {
				res[i] = diff_0[i];
				carry = '-';
			}
		}
		else if (!isDifferent(diff_0[i]) && isDifferent(diff_1[i])) {
			if (isDifferent(carry)) {
				if (carry==diff_1[i]) {
					res[i] = '-';
					carry = diff_1[i];
				}
				else {
					res[i] = '-';
					carry = '-';
				}
			}
			else {
				res[i] = diff_1[i];
				carry = '-';
			}
		}
		else if (isDifferent(diff_0[i]) && isDifferent(diff_1[i])) {
			if (isDifferent(carry)) {
				if (carry==diff_0[i] && carry==diff_1[i]) {
					res[i] = diff_1[i];
					carry = diff_1[i];
				}
				else if (carry==diff_0[i] && carry!=diff_1[i]) {
					res[i] = diff_0[i];
					carry = '-';
				}
				else if (carry!=diff_0[i] && carry==diff_1[i]) {
					res[i] = diff_1[i];
					carry = '-';
				}
				else if (carry!=diff_0[i] && carry!=diff_1[i]) {
					res[i] = diff_1[i];
					carry = '-';
				}
			}
			else {
				if (diff_0[i]==diff_1[i]) {
					res[i] = '-';
					carry = diff_0[i];
				}
				else if (diff_0[i]!=diff_1[i]) {
					res[i] = '-';
					carry = '-';
				}
			}
		}
	}
	res = getGoodDiff(res);
	return res;
}

// send true values to getRandomX functions
// if_result_e7 is the result of if(e7, e6, e5)
// we also set values of '-' bits of e6 and e5 at some places
vector<char> getRandomE8GivenE7W8(vector<char> &e7, vector<char> &e6, vector<char> &e5, vector<char> w8) {
	vector<char> res(32, 'e');
	vector<char> cap_sig_1_e7 = getRandomSigmaResultWithAllDash(CAP_SIGMA_1, e7, "CAP_SIGMA_1");
	cout << "cap_sig_1_e7: "; printSingleLine(cap_sig_1_e7);
	vector<char> if_result_e7 = getRandomIfResult(e7, e6, e5);
	cout << "if_result_e7: "; printSingleLine(if_result_e7);
	vector<char> w8_diff_value = w8;
	res = addDifferentials(cap_sig_1_e7, if_result_e7);
	res = addDifferentials(res, w8_diff_value);
	vector<char> e8 = getRandomCompleteCanceller(getOppositeDifferential(res));
	cout << "e8: "; printSingleLine(e8);
	cout << "e7: "; printSingleLine(e7);
	cout << "e6: "; printSingleLine(e6);
	cout << "e5: "; printSingleLine(e5);
	return e8;
}

vector<char> getRandomE9GivenE7E8(vector<char> &e8, vector<char> &e7, vector<char> &e6) {
	vector<char> res(32, 'e');
	vector<char> cap_sig_1_e8 = getRandomSigmaResultWithAllDash(CAP_SIGMA_1, e8, "CAP_SIGMA_1");
	cout << "cap_sig_1_e8: "; printSingleLine(cap_sig_1_e8);
	vector<char> if_result_e8 = getRandomIfResult(e8, e7, e6);
	cout << "if_result_e8: "; printSingleLine(if_result_e8);
	res = addDifferentials(cap_sig_1_e8, if_result_e8);
	vector<char> e9 = getRandomCompleteCanceller(getOppositeDifferential(res));
	cout << "e9: "; printSingleLine(e9);
	cout << "e8: "; printSingleLine(e8);
	cout << "e7: "; printSingleLine(e7);
	cout << "e6: "; printSingleLine(e6);
	return e9;
}

vector<char> getRandomE10GivenE7E8E9(vector<char> &e9, vector<char> &e8, vector<char> &e7) {
	vector<char> res(32, 'e');
	vector<char> cap_sig_1_e9 = getRandomSigmaResultWithAllDash(CAP_SIGMA_1, e9, "CAP_SIGMA_1");
	cout << "cap_sig_1_e9: "; printSingleLine(cap_sig_1_e9);
	vector<char> if_result_e9 = getRandomIfResult(e9, e8, e7);
	cout << "if_result_e9: "; printSingleLine(if_result_e9);
	res = addDifferentials(cap_sig_1_e9, if_result_e9);
	vector<char> e10 = getRandomCompleteCanceller(getOppositeDifferential(res));
	cout << "e10: "; printSingleLine(e10);
	cout << "e9: "; printSingleLine(e9);
	cout << "e8: "; printSingleLine(e8);
	cout << "e7: "; printSingleLine(e7);
	return e10;
}

vector<char> getRandomE11GivenE7E8E9E10(vector<char> &e10, vector<char> &e9, vector<char> &e8, vector<char> &e7) {
	vector<char> res(32, 'e');
	vector<char> cap_sig_1_e10 = getRandomSigmaResultWithAllDash(CAP_SIGMA_1, e10, "CAP_SIGMA_1");
	cout << "cap_sig_1_e10: "; printSingleLine(cap_sig_1_e10);
	vector<char> if_result_e10 = getRandomIfResult(e10, e9, e8);
	cout << "if_result_e10: "; printSingleLine(if_result_e10);
	res = addDifferentials(cap_sig_1_e10, if_result_e10);
	res = addDifferentials(res, e7);
	res = addDifferentials(res, e7);
	vector<char> e11 = getRandomCompleteCanceller(getOppositeDifferential(res));
	cout << "e11: "; printSingleLine(e11);
	cout << "e10: "; printSingleLine(e10);
	cout << "e9: "; printSingleLine(e9);
	cout << "e8: "; printSingleLine(e8);
	cout << "e7: "; printSingleLine(e7);
	return e11;
}

vector<char> getRandomE12GivenE8E9E10E11W12(vector<char> &e11, vector<char> &e10, vector<char> &e9, vector<char> &e8, vector<char> &w12) {
	vector<char> res(32, 'e');
	vector<char> cap_sig_1_e11 = getRandomSigmaResultWithAllDash(CAP_SIGMA_1, e11, "CAP_SIGMA_1");
	cout << "cap_sig_1_e11: "; printSingleLine(cap_sig_1_e11);
	vector<char> if_result_e11 = getRandomIfResult(e11, e10, e9);
	cout << "if_result_e11: "; printSingleLine(if_result_e11);
	res = addDifferentials(cap_sig_1_e11, if_result_e11);
	res = addDifferentials(res, e8);
	res = addDifferentials(res, w12);
	vector<char> e12 = getRandomCompleteCanceller(getOppositeDifferential(res));
	cout << "e12: "; printSingleLine(e12);
	cout << "e11: "; printSingleLine(e11);
	cout << "e10: "; printSingleLine(e10);
	cout << "e9: "; printSingleLine(e9);
	cout << "e8: "; printSingleLine(e8);
	// cout << "e7: "; printSingleLine(e7);
	return e12;
}


// to be written -> get random if value and check if Sigma can be generated 
vector<char> getRandomCheck1GivenE9E10E11E12(vector<char> &e12, vector<char> &e11, vector<char> &e10, vector<char> &e9) {
	vector<char> res(32, 'e');
	vector<char> cap_sig_1_e12 = getRandomSigmaResultWithAllDash(CAP_SIGMA_1, e12, "CAP_SIGMA_1");
	cout << "cap_sig_1_e12: "; printSingleLine(cap_sig_1_e12);
	vector<char> if_result_e12 = getRandomIfResult(e12, e11, e10);
	cout << "if_result_e12: "; printSingleLine(if_result_e12);
	res = addDifferentials(cap_sig_1_e12, if_result_e12);
	res = addDifferentials(res, e9);
	vector<char> check = getRandomCompleteCanceller(getOppositeDifferential(res));
	cout << "check: "; printSingleLine(check);
	// cout << "e11: "; printSingleLine(e11);
	// cout << "e10: "; printSingleLine(e10);
	// cout << "e9: "; printSingleLine(e9);
	// cout << "e8: "; printSingleLine(e8);
	// cout << "e7: "; printSingleLine(e7);
	return check;
}

vector<char> getRandomE7GivenW7(vector<char> w7) {
	vector<char> w7Canceller = getOppositeDifferential(w7);
	vector<char> res = getRandomCompleteCanceller(w7Canceller);
	return res;
}

bool hasZeroDifference(vector<char> v) {
	for (int i = 0; i < v.size(); ++i) {
		if (isDifferent(v[i])) {
			return false;
		}
	}
	return true;
}

int main(int argc, char const *argv[]) {
	bool found = false;
	srand(time(0));
	// we are goimg tp use this list as reference
	vector<vector<vector<char> > > possibleWDifferentials;	

	// while(!found) {
	// 	int badPoints=0;
	// 	unsigned int w8Num = getRandomUnsignedInt();
	// 	vector<int> w8NumVec = convertIntToVector(w8Num);
	// 	vector<char> w8Vec(32, '-');
	// 	for (int i = 0; i < 32; ++i) {
	// 		if (w8NumVec[i]==1) {
	// 			w8Vec[i]='x';
	// 		}
	// 	}
		// vector<char> w8Sig0 = calculateFromCross(SMALL_SIGMA_0, w8Vec);
		// vector<char> w8Sig00 = calculateFromCross(SMALL_SIGMA_0, w8Sig0);
		// vector<char> w8Sig100 = calculateFromCross(SMALL_SIGMA_1, w8Sig00);
		// if (canCancel(w8Sig100, w8Vec)!=-1 && hammingWeight(w8Vec)+hammingWeight(w8Sig0)+hammingWeight(w8Sig00)+hammingWeight(w8Sig100)<60) {
		// 	cout << w8Num << " " << hammingWeight(w8Vec) << " " << hammingWeight(w8Sig0) << " " << hammingWeight(w8Sig00) << " " << hammingWeight(w8Sig100) << endl;
		// }
		// fillPossibleW8BeginningWith(w8Vec, possibleWDifferentials);

		// we assume A8 = 0, because that makes solving other differentials easy. Maj becomes fully under control


		while(true) {
			unsigned int w8Num = getRandomUnsignedInt();
			vector<int> w8NumVec = convertIntToVector(w8Num);
			vector<char> w8Vec(32, '-');
			for (int i = 0; i < 32; ++i) {
				if (w8NumVec[i]==1) {
					w8Vec[i]='u';
				}
			}
			vector<char> w7 = w8Vec;
			vector<char> w8 = w7;
			vector<char> w12 = w7;

			setRandomComplete(w7);
			vector<char> e5(32, '-');
			vector<char> e6(32, '-');
			vector<char> e7 = getRandomE7GivenW7(w7);
			vector<char> e8 = getRandomE8GivenE7W8(e7, e6, e5, w8);
			vector<char> e9 = getRandomE9GivenE7E8(e8, e7, e6);
			vector<char> e10 = getRandomE10GivenE7E8E9(e9, e8, e7);
			vector<char> e11 = getRandomE11GivenE7E8E9E10(e10, e9, e8, e7);
			vector<char> e12 = getRandomE12GivenE8E9E10E11W12(e11, e10, e9, e8, w12);
			cout << "here" << endl;
			for (int i = 0; i < 100; ++i) {
				// make check better
				vector<char> check1 = getRandomCheck1GivenE9E10E11E12(e12, e11, e10, e9);
				if (hasZeroDifference(check1)) {
					cout << "Found " << endl;
					return 0;
				}
			}
			// vector<char> e7 = getRandomCompleteCanceller(w7);
			// printSingleLine(w7);
			// printSingleLine(e7);
			// printSingleLine(addDifferentials(getOppositeDifferential(e7), w7));
			cout << "------------" << endl;
		}
	// }
	return 0;
}