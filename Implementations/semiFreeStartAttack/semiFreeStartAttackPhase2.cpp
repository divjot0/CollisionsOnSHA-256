#include <iostream>
#include <sstream>
#include <bitset>
#include <vector>
#include <iomanip>
#include <cstring>
#include "semiFreeStartUtil.h"
#include <fstream>

unsigned int uIntHere(string str, int s, int &e) {
	string uIntString = "";
	for (int i = s; i < str.length(); ++i) {
		if (str[i]==' ' || str[i]=='\n') {
			e=i-1;
			break;
		}
		uIntString = uIntString + str[i];
	}
	return stoul(uIntString);
}

unsigned int findWWithDelta(unsigned int deltaNeeded) {
	ifstream myFile;
	string line;
	myFile.open("deltaAndWEquation.txt");
	while(getline(myFile, line)) {
		int firstWordEnd=-1;
		unsigned int delta = uIntHere(line, 0, firstWordEnd);
		if (delta==deltaNeeded) {
			return uIntHere(line, firstWordEnd+2, firstWordEnd);
		}
	}
	myFile.close();
	cout << "CODE 101 : Could not find deltaNeeded : val was " << deltaNeeded << endl;
	return -1;
}

int main(int argc, char* argv[]) {
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

	// to be filled by phase1
	unsigned int  alpha = 1875628910;
	unsigned int  beta = 1073717300;
	unsigned int  epsilon = 736125065;
	unsigned int  gamma = 509050377;
	unsigned int  W16 = 3751935;
	unsigned int  W17 = 27459584;

	vector<vector<vector<unsigned int> > > transitRegisters(2, vector<vector<unsigned int> > (23, vector<unsigned int> (8, 0)));

	vector<vector<unsigned int> > messages(2, vector<unsigned int> (19, 0));
	initializeEleventhStepOfMessages(transitRegisters, alpha, beta, epsilon, gamma);

	messages[0][11] = wToSetA(11, alpha, transitRegisters[0][11]);
	messages[1][11] = wToSetA(11, alpha, transitRegisters[1][11]);
	fillAllRegistersAtStep(12, transitRegisters, messages);

	messages[0][12] = wToSetE(12, beta, transitRegisters[0][12]);
	messages[1][12] = wToSetE(12, beta, transitRegisters[1][12]);
	fillAllRegistersAtStep(13, transitRegisters, messages);

	messages[0][13] = wToSetE(13, -1, transitRegisters[0][13]);
	messages[1][13] = wToSetE(13, 0, transitRegisters[1][13]);
	fillAllRegistersAtStep(14, transitRegisters, messages);

	messages[0][14] = wToSetE(14, 0, transitRegisters[0][14]);
	messages[1][14] = wToSetE(14, 0, transitRegisters[1][14]);
	fillAllRegistersAtStep(15, transitRegisters, messages);

	messages[0][15] = wToSetE(15, -2, transitRegisters[0][15]);
	messages[1][15] = wToSetE(15, -2, transitRegisters[1][15]);
	fillAllRegistersAtStep(16, transitRegisters, messages);

	// unsigned int deltaNeeded = EP1(epsilon-1) - EP1(epsilon) + CH(epsilon-1, 0, gamma+1) - CH(epsilon, -1, gamma+1);
	messages[0][16] = W16;
	messages[1][16] = messages[0][16]+1;
	
	messages[0][17] = W17;
	messages[1][17] = messages[0][17]-1;
	// deltaNeeded = -(CH(beta, epsilon-1, 0) - CH(beta, epsilon, -1));

	//EQUATIONS
	cout << "Equation 1 evaluates to : " << SSIG1(W16+1)-SSIG1(W16)-EP1(epsilon-1)+EP1(epsilon)-CH(epsilon-1, 0, gamma+1)+CH(epsilon, -1, gamma+1) << endl;
	cout << "Equation 2 evaluates to : " << SSIG1(W17-1)-SSIG1(W17)-CH(beta, epsilon-1, 0)+CH(beta, epsilon, -1) << endl;
	cout << "Equation 3 evaluates to : " << beta-alpha+EP0(alpha) << endl;
	cout << "Equation 4 evaluates to : " << CH(beta, beta, epsilon-1)-CH(beta, beta, epsilon)+1 << endl;

	cout << "alpha = " << alpha << endl;
	cout << "beta = " << beta << endl;
	cout << "epsilon = " << epsilon << endl;
	cout << "gamma = " << gamma << endl;
	cout << "W16 = " << W16 << endl;
	cout << "W17 = " << W17 << endl;


	goBackOneStep(11, transitRegisters, messages, -1);
	goBackOneStep(10, transitRegisters, messages, 1);
	for(int j=9; j>1; j--) {
		goBackOneStep(j, transitRegisters, messages, 0);
	}

	fillInitialMessages(messages);
	goBackWithFixedMessages(1, transitRegisters, messages, 0);

	vector<vector<unsigned int> > IV (2, vector<unsigned int> (8));
	// goBackWithFixedMessages(0, transitRegisters, messages, 0);

	// for (int i = 0; i < 16; ++i) {
	// 	messages[0][1] = wToSetA(0, transitRegisters[1])
	// }


	printMessages(messages);
	cout << "IV is : ";
	for (int i=0; i<8; i++) {
		cout << transitRegisters[0][0][i] << " ";
	}
	cout << endl;

	for (int j = 0; j < 17; ++j) {
		for (int i = 0; i < 8; ++i) {
			IV[0][i] = transitRegisters[0][j][i];
		}
		for (int i = 0; i < 8; ++i) {
			IV[1][i] = transitRegisters[1][j][i];
		}

		compute_hash_step(IV[0], messages[0], j);
		compute_hash_step(IV[1], messages[1], j);
	}


	// int count=0;
	// for (int i = 0; i < 8; ++i) {
	// 	if (IV[0][i]==IV[1][i]) {
	// 		count++;
	// 	}
	// }
	// if (count==8) {
	// 	cout << "Messages have the same IV = " << endl;
	// 	for (int i = 0; i < 8; ++i) {
	// 		cout << IV[0][i] << " ";
	// 	}
	// 	cout << endl;
	// }
	// else {
	// 	return -1;
	// }

	// bool check = true;
	// if (check) {

	// 	cout << "Writing the values for two blcoks at iterations : :: " << endl;

	// 	vector<unsigned int> block = messages[0];
	// 	vector<unsigned int> otherBlock = messages[1];

	// 	vector<unsigned int> rs = IV[0];
	// 	rs = compute_hash_step(rs, block, 0);
	// 	rs = compute_hash_step(rs, block, 1);
	// 	rs = compute_hash_step(rs, block, 2);
	// 	rs = compute_hash_step(rs, block, 3);
	// 	rs = compute_hash_step(rs, block, 4);
	// 	rs = compute_hash_step(rs, block, 5);
	// 	rs = compute_hash_step(rs, block, 6);
	// 	rs = compute_hash_step(rs, block, 7);
	// 	rs = compute_hash_step(rs, block, 8);
	// 	rs = compute_hash_step(rs, block, 9);
	// 	rs = compute_hash_step(rs, block, 10);
	// 	rs = compute_hash_step(rs, block, 11);
	// 	rs = compute_hash_step(rs, block, 12);
	// 	rs = compute_hash_step(rs, block, 13);
	// 	rs = compute_hash_step(rs, block, 14);
	// 	rs = compute_hash_step(rs, block, 15);
	// 	rs = compute_hash_step(rs, block, 16);
	// 	rs = compute_hash_step(rs, block, 17);
	// 	rs = compute_hash_step(rs, block, 18);
	// 	rs = compute_hash_step(rs, block, 19);
	// 	rs = compute_hash_step(rs, block, 20);
	// 	rs = compute_hash_step(rs, block, 21);
	// 	rs = compute_hash_step(rs, block, 22);

	// 	rs = IV[1];
	// 	rs = compute_hash_step(rs, otherBlock, 0);
	// 	rs = compute_hash_step(rs, otherBlock, 1);
	// 	rs = compute_hash_step(rs, otherBlock, 2);
	// 	rs = compute_hash_step(rs, otherBlock, 3);
	// 	rs = compute_hash_step(rs, otherBlock, 4);
	// 	rs = compute_hash_step(rs, otherBlock, 5);
	// 	rs = compute_hash_step(rs, otherBlock, 6);
	// 	rs = compute_hash_step(rs, otherBlock, 7);
	// 	rs = compute_hash_step(rs, otherBlock, 8);
	// 	rs = compute_hash_step(rs, otherBlock, 9);
	// 	rs = compute_hash_step(rs, otherBlock, 10);
	// 	rs = compute_hash_step(rs, otherBlock, 11);
	// 	rs = compute_hash_step(rs, otherBlock, 12);
	// 	rs = compute_hash_step(rs, otherBlock, 13);
	// 	rs = compute_hash_step(rs, otherBlock, 14);
	// 	rs = compute_hash_step(rs, otherBlock, 15);
	// 	rs = compute_hash_step(rs, otherBlock, 16);
	// 	rs = compute_hash_step(rs, otherBlock, 17);
	// 	rs = compute_hash_step(rs, otherBlock, 18);
	// 	rs = compute_hash_step(rs, otherBlock, 19);
	// 	rs = compute_hash_step(rs, otherBlock, 20);
	// 	rs = compute_hash_step(rs, otherBlock, 21);
	// 	rs = compute_hash_step(rs, otherBlock, 22);
	// }

	return 0;
}