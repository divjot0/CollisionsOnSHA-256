#include "SHA2Util.h"

#define HEIGHT 16

using namespace std;

int main(int argc, char const *argv[]) {
	
	vector<vector<char> > wDifferential(HEIGHT, vector<char> (32, '-'));
	vector<vector<char> > eDifferential(HEIGHT, vector<char> (32, '-'));
	vector<vector<char> > aDifferential(HEIGHT, vector<char> (32, '-'));
	vector<vector<vector<char> > > partialDifferentials = {aDifferential, eDifferential, wDifferential};
	vector<vector<vector<vector<char> > > > completeDifferentials;

	bool found = false;
	unsigned int argBestHamWeight = -1;
	int bestHamWeight = 1000;
	int bestBadPoints = 1000;
	srand(time(0));
	cout << "1st random number: " << rand() << endl;
	while(!found) {
		int badPoints = 0;
		unsigned int e7Num = getRandomUnsignedInt();
		vector<int> e7NumVec = convertIntToVector(e7Num);
		vector<char> e7Vec(32, '-');
		for (int i = 0; i < 32; ++i) {
			if (e7NumVec[i]==1) {
				e7Vec[i]='x';
			}
		}
		if (hammingWeight(e7Vec)>19) {
			continue;
		}
		vector<char> e7Sig1 = calculateFromCross(CAP_SIGMA_1, e7Vec);
		int recentBadPoints = canCancel(e7Sig1, e7Vec);
		if (recentBadPoints!=-1) {
			badPoints = badPoints + recentBadPoints;
			vector<char> e7SmallSig1 = calculateFromCross(SMALL_SIGMA_1, e7Vec);
			recentBadPoints = canCancel(e7SmallSig1, e7Vec);
			if (recentBadPoints!=-1) {
				badPoints = badPoints + recentBadPoints;
				vector<char> e7Sig0 = calculateFromCross(CAP_SIGMA_0, e7Vec);
				recentBadPoints = isSubset(e7Sig0, e7Vec);
				if (recentBadPoints!=-1) {
					badPoints = badPoints + recentBadPoints;
					// if (hammingWeight(e7Vec)*4 <= 80) {
					// 	bestHamWeight = hammingWeight(e7Vec)*4;
					// 	argBestHamWeight = e7Num;
					// 	bestBadPoints = badPoints;
					// 	// found = true;
					// 	cout << bestHamWeight << " " << bestBadPoints << endl;
					// 	if (bestHamWeight<=80) {
					// 		cout << "searching for: " << argBestHamWeight << endl;
					// 		partialDifferentials[1][7] = e7Vec;
					// 		completeDifferentials = getCompleteDifferentials(partialDifferentials);
					// 		if (completeDifferentials.size()>0) {
					// 			found=true;
					// 		}
					// 	}
					// }
					if (badPoints<46) {
						bestHamWeight = hammingWeight(e7Vec)*4;
						argBestHamWeight = e7Num;
						bestBadPoints = badPoints;
						// found = true;
						cout << bestHamWeight << " " << bestBadPoints << " " << argBestHamWeight << endl;
						if (bestHamWeight<=96) {
							cout << "searching for: " << argBestHamWeight << endl;
							partialDifferentials[1][7] = e7Vec;
							completeDifferentials = getCompleteDifferentials(partialDifferentials);
							if (completeDifferentials.size()>0) {
								found=true;
							}
						}
					}
				}
			}
		}
	}
	return 0;
}






//
if (hammingWeight(e7Vec)*4<=80) {
					bestHamWeight = hammingWeight(e7Vec)*4;
					argBestHamWeight = e7Num;
					bestBadPoints = badPoints;
					// found = true;
					if (bestHamWeight<=80) {
						cout << "searching for: " bestHamWeight << " " << bestBadPoints << " " << argBestHamWeight << endl;
						partialDifferentials[1][7] = e7Vec;
						completeDifferentials = getCompleteDifferentials(partialDifferentials);
						if (completeDifferentials.size()>0) {
							cout << "Found" << bestHamWeight << " " << bestBadPoints << " " << argBestHamWeight << endl;
						}
					}
				}