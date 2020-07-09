#include "semiFreeStartUtil.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
	vector<unsigned int> registers(8);
	unsigned int  H0 = 0x6a09e667;
	unsigned int  H1 = 0xbb67ae85;
	unsigned int  H2 = 0x3c6ef372;
	unsigned int  H3 = 0xa54ff53a;
	unsigned int  H4 = 0x510e527f;
	unsigned int  H5 = 0x9b05688c;
	unsigned int  H6 = 0x1f83d9ab;
	unsigned int  H7 = 0x5be0cd19;
	registers[0]=H0;
	registers[1]=H1;
	registers[2]=H2;
	registers[3]=H3;
	registers[4]=H4;
	registers[5]=H5;
	registers[6]=H6;
	registers[7]=H7;

	unsigned int  A = 2838881730;
unsigned int  B = 2568923338;
unsigned int  C = 2889909820;
unsigned int  D = 1148053066;
unsigned int  E = 3694445397;
unsigned int  F = 3790682427;
unsigned int  G = 2551179535;
unsigned int  H = 3980067621;

	registers[0]=A;
	registers[1]=B;
	registers[2]=C;
	registers[3]=D;
	registers[4]=E;
	registers[5]=F;
	registers[6]=G;
	registers[7]=H;

	vector<vector<unsigned int> > restrr(2, vector<unsigned int> (64, 0));
	restrr[0] = registers;

	printMessages(restrr);


	vector<vector<vector<unsigned int> > > transitRegisters(2, vector<vector<unsigned int> > (23, vector<unsigned int> (8, 0)));

	vector<vector<unsigned int> > messages(2, vector<unsigned int> (64, 0));

	messages[0][0] = 2889472478;
messages[0][1] = 0;
messages[0][2] = 0;
messages[0][3] = 0;
messages[0][4] = 0;
messages[0][5] = 0;
messages[0][6] = 0;
messages[0][7] = 0;
messages[0][8] = 1082132116;
messages[0][9] = 2249220097;
messages[0][10] = 3990362550;
messages[0][11] = 1887444886;
messages[0][12] = 2792150022;
messages[0][13] = 2054651538;
messages[0][14] = 1820467171;
messages[0][15] = 0;


// this is the second message
for (int i = 0; i < 16; ++i) {
	messages[1][i] = messages[0][i];
}
messages[1][8] = 2249220097;
messages[1][9] = 1082132116;
messages[1][11] = 3054532867;
// messages[1][13] = 4292573661;
	// messages[1][8] = 2935265077;
	// messages[1][11] = 1065654078;

	expandMessage(messages[1]);

	printMessages(messages);

	bool check=true;
	if (check) {

		cout << "Writing the values for two blcoks at iterations : :: " << endl;

		vector<unsigned int> block = messages[0];
		vector<unsigned int> otherBlock = messages[1];

		vector<unsigned int> rs = registers;
		rs = compute_hash_step(rs, block, 0);
		rs = compute_hash_step(rs, block, 1);
		rs = compute_hash_step(rs, block, 2);
		rs = compute_hash_step(rs, block, 3);
		rs = compute_hash_step(rs, block, 4);
		rs = compute_hash_step(rs, block, 5);
		rs = compute_hash_step(rs, block, 6);
		rs = compute_hash_step(rs, block, 7);
		rs = compute_hash_step(rs, block, 8);
		rs = compute_hash_step(rs, block, 9);
		rs = compute_hash_step(rs, block, 10);
		rs = compute_hash_step(rs, block, 11);
		rs = compute_hash_step(rs, block, 12);
		rs = compute_hash_step(rs, block, 13);
		rs = compute_hash_step(rs, block, 14);
		rs = compute_hash_step(rs, block, 15);
		rs = compute_hash_step(rs, block, 16);
		rs = compute_hash_step(rs, block, 17);
		rs = compute_hash_step(rs, block, 18);
		rs = compute_hash_step(rs, block, 19);
		rs = compute_hash_step(rs, block, 20);
		rs = compute_hash_step(rs, block, 21);
		rs = compute_hash_step(rs, block, 22);
		rs = compute_hash_step(rs, block, 23);
		rs = compute_hash_step(rs, block, 24);
		
		rs = registers;
		rs = compute_hash_step(rs, otherBlock, 0);
		rs = compute_hash_step(rs, otherBlock, 1);
		rs = compute_hash_step(rs, otherBlock, 2);
		rs = compute_hash_step(rs, otherBlock, 3);
		rs = compute_hash_step(rs, otherBlock, 4);
		rs = compute_hash_step(rs, otherBlock, 5);
		rs = compute_hash_step(rs, otherBlock, 6);
		rs = compute_hash_step(rs, otherBlock, 7);
		rs = compute_hash_step(rs, otherBlock, 8);
		rs = compute_hash_step(rs, otherBlock, 9);
		rs = compute_hash_step(rs, otherBlock, 10);
		rs = compute_hash_step(rs, otherBlock, 11);
		rs = compute_hash_step(rs, otherBlock, 12);
		rs = compute_hash_step(rs, otherBlock, 13);
		rs = compute_hash_step(rs, otherBlock, 14);
		rs = compute_hash_step(rs, otherBlock, 15);
		rs = compute_hash_step(rs, otherBlock, 16);
		rs = compute_hash_step(rs, otherBlock, 17);
		rs = compute_hash_step(rs, otherBlock, 18);
		rs = compute_hash_step(rs, otherBlock, 19);
		rs = compute_hash_step(rs, otherBlock, 20);
		rs = compute_hash_step(rs, otherBlock, 21);
		rs = compute_hash_step(rs, otherBlock, 22);
		rs = compute_hash_step(rs, otherBlock, 23);
		rs = compute_hash_step(rs, otherBlock, 24);
	}

	return 0;
}