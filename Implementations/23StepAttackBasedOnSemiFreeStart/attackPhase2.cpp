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

void setArrayAValue(vector<unsigned int> &A, int pos, unsigned int val) {
	A[pos+7] = val;
	return;
}

unsigned int getArrayAValue(vector<unsigned int> &A, int pos) {
	return A[pos+7];
}

unsigned int getAAtStep(vector<unsigned int> &A, int step) {
	return getArrayAValue(A, step);
}

unsigned int getBAtStep(vector<unsigned int> &A, int step) {
	return getAAtStep(A, step-1);
}

unsigned int getCAtStep(vector<unsigned int> &A, int step) {
	return getBAtStep(A, step-1);
}

unsigned int getDAtStep(vector<unsigned int> &A, int step) {
	return getCAtStep(A, step-1);
}

unsigned int getEAtStep(vector<unsigned int> &A, int step) {
	return getArrayAValue(A, step-4) + getArrayAValue(A, step) - EP0(getArrayAValue(A, step-1)) - MAJ(getArrayAValue(A, step-1), getArrayAValue(A, step-2), getArrayAValue(A, step-3));
}

unsigned int getFAtStep(vector<unsigned int> &A, int step) {
	return getEAtStep(A, step-1);
}

unsigned int getGAtStep(vector<unsigned int> &A, int step) {
	return getFAtStep(A, step-1);
}

unsigned int getHAtStep(vector<unsigned int> &A, int step) {
	return getGAtStep(A, step-1);
}

// converts current state to representation in As
vector<unsigned int> convertToOneVariable(vector<unsigned int> &registers) {
	vector<unsigned int> A(24);
	setArrayAValue(A, 0, registers[0]);
	setArrayAValue(A, -1, registers[1]);
	setArrayAValue(A, -2, registers[2]);
	setArrayAValue(A, -3, registers[3]);
	setArrayAValue(A, -4, registers[4] - getArrayAValue(A, 0) + EP0(getArrayAValue(A, -1)) + MAJ(getArrayAValue(A, -1),getArrayAValue(A, -2),getArrayAValue(A, -3)));
	setArrayAValue(A, -5, registers[5] - getArrayAValue(A, -1) + EP0(getArrayAValue(A, -2)) + MAJ(getArrayAValue(A, -2),getArrayAValue(A, -3),getArrayAValue(A, -4)));
	setArrayAValue(A, -6, registers[6] - getArrayAValue(A, -2) + EP0(getArrayAValue(A, -3)) + MAJ(getArrayAValue(A, -3),getArrayAValue(A, -4),getArrayAValue(A, -5)));
	setArrayAValue(A, -7, registers[7] - getArrayAValue(A, -3) + EP0(getArrayAValue(A, -4)) + MAJ(getArrayAValue(A, -4),getArrayAValue(A, -5),getArrayAValue(A, -6)));
	return A;
}

void setArrayAValuesGivenState(vector<unsigned int> & registers, int step, vector<unsigned int> &A) {
	setArrayAValue(A, step-0, registers[0]);
	setArrayAValue(A, step-1, registers[1]);
	setArrayAValue(A, step-2, registers[2]);
	setArrayAValue(A, step-3, registers[3]);
	setArrayAValue(A, step-4, registers[4] - getArrayAValue(A, step-0) + EP0(getArrayAValue(A, step-1)) + MAJ(getArrayAValue(A, step-1),getArrayAValue(A, step-2),getArrayAValue(A, step-3)));
	setArrayAValue(A, step-5, registers[5] - getArrayAValue(A, step-1) + EP0(getArrayAValue(A, step-2)) + MAJ(getArrayAValue(A, step-2),getArrayAValue(A, step-3),getArrayAValue(A, step-4)));
	setArrayAValue(A, step-6, registers[6] - getArrayAValue(A, step-2) + EP0(getArrayAValue(A, step-3)) + MAJ(getArrayAValue(A, step-3),getArrayAValue(A, step-4),getArrayAValue(A, step-5)));
	setArrayAValue(A, step-7, registers[7] - getArrayAValue(A, step-3) + EP0(getArrayAValue(A, step-4)) + MAJ(getArrayAValue(A, step-4),getArrayAValue(A, step-5),getArrayAValue(A, step-6)));
	return;
}

void fillStepOfTransit(vector<unsigned int> A, vector<vector<vector<unsigned int> > > &transitRegisters, int i) {
	transitRegisters[0][i][0] = getAAtStep(A, i);
	transitRegisters[0][i][1] = getBAtStep(A, i);
	transitRegisters[0][i][2] = getCAtStep(A, i);
	transitRegisters[0][i][3] = getDAtStep(A, i);
	transitRegisters[0][i][4] = getEAtStep(A, i);
	transitRegisters[0][i][5] = getFAtStep(A, i);
	transitRegisters[0][i][6] = getGAtStep(A, i);
	transitRegisters[0][i][7] = getHAtStep(A, i);
}

void fillTrasitregisters(vector<unsigned int> A, vector<vector<vector<unsigned int> > > &transitRegisters) {
	for (int i=0; i<11; i++) {
		fillStepOfTransit(A, transitRegisters, i);
	}
	for (int i = 0; i < 8; ++i) {
		transitRegisters[1][0][i] = transitRegisters[0][0][i];
	}
	return;
}

void fillForward(vector<vector<vector<unsigned int> > > &transitRegisters, vector<vector<unsigned int> > &messages, unsigned int alpha, unsigned int beta, unsigned int gamma, unsigned int epsilon) {
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
	return;
}

void expandMessagesTill17(vector<vector<unsigned int> > &messages) {
	for (int i = 16; i < 18; ++i)	{
		messages[0][i] = SSIG1(messages[0][i-2]) + messages[0][i-7] + SSIG0(messages[0][i-15]) + messages[0][i-16];
		messages[1][i] = SSIG1(messages[1][i-2]) + messages[1][i-7] + SSIG0(messages[1][i-15]) + messages[1][i-16];
	}
	return;
}

bool valid(vector<unsigned int> message, unsigned int alpha, unsigned int beta, unsigned int gamma, unsigned int epsilon) {
	unsigned int W16 = message[16];
	unsigned int W17 = message[17];

	unsigned int eq1 = SSIG1(W16+1)-SSIG1(W16)-EP1(epsilon-1)+EP1(epsilon)-CH(epsilon-1, 0, gamma+1)+CH(epsilon, -1, gamma+1);
	unsigned int eq2 = SSIG1(W17-1)-SSIG1(W17)-CH(beta, epsilon-1, 0)+CH(beta, epsilon, -1);

	if (eq1==0 && eq2==0) {
		return true;
	}
	return false;
}

unsigned int F_function(vector<unsigned int> &A, int i) {
	unsigned int k[64] = {
		0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,
		0x923f82a4,0xab1c5ed5,0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
		0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,0xe49b69c1,0xefbe4786,
		0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
		0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,
		0x06ca6351,0x14292967,0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
		0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,0xa2bfe8a1,0xa81a664b,
		0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
		0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,
		0x5b9cca4f,0x682e6ff3,0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
		0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
	};
	return getHAtStep(A, i) + EP1(getEAtStep(A, i)) + CH(getEAtStep(A, i), getFAtStep(A, i), getGAtStep(A, i)) + k[i] + EP0(getAAtStep(A, i)) + MAJ(getAAtStep(A, i), getBAtStep(A, i), getCAtStep(A, i)) - getAAtStep(A, i-7);
}

int main(int argc, char* argv[]) {
	vector<unsigned int> realInitialRegisters(8);
	unsigned int  H0 = 0x6a09e667;
	unsigned int  H1 = 0xbb67ae85;
	unsigned int  H2 = 0x3c6ef372;
	unsigned int  H3 = 0xa54ff53a;
	unsigned int  H4 = 0x510e527f;
	unsigned int  H5 = 0x9b05688c;
	unsigned int  H6 = 0x1f83d9ab;
	unsigned int  H7 = 0x5be0cd19;
	realInitialRegisters[0]=H0;
	realInitialRegisters[1]=H1;
	realInitialRegisters[2]=H2;
	realInitialRegisters[3]=H3;
	realInitialRegisters[4]=H4;
	realInitialRegisters[5]=H5;
	realInitialRegisters[6]=H6;
	realInitialRegisters[7]=H7;

	// to be filled by phase1
	unsigned int  alpha = 1737118951;
	unsigned int  beta = 268443634;
	unsigned int  epsilon = 68804737;
	unsigned int  gamma = 248592228;

	vector<unsigned int> A = convertToOneVariable(realInitialRegisters);
	
	vector<vector<vector<unsigned int> > > transitRegisters(2, vector<vector<unsigned int> > (23, vector<unsigned int> (8, 0)));
	vector<vector<unsigned int> > messages(2, vector<unsigned int> (19, 0));
	initializeEleventhStepOfMessages(transitRegisters, alpha, beta, epsilon, gamma);
	setArrayAValuesGivenState(transitRegisters[0][11], 11, A);

	for (int i = 0; i < A.size(); ++i) {
		cout << i << " " << A[i] << endl;
	}
	
	//this function fills the messages till 15th message
	fillForward(transitRegisters, messages, alpha, beta, gamma, epsilon);

	printMessages(messages);

	// keep on trying with different W0, W1, W2
	int i=0;
	while(true) {
		// if(i%1000==0) {
		// 	cout << i << endl;
		// }
		messages[0][0] = rand();
		messages[0][1] = rand();
		messages[0][2] = rand();

		setArrayAValue(A, 1, messages[0][0] + F_function(A, 0) + getArrayAValue(A, -7));
		setArrayAValue(A, 2, messages[0][1] + F_function(A, 1) + getArrayAValue(A, -6));
		setArrayAValue(A, 3, messages[0][2] + F_function(A, 2) + getArrayAValue(A, -5));

		for (int i = 3; i < 11; ++i) {
			messages[0][i] = getArrayAValue(A, i+1) - F_function(A, i) - getArrayAValue(A, i-7);
		}
		expandMessagesTill17(messages);
		if (valid(messages[0], alpha, beta, gamma, epsilon)) {
			setMessageTwoWithOne(messages);
			fillTrasitregisters(A, transitRegisters);
			break;
		}
		i++;
	}

	for (int i = 0; i < A.size(); ++i) {
		cout << i << " " << A[i] << endl;
	}

	printMessages(messages);

	cout << "alpha = " << alpha << endl;
	cout << "beta = " << beta << endl;
	cout << "epsilon = " << epsilon << endl;
	cout << "gamma = " << gamma << endl;
	cout << "W16 = " << messages[0][16] << endl;
	cout << "W17 = " << messages[0][17] << endl;

	vector<vector<unsigned int> > IV (2, vector<unsigned int> (8));
	// goBackWithFixedMessages(0, transitRegisters, messages, 0);

	// for (int i = 0; i < 16; ++i) {
	// 	messages[0][1] = wToSetA(0, transitRegisters[1])
	// }


	printMessages(messages);
	cout << "IV is : ";
	for (int i=0; i<8; i++) {
		IV[0][i] = transitRegisters[0][0][i];
		cout << transitRegisters[0][0][i] << " ";
	}
	cout << endl;

	for (int j = 0; j < 17; ++j) {
		for (int i = 0; i < 8; ++i) {
			IV[0][i] = transitRegisters[0][j][i];
		}
		compute_hash_step(IV[0], messages[0], j);
	}
	return 0;
}