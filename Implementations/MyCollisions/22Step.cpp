#include "SHA2Util.h"

using namespace std;

#define HEIGHT 22
#define ROTRIGHT(word,bits) (((word) >> (bits)) | ((word) << (32-(bits))))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SSIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SSIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))

int hm(vector<char> v) {
	int res = 0;
	for (int i = 0; i < 32; ++i) {
		if (v[i]=='x') {
			res++;
		}
	}
	return res;
}

int main(int argc, char const *argv[]) {
	Conditions conditions;

	for (int i = 0; i < HEIGHT; ++i) {
		conditions.addFullLineVariables("W", i);
		conditions.addFullLineVariables("E", i);
		conditions.addFullLineVariables("A", i);
	}

	vector<vector<char> > wDifferential(HEIGHT, vector<char> (32, '-'));
	vector<vector<char> > eDifferential(HEIGHT, vector<char> (32, '-'));
	vector<vector<char> > aDifferential(HEIGHT, vector<char> (32, '-'));


	vector<vector<char> > sigA(HEIGHT, vector<char> (32, '-'));
	vector<vector<char> > sigE(HEIGHT, vector<char> (32, '-'));

	/* First Possibility
	*/
	vector<int> crossPositionsInW7 (1, 0);
	
	int minWeight = 100;
	int argMinWeight = -1;
	for (int i = 0; i < 32; ++i) {
		vector<char> a (32, '-');
		a[i] = 'x';
		vector<char> sig1A = calculateFromCross(SMALL_SIGMA_1, a);
		vector<char> sig11A = calculateFromCross(SMALL_SIGMA_1, sig1A);
		if (hm(sig11A) + hm(sig1A) + hm(a)<minWeight) {
			minWeight = hm(sig1A) + hm(a) + hm(sig11A);
			argMinWeight = i;
		}
	}

	wDifferential[15][6] = 'x';
	wDifferential[17] = calculateFromCross(SMALL_SIGMA_1, wDifferential[15]);
	wDifferential[12] = calculateFromCross(SMALL_SIGMA_1, wDifferential[17]);



	// for E:-

	// 1
	eDifferential[13] = wDifferential[17];
	
	eDifferential[10] = calculateFromCross(CAP_SIGMA_1, eDifferential[13]);

	// 2


	// 3
	eDifferential[11][6] = 'x';


	// 4







	// for A:-

	// 1


	// 2


	// 3




	// 7
	aDifferential[9] = eDifferential[13];
	eDifferential[9] = aDifferential[9];


	cout << "s3: "; printSingleLine(calculateFromCross(CAP_SIGMA_1, eDifferential[13]));


	bool printWDiff = true;
	bool printEDiff = true;
	bool printADiff = true;
	printDifferentials(printWDiff, printEDiff, printADiff, wDifferential, eDifferential, aDifferential);
	

	return 0;
}