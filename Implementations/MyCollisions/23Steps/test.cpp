#include "SHA2Util23.h"

using namespace std;

bool isDifferentD(char a) {
	if (a=='u' || a=='n' || a=='x') {
		// cout << "here" << endl;
		return true;
	}
	return false;
}

bool copyFunction(vector<char>& w15Differential) {
	vector<char> sigW15Differential = calculateFromCross(SMALL_SIGMA_1, w15Differential);
	vector<char> cancelWith = getOppositeDifferential(w15Differential);
	vector<vector<int> > lList = getLListForCut(sigW15Differential, cancelWith);
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
			if (isDifferent(sigW15Differential[j])) {
				pureSigDifferential[j] = cancelWith[lList[i][0]];
			}
			else {
				if (cancelWith[j]!=cancelWith[lList[i][0]]) {
					// cout << i << "/" << lList.size() << endl;
					return false;
				}
			}
		}
		if (isDifferent(sigW15Differential[lList[i][1]])) {
			pureSigDifferential[lList[i][1]] = current;
		}
		else {
			if (cancelWith[lList[i][1]]!=current) {
				// cout << i << endl;
				return false;
			}
		}
	}


	// cout << "chcek if next wo cancel" << endl;
	// printSingleLine(pureSigDifferential);
	// printSingleLine(cancelWith);
	// cout << endl;

	// if (!checking) {
	// 	cout << "hr: "; printSingleLine(pureSigDifferential);
	// 	cout << "e8: "; printSingleLine(e8Differential);
	// 	cout << "a7: "; printSingleLine(a7Differential);
	// }

	// get A7 differential which gets pureSigDifferential
	vector<vector<int> > sig1CausingBits = getCausingBits("SMALL_SIGMA_1");

	Conditions conditions;
	for (int i = 0; i < 32; ++i) {
		if (!isDifferent(w15Differential[i])) {
			conditions.addParticularVariable(to_string(i), i, i);
		}
	}

	vector<int> alreadyFixed;

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(pureSigDifferential[i])) {
			int count = 0;
			int countU = 0;
			int countV = 0;
			char diffType;
			vector<int> relationVariables;
			for (int j = 0; j < sig1CausingBits[i].size(); ++j) {
				if (isDifferent(w15Differential[sig1CausingBits[i][j]])) {
					count++;
					diffType = w15Differential[sig1CausingBits[i][j]];
				}
				else {
					relationVariables.push_back(sig1CausingBits[i][j]);
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
						w15Differential[relationVariables[0]]='0';
					}
					else {
						w15Differential[relationVariables[0]]='1';
					}
					alreadyFixed.push_back(relationVariables[0]);
				}
				else {
					if (diffType==pureSigDifferential[i]) {
						if (w15Differential[relationVariables[0]]=='1') {
							return false;
						}
					}
					else {
						if (w15Differential[relationVariables[0]]=='0') {
							return false;
						}
					}
				}
			}
			if (count==3) {
				if (w15Differential[sig1CausingBits[i][0]]=='u') {
					countU++;
				}
				if (w15Differential[sig1CausingBits[i][1]]=='u') {
					countU++;
				}
				if (w15Differential[sig1CausingBits[i][2]]=='u') {
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

	if (!conditions.areSatisfiable()) {
		return false;
	}

	cout << "Conditions are isSatisfiable " << endl;

	// cout << "aaa: ";
	// for (int i = 0; i < 32; ++i) {
	// 	cout << i%10;
	// }
	// cout << endl;
	// cout << "w16: "; printSingleLine(w15Differential);
	// cout << "Conditions: "; conditions.printConditions();

	if (!setSmallValidAssignment(w15Differential, conditions, alreadyFixed)) {
		return false;
	}

	// start
	// if (!checking) {
	// 	setValidAssignment(a7Differential, conditions);
	// 	// cout << "nw: "; printSingleLine(getSigma(a7Differential, "CAP_SIGMA_0"));
	// 	// vector<char> test = a7Differential;
	// 	// for (int i = 0; i < 32; ++i) {
	// 	// 	if (test[i]=='-') {
	// 	// 		test[i]=0;
	// 	// 	}
	// 	// }
	// 	// cout << "casuing bits of 5 : " << sig0CausingBits[5][0] << " " << sig0CausingBits[5][1] << " " << sig0CausingBits[5][2] << endl;
	// 	// cout << "vak of causingBits of 5 : " << a7Differential[sig0CausingBits[5][0]] << " " << a7Differential[sig0CausingBits[5][1]] << " " << a7Differential[sig0CausingBits[5][2]] << endl;
	// }
	return true;
}

int main(int argc, char const *argv[]) {
	string s = "100nnn100nu00nu0unu1u1u1n00un1un";
	vector<char> a(32, '-');
	for (int i = 0; i < 32; ++i) {
		a[i] = s[i];
	}
	vector<vector<int> > causing = getCausingBits("SMALL_SIGMA_1");

	vector<char> res(32, '-');

	for (int i = 0; i < 32; ++i) {
		int countDiff = 0;
		int countU=0;
		int countN=0;
		if (causing[i].size()==2) {
			if (isDifferentD(a[causing[i][0]]) && isDifferentD(a[causing[i][1]])) {
				res[i]='-';
			}
			else if (!isDifferentD(a[causing[i][0]]) && !isDifferentD(a[causing[i][1]])) {
				res[i]='-';
			}
			else if (isDifferentD(a[causing[i][0]])) {
				int count1=0;
				if (a[causing[i][0]]=='1') {
					count1++;
				}
				if (a[causing[i][1]]=='1') {
					count1++;
				}
				if (count1==0) {
					if (a[causing[i][0]]=='u' || a[causing[i][0]]=='u') {
						res[i]='u';
					}
					else {
						res[i]='n';
					}
				}
				else {
					if (a[causing[i][0]]=='u' || a[causing[i][0]]=='u') {
						res[i]='n';
					}
					else {
						res[i]='u';
					}
				}
			}
			else if (isDifferentD(a[causing[i][1]])) {
				int count1=0;
				if (a[causing[i][0]]=='1') {
					count1++;
				}
				if (a[causing[i][1]]=='1') {
					count1++;
				}
				if (count1==0) {
					if (a[causing[i][0]]=='u' || a[causing[i][0]]=='u') {
						res[i]='u';
					}
					else {
						res[i]='n';
					}
				}
				else {
					if (a[causing[i][0]]=='u' || a[causing[i][0]]=='u') {
						res[i]='n';
					}
					else {
						res[i]='u';
					}
				}
			}
		}
		else {
			// cout << "here " << causing[i].size() << endl;
			for (int j = 0; j < causing[i].size(); ++j) {
				// cout << a[causing[i][j]] << endl;
				if (isDifferentD(a[causing[i][j]])) {
					countDiff++;
					if (a[causing[i][j]]=='u') {
						// cout << "here" << endl;
						countU++;
					}
					else {
						countN++;
					}
				}
			}
			if (countDiff==0) {
				res[i]='-';
			}
			if (countDiff==2) {
				res[i]='-';
			}
			if (countDiff==3) {
				if (countU==1 || countU==3) {
					res[i]='u';
				}
				else {
					res[i]='n';
				}
			}
			if (countDiff==1) {
				int count1=0;
				if (a[causing[i][0]]=='1') {
					count1++;
				}
				if (a[causing[i][1]]=='1') {
					count1++;
				}
				if (a[causing[i][2]]=='1') {
					count1++;
				}
				if (count1==0 || count1==2) {
					if (countU==1) {
						res[i]='u';
					}
					else {
						res[i]='n';
					}
				}
				else {
					if (countU==1) {
						res[i]='n';
					}
					else {
						res[i]='u';
					}
				}
			}
		}
	}

	// cout << res << endl;
	printSingleLine(a);
	printSingleLine(res);

	vector<char> w16 = getOppositeDifferential(getOppositeDifferential(a));

	copyFunction(w16);

	cout << "w16: "; printSingleLine(w16);

	cout << "These bits casues 28 : ";
	for (int i = 0; i < causing[28].size(); ++i) {
		cout << causing[28][i] << " ";
	}
	cout << endl;

	cout << "These bits casues 30 : ";
	for (int i = 0; i < causing[30].size(); ++i) {
		cout << causing[30][i] << " ";
	}
	cout << endl;

	return 0;
}