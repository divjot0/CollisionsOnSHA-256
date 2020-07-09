#include "SHA2Util23.h"

#define HEIGHT 18

using namespace std;

int main(int argc, char const *argv[]) {
	
	vector<vector<char> > wDifferential(HEIGHT, vector<char> (32, '-'));
	vector<vector<char> > eDifferential(HEIGHT, vector<char> (32, '-'));
	vector<vector<char> > aDifferential(HEIGHT, vector<char> (32, '-'));
	vector<vector<vector<char> > > partialDifferentials = {aDifferential, eDifferential, wDifferential};
	vector<vector<vector<vector<char> > > > completeDifferentials;

	bool found = false;
	int BIGTHRES = 60;
	unsigned int argBestHamWeight = -1;
	int bestHamWeight = 1000;
	int same = 0;
	unsigned int prevBest = -1;
	int bestBadPoints = 1000;
	srand(time(0));
	int trials = 0;
	cout << "1st random number: " << rand() << endl;
	while(!found) {
		trials++;
		if (trials%1000000000==0) {
			cout << "sranding..." << endl;
			srand(time(0));
		}
		int badPoints = 0;
		unsigned int e7Num = getRandomUnsignedInt();
		vector<int> e7NumVec = convertIntToVector(e7Num);
		vector<char> e7Vec(32, '-');
		for (int i = 0; i < 32; ++i) {
			if (e7NumVec[i]==1) {
				e7Vec[i]='x';
			}
		}
		if (!(hammingWeight(e7Vec)*4<=BIGTHRES)) {
			continue;
		}
		vector<char> e7Sig0 = calculateFromCross(CAP_SIGMA_0, e7Vec);
		vector<char> e7Sig1 = calculateFromCross(CAP_SIGMA_1, e7Vec);
		int recentBadPoints = canCancel(e7Sig1, e7Vec);
		if (recentBadPoints!=-1) {
			badPoints = badPoints + recentBadPoints;
			recentBadPoints = isSubset(e7Sig0, e7Vec);
			if (recentBadPoints!=-1) {
				badPoints = badPoints + recentBadPoints;
				// vector<char> e7Vec2(32, '-');
				// for (int i = 0; i < 31; ++i) {
				// 	e7Vec2[i+1] = e7Vec[i];
				// }
				// vector<char> e7SmallSig1 = calculateFromCross(SMALL_SIGMA_1, e7Vec);
				// recentBadPoints = canCancel(e7SmallSig1, e7Vec);
				recentBadPoints = 0;
				if (recentBadPoints!=-1) {
					badPoints = badPoints + recentBadPoints;
					if (hammingWeight(e7Vec)*4<=BIGTHRES) {
						argBestHamWeight = e7Num;
						bestBadPoints = badPoints;
						bestHamWeight = hammingWeight(e7Vec)*4;
						prevBest = argBestHamWeight;
						same=0;
						// found = true;
						if (bestHamWeight<=BIGTHRES && bestHamWeight<=80) {
							cout << "searching for: " << bestHamWeight << " " << bestBadPoints << " " << argBestHamWeight << endl;
							partialDifferentials[1][8] = e7Vec;
							completeDifferentials = getCompleteDifferentials(partialDifferentials);
							if (completeDifferentials.size()>0) {
								cout << bestHamWeight << " " << bestBadPoints << " " << argBestHamWeight << " " << BIGTHRES << endl;
								found=true;
								BIGTHRES = BIGTHRES-4;
							}
							else {
								cout << "Not found" << endl;
							}
							
						}
					}
				}
			}
		}
	}
	return 0;
}