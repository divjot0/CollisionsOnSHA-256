#include <iostream>
#include <vector>
#include "SHA256AttackUtil.h"

using namespace std;

int main(int argc, char* argv[]) {
	vector<unsigned int> block;
	vector<unsigned int> otherBlock(16);

	vector<unsigned int> registers(8);
	unsigned int static H0 = 0x6a09e667;
	unsigned int static H1 = 0xbb67ae85;
	unsigned int static H2 = 0x3c6ef372;
	unsigned int static H3 = 0xa54ff53a;
	unsigned int static H4 = 0x510e527f;
	unsigned int static H5 = 0x9b05688c;
	unsigned int static H6 = 0x1f83d9ab;
	unsigned int static H7 = 0x5be0cd19;
	registers[0]=H0;
	registers[1]=H1;
	registers[2]=H2;
	registers[3]=H3;
	registers[4]=H4;
	registers[5]=H5;
	registers[6]=H6;
	registers[7]=H7;

	// ---- START ALGORITHM ---- //
	string message = "";
	for (int i = 0; i < 16; ++i)
	{
		message = message + string(1, '\0'+(rand())%256);
	}
	cout << "This is message " << message << endl;
	block = convertMessageToWorkable(message);
	block[15]=block[0];
	block[1]=block[0];
	block[14]=block[0];
	cout << " These are blocks " << endl;
	for (int i = 0; i < block.size(); ++i)
	{
		cout << block[i] << endl;
	}
	cout << "Done" << endl;
	unsigned int delta = SSIG1(block[15] & 0xFFFFFFFF) - SSIG1((block[15]-1) & 0xFFFFFFFF);
	vector<unsigned int> rs = registers;
	rs = compute_hash_step(rs, block, 0);
	rs = compute_hash_step(rs, block, 1);
	block[2] = wToSetA(2, delta-1+MAJ(-1,-2, delta-3), rs);
	rs = compute_hash_step(rs, block, 2); //5
	block[3] = wToSetA(3, delta-3, rs);
	rs = compute_hash_step(rs, block, 3); //7
	block[4] = wToSetA(4, -2, rs);
	rs = compute_hash_step(rs, block, 4); //9
	block[5] = wToSetA(5, -1, rs);
	rs = compute_hash_step(rs, block, 5); //11
	block[6] = wToSetA(6, -1, rs);
	rs = compute_hash_step(rs, block, 6); //13
	block[7] = wToSetA(7, -1, rs);
	rs = compute_hash_step(rs, block, 7); //15
	unsigned int deltaW8 = -1-CH(rs[4]+1, rs[5], rs[6])+CH(rs[4],rs[5],rs[6])-EP1(rs[4]+1)+EP1(rs[4]);
	block[8] = wToSetA(8, 0, rs);
	rs = compute_hash_step(rs, block, 8); //17
	block[9] = wToSetA(9, 0, rs);
	rs = compute_hash_step(rs, block, 9); //19
	unsigned int deltaW10 = -CH(rs[4]-1, rs[5]-1, rs[6]+1)+CH(rs[4],rs[5],rs[6])-EP1(rs[4]-1)+EP1(rs[4]);
	block[10] = wToSetE(10, -1, rs);
	rs = compute_hash_step(rs, block, 10); //21
	block[11] = wToSetE(11, -1, rs);
	rs = compute_hash_step(rs, block, 11); //23
	block[12] = wToSetE(12, -1, rs);
	rs = compute_hash_step(rs, block, 12); //25
	block[13] = wToSetE(13, -1, rs);
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

	cout << "Check next" << endl;
	for (int i = 0; i < 16; ++i)
	{
		cout << block[i] << endl;
	}
	cout << "Dones" << endl;
	// cout << compute_hash(block) << endl;
	// rs = compute_hash_step(rs, block, 2);
	// cout << rs[0] << endl;
	for (int i = 0; i < 16; ++i)
	{
		otherBlock[i] = block[i];
	}
	otherBlock[7]=otherBlock[7]+1;
	otherBlock[15]=otherBlock[15]-1;
	otherBlock[8]=otherBlock[8]+deltaW8;
	otherBlock[10]=otherBlock[10]+deltaW10;
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
	// cout << compute_hash(otherBlock) << endl;
	cout << endl << endl;
	cout << "These are the messages :-" << endl;
	cout << "Message 1 : ";
	for (int i = 0; i < 16; ++i) {
		cout << block[i] << " ";
	}
	cout << endl;
	cout << "Message 2 : ";
	for (int i = 0; i < 16; ++i) {
		cout << otherBlock[i] << " ";
	}
	cout << endl;
	return 0;
}