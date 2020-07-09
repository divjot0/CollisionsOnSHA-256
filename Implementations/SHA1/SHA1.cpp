#include <iostream>
#include <sstream>
#include <bitset>
#include <vector>
#include <iomanip>
#include <cstring>

using namespace std;

// definitions of some basic functions
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define ROTLEFT(word,bits) (((word) << (bits)) | ((word) >> (32-(bits))))
#define ROTRIGHT(word,bits) (((word) >> (bits)) | ((word) << (32-(bits))))

// to print hex
string show_as_hex(unsigned int input)
{
	bitset<32> bs(input);
	unsigned n = (unsigned int)bs.to_ulong();

	stringstream sstream;
	sstream << std::hex << std::setw(8) << std::setfill('0') << n;
	string temp;
	sstream >> temp;

	return temp;
}

// fill in the state registers value at the start of computatuibn of the hash
void intializeRegisters(vector<unsigned int> &registers) {
	registers[0] = 0x67452301;
	registers[1] = 0xefcdab89;
	registers[2] = 0x98badcfe;
	registers[3] = 0x10325476;
	registers[4] = 0xc3d2e1f0;
	return;
}

//given a message of 16 words it sends back the corresponding message words
vector<unsigned int> messageToWords(vector<unsigned int> message) {
	vector<unsigned int> W;
	for (int i = 0; i < 16; ++i) {
		W.push_back(message[i]);
	}
	for (int i = 16; i < 80; ++i) {
		unsigned int w_i = W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16];
		w_i = ROTLEFT(w_i, 1);
		W.push_back(w_i);
	}
	return W;
}

//given initial state(registers and the expanded messagewords) compute the final set of registers
vector<unsigned int> finalRegisterValues(vector<unsigned int> registers, vector<unsigned int> W) {
	vector<unsigned int> currentRegisters(5, 0);
	for (int i = 0; i < 5; ++i) {
		currentRegisters[i] = registers[i];
	}
	for (int i = 0; i < 80; ++i) {
		unsigned int a = currentRegisters[0];
		unsigned int b = currentRegisters[1];
		unsigned int c = currentRegisters[2];
		unsigned int d = currentRegisters[3];
		unsigned int e = currentRegisters[4];

		unsigned int k_i;
		unsigned int f_i;
		if (i<20) {
			k_i = 0x5A827999;
			f_i = CH(b, c, d);
		}
		else if (i<40) {
			k_i = 0x6ED9EBA1;
			f_i = b ^ c ^ d;
		}
		else if (i<60) {
			k_i = 0x8F1BBCDC;
			f_i = MAJ(b, c, d);
		}
		else {
			k_i = 0xCA62C1D6;
			f_i = b ^ c ^ d;
		}

		currentRegisters[0] = e + ROTLEFT(a,5) + k_i + f_i + W[i];
		currentRegisters[1] = a;
		currentRegisters[2] = ROTRIGHT(b,2);
		currentRegisters[3] = c;
		currentRegisters[4] = d;
	}
	return currentRegisters;
}

//prints the set of registers at any time
void printRegisters(vector<unsigned int> registers) {
	for (int i = 0; i < 5; ++i) {
		cout << show_as_hex(registers[i]) << " ";
	}
	cout << endl;
	return;
}

void printBuckets(vector<vector<unsigned int> > &buckets) {
	for (int i = 0; i < buckets.size(); i++) {
		for (int j=0; j < 50; j++) {
			cout << buckets[i][j] << "|";
		}
		cout << 1 << endl;
	}
	return;
}



int main() {
	vector<unsigned int> registers(5, 0);
	intializeRegisters(registers);
	vector<unsigned int> message(16, 0);
	vector<unsigned int> W = messageToWords(message);
	registers = finalRegisterValues(registers, W);
	printRegisters(registers);
	// buckets
	vector<vector<unsigned int> > buckets(80, vector<unsigned int> (160*32, 0));
	// fillBucketsWithMessage();
	printBuckets(buckets);
	return 0;
}