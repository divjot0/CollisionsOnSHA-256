#include "semiFreeStartUtil.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
	vector<unsigned int> registers(8);
	// unsigned int  H0 = 0x6a09e667;
	// unsigned int  H1 = 0xbb67ae85;
	// unsigned int  H2 = 0x3c6ef372;
	// unsigned int  H3 = 0xa54ff53a;
	// unsigned int  H4 = 0x510e527f;
	// unsigned int  H5 = 0x9b05688c;
	// unsigned int  H6 = 0x1f83d9ab;
	// unsigned int  H7 = 0x5be0cd19;

	unsigned int  H0 = 1779033703;
	unsigned int  H1 = 3144134277;
	unsigned int  H2 = 1013904242;
	unsigned int  H3 = 2773480762;
	unsigned int  H4 = 1359893119;
	unsigned int  H5 = 2600822924;
	unsigned int  H6 = 528734635;
	unsigned int  H7 = 1541459225;
	registers[0]=H0;
	registers[1]=H1;
	registers[2]=H2;
	registers[3]=H3;
	registers[4]=H4;
	registers[5]=H5;
	registers[6]=H6;
	registers[7]=H7;

	vector<vector<vector<unsigned int> > > transitRegisters(2, vector<vector<unsigned int> > (23, vector<unsigned int> (8, 0)));

	vector<vector<unsigned int> > messages(2, vector<unsigned int> (19, 0));

	cout << "IV is : " << endl;
	for (int i = 0; i < 8; ++i) {
		cout << registers[i] << endl;
	}

	messages[0][0] = 969271519;
	messages[0][1] = 1110335190;
	messages[0][2] = 1793287486;
	messages[0][3] = 1138110727;
	messages[0][4] = 348426595;
	messages[0][5] = 320141134;
	messages[0][6] = 1795054104;
	messages[0][7] = 2548410122;
	messages[0][8] = 34395578;
	messages[0][9] = 993686178;
	messages[0][10] = 2592560752;
	messages[0][11] = 966079816;
	messages[0][12] = 2383576875;
	messages[0][13] = 50658305;
	messages[0][14] = 3876685175;
	messages[0][15] = 1135684775;

	for (int i = 0; i < 16; ++i) {
		messages[1][i] = messages[0][i];
	}
	messages[1][9] = 993686179;
	messages[1][10] = 2592560751;
	messages[1][11] = 966120776;
	messages[1][12] = 2433916714;

	printMessages(messages);

	bool check=true;
	if (check) {

		cout << "Writing the values for two blcoks at iterations : :: " << endl;

		vector<unsigned int> block = messages[0];
		vector<unsigned int> otherBlock = messages[1];

		vector<unsigned int> rs = registers;
		compute_hash(block);

		rs = registers;
		compute_hash(otherBlock);
	}

	return 0;
}