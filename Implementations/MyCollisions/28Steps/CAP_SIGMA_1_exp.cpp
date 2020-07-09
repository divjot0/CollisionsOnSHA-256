#include "SHA2Util28.h"

using namespace std;

void setRandomComplete(vector<char> &v) {
	for (int i = 0; i < v.size(); ++i) {
		if (v[i]=='x') {
			if (rand()%2==0) {
				v[i]='n';
			}
			else {
				v[i]='n';
			}
		}
	}
	return;
}

bool findACancelB(vector<char>& aVec, vector<char> bVec) {
	vector<char> sigADifferential = calculateFromCross(CAP_SIGMA_1, aVec);
	vector<char> cancelWith = bVec;
	vector<vector<int> > lList = getLListForCut(sigADifferential, cancelWith);
	// cout << lList.size() << endl;
	// printSingleLine(cancelWith);
	// if (!checking) {
		// cout << "THis is lList " << endl;
		// for (int i = 0; i < lList.size(); ++i) {
		// 	cout << lList[i][0] << " " << lList[i][1] << endl;
		// }
	// }
	// ls need to get canceled with E8
	vector<char> pureSigDifferential(32, '-');
	// vector<char> e8Differential = getOppositeDifferential(a7Differential);
	for (int i = 0; i < lList.size(); ++i) {
		char current;
		if (cancelWith[lList[i][0]]=='n') {
			current = 'u';
		}
		else {
			current = 'n';
		}
		for (int j = lList[i][0]; j < lList[i][1]; ++j) {
			if (isDifferent(sigADifferential[j])) {
				pureSigDifferential[j] = cancelWith[lList[i][0]];
			}
			else {
				if (cancelWith[j]!=cancelWith[lList[i][0]]) {
					// cout << i << "/" << lList.size() << endl;
					return false;
				}
			}
		}
		if (isDifferent(sigADifferential[lList[i][1]])) {
			pureSigDifferential[lList[i][1]] = current;
		}
		else {
			if (cancelWith[lList[i][1]]!=current) {
				// cout << i << endl;
				return false;
			}
		}
	}

	// get A7 differential which gets pureSigDifferential
	vector<vector<int> > sig0CausingBits = getCausingBits("CAP_SIGMA_1");

	Conditions conditions;
	for (int i = 0; i < 32; ++i) {
		if (!isDifferent(aVec[i])) {
			conditions.addParticularVariable(to_string(i), i, i);
		}
	}

	vector<int> alreadyFixed;

	bool newReturn = false;

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(pureSigDifferential[i])) {
			int count = 0;
			int countU = 0;
			int countV = 0;
			char diffType;
			vector<int> relationVariables;
			for (int j = 0; j < sig0CausingBits[i].size(); ++j) {
				if (isDifferent(aVec[sig0CausingBits[i][j]])) {
					count++;
					diffType = aVec[sig0CausingBits[i][j]];
				}
				else {
					relationVariables.push_back(sig0CausingBits[i][j]);
				}
			}
			if (count==1 && relationVariables.size()==2) {
				if (diffType == pureSigDifferential[i]) {
					conditions.setEqual(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
				else {
					conditions.setDifferent(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
			}
			else if (count==1 && relationVariables.size()==1) {
				// cout << "Hi " << endl;
				// cout << "Hi " << relationVariables[0] << " " << i << endl;
				if (find(alreadyFixed.begin(), alreadyFixed.end(), relationVariables[0])==alreadyFixed.end()) {
					if (diffType==pureSigDifferential[i]) {
						aVec[relationVariables[0]]='0';
					}
					else {
						aVec[relationVariables[0]]='1';
					}
					alreadyFixed.push_back(relationVariables[0]);
				}
				else {
					if (diffType==pureSigDifferential[i]) {
						if (aVec[relationVariables[0]]=='1') {
							newReturn=true;
							// return false;
						}
					}
					else {
						if (aVec[relationVariables[0]]=='0') {
							newReturn=true;
							// return false;
						}
					}
				}
			}
			if (count==3) {
				if (aVec[sig0CausingBits[i][0]]=='u') {
					countU++;
				}
				if (aVec[sig0CausingBits[i][1]]=='u') {
					countU++;
				}
				if (aVec[sig0CausingBits[i][2]]=='u') {
					countU++;
				}
				if (countU==1 || countU==3) {
					if (pureSigDifferential[i]!='u') {
						return false;
					}
				}
				else {
					if (pureSigDifferential[i]!='n') {
						return false;
					}
				}
			}
		}
	}
	// cout << "here" << endl;

	if (!conditions.areSatisfiable()) {
		return false;
	}

	cout << "Conditions are isSatisfiable " << endl;

	if (newReturn) {
		return false;
	}

	if (!setSmallValidAssignment(aVec, conditions, alreadyFixed)) {
		return false;
	}

	return true;
}

bool makeAAltAppt(vector<char> &aAltVec, vector<char> aVec) {
	vector<char> cancelWith = getOppositeDifferential(aVec);
	vector<char> w15Differential = aAltVec;
	vector<vector<int> > lList = getLListForCut(w15Differential, cancelWith);
	// if (!checking) {
	// 	cout << "THis is lList " << endl;
	// 	for (int i = 0; i < lList.size(); ++i) {
	// 		cout << lList[i][0] << " " << lList[i][1] << endl;
	// 	}
	// }
	// ls need to get canceled with E8
	vector<char> pureSigDifferential(32, '-');
	// vector<char> e8Differential = getOppositeDifferential(a7Differential);
	for (int i = 0; i < lList.size(); ++i) {
		char current;
		if (cancelWith[lList[i][0]]=='n') {
			current = 'u';
		}
		else {
			current = 'n';
		}
		for (int j = lList[i][0]; j < lList[i][1]; ++j) {
			if (isDifferent(w15Differential[j])) {
				pureSigDifferential[j] = cancelWith[lList[i][0]];
			}
			else {
				if (cancelWith[j]!=cancelWith[lList[i][0]]) {
					return false;
				}
			}
		}
		if (isDifferent(w15Differential[lList[i][1]])) {
			pureSigDifferential[lList[i][1]] = current;
		}
		else {
			if (cancelWith[lList[i][1]]!=current) {
				return false;
			}
		}
	}

	w15Differential = pureSigDifferential;
	aAltVec = w15Differential;
	return true;
}

vector<char> inputDifferential() {
	string a;
	cout << "input string: ";
	cin >> a;
	vector<char> res(32, '0');
	for (int i = 0; i < 32; ++i) {
		res[i] = a[i];
	}
	return res;
}

int main(int argc, char const *argv[]) {
	float totalTrials = 0;
	int total = 40;
	srand(time(0));
	cout << "This is the first random number : " << rand() << endl;
	for (int ik = 0; ik < total; ++ik) {
		// continue;
		cout << "total number : " << ik << endl;
		float trials = 0;
		float success = 0;
		bool found = false;
		unsigned int AFound;
		unsigned int BFound;
		while(!found) {
			if ((int)trials%100==0) {
				cout << "trial number : " << trials << endl;
			}
			trials++;
			unsigned int aNum = getRandomUnsignedInt();
			unsigned int bNum = getRandomUnsignedInt();
			vector<int> aNumVec = convertIntToVector(aNum);
			vector<char> aVec(32, '-');
			// aVec = inputDifferential(); // w15 and w15 = w10
			for (int i = 0; i < 32; ++i) {
				if (aNumVec[i]==1) {
					aVec[i]='x';
				}
			}
			vector<int> bNumVec = convertIntToVector(bNum);
			vector<char> bVec(32, '-');
			bVec = aVec;
			for (int i = 0; i < 32; ++i) {
				if (bNumVec[i]==1) {
					bVec[i]='x';
				}
			}
			setRandomComplete(aVec);
			setRandomComplete(bVec);
			vector<char> goodA = getGoodDiff(aVec);
			vector<char> goodB = getGoodDiff(bVec);
			// printSingleLine(goodA);
			// printSingleLine(goodB);
			for (int i = 0; i < 200000; ++i) {
				// cout << "here" << endl;
				// printSingleLine(aVec);
				// printSingleLine(bVec);
				int r = rand();
				vector<char> aAltVec = getRandomCanceler(aVec, false, r, goodA);
				vector<char> aAltSig0 = calculateFromCross(CAP_SIGMA_1, aAltVec);
				if (!makeAAltAppt(aAltVec, aVec)) {
					cout << "SOMETHING WRONG!!!" << endl;
				}
				// printSingleLine(aAltVec);
				// printSingleLine(aAltSig0);
				if (canCancel(goodA, aAltSig0)!=-1) {
					// cout << "Can Cancel" << endl;
					// printSingleLine(aAltVec);
					// printSingleLine(goodB);
					if (findACancelB(aAltVec, goodA)) {
						success++;
						found=true;
						AFound = aNum;
						BFound = bNum;
						cout << "aNum = " << aNum << endl;
						cout << "bNum = " << bNum << endl;
						cout << "aAltVec: "; printSingleLine(aAltVec);
						break;
					}
				}
			}
		}
		cout << "trials here : " << trials << " and value found was " << AFound << ", " << BFound << endl;
		totalTrials = totalTrials + trials;
	}
	string fileName = "CAP_SIGMA_1_exp_result.txt";
	ofstream myFile(fileName);
	myFile << "CAP_SIGMA_1 : " << totalTrials/total << endl;
	myFile.close();
	return 0;
}