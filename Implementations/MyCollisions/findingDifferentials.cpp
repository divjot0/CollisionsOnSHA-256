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
	int BIGTHRES = 80;
	unsigned int argBestHamWeight = -1;
	int bestHamWeight = 1000;
	int same = 0;
	unsigned int prevBest = -1;
	int bestBadPoints = 1000;
	srand(time(0));
	cout << "1st random number: " << rand() << endl;
	int trials = 0;
	while(!found) {
		trials++;
		if (trials%10000000==0) {
			cout << "sranding..." << endl;
			srand(time(0));
		}
		int badPoints = 0;
		unsigned int e7Num = getRandomUnsignedInt();
		vector<int> e7NumVec = convertIntToVector(e7Num);
		vector<char> e7Vec(32, '-');
		// printSingleLine(e7Vec);
		for (int i = 0; i < 32; ++i) {
			if (e7NumVec[i]==1) {
				e7Vec[i]='x';
			}
		}
		if (!(hammingWeight(e7Vec)*4<=BIGTHRES)) {
			continue;
		}
		vector<char> e7Sig1 = calculateFromCross(CAP_SIGMA_1, e7Vec);
		int recentBadPoints = canCancel(e7Sig1, e7Vec);
		if (recentBadPoints!=-1) {
			badPoints = badPoints + recentBadPoints;
			// vector<char> e7SmallSig1 = calculateFromCross(SMALL_SIGMA_1, e7Vec);
			// recentBadPoints = canCancel(e7SmallSig1, e7Vec);
			// if (recentBadPoints!=-1) {
			// badPoints = badPoints + recentBadPoints;
			vector<char> e7Sig0 = calculateFromCross(CAP_SIGMA_0, e7Vec);
			recentBadPoints = canCancel(e7Sig0, e7Vec);
			if (recentBadPoints!=-1) {
				badPoints = badPoints + recentBadPoints;
				// vector<char> e7SmallSig1 = calculateFromCross(SMALL_SIGMA_1, e7Vec);
				// recentBadPoints = canCancel(e7SmallSig1, e7Vec);
				// int goToNext = -1;
				// if (recentBadPoints!=-1) {
				// 	badPoints = badPoints + recentBadPoints;
				// 	goToNext = 1;
				// }
				// else {
				// 	vector<char> e7SmallSig0 = calculateFromCross(SMALL_SIGMA_0, e7Vec);
				// 	recentBadPoints = canCancel(e7SmallSig0, e7Vec);
				// 	if (recentBadPoints!=-1) {
				// 		badPoints = badPoints + recentBadPoints;
				// 		goToNext = 0;
				// 	}
				// }
				// cout << recentBadPoints << endl;
				recentBadPoints=0;
				if (recentBadPoints!=-1) {
					badPoints = badPoints + recentBadPoints;
					// if (hammingWeight(e7Vec)*4<=BIGTHRES) {
					// 	if (prevBest==argBestHamWeight) {
					// 		same++;
					// 	}
					// }
					if (hammingWeight(e7Vec)*4<=BIGTHRES ) {
						argBestHamWeight = e7Num;
						bestBadPoints = badPoints;
						bestHamWeight = hammingWeight(e7Vec)*4;
						prevBest = argBestHamWeight;
						same=0;
						// found = true;
						if (bestHamWeight<=BIGTHRES && bestHamWeight<=80) {
							cout << "searching for: " << bestHamWeight << " " << bestBadPoints << " " << argBestHamWeight << endl;
							partialDifferentials[1][7] = e7Vec;
							// completeDifferentials = getCompleteDifferentials(partialDifferentials);
							// if (completeDifferentials.size()>0) {
							// 	cout << bestHamWeight << " " << bestBadPoints << " " << argBestHamWeight << endl;
							// 	found=true;
							// 	BIGTHRES = BIGTHRES-4;
							// }
							// else {
							// 	cout << "Not found" << endl;
							// }
							// else {
							// 	cout << "N"
							// }
						}
					}
					// if (same==20) {
					// 	cout << "Changed because same." << endl;
					// 	same = 0;
					// 	argBestHamWeight = -1;
					// 	bestHamWeight = 1000;
					// 	prevBest = -1;
					// 	bestBadPoints = 1000;
					// }
				}
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
				// }
			}
		}
	}
	return 0;
}

// 1345172059