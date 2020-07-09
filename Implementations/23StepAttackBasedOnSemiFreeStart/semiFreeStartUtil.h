#include <iostream>
#include <sstream>
#include <bitset>
#include <vector>
#include <iomanip>
#include <cstring>

// By: hak8or
// To compile: g++ -std=c++0x SHA_256_hak.cpp -o SHA_256_hak
// To run self test just run the following: SHA_256_hak
// To run and generate a hash run the following: SHA_256_hak stringtohashgoeshere

// Description: 
//	SHA-256 is one of the most common one way security hashes used in the wild,
//	SSL, SSH, PGP, and bitcoin all rely on this hash function. I had originally
//	hoped on doing SHA-3, but the standard is still not done, and the math behind
//	it seeems a bit over my head due to the lack of a "plain english" explanation
//	like NIST provided for SHA-2.
//
//	I find the coolest part of this is how the core of this can be simplified to
//	just rotating bits around, which isn't that difficult with logic gates and
//	whatnot on an FPGA. This being at its core just rotating bits mean it is
//	easy to impliment on a hardware level for even the most space conservative 
//	IC's or even in software on low power device.
//
// Implimentation:
//	This implimentation is for SHA-256, which generates a 256 bit hash for any
//	message up to a size of 2^64 bits (2^64 bits / 8 bit ASCII chars gives about
//	two quintillion characters).
//
//		Step 1)
//	Pre Processing, we take the message and add "padding" to it consisting of 1 
// 	and 0's so the message is in 512 bit incriments, or blocks. This implimentation
//	assumes that the message will be no inter than 55 characters as it does not
//	handle messages which require more than one 512 bit block for inter length
//	messages. Lastly, the initial hash values are set which will be used for
//	the hash computation.
//
//	Padding specifically consist of having your message, adding 0's to it so you
//	will have the current length be 448 bits int, and then adding 64 bits which
//	say how int the original message was, giving you a total of 512 bits.
//
//	Initilizing just means initializing variables used in the hash computation so
//	the first round of hashing does not start with 0's, which would make this
//	hashing broken security wise.
//
//		Step 2)
//	The actual hash computation. It is based on a message queue which sends 64
//	thirty two bit words to the next round, eight working variables with two
//	temporary variables, and 8 hash segments which will be combined at the end
//	to the 256 bit hash.
//

// Primary resource for getting an idea of what is going on: 
//	http://csrc.nist.gov/publications/fips/fips180-4/fips-180-4.pdf
// For more information:
//	http://tools.ietf.org/html/rfc6234
// Testing:
//	http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/SHA256.pdf
// Concrete Specification:
//	http://tools.ietf.org/html/rfc6234
//
// Useful links:
//	http://www.miniwebtool.com/hex-calculator
//	http://en.cppreference.com/w/cpp/language/ascii
//	https://github.com/adambard/learnxinyminutes-docs
//
// Should put this in a class someday instead of this mess of functions.
// If will use boost program options, put in a help option.

using namespace std;

// Converts the ASCII string to a binary representation.
vector<unsigned int> convert_to_binary(const string);

vector<unsigned int> convertMessageToWorkable(string message);

// Pads the messages to make sure they are a multiple of 512 bits.
vector<unsigned int> pad_to_512bits(const vector<unsigned int>);

// Changes the n 8 bit segments representing every ASCII character to 32 bit words.
vector<unsigned int> resize_block(vector<unsigned int>);

// The actual hash computing.
string compute_hash(const vector<unsigned int>);

unsigned int wToSetA(int step, unsigned int desiredA, vector<unsigned int> registers);
unsigned int wToSetE(int step, unsigned int desiredA, vector<unsigned int> registers);
void initializeEleventhStepOfMessages(vector<vector<vector<unsigned int> > > &transitRegisters, unsigned int alpha, unsigned int beta, unsigned int epsilon, unsigned int gamma);
void fillAllRegistersAtStep(int step, vector<vector<vector<unsigned int> > > &transitRegisters, vector<vector<unsigned int> > &messages);
unsigned int calculateT1(int step, vector<unsigned int> currentRegisters, unsigned int *k, vector<unsigned int> message);
unsigned int calculateT2(int step, vector<unsigned int> currentRegisters, unsigned int *k, vector<unsigned int> message);
void goBackOneStep(int step, vector<vector<vector<unsigned int> > > &transitRegisters, vector<vector<unsigned int> > &messages, unsigned int differenceBetweenMessages);
unsigned int dGivenElse(int step, vector<vector<unsigned int> > &registers, vector<unsigned int> &message);
unsigned int hGivenElse(int step, vector<vector<unsigned int> > &registers, vector<unsigned int> &message);
void fillInitialMessages(vector<vector<unsigned int> > &messages);
// step
vector<unsigned int> compute_hash_step(vector<unsigned int> registers, const vector<unsigned int> block, int step);

// Used for debugging. Decided to leave this in for if I ever will expand on this.
	string show_as_hex(unsigned int);
	void cout_block_state(vector<unsigned int>);
	string show_as_binary(unsigned int);
	const bool show_block_state_add_1 = 0;
	const bool show_distance_from_512bit = 0;
	const bool show_padding_results = false;
	const bool show_working_vars_for_t = 1;
	const bool show_T1_calculation = false;
	const bool show_T2_calculation = false;
	const bool show_hash_segments = false;
	const bool show_Wt = false;

// Taken from NIST spec. I find it amazing how this can all be done by just
// bit rotations.
	#define ROTRIGHT(word,bits) (((word) >> (bits)) | ((word) << (32-(bits))))
	#define SSIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
	#define SSIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))
	#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
	#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

// Supposed incorrect implimentation from NIST.
// BSIG0 is replaced with EP0 and BSIG1 is replaced with EP0 in the implimentation.
	#define BSIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
	#define BSIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))

// 
/* Function : main
The beginning of my program for this extra credit implimentation of SHA-256.
First it checks to see if testing is enabled by lack of command line arguments,
causing it to hash "abc" and comparing it to a hard coded hash I know is correct.
1) Converts the ASCII string into n 8 bit segments representing the ASCII value
of each character.
2) Pads the message so it will be 512 bits int.
3) Combines seperate 8 bit ASCII values to 32 bit words.
4) Computes the hash.
5) If testing, compare result with expected result. Yell if result is not as
   expected.
Notes: Should be using Boost.Program_options here.
Input : Message as a command line argument or nothing indicating to test.
Output : Encrypted SHA-256 hash and debugging information if requested.
Errors : The message length should be no more than 55 characters.*/
// int main(int argc, char* argv[]) {
// 	vector<unsigned int> registers(8);
// 	unsigned int static H0 = 0x6a09e667;
// 	unsigned int static H1 = 0xbb67ae85;
// 	unsigned int static H2 = 0x3c6ef372;
// 	unsigned int static H3 = 0xa54ff53a;
// 	unsigned int static H4 = 0x510e527f;
// 	unsigned int static H5 = 0x9b05688c;
// 	unsigned int static H6 = 0x1f83d9ab;
// 	unsigned int static H7 = 0x5be0cd19;
// 	registers[0]=H0;
// 	registers[1]=H1;
// 	registers[2]=H2;
// 	registers[3]=H3;
// 	registers[4]=H4;
// 	registers[5]=H5;
// 	registers[6]=H6;
// 	registers[7]=H7;

// 	// to be filled by phase1
// 	unsigned int static alpha = 0x00b321e3;
// 	unsigned int static beta = 0xfcffe000;
// 	unsigned int static epsilon = 0x7f5f7200;
// 	unsigned int static gamma = 0x0000017c;
// 	unsigned int static W16 = 1;
// 	unsigned int static W17 = 1;

// 	vector<vector<vector<unsigned int> > > transitRegisters(2, vector<vector<unsigned int> > (23, vector<unsigned int> (8, 0)));

// 	vector<vector<unsigned int> > messages(2, vector<unsigned int> (19, 0));
// 	initializeEleventhStepOfMessages(transitRegisters, alpha, beta, epsilon, gamma);

// 	messages[0][11] = wToSetA(11, alpha, transitRegisters[0][11]);
// 	messages[1][11] = wToSetA(11, alpha, transitRegisters[1][11]);
// 	fillAllRegistersAtStep(12, transitRegisters, messages);

// 	messages[0][12] = wToSetE(12, beta, transitRegisters[0][12]);
// 	messages[1][12] = wToSetE(12, beta, transitRegisters[1][12]);
// 	fillAllRegistersAtStep(13, transitRegisters, messages);

// 	messages[0][13] = wToSetE(13, -1, transitRegisters[0][13]);
// 	messages[1][13] = wToSetE(13, -2, transitRegisters[1][13]);
// 	fillAllRegistersAtStep(14, transitRegisters, messages);

// 	messages[0][14] = wToSetE(14, 0, transitRegisters[0][14]);
// 	messages[1][14] = wToSetE(14, 0, transitRegisters[1][14]);
// 	fillAllRegistersAtStep(15, transitRegisters, messages);

// 	messages[0][15] = wToSetE(15, -2, transitRegisters[0][15]);
// 	messages[1][15] = wToSetE(15, -2, transitRegisters[1][15]);
// 	fillAllRegistersAtStep(16, transitRegisters, messages);

// 	messages[0][16] = W16;
// 	messages[1][16] = W16-1;
	
// 	messages[0][17] = W17;
// 	messages[1][17] = W17+1;

// 	goBackOneStep(11, transitRegisters, messages, -1);
// 	goBackOneStep(10, transitRegisters, messages, 1);
// 	for(int j=9; j>1; j--) {
// 		goBackOneStep(j, transitRegisters, messages, 0);
// 	}

// 	fillInitialMessages(messages);

// 	return 0;
// }

void fillInitialMessages(vector<vector<unsigned int> > &messages) {
	messages[0][1] = messages[0][17] - SSIG1(messages[0][15]) - messages[0][10] - SSIG0(messages[0][2]);
	messages[1][1] = messages[1][17] - SSIG1(messages[1][15]) - messages[1][10] - SSIG0(messages[1][2]);

	messages[0][0] = messages[0][16] - SSIG1(messages[0][14]) - messages[0][9] - SSIG0(messages[0][1]);
	messages[1][0] = messages[1][16] - SSIG1(messages[1][14]) - messages[1][9] - SSIG0(messages[1][1]);

	return;
}

void setMessageTwoWithOne(vector<vector<unsigned int> > &messages) {
	for (int i = 0; i < 10; ++i) {
		if (i!=9) {
			messages[1][i] = messages[0][i];
		}
	}
	messages[1][9] = messages[0][9]+1;
	messages[1][10] = messages[0][10]-1;

	return;
}

void printMessages(vector<vector<unsigned int> > &messages) {
	cout << "Message 1 : ";
	for (int i = 0; i < 16; ++i) {
		cout << messages[0][i] << " ";
	}
	cout << endl << "Message 2 : ";
	for (int i = 0; i < 16; ++i) {
		cout << messages[1][i] << " ";
	}
	cout << endl;
	return;
}

// step from where we need to go back
// difference in messages to where we go back
void goBackOneStep(int step, vector<vector<vector<unsigned int> > > &transitRegisters, vector<vector<unsigned int> > &messages, unsigned int differenceBetweenMessages) {
	for (int i = 0; i < 8; ++i) {
		if (i!=0 && i!=4) {
			transitRegisters[0][step-1][i-1] = transitRegisters[0][step][i];
			transitRegisters[1][step-1][i-1] = transitRegisters[1][step][i];
		}
	}
	
	transitRegisters[0][step-1][7] = 0; // randomly chosen
	messages[0][step-1] = wToSetA(step-1, transitRegisters[0][step][0], transitRegisters[0][step-1]);
	transitRegisters[0][step-1][3] = dGivenElse(step-1, transitRegisters[0], messages[0]);

	messages[1][step-1] = messages[0][step-1]-differenceBetweenMessages;
	transitRegisters[1][step-1][7] = hGivenElse(step-1, transitRegisters[1], messages[1]);
	transitRegisters[1][step-1][3] = dGivenElse(step-1, transitRegisters[1], messages[1]);

	return;
}

void initializeEleventhStepOfMessages(vector<vector<vector<unsigned int> > > &transitRegisters, unsigned int alpha, unsigned int beta, unsigned int epsilon, unsigned int gamma) {
	transitRegisters[0][11][0] = alpha;
	transitRegisters[0][11][1] = -1;
	transitRegisters[0][11][2] = alpha;
	transitRegisters[0][11][3] = alpha;
	transitRegisters[0][11][4] = epsilon;
	transitRegisters[0][11][5] = -1;
	transitRegisters[0][11][6] = gamma+1;
	transitRegisters[0][11][7] = gamma;

	transitRegisters[1][11][0] = alpha;
	transitRegisters[1][11][1] = 0;
	transitRegisters[1][11][2] = alpha;
	transitRegisters[1][11][3] = alpha;
	transitRegisters[1][11][4] = epsilon-1;
	transitRegisters[1][11][5] = 0;
	transitRegisters[1][11][6] = gamma+1;
	transitRegisters[1][11][7] = gamma;

	return;
}

void fillAllRegistersAtStep(int step, vector<vector<vector<unsigned int> > > &transitRegisters, vector<vector<unsigned int> > &messages) {
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

	for (int i = 0; i < 8; ++i) {
		if (i!=0 && i!=4) {
			transitRegisters[0][step][i] = transitRegisters[0][step-1][i-1];
			transitRegisters[1][step][i] = transitRegisters[1][step-1][i-1];
		}
	}

	unsigned int T1 = calculateT1(step-1, transitRegisters[0][step-1], k, messages[0]);
	unsigned int T2 = calculateT2(step-1, transitRegisters[0][step-1], k, messages[0]);
	transitRegisters[0][step][0] = T1 + T2;
	transitRegisters[0][step][4] = transitRegisters[0][step-1][3] + T1;

	T1 = calculateT1(step-1, transitRegisters[1][step-1], k, messages[1]);
	T2 = calculateT2(step-1, transitRegisters[1][step-1], k, messages[1]);
	transitRegisters[1][step][0] = T1 + T2;
	transitRegisters[1][step][4] = transitRegisters[1][step-1][3] + T1;

	return;
}

unsigned int calculateT1(int step, vector<unsigned int> currentRegisters, unsigned int *k, vector<unsigned int> message) {
	return currentRegisters[7] + EP1(currentRegisters[4]) + CH(currentRegisters[4], currentRegisters[5], currentRegisters[6]) + k[step] + message[step];
}

unsigned int calculateT2(int step, vector<unsigned int> currentRegisters, unsigned int *k, vector<unsigned int> message) {
	return EP0(currentRegisters[0]) + MAJ(currentRegisters[0], currentRegisters[1], currentRegisters[2]);
}

//uses the step we were previously at
unsigned int wToSetA(int step, unsigned int desiredA, vector<unsigned int> registers) {
	unsigned int K[64] = {
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
	return desiredA - EP0(registers[0]) - MAJ(registers[0], registers[1], registers[2]) - EP1(registers[4]) - CH(registers[4], registers[5], registers[6]) - registers[7] - K[step];
}

unsigned int wToSetE(int step, unsigned int desiredE, vector<unsigned int> registers) {
	unsigned int K[64] = {
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
	return desiredE - registers[3] - EP1(registers[4]) - CH(registers[4], registers[5], registers[6]) - registers[7] - K[step];
}

unsigned int dGivenElse(int step, vector<vector<unsigned int> > &registers, vector<unsigned int> &message) {
	unsigned int K[64] = {
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
	return registers[step+1][4] - EP1(registers[step][4]) - CH(registers[step][4], registers[step][5], registers[step][6]) - registers[step][7] - K[step] - message[step];
}

unsigned int hGivenElse(int step, vector<vector<unsigned int> > &registers, vector<unsigned int> &message) {
	unsigned int K[64] = {
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
	return registers[step+1][0] - EP1(registers[step][4]) - CH(registers[step][4], registers[step][5], registers[step][6]) - K[step] - message[step] - EP0(registers[step][0]) - MAJ(registers[step][0], registers[step][1], registers[step][2]);
}

vector<unsigned int> convertMessageToWorkable(string message) {
	// First convert this guy to a vector of strings representing 8 bit variables.
	vector<unsigned int> block = convert_to_binary(message);

	// Pad it so that the message will be a full 512 bits int.
	block = pad_to_512bits(block);

	// Combine the seperate 8 bit sections into single 32 bit sections.
	block = resize_block(block);

	return block;
}

/* Function : resize_block
Resize the blocks from 64 8 bit sections to 16 32 bit sections.
Input : Vector of individual 8 bit ascii values
Output : Vector of 32 bit words which are combined ascii values.
Errors : The message length should be no more than 55 characters.*/
vector<unsigned int> resize_block(vector<unsigned int> input)
{
	vector<unsigned int> output(16);

	// Loop through the 64 sections by 4 steps and merge those 4 sections.
	for(int i = 0; i < 64; i = i + 4)
	{
		// Lets make a big 32 bit section first.
		bitset<32> temp(0);

		// Shift the blocks to their assigned spots and OR them with the original
		// to combine them.
		temp = (unsigned int) input[i] << 24;
		temp |= (unsigned int) input[i + 1] << 16;
		temp |= (unsigned int) input[i + 2] << 8;
		temp |= (unsigned int) input[i + 3];

		// Puts the new 32 bit word into the correct output array location.
		output[i/4] = (unsigned int)temp.to_ulong();
	}

	return output;
}

/* Function : cout_block_state
Shows the current contents of all the blocks in binary. This is used solely
for debugging and has no actual use in the implimentation.
Input : Vector of the current blocks.
Output : Contents of each block in binary and hex.*/
void cout_block_state(vector<unsigned int> block)
{
	cout << "---- Current State of block ----\n"; 
	for (int i = 0; i < block.size(); i++)
	{
		cout << "block[" << i << "] binary: " << show_as_binary(block[i])
			<< "     hex y: 0x" << show_as_hex(block[i]) << endl;
	}
}

/* Function : show_as_hex
Shows the current contents of the block in hex.
This is mainly to avoid having to change the stream from hex to dec and back
again in the cout statement which would have been error prone.
Input : A 32 or less bit block
Output : Contents of the block in hex as a string. */
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



/* Function : show_as_binary
Shows the current contents of the block in hex.
This is mainly to avoid having to change the stream from hex to dec and back
again in the cout statement which would have been error prone.
Input : A 32 or less bit block
Output : Contents of the block in binary as a stromg. */
string show_as_binary(unsigned int input)
{
	bitset<8> bs(input);
	return bs.to_string();
}

/* Function : convert_to_binary
Takes the string and convers all characters to their ASCII Binary equivilents.
Input : A string of any length
Output : A vector consisting of one 8 bit value per ASCII character. */
vector<unsigned int> convert_to_binary(const string input)
{
	// Lets make a vector to hold all the ASCII character values.
	vector<unsigned int> block;

	// For each character, convert the ASCII chararcter to its binary
	// representation.
	for (int i = 0; i < input.size(); ++i)
	{
		// Make a temporary variable called B to store the 8 bit pattern
		// for the ASCII value.
		bitset<8> b(input.c_str()[i]);

		// Add that 8 bit pattern into the block.
		block.push_back((unsigned int)b.to_ulong());
	}

	return block;
}

/* Function : pad_to_512bits
Takes the vector of ASCII values in binary and pad it so that there will be a
total of 512 bits.
Padding specifically consist of having your message, adding 1, and adding 0's to
it so you will have the current length be 448 bits int, and then adding 64 bits
which say how int the original message was, giving you a total of 512 bits.
Input : The message in the form of a vector containing 8 bit binary ASCII values.
Output : A padded vector consisting of one 8 bit value per ASCII character. */
vector<unsigned int> pad_to_512bits(vector<unsigned int> block)
{
	// Variable names as per NIST.

	// l will represent the length of the message in terms of bits, with each
	// block representing one 8 bit ascii character.
	int l = block.size() * 8;
	
	// Equation for padding is l + 1 + k = 448 mod 512
	// Simplified to: l + 1 + k = 448
	//		  448 - 1 - l = k
	//		  447 - l = k
	// l = length of message in bits
	// k = how much zero's to add so new message will be a multiple of 512.
	int k = 447 - l;

	// First add in another 8 bit block with the first bit set to 1
	if(show_block_state_add_1)
		cout_block_state(block);

	unsigned int t1 = 0x80;
	block.push_back(t1);

	if(show_block_state_add_1)
		cout_block_state(block);

	// We just added in 7 zero's, so subtract 7 from k.
	k = k - 7;

	// Find how far away we are from a 512 bit message. Just debugging.
	if (show_distance_from_512bit)
	{
		cout << "l: " << l << endl;
		cout << "k: " << k + 7 << endl; // Plus 7 so this follows the paper.
	}

	// Just debugging.
	if (show_distance_from_512bit)
		cout << "adding " << k / 8 << " empty eight bit blocks!\n";

	// Add 8 bit blocks containing zero's
	for(int i = 0; i < k / 8; i++)
		block.push_back(0x00000000);

	// We are now at 488 bits out of 512. We need to add l in the binary
	// form of eight 8 bit blocks.
	bitset<64> big_64bit_blob(l);
	if (show_distance_from_512bit)
		cout << "l in a 64 bit binary blob: \n\t" << big_64bit_blob << endl;
	
	// Split up that 64 bit blob into 8 bit sections.
	string big_64bit_string = big_64bit_blob.to_string();
	
	// Push the first block into the 56th position.
	bitset<8> temp_string_holder1(big_64bit_string.substr(0,8));
	block.push_back((unsigned int)temp_string_holder1.to_ulong());

	// Push all the rest of the 8 bit blocks in.
	for(int i = 8; i < 63; i=i+8)
	{
		bitset<8> temp_string_holder2(big_64bit_string.substr(i,8));
		block.push_back((unsigned int)temp_string_holder2.to_ulong());
	}

	// Just display everything so we know what is going on.
	if (show_padding_results)
	{
		cout << "Current 512 bit pre-processed hash in binary: \n";
			for(int i = 0; i < block.size(); i=i+4)
				cout << i << ": " << show_as_binary(block[i]) << "     " 
				     << i + 1 << ": " << show_as_binary(block[i+1]) << "     " 
				     << i + 2 << ": " << show_as_binary(block[i+2]) << "     " 
				     << i + 3 << ": " << show_as_binary(block[i+3]) << endl;

		cout << "Current 512 bit pre-processed hash in hex: \n";
		for(int i = 0; i < block.size(); i=i+4)
			cout << i << ": " << "0x" + show_as_hex(block[i]) << "     " 
			     << i + 1 << ": " << "0x" + show_as_hex(block[i+1]) << "     " 
			     << i + 2 << ": " << "0x" + show_as_hex(block[i+2]) << "     " 
			     << i + 3 << ": " << "0x" + show_as_hex(block[i+3]) << endl;	
	}
	return block;
}

/* Function : compute_hash
Note:	This is a very int function because a large portion of it is solely
		debugging assistance. Originally planned to be purged of those but in
		the end decided to keep them so it will be much easier to debug this
		in the future.
Input : The 512 bit padded message as a vector containing 8 bit binary ASCII values.
Output : A string representing the hash. */
string compute_hash(const vector<unsigned int> block)
{
	// Taken from the spec
	//   SHA-224 and SHA-256 use the same sequence of sixty-four constant
	//   32-bit words, K0, K1, ..., K63.  These words represent the first 32
	//   bits of the fractional parts of the cube roots of the first sixty-
	//   four prime numbers.
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

	// Initial Hash Values, first thirty-two bits of the fractional parts of
	// the square roots of the first eight prime numbers. 
	unsigned int static H0 = 0x6a09e667;
	unsigned int static H1 = 0xbb67ae85;
	unsigned int static H2 = 0x3c6ef372;
	unsigned int static H3 = 0xa54ff53a;
	unsigned int static H4 = 0x510e527f;
	unsigned int static H5 = 0x9b05688c;
	unsigned int static H6 = 0x1f83d9ab;
	unsigned int static H7 = 0x5be0cd19;

	unsigned int W[64];

	for(int t = 0; t <= 15; t++)
	{
		W[t] = block[t] & 0xFFFFFFFF;

		if (show_Wt)
			cout << "W[" << t << "]: 0x" << show_as_hex(W[t]) << endl;
	}

	for(int t = 16; t <= 63; t++)
	{
		// Also taken from spec.
		W[t] = SSIG1(W[t-2]) + W[t-7] + SSIG0(W[t-15]) + W[t-16];

		// Have to make sure we are still dealing with 32 bit numbers.
		W[t] = W[t] & 0xFFFFFFFF;

		if (show_Wt)
			cout << "W[" << t << "]: " << W[t];
	}

	unsigned int temp1;
	unsigned int temp2;
	unsigned int a = H0;
	unsigned int b = H1;
	unsigned int c = H2;
	unsigned int d = H3;
	unsigned int e = H4;
	unsigned int f = H5;
	unsigned int g = H6;
	unsigned int h = H7;

	if(show_working_vars_for_t)
		cout << "         A        B        C        D        "
		     << "E        F        G        H        T1       T2\n";

	for( int t = 0; t < 64; t++)
	{
		// Seems the Official spec is wrong!? BSIG1 is incorrect.
		// Replacing BSIG1 with EP1.
		temp1 = h + EP1(e) + CH(e,f,g) + k[t] + W[t];
		if ((t == 20) & show_T1_calculation)
		{
			cout << "h: 0x" << hex << h << "  dec:" << dec << h 
			     << "  sign:" << dec << (int)h << endl;
			cout << "EP1(e): 0x" << hex << EP1(e) << "  dec:" 
			     << dec << EP1(e) << "  sign:" << dec << (int)EP1(e) 
			     << endl;
			cout << "CH(e,f,g): 0x" << hex << CH(e,f,g) << "  dec:" 
			     << dec << CH(e,f,g) << "  sign:" << dec 
			     << (int)CH(e,f,g) << endl;
			cout << "k[t]: 0x" << hex << k[t] << "  dec:" << dec 
			     << k[t] << "  sign:" << dec << (int)k[t] << endl;
			cout << "W[t]: 0x" << hex << W[t] << "  dec:" << dec 
			     << W[t] << "  sign:" << dec << (int)W[t] << endl;
			cout << "temp1: 0x" << hex << temp1 << "  dec:" << dec 
			     << temp1 << "  sign:" << dec << (int)temp1 << endl;
		}
		

		// Seems the Official spec is wrong!? BSIG0 is incorrect.
		// Replacing BSIG0 with EP0.
		temp2 = EP0(a) + MAJ(a,b,c);

		// Shows the variables and operations for getting T2.
		if ((t == 20) & show_T2_calculation)
		{
			cout << "a: 0x" << hex << a << "  dec:" << dec << a 
			     << "  sign:" << dec << (int)a << endl;
			cout << "b: 0x" << hex << b << "  dec:" << dec << b 
			     << "  sign:" << dec << (int)b << endl;
			cout << "c: 0x" << hex << c << "  dec:" << dec << c 
			     << "  sign:" << dec << (int)c << endl;
			cout << "EP0(a): 0x" << hex << EP0(a) << "  dec:" 
			     << dec << EP0(a) << "  sign:" << dec << (int)EP0(a) 
			     << endl;
			cout << "MAJ(a,b,c): 0x" << hex << MAJ(a,b,c) << "  dec:" 
			     << dec << MAJ(a,b,c) << "  sign:" << dec 
			     << (int)MAJ(a,b,c) << endl;
			cout << "temp2: 0x" << hex << temp2 << "  dec:" << dec 
			     << temp2 << "  sign:" << dec << (int)temp2 << endl;
		}

		// Do the working variables operations as per NIST.
		h = g;
		g = f;
		f = e;
		e = (d + temp1) & 0xFFFFFFFF; // Makes sure that we are still using 32 bits.
		d = c;
		c = b;
		b = a;
		a = (temp1 + temp2) & 0xFFFFFFFF; // Makes sure that we are still using 32 bits.

		// Shows the contents of each working variable for the turn T.
		if (show_working_vars_for_t)
		{
			cout << "t= " << t << " ";
			cout << show_as_hex (a) << " " << show_as_hex (b) << " " 
			     << show_as_hex (c) << " " << show_as_hex (d) << " " 
			     << show_as_hex (e) << " " << show_as_hex (f) << " "
			     << show_as_hex (g) << " " << show_as_hex (h) << " " 
			     << endl;
		}
	}

	// Shows the contents of each hash segment.
	if(show_hash_segments)
	{
		cout << "H0: " << show_as_hex (H0) << " + " << show_as_hex (a)
			 << " " << show_as_hex (H0 + a) << endl;
		cout << "H1: " << show_as_hex (H1) << " + " << show_as_hex (b)
			 << " " << show_as_hex (H1 + b) << endl;
		cout << "H2: " << show_as_hex (H2) << " + " << show_as_hex (c)
			 << " " << show_as_hex (H2 + c) << endl;
		cout << "H3: " << show_as_hex (H3) << " + " << show_as_hex (d)
			 << " " << show_as_hex (H3 + d) << endl;
		cout << "H4: " << show_as_hex (H4) << " + " << show_as_hex (e)
			 << " " << show_as_hex (H4 + e) << endl;
		cout << "H5: " << show_as_hex (H5) << " + " << show_as_hex (f)
			 << " " << show_as_hex (H5 + f) << endl;
		cout << "H6: " << show_as_hex (H6) << " + " << show_as_hex (g)
			 << " " << show_as_hex (H6 + g) << endl;
		cout << "H7: " << show_as_hex (H7) << " + " << show_as_hex (h)
			 << " " << show_as_hex (H7 + h) << endl;
	}

	// Add up all the working variables to each hash and make sure we are still
	// working with solely 32 bit variables.
	H0 = (H0 + a) & 0xFFFFFFFF;
	H1 = (H1 + b) & 0xFFFFFFFF;
	H2 = (H2 + c) & 0xFFFFFFFF;
	H3 = (H3 + d) & 0xFFFFFFFF;
	H4 = (H4 + e) & 0xFFFFFFFF;
	H5 = (H5 + f) & 0xFFFFFFFF;
	H6 = (H6 + g) & 0xFFFFFFFF;
	H7 = (H7 + h) & 0xFFFFFFFF;

	// Append the hash segments together one after the other to get the full
	// 256 bit hash.
	return show_as_hex(H0) + show_as_hex(H1) + show_as_hex(H2) +
		   show_as_hex(H3) + show_as_hex(H4) + show_as_hex(H5) +
		   show_as_hex(H6) + show_as_hex(H7);
}


//----- DIVJOT CHANGES -----

vector<unsigned int> compute_hash_step(vector<unsigned int> registers, const vector<unsigned int> block, int step)
{
	// Taken from the spec
	//   SHA-224 and SHA-256 use the same sequence of sixty-four constant
	//   32-bit words, K0, K1, ..., K63.  These words represent the first 32
	//   bits of the fractional parts of the cube roots of the first sixty-
	//   four prime numbers.
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

	// Initial Hash Values, first thirty-two bits of the fractional parts of
	// the square roots of the first eight prime numbers. 
	unsigned int  H0 = 0x6a09e667;
	unsigned int  H1 = 0xbb67ae85;
	unsigned int  H2 = 0x3c6ef372;
	unsigned int  H3 = 0xa54ff53a;
	unsigned int  H4 = 0x510e527f;
	unsigned int  H5 = 0x9b05688c;
	unsigned int  H6 = 0x1f83d9ab;
	unsigned int  H7 = 0x5be0cd19;

	unsigned int W[64];

	for(int t = 0; t <= 15; t++)
	{
		W[t] = block[t] & 0xFFFFFFFF;

		if (show_Wt)
			cout << "W[" << t << "]: 0x" << show_as_hex(W[t]) << endl;
	}

	for(int t = 16; t <= 63; t++)
	{
		// Also taken from spec.
		W[t] = SSIG1(W[t-2]) + W[t-7] + SSIG0(W[t-15]) + W[t-16];

		// Have to make sure we are still dealing with 32 bit numbers.
		W[t] = W[t] & 0xFFFFFFFF;

		if (show_Wt)
			cout << "W[" << t << "]: " << W[t];
	}

	unsigned int temp1;
	unsigned int temp2;
	unsigned int a = registers[0];
	unsigned int b = registers[1];
	unsigned int c = registers[2];
	unsigned int d = registers[3];
	unsigned int e = registers[4];
	unsigned int f = registers[5];
	unsigned int g = registers[6];
	unsigned int h = registers[7];

	if(show_working_vars_for_t)
		cout << "         A        B        C        D        "
		     << "E        F        G        H        T1       T2\n";

	for( int t = step; t < step+1; t++)
	{
		// Seems the Official spec is wrong!? BSIG1 is incorrect.
		// Replacing BSIG1 with EP1.
		temp1 = h + EP1(e) + CH(e,f,g) + k[step] + W[step];
		if ((t == 20) & show_T1_calculation)
		{
			cout << "h: 0x" << hex << h << "  dec:" << dec << h 
			     << "  sign:" << dec << (int)h << endl;
			cout << "EP1(e): 0x" << hex << EP1(e) << "  dec:" 
			     << dec << EP1(e) << "  sign:" << dec << (int)EP1(e) 
			     << endl;
			cout << "CH(e,f,g): 0x" << hex << CH(e,f,g) << "  dec:" 
			     << dec << CH(e,f,g) << "  sign:" << dec 
			     << (int)CH(e,f,g) << endl;
			cout << "k[t]: 0x" << hex << k[t] << "  dec:" << dec 
			     << k[t] << "  sign:" << dec << (int)k[t] << endl;
			cout << "W[t]: 0x" << hex << W[t] << "  dec:" << dec 
			     << W[t] << "  sign:" << dec << (int)W[t] << endl;
			cout << "temp1: 0x" << hex << temp1 << "  dec:" << dec 
			     << temp1 << "  sign:" << dec << (int)temp1 << endl;
		}
		

		// Seems the Official spec is wrong!? BSIG0 is incorrect.
		// Replacing BSIG0 with EP0.
		temp2 = EP0(a) + MAJ(a,b,c);

		// Shows the variables and operations for getting T2.
		if ((t == 20) & show_T2_calculation)
		{
			cout << "a: 0x" << hex << a << "  dec:" << dec << a 
			     << "  sign:" << dec << (int)a << endl;
			cout << "b: 0x" << hex << b << "  dec:" << dec << b 
			     << "  sign:" << dec << (int)b << endl;
			cout << "c: 0x" << hex << c << "  dec:" << dec << c 
			     << "  sign:" << dec << (int)c << endl;
			cout << "EP0(a): 0x" << hex << EP0(a) << "  dec:" 
			     << dec << EP0(a) << "  sign:" << dec << (int)EP0(a) 
			     << endl;
			cout << "MAJ(a,b,c): 0x" << hex << MAJ(a,b,c) << "  dec:" 
			     << dec << MAJ(a,b,c) << "  sign:" << dec 
			     << (int)MAJ(a,b,c) << endl;
			cout << "temp2: 0x" << hex << temp2 << "  dec:" << dec 
			     << temp2 << "  sign:" << dec << (int)temp2 << endl;
		}

		// Do the working variables operations as per NIST.
		h = g;
		g = f;
		f = e;
		e = (d + temp1) & 0xFFFFFFFF; // Makes sure that we are still using 32 bits.
		d = c;
		c = b;
		b = a;
		a = (temp1 + temp2) & 0xFFFFFFFF; // Makes sure that we are still using 32 bits.

		// Shows the contents of each working variable for the turn T.
		if (show_working_vars_for_t)
		{
			cout << "t= " << t << " ";
			cout << show_as_hex (a) << " " << show_as_hex (b) << " " 
			     << show_as_hex (c) << " " << show_as_hex (d) << " " 
			     << show_as_hex (e) << " " << show_as_hex (f) << " "
			     << show_as_hex (g) << " " << show_as_hex (h) << " " 
			     << endl;
		}
	}

	// Shows the contents of each hash segment.
	if(show_hash_segments)
	{
		cout << "H0: " << show_as_hex (H0) << " + " << show_as_hex (a)
			 << " " << show_as_hex (H0 + a) << endl;
		cout << "H1: " << show_as_hex (H1) << " + " << show_as_hex (b)
			 << " " << show_as_hex (H1 + b) << endl;
		cout << "H2: " << show_as_hex (H2) << " + " << show_as_hex (c)
			 << " " << show_as_hex (H2 + c) << endl;
		cout << "H3: " << show_as_hex (H3) << " + " << show_as_hex (d)
			 << " " << show_as_hex (H3 + d) << endl;
		cout << "H4: " << show_as_hex (H4) << " + " << show_as_hex (e)
			 << " " << show_as_hex (H4 + e) << endl;
		cout << "H5: " << show_as_hex (H5) << " + " << show_as_hex (f)
			 << " " << show_as_hex (H5 + f) << endl;
		cout << "H6: " << show_as_hex (H6) << " + " << show_as_hex (g)
			 << " " << show_as_hex (H6 + g) << endl;
		cout << "H7: " << show_as_hex (H7) << " + " << show_as_hex (h)
			 << " " << show_as_hex (H7 + h) << endl;
	}

	// Add up all the working variables to each hash and make sure we are still
	// working with solely 32 bit variables.
	H0 = (H0 + a) & 0xFFFFFFFF;
	H1 = (H1 + b) & 0xFFFFFFFF;
	H2 = (H2 + c) & 0xFFFFFFFF;
	H3 = (H3 + d) & 0xFFFFFFFF;
	H4 = (H4 + e) & 0xFFFFFFFF;
	H5 = (H5 + f) & 0xFFFFFFFF;
	H6 = (H6 + g) & 0xFFFFFFFF;
	H7 = (H7 + h) & 0xFFFFFFFF;

	registers[0]=a;
	registers[1]=b;
	registers[2]=c;
	registers[3]=d;
	registers[4]=e;
	registers[5]=f;
	registers[6]=g;
	registers[7]=h;

	// Append the hash segments together one after the other to get the full
	// 256 bit hash.
	// return show_as_hex(a) + show_as_hex(b) + show_as_hex(c) +
	// 	   show_as_hex(d) + show_as_hex(e) + show_as_hex(f) +
	// 	   show_as_hex(g) + show_as_hex(h);
	return registers;
}
