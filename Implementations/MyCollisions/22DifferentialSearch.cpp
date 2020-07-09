#include "SHA2UtilFor22.h"

using namespace std;

int main(int argc, char const *argv[]) {
	
	// vector<vector<char> > wDifferential(HEIGHT, vector<char> (32, '-'));
	// vector<vector<char> > eDifferential(HEIGHT, vector<char> (32, '-'));
	// vector<vector<char> > aDifferential(HEIGHT, vector<char> (32, '-'));
	// vector<vector<vector<char> > > partialDifferentials = {aDifferential, eDifferential, wDifferential};
	// vector<vector<vector<vector<char> > > > completeDifferentials;

	bool found = false;
	int BIGTHRES = 76;
	unsigned int argBestHamWeight = -1;
	int bestHamWeight = 1000;
	int same = 0;
	unsigned int prevBest = -1;
	int bestBadPoints = 1000;
	srand(time(0));
	cout << "1st random number: " << rand() << endl;
	// while(!found) {
	// 	int badPoints = 0;
	// 	unsigned int e7Num = getRandomUnsignedInt();
	// 	vector<int> e7NumVec = convertIntToVector(e7Num);
	// 	vector<char> e7Vec(32, '-');
	// 	for (int i = 0; i < 32; ++i) {
	// 		if (e7NumVec[i]==1) {
	// 			e7Vec[i]='x';
	// 		}
	// 	}
	// 	if (!(hammingWeight(e7Vec)*4<=BIGTHRES)) {
	// 		continue;
	// 	}
	// 	vector<char> e7Sig1 = calculateFromCross(CAP_SIGMA_1, e7Vec);
	// 	int recentBadPoints = canCancel(e7Sig1, e7Vec);
	// 	if (recentBadPoints!=-1) {
	// 		badPoints = badPoints + recentBadPoints;
	// 		// vector<char> e7SmallSig1 = calculateFromCross(SMALL_SIGMA_1, e7Vec);
	// 		// recentBadPoints = canCancel(e7SmallSig1, e7Vec);
	// 		// if (recentBadPoints!=-1) {
	// 		// badPoints = badPoints + recentBadPoints;
	// 		vector<char> e7Sig0 = calculateFromCross(CAP_SIGMA_0, e7Vec);
	// 		recentBadPoints = isSubset(e7Sig0, e7Vec);
	// 		if (recentBadPoints!=-1) {
	// 			badPoints = badPoints + recentBadPoints;
	// 			vector<char> e7SmallSig1 = calculateFromCross(SMALL_SIGMA_1, e7Vec);
	// 			recentBadPoints = canCancel(e7SmallSig1, e7Vec);
	// 			int goToNext = -1;
	// 			if (recentBadPoints!=-1) {
	// 				badPoints = badPoints + recentBadPoints;
	// 				goToNext = 1;
	// 			}
	// 			// else {
	// 			// 	vector<char> e7SmallSig0 = calculateFromCross(SMALL_SIGMA_0, e7Vec);
	// 			// 	recentBadPoints = canCancel(e7SmallSig0, e7Vec);
	// 			// 	if (recentBadPoints!=-1) {
	// 			// 		badPoints = badPoints + recentBadPoints;
	// 			// 		goToNext = 0;
	// 			// 	}
	// 			// }
	// 			if (goToNext!=-1) {
	// 				if (hammingWeight(e7Vec)*4<=BIGTHRES) {
	// 					if (prevBest==argBestHamWeight) {
	// 						same++;
	// 					}
	// 				}
	// 				if (hammingWeight(e7Vec)*4<=BIGTHRES && badPoints<40) {
	// 					bestHamWeight = hammingWeight(e7Vec)*4;
	// 					argBestHamWeight = e7Num;
	// 					bestBadPoints = badPoints;
	// 					prevBest = argBestHamWeight;
	// 					same=0;
	// 					// found = true;
	// 					if (bestHamWeight<=BIGTHRES && bestHamWeight<=80) {
	// 						cout << "searching for: " << bestHamWeight << " " << bestBadPoints << " " << argBestHamWeight << " " << goToNext << endl;
	// 						partialDifferentials[1][7] = e7Vec;
	// 						completeDifferentials = getCompleteDifferentials(partialDifferentials);
	// 						if (completeDifferentials.size()>0) {
	// 							cout << bestHamWeight << " " << bestBadPoints << " " << argBestHamWeight << endl;
	// 							// found=true;
	// 							// BIGTHRES = BIGTHRES-4;
	// 						}
	// 						// else {
	// 						// 	cout << "N"
	// 						// }
	// 					}
	// 				}
	// 				if (same==10) {
	// 					cout << "Changed because same." << endl;
	// 					same = 0;
	// 					argBestHamWeight = -1;
	// 					bestHamWeight = 1000;
	// 					prevBest = -1;
	// 					bestBadPoints = 1000;
	// 				}
	// 			}
	// 		}
	// 	}
	// }

	string e7String = "nn----n-un-n-u-n--u--n---u-u----";
		   // e7String = "nn----n-n--n-u-n--u--n---u-u----";

	vector<char> e7Vec(32, '-');
	for (int i = 0; i < 32; ++i) {
		e7Vec[i] = e7String[i];
	}

	// we need to find w15, w10 such that w15 cancels e7Vec and w10(= SMALLSIG1(w15)) cancels e7Vec
	int state = -1;

	while(!found && state!=-2) {
		vector<char> w15Vec = getRandomCanceler(e7Vec, true, state);
		vector<char> w15SmallSig1 = calculateFromCross(SMALL_SIGMA_1, w15Vec);
		if (canCancel(e7Vec, w15Vec)!=-1 && canCancel(w15SmallSig1, w15Vec)!=-1) {
			vector<char> w10Vec(32, '-');
			cout << "testing 1 " << state << endl;
			if (finalConstructionW15(w15Vec, e7Vec) && finalConstructionW10WithW15(w10Vec, e7Vec, w15Vec)) {
				cout << state << " " << hammingWeight(w15Vec) << " " << hammingWeight(w15SmallSig1) << endl;
				printSingleLine(e7Vec);
				printSingleLine(w15Vec);
				printSingleLine(w15SmallSig1);
				cout << endl;
				found = true;
			}
		}
	}

	return 0;
}