#include <bits/stdc++.h> 

using namespace std;

#define ROTRIGHT(word,bits) (((word) >> (bits)) | ((word) << (32-(bits))))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SSIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SSIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))


// interfaces provided :::
	// conditions (which are implemented as graph here)

// functions provided :::
	// getCapSig(<<<x, <<<y, <<<z);
	// getSmallSig(<<<x, <<<y, <<z);

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

class Graph {
public:
	Graph() {}

	void dfsFirst(string v, stack<string> &firstOrderOfTraversal, map<string, bool> &visited) {
		if (visited.find(v)->second==true) {
			return;
		}
		visited.find(v)->second = true;

		vector<string> vNeighbours = this->getEdgesOf(v);
		for (int i = 0; i < vNeighbours.size(); ++i) {
			dfsFirst(vNeighbours[i], firstOrderOfTraversal, visited);
		}

		firstOrderOfTraversal.push(v);
		return;
	}

	void dfsSecond(string v, int counter, map<string, bool> &visited, map<string, int> &sccNumber, Graph &invGraph) {
		if (visited.find(v)->second==true) {
			return;
		}
		visited.find(v)->second = true;

		vector<string> vNeighbours = invGraph.getEdgesOf(v);
		for (int i = 0; i < vNeighbours.size(); ++i) {
			dfsSecond(vNeighbours[i], counter, visited, sccNumber, invGraph);
		}

		sccNumber.find(v)->second = counter;
		return;
	}

	string getOppositeVertex(string v) {
		string u = v;
		if (v[v.size()-1]=='0') {
			u[u.size()-1]='1';
		}
		else {
			u[u.size()-1]='0';
		}
		return u;
	}

	bool isSatisfiable() {
		stack<string> firstOrderOfTraversal;
		
		map<string, bool> visitedFirstTraversal;
		unordered_set<string> vertices = this->getVertices();
		for (auto vertex : vertices) {
			visitedFirstTraversal.insert(pair<string, bool>(vertex, false));
		}
		for (auto vertex : vertices) {
			if (visitedFirstTraversal.find(vertex)->second == false) {
				dfsFirst(vertex, firstOrderOfTraversal, visitedFirstTraversal);
			}
		}


		Graph invGraph = this->getInverseGraph();
		map<string, bool> visitedSecondTraversal;
		map<string, int> sccNumber;
		for (auto vertex : vertices) {
			visitedSecondTraversal.insert(pair<string, bool> (vertex, false));
			sccNumber.insert(pair<string, int> (vertex, -1));
		}

		int counter = 0;
		while(!firstOrderOfTraversal.empty()) {
			string v = firstOrderOfTraversal.top();
			firstOrderOfTraversal.pop();
			if (!visitedSecondTraversal.find(v)->second) {
				dfsSecond(v, counter, visitedSecondTraversal, sccNumber, invGraph);
				counter++;
			}
		}

		for (auto vertex : vertices) {
			// cout << vertex << " " << sccNumber.find(vertex)->second << endl;
			if (sccNumber.find(vertex)->second == sccNumber.find(this->getOppositeVertex(vertex))->second) {
				return false;
			}
		}
		return true;
	}

	vector<vector<string> > getSCC() {
		stack<string> firstOrderOfTraversal;
		
		map<string, bool> visitedFirstTraversal;
		unordered_set<string> vertices = this->getVertices();
		for (auto vertex : vertices) {
			visitedFirstTraversal.insert(pair<string, bool>(vertex, false));
		}
		for (auto vertex : vertices) {
			if (visitedFirstTraversal.find(vertex)->second == false) {
				dfsFirst(vertex, firstOrderOfTraversal, visitedFirstTraversal);
			}
		}


		Graph invGraph = this->getInverseGraph();
		map<string, bool> visitedSecondTraversal;
		map<string, int> sccNumber;
		for (auto vertex : vertices) {
			visitedSecondTraversal.insert(pair<string, bool> (vertex, false));
			sccNumber.insert(pair<string, int> (vertex, -1));
		}

		int counter = 0;
		while(!firstOrderOfTraversal.empty()) {
			string v = firstOrderOfTraversal.top();
			firstOrderOfTraversal.pop();
			if (!visitedSecondTraversal.find(v)->second) {
				dfsSecond(v, counter, visitedSecondTraversal, sccNumber, invGraph);
				counter++;
			}
		}

		int maxSccNumber=0;

		for (auto vertex : vertices) {
			// cout << vertex << " " << sccNumber.find(vertex)->second << endl;
			if (maxSccNumber<sccNumber.find(vertex)->second) {
				maxSccNumber = sccNumber.find(vertex)->second;
			}
		}
		
		vector<vector<string> > scc(maxSccNumber+1, vector<string>());
		for (auto vertex : vertices) {
			// cout << vertex << " " << sccNumber.find(vertex)->second << endl;
			scc[sccNumber.find(vertex)->second].push_back(vertex);
		}

		return scc;
	}

	Graph getInverseGraph() {
		Graph invGraph;
		unordered_set<string> vertices = this->getVertices();
		map<string, vector<string> > edgesOfAll = this->getEdges();
		for (auto vertex : vertices) {
			string temp = vertex;
			invGraph.addVertex(temp);
		}
		for (auto edgesOfV : edgesOfAll) {
			string v = edgesOfV.first;
			vector<string> vNeighbours = edgesOfV.second;
			for (int i = 0; i < vNeighbours.size(); ++i) {
				invGraph.addEdge(vNeighbours[i], v);
			}
		}
		return invGraph;
	}

	bool addVertex(string newVertex) {
		if (vertices.find(newVertex)!=vertices.end()) {
			//already present
			return false;
		}

		//insert node
		vertices.insert(newVertex);
		this->adjacencyList.insert(pair<string, vector<string> > (newVertex, vector<string> ()));
		return true;
	}

	bool addEdge(string v1, string v2) {
		if (vertices.find(v1)==vertices.end() || vertices.find(v2)==vertices.end()) {
			// one or more not intiated
			return false;
		}
		vector<string> v1Neighbours = this->adjacencyList.find(v1)->second;
		for (int i = 0; i < v1Neighbours.size(); ++i) {
			if (v1Neighbours[i]==v2) {
				// already has v2 neighbour
				return false;
			}
		}
		this->adjacencyList.find(v1)->second.push_back(v2);
		return true;
	}

	map<string, vector<string> > getEdges() {
		return adjacencyList;
	}

	vector<string> getEdgesOf(string vertex) {
		return adjacencyList.find(vertex)->second;
	}

	unordered_set<string> getVertices() {
		return vertices;
	}

private:
	unordered_set<string> vertices;
	map<string, vector<string> > adjacencyList;
};


string getVariableNameString(string W, int lineNumber, int bitPosition) {
	string variableNameString = W + "[" + to_string(lineNumber) + "]" + "[" + to_string(bitPosition) + "]";
	return variableNameString;
}

class Conditions {
public:
	Conditions() {}

	bool areSatisfiable() {
		return conditionsGraph.isSatisfiable();
	}
	
	void addFullLineVariables(string W, int lineNumber) {
		for (int i = 0; i < 32; ++i) {
			string variableNameString = getVariableNameString(W, lineNumber, i);
			conditionsGraph.addVertex(variableNameString + "=0");
			conditionsGraph.addVertex(variableNameString + "=1");
		}
		return;
	}

	void addParticularVariable(string W, int lineNumber, int bitPosition) {
		string variableNameString = getVariableNameString(W, lineNumber, bitPosition);
		conditionsGraph.addVertex(variableNameString + "=0");
		conditionsGraph.addVertex(variableNameString + "=1");
		return;
	}

	void setEqual(string W1, int lineNumber1, int bitPosition1, string W2, int lineNumber2, int bitPosition2) {
		string v1 = getVariableNameString(W1, lineNumber1, bitPosition1);
		string v2 = getVariableNameString(W2, lineNumber2, bitPosition2);
		if (conditionsStringSet.find(v1 + "=" + v2)!=conditionsStringSet.end()) {
			// already present
			return;
		}
		conditionsStringSet.insert(v1 + "=" + v2);
		conditionsGraph.addEdge(v1 + "=0", v2 + "=0");
		conditionsGraph.addEdge(v2 + "=0", v1 + "=0");
		conditionsGraph.addEdge(v1 + "=1", v2 + "=1");
		conditionsGraph.addEdge(v2 + "=1", v1 + "=1");
		return;
	}

	void setDifferent(string W1, int lineNumber1, int bitPosition1, string W2, int lineNumber2, int bitPosition2) {
		string v1 = getVariableNameString(W1, lineNumber1, bitPosition1);
		string v2 = getVariableNameString(W2, lineNumber2, bitPosition2);
		if (conditionsStringSet.find(v1 + "!=" + v2)!=conditionsStringSet.end()) {
			// already present
			return;
		}
		conditionsStringSet.insert(v1 + "!=" + v2);
		conditionsGraph.addEdge(v1 + "=0", v2 + "=1");
		conditionsGraph.addEdge(v2 + "=1", v1 + "=0");
		conditionsGraph.addEdge(v1 + "=1", v2 + "=0");
		conditionsGraph.addEdge(v2 + "=0", v1 + "=1");
		return;
	}

	bool canSetEqual(string W1, int lineNumber1, int bitPosition1, string W2, int lineNumber2, int bitPosition2) {
		Graph newConditionsGraph = conditionsGraph;
		string v1 = getVariableNameString(W1, lineNumber1, bitPosition1);
		string v2 = getVariableNameString(W2, lineNumber2, bitPosition2);
		if (conditionsStringSet.find(v1 + "=" + v2)!=conditionsStringSet.end()) {
			// already present
			return this->areSatisfiable();
		}
		newConditionsGraph.addEdge(v1 + "=0", v2 + "=0");
		newConditionsGraph.addEdge(v2 + "=0", v1 + "=0");
		newConditionsGraph.addEdge(v1 + "=1", v2 + "=1");
		newConditionsGraph.addEdge(v2 + "=1", v1 + "=1");
		if (newConditionsGraph.isSatisfiable()) {
			return true;
		}
		return false;
	}

	vector<vector<string> > getSCC() {
		return this->conditionsGraph.getSCC();
	}

	bool canSetDifferent(string W1, int lineNumber1, int bitPosition1, string W2, int lineNumber2, int bitPosition2) {
		Graph newConditionsGraph = conditionsGraph;
		string v1 = getVariableNameString(W1, lineNumber1, bitPosition1);
		string v2 = getVariableNameString(W2, lineNumber2, bitPosition2);
		if (conditionsStringSet.find(v1 + "!=" + v2)!=conditionsStringSet.end()) {
			// already present
			return this->areSatisfiable();
		}
		newConditionsGraph.addEdge(v1 + "=0", v2 + "=1");
		newConditionsGraph.addEdge(v2 + "=1", v1 + "=0");
		newConditionsGraph.addEdge(v1 + "=1", v2 + "=0");
		newConditionsGraph.addEdge(v2 + "=0", v1 + "=1");
		if (newConditionsGraph.isSatisfiable()) {
			return true;
		}
		return false;
	}

	void printConditions() {
		for (auto condition: conditionsStringSet) {
			cout << condition << ", ";
		}
		cout << endl;
	}

private:
	Graph conditionsGraph;
	unordered_set<string> conditionsStringSet;
};


class AVarSHA2 {
public:
	AVarSHA2(int height) {
		this->AVarSHA2Values = vector<unsigned int> (height+4, -1);
		setIndex(-4, 0xa54ff53a);
		setIndex(-3, 0x3c6ef372);
		setIndex(-2, 0xbb67ae85);
		setIndex(-1, 0x6a09e667);
	}

	void setIndex(int ind, unsigned int value) {
		this->AVarSHA2Values[ind+4] = value;
		return;
	}

	unsigned int getIndex(int ind) {
		return this->AVarSHA2Values[ind+4];
	}

	void printFullVariable() {
		for (int i = 0; i < AVarSHA2Values.size(); ++i) {
			cout << "Step " << i-4 << ": ";
			cout << AVarSHA2Values[i] << endl;
		}
		return;
	}

private:
	vector<unsigned int> AVarSHA2Values;	
};


class EVarSHA2 {
public:
	EVarSHA2(int height) {
		this->EVarSHA2Values = vector<unsigned int> (height+4, -1);
		setIndex(-4, 0x5be0cd19);
		setIndex(-3, 0x1f83d9ab);
		setIndex(-2, 0x9b05688c);
		setIndex(-1, 0x510e527f);
	}

	void setIndex(int ind, unsigned int value) {
		this->EVarSHA2Values[ind+4] = value;
		return;
	}

	unsigned int getIndex(int ind) {
		return this->EVarSHA2Values[ind+4];
	}

	void printFullVariable() {
		for (int i = 0; i < EVarSHA2Values.size(); ++i) {
			cout << "Step " << i-4 << ": ";
			cout << EVarSHA2Values[i] << endl;
		}
		return;
	}

private:
	vector<unsigned int> EVarSHA2Values;	
};




// next are the functions for SHA256

bool isDifferent(char c) {
	if (c=='x' || c=='u' || c=='n') {
		return true;
	}
	return false;
}

vector<vector<int> > getCapSig(int leftRotate0, int leftRotate1, int leftRotate2) {
	vector<vector<int> > capSigmaFunction = vector<vector<int> > (32, vector<int> (32, 0));
	for (int i = 0; i < capSigmaFunction.size(); ++i) {
		capSigmaFunction[i][(32+i-leftRotate0)%32]=1;
		capSigmaFunction[i][(32+i-leftRotate1)%32]=1;
		capSigmaFunction[i][(32+i-leftRotate2)%32]=1;
	}
	return capSigmaFunction;
}

vector<vector<int> > getSmallSig(int leftRotate0, int leftRotate1, int leftShift0) {
	vector<vector<int> > smallSigmaFunction = vector<vector<int> > (32, vector<int> (32, 0));
	for (int i = 0; i < smallSigmaFunction.size(); ++i) {
		smallSigmaFunction[i][(32+i-leftRotate0)%32]=1;
		smallSigmaFunction[i][(32+i-leftRotate1)%32]=1;
		if (i-leftShift0>-1) {
			smallSigmaFunction[i][(i-leftShift0)]=1;
		}
	}
	return smallSigmaFunction;
}

// vector<vector<int> > getInverse(vector<vector<int> > sigma) {
// 	vector<vector<int> > invSigma(32, vector<int> (32, 0));
// }

vector<vector<int> > SMALL_SIGMA_0 = getSmallSig(7, 18, 3);
vector<vector<int> > SMALL_SIGMA_1 = getSmallSig(17, 19, 10);
vector<vector<int> > CAP_SIGMA_0 = getCapSig(2, 13, 22);
vector<vector<int> > CAP_SIGMA_1 = getCapSig(6, 11, 25);

vector<int> convertNumCrossToNumVal(vector<char> &numCross) {
	vector<int> numVal(32, 0);
	for (int i = 0; i < numCross.size(); ++i) {
		if (numCross[i]=='-' || numCross[i]=='0' || numCross[i]=='1') {
			numVal[i] = 0;
		}
		else {
			numVal[i] = 1;
		}
	}
	return numVal;
}

string get0PaddedNum(int n) {
	string s = "";
	if (n<10) {
		s = "0";
	}
	s = s + to_string(n);
	return s;
}

vector<char> convertNumValToNumCross(vector<int> &numVal) {
	vector<char> numCross(32, '-');
	for (int i = 0; i < numVal.size(); ++i) {
		if (numVal[i]==0) {
			numCross[i] = '-';
		}
		else {
			numCross[i] = 'x';
		}
	}
	return numCross;
}

vector<int> matMul(vector<int> numVal, vector<vector<int> > &sigma) {
	vector<int> resVal(sigma[0].size(), 0);
	for (int i = 0; i < numVal.size(); ++i) {
		if (numVal[i]==1) {
			for (int j = 0; j < sigma[0].size(); ++j) {
				resVal[j] = (resVal[j] + sigma[i][j]);
			}
		}
	}
	return resVal;
}

vector<int> matMulMod2(vector<int> numVal, vector<vector<int> > &sigma) {
	vector<int> resVal(sigma[0].size(), 0);
	for (int i = 0; i < numVal.size(); ++i) {
		if (numVal[i]==1) {
			for (int j = 0; j < sigma[0].size(); ++j) {
				resVal[j] = (resVal[j] + sigma[i][j])%2;
			}
		}
	}
	return resVal;
}

vector<char> calculateFromCross(vector<vector<int> > &sigma, vector<char> &numCross) {
	vector<int> numVal = convertNumCrossToNumVal(numCross);
	vector<int> resVal = matMulMod2(numVal, sigma);
	vector<char> resCross = convertNumValToNumCross(resVal);
	return resCross;
}

vector<int> getNumbers(vector<vector<int> > &sigma, vector<char> &numCross) {
	vector<int> numVal = convertNumCrossToNumVal(numCross);
	vector<int> resVal = matMul(numVal, sigma);
	return resVal;
}

bool hasDifferenceVec(vector<char> v) {
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(v[i])) {
			return true;
		}
	}
	return false;
}

vector<int> convertIntToVector(unsigned int num) {
	vector<int> res(32, 0);
	int i=0;
	while(num) {
		if (num%2==1) {
			res[i]=1;
		}
		num = num/2;
		i++;
	}
	return res;
}

unsigned int convertVectorToInt(vector<int> v) {
	unsigned int res = 0;
	unsigned int power = 1;
	for (int i = 0; i < 32; ++i) {
		res = res + v[i]*power;
		power = power*2;
	}
	return res;
}

void printSingleLineForLatex(vector<char> v) {
	for (int i = 0; i < v.size(); ++i) {
		// cout << v[i];
		if (v[i]=='x') {
			cout << "\\Sz";
		}
		else if (v[i]=='-') {
			cout << "\\Sm";
		}
		else if (v[i]=='0') {
			cout << "\\Se";
		}
		else if (v[i]=='1') {
			cout << "\\Sf";
		}
		else if (v[i]=='u') {
			cout << "\\SA";
		}
		else if (v[i]=='n') {
			cout << "\\Sa";
		}
		else if (v[i]=='?') {
			cout << "\\Sp";
		}
	}
	cout << "\\\\" << endl;
}

void printSingleLine(vector<char> v) {
	for (int i = 0; i < v.size(); ++i) {
		cout << v[i];
	}
	cout << endl;
}

void printMessages(vector<unsigned int> w, vector<unsigned int> w2) {
	for (int i = 0; i < w.size(); ++i) {
		cout << w[i] << " " << w2[i] << endl;
	}
}

// vector<char> calculateFromFixed(vector<vector<int> > &sigma, vector<char> &numCross) {
// 	vector<char> numCrossU(32, '-');
// 	vector<char> numCrossN(32, '-');
// 	for (int i = 0; i < 32; ++i) {
// 		if (numCross[i]=='u') {
// 			numCrossU[i] = 'u';
// 		}
// 		if (numCross[i]=='n') {
// 			numCrossN[i] = 'n';
// 		}
// 	}
// 	vector<int> numValU = convertNumCrossToNumVal(numCrossU);
// 	vector<int> resValU = matMul(numValU, sigma);
// 	vector<char> resCrossU = convertNumValToNumCross(resValU);
// 	vector<int> numValN = convertNumCrossToNumVal(numCrossN);
// 	vector<int> resValN = matMul(numValN, sigma);
// 	vector<char> resCrossN = convertNumValToNumCross(resValN);
// 	vector<int> numVal = convertNumCrossToNumVal(numCross);
// 	vector<int> resVal = matMulMod2(numVal, sigma);
// 	vector<char> resCross = convertNumValToNumCross(resVal);

// 	// printSingleLine(numCrossU);
// 	// printSingleLine(numCrossN);
// 	// printSingleLine(resCrossU);
// 	// printSingleLine(resCrossN);
// 	// for (int i = 0; i < 12; ++i) {
// 	// 	cout << " ";
// 	// }
// 	// for (int i = 0; i < 32; ++i) {
// 	// 	cout << resValN[i];
// 	// }
// 	// cout << endl;

// 	// for (int i = 0; i < 12; ++i) {
// 	// 	cout << " ";
// 	// }
// 	// for (int i = 0; i < 32; ++i) {
// 	// 	cout << resValU[i];
// 	// }
// 	// cout << endl;

// 	for (int i = 0; i < 32; ++i) {
// 		cout << resValU[i]+resValN[i];
// 	}
// 	cout << endl;

// 	for (int i = 0; i < 32; ++i) {
// 		if (resVal[i]==1) {
// 			// if ((resValN[i] + resValU[i])==3) {
// 			// 	// cout << "here";
// 			// 	if (resValU[i]>resValN[i]) {
// 			// 		resCross[i]='u';
// 			// 	}
// 			// 	else {
// 			// 		resCross[i]='n';
// 			// 	}
// 			// }
// 			// else if ((resValN[i] + resValU[i])==1) {
// 			// 	if (resValU[i]>resValN[i]) {
// 			// 		resCross[i]='u';
// 			// 	}
// 			// 	else {
// 			// 		resCross[i]='n';
// 			// 	}
// 			// }
// 			// else {
// 			// 	resCross[i] = 'x';
// 			// }
// 			resCross[i] = 'x';
// 		}
// 	}
// 	return resCross;
// }



void printForInverse(vector<char> v) {
	cout << "[";
	for (int i = 0; i < 31; ++i) {
		if (v[i]=='-') {		
			cout << 0 << ", ";
		}
		else if (v[i]=='x') {
			cout << 1 << ", ";
		}
	}
	if (v[31]=='-') {		
		cout << 0 << "]" << endl;
	}
	else if (v[31]=='x') {
		cout << 1 << "]" << endl;
	}
}


void printDifferentials(bool printWDiff, bool printEDiff, bool printADiff, vector<vector<char> > wDifferential, vector<vector<char> > eDifferential, vector<vector<char> > aDifferential) {
	if (printWDiff) {
		cout << "This is w differential: " << endl;
		for (int i = 0; i < wDifferential.size(); ++i) {
			// if (i<10) {
			// 	cout << "0";
			// }
			// cout << i << ": ";
			// printSingleLine(wDifferential[i]);
			cout << i << " & ";
			printSingleLineForLatex(wDifferential[i]);
			cout << "\\\\";
		}
		cout << endl;
	}
	if (printEDiff) {
		cout << "This is e differential: " << endl;
		for (int i = 0; i < eDifferential.size(); ++i) {
			// if (i<10) {
			// 	cout << "0";
			// }
			// cout << i << ": ";
			// printSingleLine(eDifferential[i]);
			cout << i << " & ";
			printSingleLineForLatex(eDifferential[i]);
			cout << "\\\\";
		}
		cout << endl;
	}
	if (printADiff) {
		cout << "This is a differential: " << endl;
		for (int i = 0; i < aDifferential.size(); ++i) {
			// if (i<10) {
			// 	cout << "0";
			// }
			// cout << i << ": ";
			// printSingleLine(aDifferential[i]);
			cout << i << " & ";
			printSingleLineForLatex(aDifferential[i]);
			cout << "\\\\";
		}
		cout << endl;
	}
}

// sigmaNumber is 1, 2, 3, 4 for SMAll0, SMAll1, CAp0, Cap1
vector<char> calculateFromPrecisely(int sigmaNumber, vector<char> v) {
	vector<char> diff(32, '-');
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(v[i])) {
			diff[i] = 'x';
		}
	}
	if (sigmaNumber==3) {
		vector<char> res = calculateFromCross(CAP_SIGMA_0, diff);
		vector<int> rotationNumbers = {2, 13, 22};
		for (int i = 0; i < 32; ++i) {
			if (isDifferent(res[i])) {
				// if ((!isConstant(v[(i+rotationNumbers[0])%32])) && (!isConstant(v[(i+rotationNumbers[1])%32])) && (!isConstant(v[(i+rotationNumbers[2])%32])) ) {
					
				// }
				// else {
				// 	if ((!isConstant(v[(i+rotationNumbers[0])%32]))) {
				// 		if ()
				// 		{
				// 			/* code */
				// 		}
				// 	}
				// }
			}
		}
	}
	return diff;
}

void printDifferentialForVars(unsigned int num, unsigned int num2) {
	string res = "";
	while(num>0) {
		if (num%2==1) {
			res = res+"1";
		}
		else {
			res = res+"0";
		}
		num = num/2;
	}
	for (int i = res.length(); i < 32; ++i) {
		res = res+"0";
	}

	string res2 = "";
	while(num2>0) {
		if (num2%2==1) {
			res2 = res2+"1";
		}
		else {
			res2 = res2+"0";
		}
		num2 = num2/2;
	}
	for (int i = res2.length(); i < 32; ++i) {
		res2 = res2+"0";
	}

	string finalRes = "";
	for (int i = 0; i < 32; ++i) {
		finalRes = finalRes + "A";
	}
	for (int i = 0; i < 32; ++i) {
		if (res[i]==res2[i]) {
			finalRes[i] = res[i];
		}
		else if (res[i]=='1' && res2[i]=='0') {
			finalRes[i] = 'u';
		}
		else if (res[i]=='0' && res2[i]=='1') {
			finalRes[i] = 'n';
		}
	}
	cout << finalRes << endl;
	return;
}

vector<char> getDifferentialForVars(unsigned int num, unsigned int num2) {
	string res = "";
	while(num>0) {
		if (num%2==1) {
			res = res+"1";
		}
		else {
			res = res+"0";
		}
		num = num/2;
	}
	for (int i = res.length(); i < 32; ++i) {
		res = res+"0";
	}

	string res2 = "";
	while(num2>0) {
		if (num2%2==1) {
			res2 = res2+"1";
		}
		else {
			res2 = res2+"0";
		}
		num2 = num2/2;
	}
	for (int i = res2.length(); i < 32; ++i) {
		res2 = res2+"0";
	}

	string finalRes = "";
	for (int i = 0; i < 32; ++i) {
		finalRes = finalRes + "A";
	}
	for (int i = 0; i < 32; ++i) {
		if (res[i]==res2[i]) {
			finalRes[i] = res[i];
		}
		else if (res[i]=='1' && res2[i]=='0') {
			finalRes[i] = 'u';
		}
		else if (res[i]=='0' && res2[i]=='1') {
			finalRes[i] = 'n';
		}
	}
	vector<char> answer(32, '-');
	for (int i = 0; i < 32; ++i) {
		answer[i] = finalRes[i];
	}
	return answer;
}

vector<char> getOppositeDifferential(vector<char> differential) {
	vector<char> res(32, '-');
	for (int i = 0; i < 32; ++i) {
		if (differential[i]=='x') {
			res[i]='x';
		}
		else if (differential[i]=='n') {
			res[i]='u';
		}
		else if (differential[i]=='u') {
			res[i]='n';
		}
		else if (differential[i]=='0' || differential[i]=='1') {
			res[i]='-';
		}
	}
	return res;
}

bool hasCross(vector<char> v) {
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(v[i])) {
			return true;
		}
	}
	return false;
}

bool hasFixedValues(vector<char> v) {
	for (int i = 0; i < 32; ++i) {
		if (v[i]=='0' || v[i]=='1') {
			return true;
		}
	}
	return false;
}


void printTableDetails(AVarSHA2 &aVar, EVarSHA2 &eVar, vector<unsigned int> &w, AVarSHA2 &aVar2, EVarSHA2 &eVar2, vector<unsigned int> &w2, vector<vector<vector<char> > > &differentials, Conditions &conditions, int step) {
	cout << "E Table: " << endl;
	for (int i = step-4; i < step+1; ++i) {
		cout << get0PaddedNum(i) << ": "; printDifferentialForVars(eVar.getIndex(i), eVar2.getIndex(i));
	}
	cout << "A Table: " << endl;
	for (int i = step-4; i < step+1; ++i) {
		cout << get0PaddedNum(i) << ": "; printDifferentialForVars(aVar.getIndex(i), aVar2.getIndex(i));
	}
	cout << "W Table: " << endl;
	for (int i = step-4; i < step+1; ++i) {
		cout << get0PaddedNum(i) << ": "; printDifferentialForVars(w[i], w2[i]);
	}
	return;
}

vector<char> getE7WithState(int currentState, vector<char> partialDifferential) {
	vector<char> fullDifferential(32, '-');
	vector<int> currentStateVec = convertIntToVector(currentState);
	int vecPosition = 0;
	for (int i = 0; i < 32; ++i) {
		if (partialDifferential[i]=='x') {
			if (currentStateVec[vecPosition]==0) {
				fullDifferential[i]='u';
			}
			else {
				fullDifferential[i]='n';
			}
			vecPosition++;
		}
	}
	return fullDifferential;
}

void setAllDifferentials(vector<vector<char> >& aDifferential, vector<vector<char> >& eDifferential, vector<vector<char> >& wDifferential, vector<char> reference) {
	eDifferential[8] = reference;
	aDifferential[8] = reference;
	eDifferential[12] = reference;
	eDifferential[9] = getOppositeDifferential(reference);
	eDifferential[10] = getOppositeDifferential(reference);
	wDifferential[16] = getOppositeDifferential(reference);
	wDifferential[9] = getOppositeDifferential(reference);
	wDifferential[8] = vector<char> (32, '?');
	return;
}

vector<vector<int> > getLList(vector<char> sig, vector<char> a7) {
	vector<vector<int> > lList;
	int currentPos=0;
	int currentStart=0;
	int currentEnd=0;
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(sig[i])) {
			currentStart = i;
			currentPos = i;
			currentEnd = i;
			break;
		}
	}
	currentPos++;
	while(currentPos<32) {
		if (isDifferent(sig[currentPos]) && isDifferent(a7[currentPos])) {
			if (currentStart!=-1) {			
				lList.push_back({currentStart, currentEnd});
			}
			currentStart = currentPos;
			currentEnd = currentPos;
		}
		else if (isDifferent(sig[currentPos]) && !isDifferent(a7[currentPos])) {
			currentEnd = currentPos;
		}
		else if (!isDifferent(sig[currentPos]) && isDifferent(a7[currentPos])) {
			// do nothing
		}
		else if (!isDifferent(sig[currentPos]) && !isDifferent(a7[currentPos])) {
			if (currentStart!=-1) {
				lList.push_back({currentStart, currentEnd});
				currentStart=-1;
			}
		}
		currentPos++;
	}
	if (currentStart!=-1) {
		lList.push_back({currentStart, currentEnd});
	}
	return lList;
}

vector<vector<int> > getLListForCut(vector<char> sig, vector<char> a7) {
	vector<vector<int> > lList;
	int currentPos=0;
	int currentStart=0;
	int currentEnd=0;
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(sig[i])) {
			currentStart = i;
			currentPos = i;
			currentEnd = i;
			break;
		}
	}
	currentPos++;
	while(currentPos<32) {
		if (isDifferent(sig[currentPos]) && isDifferent(a7[currentPos])) {
			if (currentStart!=-1) {			
				lList.push_back({currentStart, currentEnd});
			}
			currentStart = currentPos;
			currentEnd = currentPos;
		}
		else if (isDifferent(sig[currentPos]) && !isDifferent(a7[currentPos])) {
			currentEnd = currentPos;
		}
		else if (!isDifferent(sig[currentPos]) && isDifferent(a7[currentPos])) {
			currentEnd = currentPos;
		}
		else if (!isDifferent(sig[currentPos]) && !isDifferent(a7[currentPos])) {
			if (currentStart!=-1) {
				lList.push_back({currentStart, currentEnd});
				currentStart=-1;
			}
		}
		currentPos++;
	}
	if (currentStart!=-1) {
		lList.push_back({currentStart, currentEnd});
	}
	return lList;
}



vector<vector<int> > getCausingBits(string sigma) {
	vector<vector<int> > causingBits(32, vector<int>());
	if (sigma=="CAP_SIGMA_0") {
		int r1 = 2;
		int r2 = 13;
		int r3 = 22;
		for (int i = 0; i < 32; ++i) {
			causingBits[i].push_back((i+r1)%32);
			causingBits[i].push_back((i+r2)%32);
			causingBits[i].push_back((i+r3)%32);
		}
	}
	else if (sigma=="CAP_SIGMA_1") {
		int r1 = 6;
		int r2 = 11;
		int r3 = 25;
		for (int i = 0; i < 32; ++i) {
			causingBits[i].push_back((i+r1)%32);
			causingBits[i].push_back((i+r2)%32);
			causingBits[i].push_back((i+r3)%32);
		}
	}
	else if (sigma=="SMALL_SIGMA_1") {
		int r1 = 17;
		int r2 = 19;
		for (int i = 0; i < 32; ++i) {
			causingBits[i].push_back((i+r1)%32);
			causingBits[i].push_back((i+r2)%32);
		}
		int s1 = 10;
		for (int i = 0; i < 32; ++i) {
			if ((i+s1)%32<s1) {
				continue;
			}
			else {
				causingBits[i].push_back((i+s1)%32);
			}
		}
	}
	else if (sigma=="SMALL_SIGMA_0") {
		int r1 = 7;
		int r2 = 18;
		for (int i = 0; i < 32; ++i) {
			causingBits[i].push_back((i+r1)%32);
			causingBits[i].push_back((i+r2)%32);
		}
		int s1 = 3;
		for (int i = 0; i < 32; ++i) {
			if ((i+s1)%32<s1) {
				continue;
			}
			else {
				causingBits[i].push_back((i+s1)%32);
			}
		}
	}
	return causingBits;
}
vector<char> getSigma(vector<char> source, string sigma) {
	vector<vector<int> > causingBits = getCausingBits(sigma);
	vector<char> res(32, '-');
	for (int i = 0; i < 32; ++i) {
		int uCount=0;
		int nCount=0;
		int aCount=0;
		int bCount=0;
		if (source[causingBits[i][0]]=='u') {
			uCount++;
		}
		if (source[causingBits[i][1]]=='u') {
			uCount++;
		}
		if (source[causingBits[i][2]]=='u') {
			uCount++;
		}
		if (source[causingBits[i][0]]=='n') {
			nCount++;
		}
		if (source[causingBits[i][1]]=='n') {
			nCount++;
		}
		if (source[causingBits[i][2]]=='n') {
			nCount++;
		}
		if (uCount+nCount==3) {
			if (uCount>nCount) {
				res[i]='u';
			}
			else {
				res[i]='n';
			}
		}
		else if (uCount+nCount==1) {
			if (source[causingBits[i][0]]=='1') {
				aCount++;
			}
			if (source[causingBits[i][1]]=='1') {
				aCount++;
			}
			if (source[causingBits[i][2]]=='1') {
				aCount++;
			}
			if (source[causingBits[i][0]]=='0') {
				bCount++;
			}
			if (source[causingBits[i][1]]=='0') {
				bCount++;
			}
			if (source[causingBits[i][2]]=='0') {
				bCount++;
			}
			if (bCount+aCount!=2) {
				cout << "WROONONDONDONG~!!!" << endl;
			}
			else {
				if (bCount==aCount) {
					if (uCount==1) {
						res[i]='n';
					}
					else {
						res[i]='u';
					}
				}
				else {
					if (uCount==1) {
						res[i]='u';
					}
					else {
						res[i]='n';
					}
				}
			}
		}
	}
	return res;
}

int getVarNum(string var) {
	string res = "";
	res = res + var[0];
	if (var[1]!='[') {
		res = res + var[1];
	}
	stringstream geek(res);
	int x;
	geek >> x;
	return x;
}

char getVarVal(string var) {
	return var[var.length()-1];
}

void setValidAssignment(vector<char>& source, Conditions conditions) {
	vector<vector<string> > scc = conditions.getSCC();

	// for (int i = 0; i < scc.size(); ++i) {
	// 	cout << "SCC Number " << i << ": ";
	// 	for (int j = 0; j < scc[i].size(); ++j) {
	// 		cout << scc[i][j] << " ";
	// 	}
	// 	cout << endl;
	// }

	for (int i = 0; i < scc.size(); ++i) {
		if (scc[i].size()!=1) {
			if (source[getVarNum(scc[i][0])]=='-') {
				for (int j = 0; j < scc[i].size(); ++j) {
					source[getVarNum(scc[i][j])] = getVarVal(scc[i][j]);
				}
			}
		}
	}

	return;
}

bool containsWithVal(vector<string> currentScc, vector<int> alreadyFixed, vector<char> source) {
	for (int i = 0; i < alreadyFixed.size(); ++i) {
		for (int j = 0; j < currentScc.size(); ++j) {
			if (getVarNum(currentScc[j])==alreadyFixed[i] && getVarVal(currentScc[j])==source[alreadyFixed[i]]) {
				return true;
			}
		}
	}
	return false;
}

bool setSmallValidAssignment(vector<char>& source, Conditions conditions, vector<int> alreadyFixed) {
	vector<vector<string> > scc = conditions.getSCC();

	// for (int i = 0; i < scc.size(); ++i) {
	// 	cout << "SCC Number " << i << ": ";
	// 	for (int j = 0; j < scc[i].size(); ++j) {
	// 		cout << scc[i][j] << " ";
	// 	}
	// 	cout << endl;
	// }

	for (int i = 0; i < scc.size(); ++i) {
		if (containsWithVal(scc[i], alreadyFixed, source)) {
			for (int j = 0; j < scc[i].size(); ++j) {
				if ((source[getVarNum(scc[i][j])]==getVarVal(scc[i][j])) || (source[getVarNum(scc[i][j])]=='-')) {
					source[getVarNum(scc[i][j])] = getVarVal(scc[i][j]);
				}
				else {
					return false;
				}
			}
		}
	}

	for (int i = 0; i < scc.size(); ++i) {
		if (scc[i].size()!=1) {
			if (source[getVarNum(scc[i][0])]=='-') {
				for (int j = 0; j < scc[i].size(); ++j) {
					source[getVarNum(scc[i][j])] = getVarVal(scc[i][j]);
				}
			}
		}
	}

	return true;
}

// depends on that the L never has a7 in middle
bool setA7appropriately(vector<char>& a7Differential, bool checking) {
	vector<char> sigA7Differential = calculateFromCross(CAP_SIGMA_0, a7Differential);
	vector<vector<int> > lList = getLList(sigA7Differential, a7Differential);
	// if (!checking) {
	// 	cout << "THis is lList " << endl;
	// 	for (int i = 0; i < lList.size(); ++i) {
	// 		cout << lList[i][0] << " " << lList[i][1] << endl;
	// 	}
	// }
	// ls need to get canceled with E8
	vector<char> pureSigDifferential(32, '-');
	vector<char> e8Differential = getOppositeDifferential(a7Differential);
	for (int i = 0; i < lList.size(); ++i) {
		char current;
		if (e8Differential[lList[i][0]]=='n') {
			current = 'u';
		}
		else {
			current = 'n';
		}
		for (int j = lList[i][0]; j < lList[i][1]; ++j) {
			if (isDifferent(sigA7Differential[j])) {
				pureSigDifferential[j] = e8Differential[lList[i][0]];
			}
			else {
				if (e8Differential[j]!=e8Differential[lList[i][0]]) {
					return false;
				}
			}
		}
		if (isDifferent(sigA7Differential[lList[i][1]])) {
			pureSigDifferential[lList[i][1]] = current;
		}
		else {
			if (e8Differential[lList[i][1]]!=current) {
				return false;
			}
		}
	}
	// if (!checking) {
	// 	cout << "hr: "; printSingleLine(pureSigDifferential);
	// 	cout << "e8: "; printSingleLine(e8Differential);
	// 	cout << "a7: "; printSingleLine(a7Differential);
	// }

	// get A7 differential which gets pureSigDifferential
	vector<vector<int> > sig0CausingBits = getCausingBits("CAP_SIGMA_0");

	Conditions conditions;
	for (int i = 0; i < 32; ++i) {
		if (!isDifferent(a7Differential[i])) {
			conditions.addParticularVariable(to_string(i), i, i);
		}
	}

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(pureSigDifferential[i])) {
			int count = 0;
			int countU = 0;
			int countV = 0;
			char diffType;
			vector<int> relationVariables;
			for (int j = 0; j < sig0CausingBits[i].size(); ++j) {
				if (isDifferent(a7Differential[sig0CausingBits[i][j]])) {
					count++;
					diffType = a7Differential[sig0CausingBits[i][j]];
				}
				else {
					relationVariables.push_back(sig0CausingBits[i][j]);
				}
			}
			if (count==1) {
				if (diffType == pureSigDifferential[i]) {
					conditions.setEqual(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
				else {
					conditions.setDifferent(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
			}
			if (count==3) {
				if (a7Differential[sig0CausingBits[i][0]]=='u') {
					countU++;
				}
				if (a7Differential[sig0CausingBits[i][1]]=='u') {
					countU++;
				}
				if (a7Differential[sig0CausingBits[i][2]]=='u') {
					countU++;
				}
				if (countU==1 || countU==3) {
					if (pureSigDifferential[i]!='u') {
						return false;
					}
				}
				else {
					if (pureSigDifferential[i]!='n') {
						return false;
					}
				}
			}
		}
	}
	if (!conditions.areSatisfiable()) {
		return false;
	}

	if (!checking) {
		setValidAssignment(a7Differential, conditions);
		// cout << "nw: "; printSingleLine(getSigma(a7Differential, "CAP_SIGMA_0"));
		// vector<char> test = a7Differential;
		// for (int i = 0; i < 32; ++i) {
		// 	if (test[i]=='-') {
		// 		test[i]=0;
		// 	}
		// }
		// cout << "casuing bits of 5 : " << sig0CausingBits[5][0] << " " << sig0CausingBits[5][1] << " " << sig0CausingBits[5][2] << endl;
		// cout << "vak of causingBits of 5 : " << a7Differential[sig0CausingBits[5][0]] << " " << a7Differential[sig0CausingBits[5][1]] << " " << a7Differential[sig0CausingBits[5][2]] << endl;
	}
	return true;
}

bool setA7appropriately2(vector<char>& a7Differential, bool checking, vector<char> cancelWith) {
	vector<char> sigA7Differential = calculateFromCross(CAP_SIGMA_0, a7Differential);
	vector<vector<int> > lList = getLList(sigA7Differential, a7Differential);
	// ls need to get canceled with E8
	vector<char> pureSigDifferential(32, '-');
	vector<char> e8Differential = cancelWith;
	for (int i = 0; i < lList.size(); ++i) {
		char current;
		if (e8Differential[lList[i][0]]=='n') {
			current = 'u';
		}
		else {
			current = 'n';
		}
		for (int j = lList[i][0]; j < lList[i][1]; ++j) {
			if (isDifferent(sigA7Differential[j])) {
				pureSigDifferential[j] = e8Differential[lList[i][0]];
			}
			else {
				if (e8Differential[j]!=e8Differential[lList[i][0]]) {
					return false;
				}
			}
		}
		if (isDifferent(sigA7Differential[lList[i][1]])) {
			pureSigDifferential[lList[i][1]] = current;
		}
		else {
			if (e8Differential[lList[i][1]]!=current) {
				return false;
			}
		}
	}

	// get A7 differential which gets pureSigDifferential
	vector<vector<int> > sig0CausingBits = getCausingBits("CAP_SIGMA_0");

	Conditions conditions;
	for (int i = 0; i < 32; ++i) {
		if (!isDifferent(a7Differential[i])) {
			conditions.addParticularVariable(to_string(i), i, i);
		}
	}

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(pureSigDifferential[i])) {
			int count = 0;
			int countU = 0;
			int countV = 0;
			char diffType;
			vector<int> relationVariables;
			for (int j = 0; j < sig0CausingBits[i].size(); ++j) {
				if (isDifferent(a7Differential[sig0CausingBits[i][j]])) {
					count++;
					diffType = a7Differential[sig0CausingBits[i][j]];
				}
				else {
					relationVariables.push_back(sig0CausingBits[i][j]);
				}
			}
			if (count==1) {
				if (diffType == pureSigDifferential[i]) {
					conditions.setEqual(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
				else {
					conditions.setDifferent(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
			}
			if (count==3) {
				if (a7Differential[sig0CausingBits[i][0]]=='u') {
					countU++;
				}
				if (a7Differential[sig0CausingBits[i][1]]=='u') {
					countU++;
				}
				if (a7Differential[sig0CausingBits[i][2]]=='u') {
					countU++;
				}
				if (countU==1 || countU==3) {
					if (pureSigDifferential[i]!='u') {
						return false;
					}
				}
				else {
					if (pureSigDifferential[i]!='n') {
						return false;
					}
				}
			}
		}
	}
	if (!conditions.areSatisfiable()) {
		return false;
	}

	if (!checking) {
		setValidAssignment(a7Differential, conditions);
	}
	
	return true;
}

bool setE8appropriately(vector<char>& e8Differential, bool checking) {
	// same procedure
	// get Ls
	// get pureSigma
	// check whether possible or not


	//prelim check  (now put inside)
	// if (e8Differential[24]!=e8Differential[22]) {
	// 	return false;
	// }

	vector<char> sigE8Differential = calculateFromCross(CAP_SIGMA_1, e8Differential);
	vector<vector<int> > lList = getLListForCut(sigE8Differential, e8Differential);

	// if (checking) {
	// 	for (int i = 0; i < lList.size(); ++i) {
	// 		cout << lList[i][0] << " " << lList[i][1] << endl;
	// 	}
	// }

	vector<char> e7Differential = getOppositeDifferential(e8Differential);

	vector<char> pureSigDifferential(32, '-');
	for (int i = 0; i < lList.size(); ++i) {
		char current;
		if (e7Differential[lList[i][0]]=='n') {
			current = 'u';
		}
		else {
			current = 'n';
		}
		for (int j = lList[i][0]; j < lList[i][1]; ++j) {
			if (isDifferent(sigE8Differential[j])) {
				pureSigDifferential[j] = e7Differential[lList[i][0]];
			}
			else {
				if (e7Differential[j]!=e7Differential[lList[i][0]]) {
					return false;
				}
			}
		}
		if (isDifferent(sigE8Differential[lList[i][1]])) {
			pureSigDifferential[lList[i][1]] = current;
		}
		else {
			if (e7Differential[lList[i][1]]!=current) {
				return false;
			}
		}
	}

	// if (!checking) {
	// 	cout << "printing diffs now : " << endl;
	// 	cout << "e7: "; printSingleLine(e7Differential);
	// 	cout << "e8: "; printSingleLine(e8Differential);
	// 	cout << "s1: "; printSingleLine(pureSigDifferential);
	// }

	vector<vector<int> > sig1CausingBits = getCausingBits("CAP_SIGMA_1");

	Conditions conditions;
	for (int i = 0; i < 32; ++i) {
		conditions.addParticularVariable(to_string(i), i, i);
	}

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(pureSigDifferential[i])) {
			int count = 0;
			int countU = 0;
			int countV = 0;
			char diffType;
			vector<int> relationVariables;
			for (int j = 0; j < sig1CausingBits[i].size(); ++j) {
				if (isDifferent(e8Differential[sig1CausingBits[i][j]])) {
					count++;
					diffType = e8Differential[sig1CausingBits[i][j]];
				}
				else {
					relationVariables.push_back(sig1CausingBits[i][j]);
				}
			}
			if (count==1) {
				if (diffType == pureSigDifferential[i]) {
					conditions.setEqual(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
				else {
					conditions.setDifferent(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
			}
			if (count==3) {
				if (e8Differential[sig1CausingBits[i][0]]=='u') {
					countU++;
				}
				if (e8Differential[sig1CausingBits[i][1]]=='u') {
					countU++;
				}
				if (e8Differential[sig1CausingBits[i][2]]=='u') {
					countU++;
				}
				if (countU==1 || countU==3) {
					if (pureSigDifferential[i]!='u') {
						return false;
					}
				}
				else {
					if (pureSigDifferential[i]!='n') {
						return false;
					}
				}
			}
		}
	}

	if (!conditions.areSatisfiable()) {
		return false;
	}

	if (!checking) {
		setValidAssignment(e8Differential, conditions);
	}
	return true;
}

bool setE11appropriately(vector<char>& e11Differential, bool checking) {
	// same procedure
	// get Ls
	// get pureSigma
	// check whether possible or not


	//prelim check  (now put inside)
	// if (e8Differential[24]!=e8Differential[22]) {
	// 	return false;
	// }

	vector<char> e8Differential = getOppositeDifferential(e11Differential);
	vector<char> sigE11Differential = calculateFromCross(CAP_SIGMA_1, e11Differential);
	vector<vector<int> > lList = getLListForCut(sigE11Differential, e8Differential);

	vector<char> pureSigDifferential(32, '-');
	for (int i = 0; i < lList.size(); ++i) {
		char current;
		if (e8Differential[lList[i][0]]=='n') {
			current = 'u';
		}
		else {
			current = 'n';
		}
		for (int j = lList[i][0]; j < lList[i][1]; ++j) {
			if (isDifferent(sigE11Differential[j])) {
				pureSigDifferential[j] = e8Differential[lList[i][0]];
			}
			else {
				if (e8Differential[j]!=e8Differential[lList[i][0]]) {
					return false;
				}
			}
		}
		if (isDifferent(sigE11Differential[lList[i][1]])) {
			pureSigDifferential[lList[i][1]] = current;
		}
		else {
			if (e8Differential[lList[i][1]]!=current) {
				return false;
			}
		}
	}

	// cout << "printing diffs now : " << endl;
	// cout << "e7: "; printSingleLine(e7Differential);
	// cout << "e8: "; printSingleLine(e8Differential);
	// cout << "s1: "; printSingleLine(pureSigDifferential);

	vector<vector<int> > sig1CausingBits = getCausingBits("CAP_SIGMA_1");

	Conditions conditions;
	for (int i = 0; i < 32; ++i) {
		conditions.addParticularVariable(to_string(i), i, i);
	}

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(pureSigDifferential[i])) {
			int count = 0;
			int countU = 0;
			int countV = 0;
			char diffType;
			vector<int> relationVariables;
			for (int j = 0; j < sig1CausingBits[i].size(); ++j) {
				if (isDifferent(e11Differential[sig1CausingBits[i][j]])) {
					count++;
					diffType = e11Differential[sig1CausingBits[i][j]];
				}
				else {
					relationVariables.push_back(sig1CausingBits[i][j]);
				}
			}
			if (count==1) {
				if (diffType == pureSigDifferential[i]) {
					conditions.setEqual(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
				else {
					conditions.setDifferent(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
			}
			if (count==3) {
				if (e11Differential[sig1CausingBits[i][0]]=='u') {
					countU++;
				}
				if (e11Differential[sig1CausingBits[i][1]]=='u') {
					countU++;
				}
				if (e11Differential[sig1CausingBits[i][2]]=='u') {
					countU++;
				}
				if (countU==1 || countU==3) {
					if (pureSigDifferential[i]!='u') {
						return false;
					}
				}
				else {
					if (pureSigDifferential[i]!='n') {
						return false;
					}
				}
			}
		}
	}

	if (!conditions.areSatisfiable()) {
		return false;
	}	

	if (!checking) {
		setValidAssignment(e11Differential, conditions);
	}
	return true;
}

bool setW15appropriately(vector<char>& w15Differential, bool checking, vector<char>& w10Differential) {
	// same procedure
	// get Ls
	// get pureSigma
	// check whether possible or not


	//prelim check  (now put inside)
	// if (e8Differential[24]!=e8Differential[22]) {
	// 	return false;
	// }

	vector<char> e7Differential = getOppositeDifferential(w15Differential);
	vector<char> sigW15Differential = calculateFromCross(SMALL_SIGMA_1, w15Differential);
	vector<vector<int> > lList = getLListForCut(sigW15Differential, e7Differential);

	vector<char> pureSigDifferential(32, '-');
	for (int i = 0; i < lList.size(); ++i) {
		char current;
		if (e7Differential[lList[i][0]]=='n') {
			current = 'u';
		}
		else {
			current = 'n';
		}
		for (int j = lList[i][0]; j < lList[i][1]; ++j) {
			if (isDifferent(sigW15Differential[j])) {
				pureSigDifferential[j] = e7Differential[lList[i][0]];
			}
			else {
				if (e7Differential[j]!=e7Differential[lList[i][0]]) {
					return false;
				}
			}
		}
		if (isDifferent(sigW15Differential[lList[i][1]])) {
			pureSigDifferential[lList[i][1]] = current;
		}
		else {
			if (e7Differential[lList[i][1]]!=current) {
				return false;
			}
		}
	}

	// cout << "printing diffs now : " << endl;
	// cout << "e7: "; printSingleLine(e7Differential);
	// cout << "e8: "; printSingleLine(e8Differential);
	// cout << "s1: "; printSingleLine(pureSigDifferential);

	w10Differential = pureSigDifferential;

	vector<vector<int> > sig1CausingBits = getCausingBits("SMALL_SIGMA_1");

	Conditions conditions;
	for (int i = 0; i < 32; ++i) {
		conditions.addParticularVariable(to_string(i), i, i);
	}

	vector<int> alreadyFixed;

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(pureSigDifferential[i])) {
			int count = 0;
			int countU = 0;
			int countV = 0;
			char diffType;
			vector<int> relationVariables;
			for (int j = 0; j < sig1CausingBits[i].size(); ++j) {
				if (isDifferent(w15Differential[sig1CausingBits[i][j]])) {
					count++;
					diffType = w15Differential[sig1CausingBits[i][j]];
				}
				else {
					relationVariables.push_back(sig1CausingBits[i][j]);
				}
			}
			if (count==1 && relationVariables.size()==2) {
				if (diffType == pureSigDifferential[i]) {
					conditions.setEqual(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
				else {
					conditions.setDifferent(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
			}
			else if (count==1) {
				// cout << "Hi " << endl;
				if (diffType==pureSigDifferential[i]) {
					w15Differential[relationVariables[0]]='0';
				}
				else {
					w15Differential[relationVariables[0]]='1';
				}
				alreadyFixed.push_back(relationVariables[0]);
			}
			if (count==3) {
				if (w15Differential[sig1CausingBits[i][0]]=='u') {
					countU++;
				}
				if (w15Differential[sig1CausingBits[i][1]]=='u') {
					countU++;
				}
				if (w15Differential[sig1CausingBits[i][2]]=='u') {
					countU++;
				}
				if (countU==1 || countU==3) {
					if (pureSigDifferential[i]!='u') {
						return false;
					}
				}
				else {
					if (pureSigDifferential[i]!='n') {
						return false;
					}
				}
			}
		}
	}

	if (!conditions.areSatisfiable()) {
		return false;
	}

	if (!checking) {
		if (!setSmallValidAssignment(w15Differential, conditions, alreadyFixed)) {
			return false;
		}
	}
	return true;
}

unsigned int getRandomUnsignedInt() {
	unsigned int res = rand();
	if (res%2==1) {
		return res = RAND_MAX + rand();
	}
	return rand();
}

bool finalConstructionW15(vector<char>& w15Differential, vector<char> cancelWith) {
	// vector<char> sigW15Differential = calculateFromCross(SMALL_SIGMA_1, w15Differential);
	vector<vector<int> > lList = getLListForCut(w15Differential, cancelWith);
	// if (!checking) {
	// 	cout << "THis is lList " << endl;
	// 	for (int i = 0; i < lList.size(); ++i) {
	// 		cout << lList[i][0] << " " << lList[i][1] << endl;
	// 	}
	// }
	// ls need to get canceled with E8
	vector<char> pureSigDifferential(32, '-');
	// vector<char> e8Differential = getOppositeDifferential(a7Differential);
	for (int i = 0; i < lList.size(); ++i) {
		char current;
		if (cancelWith[lList[i][0]]=='n') {
			current = 'u';
		}
		else {
			current = 'n';
		}
		for (int j = lList[i][0]; j < lList[i][1]; ++j) {
			if (isDifferent(w15Differential[j])) {
				pureSigDifferential[j] = cancelWith[lList[i][0]];
			}
			else {
				if (cancelWith[j]!=cancelWith[lList[i][0]]) {
					return false;
				}
			}
		}
		if (isDifferent(w15Differential[lList[i][1]])) {
			pureSigDifferential[lList[i][1]] = current;
		}
		else {
			if (cancelWith[lList[i][1]]!=current) {
				return false;
			}
		}
	}

	w15Differential = pureSigDifferential;
	return true;
}



int hammingWeight(vector<char> v) {
	int res=0;
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(v[i])) {
			res++;
		}
	}
	return res;
}

int canCancel(vector<char> a, vector<char> b) {
	vector<vector<int> > lList;
	int currentPos=0;
	int currentStart=-1;
	int currentEnd=0;
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(b[i])) {
			if (isDifferent(a[i])) {
				currentStart = i;
				currentPos = i;
				currentEnd = i;
				break;
			}
			else {
				return -1;
			}
		}
		else if (isDifferent(a[i])) {
			if (isDifferent(b[i])) {
				currentStart = i;
				currentPos = i;
				currentEnd = i;
				break;
			}
			else {
				return -1;
			}
		}
	}
	currentPos++;
	while(currentPos<32) {
		if (isDifferent(a[currentPos]) && isDifferent(b[currentPos])) {
			if (currentStart!=-1) {			
				lList.push_back({currentStart, currentEnd});
			}
			currentStart = currentPos;
			currentEnd = currentPos;
		}
		else if (isDifferent(a[currentPos]) && !isDifferent(b[currentPos]) && currentStart!=-1) {
			currentEnd = currentPos;
		}
		else if (!isDifferent(a[currentPos]) && isDifferent(b[currentPos]) && currentStart!=-1) {
			currentEnd = currentPos;
		}
		else if (isDifferent(a[currentPos]) && !isDifferent(b[currentPos]) && currentStart==-1) {
			return -1;
		}
		else if (!isDifferent(a[currentPos]) && isDifferent(b[currentPos]) && currentStart==-1) {
			return -1;
		}
		else if (!isDifferent(a[currentPos]) && !isDifferent(b[currentPos])) {
			if (currentStart!=-1) {
				lList.push_back({currentStart, currentEnd});
				currentStart=-1;
			}
		}
		currentPos++;
	}
	if (currentStart!=-1) {
		lList.push_back({currentStart, currentEnd});
	}
	int res=0;
	for (int i = 0; i < lList.size(); ++i) {
		res = res + lList[i][1] - lList[i][0];
	}
	// return lList;
	return res;
}

int isSubset(vector<char> a, vector<char> b) {
	vector<vector<int> > lList;
	int currentPos=0;
	int currentStart=0;
	int currentEnd=0;
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(a[i])) {
			if (isDifferent(b[i])) {
				currentStart = i;
				currentPos = i;
				currentEnd = i;
				break;
			}
			else {
				return -1;
			}
		}
	}
	currentPos++;
	while(currentPos<32) {
		if (isDifferent(a[currentPos]) && isDifferent(b[currentPos])) {
			if (currentStart!=-1) {			
				lList.push_back({currentStart, currentEnd});
			}
			currentStart = currentPos;
			currentEnd = currentPos;
		}
		else if (isDifferent(a[currentPos]) && !isDifferent(b[currentPos]) && currentStart!=-1) {
			currentEnd = currentPos;
		}
		else if (!isDifferent(a[currentPos]) && isDifferent(b[currentPos]) && currentStart!=-1) {
			// do nothing
		}
		else if (isDifferent(a[currentPos]) && !isDifferent(b[currentPos]) && currentStart==-1) {
			return -1;
		}
		else if (!isDifferent(a[currentPos]) && isDifferent(b[currentPos]) && currentStart==-1) {
			// do nothing
		}
		else if (!isDifferent(a[currentPos]) && !isDifferent(b[currentPos])) {
			if (currentStart!=-1) {
				lList.push_back({currentStart, currentEnd});
				currentStart=-1;
			}
		}
		currentPos++;
	}
	if (currentStart!=-1) {
		lList.push_back({currentStart, currentEnd});
	}
	int res=0;
	for (int i = 0; i < lList.size(); ++i) {
		res = res + lList[i][1] - lList[i][0];
	}
	// cout << res << endl;
	// return lList;
	return res;
}

bool finalConstructionW10WithW15(vector<char>& w10Differential, vector<char> cancelWith, vector<char>& w15Differential) {
	vector<char> sigW15Differential = calculateFromCross(SMALL_SIGMA_1, w15Differential);
	cancelWith = getOppositeDifferential(w15Differential);
	vector<vector<int> > lList = getLListForCut(sigW15Differential, cancelWith);
	// cout << lList.size() << endl;
	// printSingleLine(cancelWith);
	// if (!checking) {
		// cout << "THis is lList " << endl;
		// for (int i = 0; i < lList.size(); ++i) {
		// 	cout << lList[i][0] << " " << lList[i][1] << endl;
		// }
	// }
	// ls need to get canceled with E8
	vector<char> pureSigDifferential(32, '-');
	// vector<char> e8Differential = getOppositeDifferential(a7Differential);
	for (int i = 0; i < lList.size(); ++i) {
		char current;
		if (cancelWith[lList[i][0]]=='n') {
			current = 'u';
		}
		else {
			current = 'n';
		}
		for (int j = lList[i][0]; j < lList[i][1]; ++j) {
			if (isDifferent(sigW15Differential[j])) {
				pureSigDifferential[j] = cancelWith[lList[i][0]];
			}
			else {
				if (cancelWith[j]!=cancelWith[lList[i][0]]) {
					// cout << i << "/" << lList.size() << endl;
					return false;
				}
			}
		}
		if (isDifferent(sigW15Differential[lList[i][1]])) {
			pureSigDifferential[lList[i][1]] = current;
		}
		else {
			if (cancelWith[lList[i][1]]!=current) {
				// cout << i << endl;
				return false;
			}
		}
	}


	// cout << "chcek if next wo cancel" << endl;
	// printSingleLine(pureSigDifferential);
	// printSingleLine(cancelWith);
	// cout << endl;

	// if (!checking) {
	// 	cout << "hr: "; printSingleLine(pureSigDifferential);
	// 	cout << "e8: "; printSingleLine(e8Differential);
	// 	cout << "a7: "; printSingleLine(a7Differential);
	// }

	// get A7 differential which gets pureSigDifferential
	vector<vector<int> > sig1CausingBits = getCausingBits("SMALL_SIGMA_1");

	Conditions conditions;
	for (int i = 0; i < 32; ++i) {
		if (!isDifferent(w15Differential[i])) {
			conditions.addParticularVariable(to_string(i), i, i);
		}
	}

	vector<int> alreadyFixed;

	bool newReturn = false;

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(pureSigDifferential[i])) {
			int count = 0;
			int countU = 0;
			int countV = 0;
			char diffType;
			vector<int> relationVariables;
			for (int j = 0; j < sig1CausingBits[i].size(); ++j) {
				if (isDifferent(w15Differential[sig1CausingBits[i][j]])) {
					count++;
					diffType = w15Differential[sig1CausingBits[i][j]];
				}
				else {
					relationVariables.push_back(sig1CausingBits[i][j]);
				}
			}
			if (count==1 && relationVariables.size()==2) {
				if (diffType == pureSigDifferential[i]) {
					conditions.setEqual(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
				else {
					conditions.setDifferent(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
			}
			else if (count==1 && relationVariables.size()==1) {
				// cout << "Hi " << endl;
				// cout << "Hi " << relationVariables[0] << " " << i << endl;
				if (find(alreadyFixed.begin(), alreadyFixed.end(), relationVariables[0])==alreadyFixed.end()) {
					if (diffType==pureSigDifferential[i]) {
						w15Differential[relationVariables[0]]='0';
					}
					else {
						w15Differential[relationVariables[0]]='1';
					}
					alreadyFixed.push_back(relationVariables[0]);
				}
				else {
					if (diffType==pureSigDifferential[i]) {
						if (w15Differential[relationVariables[0]]=='1') {
							newReturn=true;
							// return false;
						}
					}
					else {
						if (w15Differential[relationVariables[0]]=='0') {
							newReturn=true;
							// return false;
						}
					}
				}
			}
			if (count==3) {
				if (w15Differential[sig1CausingBits[i][0]]=='u') {
					countU++;
				}
				if (w15Differential[sig1CausingBits[i][1]]=='u') {
					countU++;
				}
				if (w15Differential[sig1CausingBits[i][2]]=='u') {
					countU++;
				}
				if (countU==1 || countU==3) {
					if (pureSigDifferential[i]!='u') {
						return false;
					}
				}
				else {
					if (pureSigDifferential[i]!='n') {
						return false;
					}
				}
			}
		}
	}

	if (!conditions.areSatisfiable()) {
		return false;
	}

	cout << "Conditions are isSatisfiable " << endl;

	if (newReturn) {
		return false;
	}

	// cout << "aaa: ";
	// for (int i = 0; i < 32; ++i) {
	// 	cout << i%10;
	// }
	// cout << endl;
	// cout << "w16: "; printSingleLine(w15Differential);
	// cout << "Conditions: "; conditions.printConditions();

	if (!setSmallValidAssignment(w15Differential, conditions, alreadyFixed)) {
		return false;
	}

	// start
	// if (!checking) {
	// 	setValidAssignment(a7Differential, conditions);
	// 	// cout << "nw: "; printSingleLine(getSigma(a7Differential, "CAP_SIGMA_0"));
	// 	// vector<char> test = a7Differential;
	// 	// for (int i = 0; i < 32; ++i) {
	// 	// 	if (test[i]=='-') {
	// 	// 		test[i]=0;
	// 	// 	}
	// 	// }
	// 	// cout << "casuing bits of 5 : " << sig0CausingBits[5][0] << " " << sig0CausingBits[5][1] << " " << sig0CausingBits[5][2] << endl;
	// 	// cout << "vak of causingBits of 5 : " << a7Differential[sig0CausingBits[5][0]] << " " << a7Differential[sig0CausingBits[5][1]] << " " << a7Differential[sig0CausingBits[5][2]] << endl;
	// }
	return true;
}


bool finalConstructionW8WithW16(vector<char>& w8Differential, vector<char>& w16Differential) {
	vector<char> sigW8Differential = calculateFromCross(SMALL_SIGMA_0, w8Differential);
	vector<char> cancelWith = w16Differential;
	vector<vector<int> > lList = getLListForCut(sigW8Differential, cancelWith);
	// cout << lList.size() << endl;
	// printSingleLine(cancelWith);
	// if (!checking) {
		// cout << "THis is lList " << endl;
		// for (int i = 0; i < lList.size(); ++i) {
		// 	cout << lList[i][0] << " " << lList[i][1] << endl;
		// }
	// }
	// ls need to get canceled with E8
	vector<char> pureSigDifferential(32, '-');
	// vector<char> e8Differential = getOppositeDifferential(a7Differential);
	for (int i = 0; i < lList.size(); ++i) {
		char current;
		if (cancelWith[lList[i][0]]=='n') {
			current = 'u';
		}
		else {
			current = 'n';
		}
		for (int j = lList[i][0]; j < lList[i][1]; ++j) {
			if (isDifferent(sigW8Differential[j])) {
				pureSigDifferential[j] = cancelWith[lList[i][0]];
			}
			else {
				if (cancelWith[j]!=cancelWith[lList[i][0]]) {
					// cout << i << "/" << lList.size() << endl;
					return false;
				}
			}
		}
		if (isDifferent(sigW8Differential[lList[i][1]])) {
			pureSigDifferential[lList[i][1]] = current;
		}
		else {
			if (cancelWith[lList[i][1]]!=current) {
				// cout << i << endl;
				return false;
			}
		}
	}


	// cout << "chcek if next wo cancel" << endl;
	// printSingleLine(pureSigDifferential);
	// printSingleLine(cancelWith);
	// cout << endl;

	// if (!checking) {
	// 	cout << "hr: "; printSingleLine(pureSigDifferential);
	// 	cout << "e8: "; printSingleLine(e8Differential);
	// 	cout << "a7: "; printSingleLine(a7Differential);
	// }

	// get A7 differential which gets pureSigDifferential
	vector<vector<int> > sig0CausingBits = getCausingBits("SMALL_SIGMA_0");

	Conditions conditions;
	for (int i = 0; i < 32; ++i) {
		if (!isDifferent(w8Differential[i])) {
			conditions.addParticularVariable(to_string(i), i, i);
		}
	}

	vector<int> alreadyFixed;

	bool newReturn = false;

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(pureSigDifferential[i])) {
			int count = 0;
			int countU = 0;
			int countV = 0;
			char diffType;
			vector<int> relationVariables;
			for (int j = 0; j < sig0CausingBits[i].size(); ++j) {
				if (isDifferent(w8Differential[sig0CausingBits[i][j]])) {
					count++;
					diffType = w8Differential[sig0CausingBits[i][j]];
				}
				else {
					relationVariables.push_back(sig0CausingBits[i][j]);
				}
			}
			if (count==1 && relationVariables.size()==2) {
				if (diffType == pureSigDifferential[i]) {
					conditions.setEqual(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
				else {
					conditions.setDifferent(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
			}
			else if (count==1 && relationVariables.size()==1) {
				// cout << "Hi " << endl;
				// cout << "Hi " << relationVariables[0] << " " << i << endl;
				if (find(alreadyFixed.begin(), alreadyFixed.end(), relationVariables[0])==alreadyFixed.end()) {
					if (diffType==pureSigDifferential[i]) {
						w8Differential[relationVariables[0]]='0';
					}
					else {
						w8Differential[relationVariables[0]]='1';
					}
					alreadyFixed.push_back(relationVariables[0]);
				}
				else {
					if (diffType==pureSigDifferential[i]) {
						if (w8Differential[relationVariables[0]]=='1') {
							newReturn=true;
							// return false;
						}
					}
					else {
						if (w8Differential[relationVariables[0]]=='0') {
							newReturn=true;
							// return false;
						}
					}
				}
			}
			if (count==3) {
				if (w8Differential[sig0CausingBits[i][0]]=='u') {
					countU++;
				}
				if (w8Differential[sig0CausingBits[i][1]]=='u') {
					countU++;
				}
				if (w8Differential[sig0CausingBits[i][2]]=='u') {
					countU++;
				}
				if (countU==1 || countU==3) {
					if (pureSigDifferential[i]!='u') {
						return false;
					}
				}
				else {
					if (pureSigDifferential[i]!='n') {
						return false;
					}
				}
			}
		}
	}

	if (!conditions.areSatisfiable()) {
		return false;
	}

	cout << "Conditions are isSatisfiable " << endl;

	if (newReturn) {
		return false;
	}

	// cout << "aaa: ";
	// for (int i = 0; i < 32; ++i) {
	// 	cout << i%10;
	// }
	// cout << endl;
	// cout << "w16: "; printSingleLine(w15Differential);
	// cout << "Conditions: "; conditions.printConditions();

	if (!setSmallValidAssignment(w8Differential, conditions, alreadyFixed)) {
		return false;
	}

	// start
	// if (!checking) {
	// 	setValidAssignment(a7Differential, conditions);
	// 	// cout << "nw: "; printSingleLine(getSigma(a7Differential, "CAP_SIGMA_0"));
	// 	// vector<char> test = a7Differential;
	// 	// for (int i = 0; i < 32; ++i) {
	// 	// 	if (test[i]=='-') {
	// 	// 		test[i]=0;
	// 	// 	}
	// 	// }
	// 	// cout << "casuing bits of 5 : " << sig0CausingBits[5][0] << " " << sig0CausingBits[5][1] << " " << sig0CausingBits[5][2] << endl;
	// 	// cout << "vak of causingBits of 5 : " << a7Differential[sig0CausingBits[5][0]] << " " << a7Differential[sig0CausingBits[5][1]] << " " << a7Differential[sig0CausingBits[5][2]] << endl;
	// }
	return true;
}

vector<char> getGoodDiff(vector<char> v) {
	// printSingleLine(v);
	vector<char> goodV(32, '-');
	int i=0;
	bool pushed = true;
	int pushStart = -1;
	int pushEnd = -1;
	while (i < 32) {
		// cout << i << endl;
		if (!isDifferent(v[i])) {
			i++;
		}
		else {
			if (v[i]=='u' || v[i]=='n') {
				pushed = false;
				pushStart = i;
				int j = i+1;
				while(j<32) {
					if (!isDifferent(v[j])) {
						for (int k = i; k < j; ++k) {
							goodV[k]=v[k];
						}
						i = j;
						pushed = true;
						break;
					}
					else if (v[j]==v[i]){
						j++;
						pushed = false;
					}
					else {
						//end
						// cout << "herer0" << endl;
						goodV[i]=v[j];
						i = j+1;
						pushed = true;
						break;
					}
				}
				if (j==32) {
					break;
				}
			}
		}
	}
	if (!pushed) {
		for (int i = pushStart; i < 32; ++i) {
			goodV[i] = v[i];
		}
	}
	return goodV;
}

vector<char> getRandomCanceler(vector<char> v, bool checkAll, int &state, vector<char> goodV) {
	vector<char> res(32, '-');

	// printSingleLine(v);
	// printSingleLine(goodV);

	if (checkAll) {
		// cout << "here0 0 " << endl;
		while (canCancel(res, goodV)==-1) {
			// cout << state << "state" << endl;
			state = state+1;
			vector<int> stateInBinary = convertIntToVector(state);
			int posOfState=0;
			for (int i = 0; i < 32; ++i) {
				if (isDifferent(goodV[i])) {
					res[i]='x';
				}
				else {
					if (stateInBinary[posOfState]==1) {
						res[i]='x';
					}
					else {
						res[i]='-';
					}
					posOfState++;
				}
			}
			for (int i = posOfState; i < 32; ++i) {
				if (stateInBinary[posOfState]==1) {
					state = -2;
					// cout << "End" << endl;
					return res;
				}
			}
		}
		// cout << "Found" << endl;
		return res;
	}

	for (int i = 0; i < 32; ++i) {
		if (!isDifferent(goodV[i])) {
			continue;
		}
		else {
			int lengthLimit = 1;
			for (int j = i+1; j < 32; ++j) {
				if (!isDifferent(goodV[j])) {
					lengthLimit++;
				}
				else {
					break;
				}
			}
			int lengthOfResCross = rand()%lengthLimit+1;
			for (int j = 0; j < lengthOfResCross; ++j) {
				res[i+j] = 'x';
			}
		}
	}

	// printSingleLine(res);

	return res;

}

bool canFind(vector<char> &w10Differential, vector<char> &w15Differential, vector<char> e7Differential) {
	int state = -1;
	bool found = false;
	bool checkAll = false;
	// cout << "before e7" << endl;
	vector<char> e7Vec = e7Differential;
	// cout << "before goodV" << endl;
	vector<char> goodV = getGoodDiff(e7Vec);
	// cout << "before triasl" << endl;
	// if (32-hammingWeight(goodV)<15) {
	// 	cout << "Checking all here :";
	// 	printSingleLine(goodV);
	// 	checkAll=true;
	// }
	int trials = 0;
	while(!found && state!=-2) {
		trials++;
		// cout << trials << endl;
		if (trials>20000) {
			break;
		}
		// cout << "before " << endl;
		vector<char> w15Vec = getRandomCanceler(e7Vec, checkAll, state, goodV);
		vector<char> w15SmallSig1 = calculateFromCross(SMALL_SIGMA_1, w15Vec);
		// cout << "w15Vec : "; printSingleLine(w15Vec);
		if (canCancel(w15SmallSig1, w15Vec)!=-1) {
			vector<char> w10Vec(32, '-');
			// cout << "testing 1 " << state << endl;
			if (finalConstructionW15(w15Vec, e7Vec) && finalConstructionW10WithW15(w10Vec, e7Vec, w15Vec)) {
				// cout << state << " " << hammingWeight(w15Vec) << " " << hammingWeight(w15SmallSig1) << endl;
				// printSingleLine(e7Vec);
				// printSingleLine(w15Vec);
				// printSingleLine(w15SmallSig1);
				// cout << endl;
				w15Differential = w15Vec;
				// w10Differential = getOppositeDifferential(w15Vec);
				w10Differential = getOppositeDifferential(getOppositeDifferential(e7Differential));
				found = true;
			}
		}
	}
	return found;
}



bool canFindW8(vector<char> &w8Differential, vector<char> w16Differential) {
	int state = -1;
	bool found = false;
	bool checkAll = false;
	// cout << "before e7" << endl;
	// cout << "before goodV" << endl;
	vector<char> goodV = getGoodDiff(w8Differential);
	// cout << "before triasl" << endl;
	// if (32-hammingWeight(goodV)<15) {
	// 	cout << "Checking all here :";
	// 	printSingleLine(goodV);
	// 	checkAll=true;
	// }
	vector<char> temp(0);
	int trials = 0;
	while(!found && state!=-2) {
		trials++;
		// cout << trials << endl;
		if (trials>20000) {
			break;
		}
		// cout << "before " << endl;
		vector<char> w8Vec = getRandomCanceler(temp, checkAll, state, goodV);
		vector<char> w8SmallSig0 = calculateFromCross(SMALL_SIGMA_0, w8Vec);
		// cout << "w15Vec : "; printSingleLine(w15Vec);
		if (canCancel(w8SmallSig0, w16Differential)!=-1) {
			vector<char> w10Vec(32, '-');
			cout << "testing 1 " << state << endl;
			if (finalConstructionW8WithW16(w8Vec, w16Differential)) {
				// cout << state << " " << hammingWeight(w15Vec) << " " << hammingWeight(w15SmallSig1) << endl;
				// printSingleLine(e7Vec);
				// printSingleLine(w15Vec);
				// printSingleLine(w15SmallSig1);
				// cout << endl;
				// w15Differential = w15Vec;
				// w10Differential = getOppositeDifferential(w15Vec);
				w8Differential = w8Vec;
				found = true;
			}
		}
	}
	return found;
}

bool setA8appropriatelyFor23(vector<char> &a8Differential, bool checking) {
	vector<char> sigA8Differential = calculateFromCross(CAP_SIGMA_0, a8Differential);
	vector<char> e9Differential = getOppositeDifferential(a8Differential);
	vector<vector<int> > lList = getLList(sigA8Differential, e9Differential);
	vector<char> pureSigDifferential(32, '-');
	for (int i = 0; i < lList.size(); ++i) {
		char current;
		if (e9Differential[lList[i][0]]=='n') {
			current = 'u';
		}
		else {
			current = 'n';
		}
		for (int j = lList[i][0]; j < lList[i][1]; ++j) {
			if (isDifferent(sigA8Differential[j])) {
				pureSigDifferential[j] = e9Differential[lList[i][0]];
			}
			else {
				if (e9Differential[j]!=e9Differential[lList[i][0]]) {
					return false;
				}
			}
		}
		if (isDifferent(sigA8Differential[lList[i][1]])) {
			pureSigDifferential[lList[i][1]] = current;
		}
		else {
			if (e9Differential[lList[i][1]]!=current) {
				return false;
			}
		}
	}
	
	// get A8 differential which gets pureSigDifferential
	vector<vector<int> > sig0CausingBits = getCausingBits("CAP_SIGMA_0");

	Conditions conditions;
	for (int i = 0; i < 32; ++i) {
		if (!isDifferent(a8Differential[i])) {
			conditions.addParticularVariable(to_string(i), i, i);
		}
	}

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(pureSigDifferential[i])) {
			int count = 0;
			int countU = 0;
			int countV = 0;
			char diffType;
			vector<int> relationVariables;
			for (int j = 0; j < sig0CausingBits[i].size(); ++j) {
				if (isDifferent(a8Differential[sig0CausingBits[i][j]])) {
					count++;
					diffType = a8Differential[sig0CausingBits[i][j]];
				}
				else {
					relationVariables.push_back(sig0CausingBits[i][j]);
				}
			}
			if (count==1) {
				if (diffType == pureSigDifferential[i]) {
					conditions.setEqual(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
				else {
					conditions.setDifferent(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
			}
			if (count==3) {
				if (a8Differential[sig0CausingBits[i][0]]=='u') {
					countU++;
				}
				if (a8Differential[sig0CausingBits[i][1]]=='u') {
					countU++;
				}
				if (a8Differential[sig0CausingBits[i][2]]=='u') {
					countU++;
				}
				if (countU==1 || countU==3) {
					if (pureSigDifferential[i]!='u') {
						return false;
					}
				}
				else {
					if (pureSigDifferential[i]!='n') {
						return false;
					}
				}
			}
		}
	}

	if (!conditions.areSatisfiable()) {
		return false;
	}

	if (!checking) {
		setValidAssignment(a8Differential, conditions);
	}

	return true;
}

bool setE12appropriatelyFor23(vector<char> &e12Differential, bool checking) {
	vector<char> sigE12Differential = calculateFromCross(CAP_SIGMA_1, e12Differential);
	vector<char> e9Differential = getOppositeDifferential(e12Differential);
	vector<vector<int> > lList = getLListForCut(sigE12Differential, e9Differential);
	vector<char> pureSigDifferential(32, '-');
	for (int i = 0; i < lList.size(); ++i) {
		char current;
		if (e9Differential[lList[i][0]]=='n') {
			current = 'u';
		}
		else {
			current = 'n';
		}
		for (int j = lList[i][0]; j < lList[i][1]; ++j) {
			if (isDifferent(sigE12Differential[j])) {
				pureSigDifferential[j] = e9Differential[lList[i][0]];
			}
			else {
				if (e9Differential[j]!=e9Differential[lList[i][0]]) {
					return false;
				}
			}
		}
		if (isDifferent(sigE12Differential[lList[i][1]])) {
			pureSigDifferential[lList[i][1]] = current;
		}
		else {
			if (e9Differential[lList[i][1]]!=current) {
				return false;
			}
		}
	}
	
	// get A8 differential which gets pureSigDifferential
	vector<vector<int> > sig1CausingBits = getCausingBits("CAP_SIGMA_1");

	Conditions conditions;
	for (int i = 0; i < 32; ++i) {
		if (!isDifferent(e12Differential[i])) {
			conditions.addParticularVariable(to_string(i), i, i);
		}
	}

	// --- start here

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(pureSigDifferential[i])) {
			int count = 0;
			int countU = 0;
			int countV = 0;
			char diffType;
			vector<int> relationVariables;
			for (int j = 0; j < sig1CausingBits[i].size(); ++j) {
				if (isDifferent(e12Differential[sig1CausingBits[i][j]])) {
					count++;
					diffType = e12Differential[sig1CausingBits[i][j]];
				}
				else {
					relationVariables.push_back(sig1CausingBits[i][j]);
				}
			}
			if (count==1) {
				if (diffType == pureSigDifferential[i]) {
					conditions.setEqual(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
				else {
					conditions.setDifferent(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
			}
			if (count==3) {
				if (e12Differential[sig1CausingBits[i][0]]=='u') {
					countU++;
				}
				if (e12Differential[sig1CausingBits[i][1]]=='u') {
					countU++;
				}
				if (e12Differential[sig1CausingBits[i][2]]=='u') {
					countU++;
				}
				if (countU==1 || countU==3) {
					if (pureSigDifferential[i]!='u') {
						return false;
					}
				}
				else {
					if (pureSigDifferential[i]!='n') {
						return false;
					}
				}
			}
		}
	}

	if (!conditions.areSatisfiable()) {
		return false;
	}

	if (!checking) {
		setValidAssignment(e12Differential, conditions);
	}

	return true;
}

bool setE9appropriatelyFor23(vector<char> &e9Differential, bool checking) {
	vector<char> sigE9Differential = calculateFromCross(CAP_SIGMA_1, e9Differential);
	vector<char> e8Differential = getOppositeDifferential(e9Differential);
	vector<vector<int> > lList = getLListForCut(sigE9Differential, e8Differential);
	vector<char> pureSigDifferential(32, '-');
	for (int i = 0; i < lList.size(); ++i) {
		char current;
		if (e8Differential[lList[i][0]]=='n') {
			current = 'u';
		}
		else {
			current = 'n';
		}
		for (int j = lList[i][0]; j < lList[i][1]; ++j) {
			if (isDifferent(sigE9Differential[j])) {
				pureSigDifferential[j] = e8Differential[lList[i][0]];
			}
			else {
				if (e8Differential[j]!=e8Differential[lList[i][0]]) {
					return false;
				}
			}
		}
		if (isDifferent(sigE9Differential[lList[i][1]])) {
			pureSigDifferential[lList[i][1]] = current;
		}
		else {
			if (e8Differential[lList[i][1]]!=current) {
				return false;
			}
		}
	}
	
	// get A8 differential which gets pureSigDifferential
	vector<vector<int> > sig1CausingBits = getCausingBits("CAP_SIGMA_1");

	Conditions conditions;
	for (int i = 0; i < 32; ++i) {
		if (!isDifferent(e9Differential[i])) {
			conditions.addParticularVariable(to_string(i), i, i);
		}
	}

	// --- start here

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(pureSigDifferential[i])) {
			int count = 0;
			int countU = 0;
			int countV = 0;
			char diffType;
			vector<int> relationVariables;
			for (int j = 0; j < sig1CausingBits[i].size(); ++j) {
				if (isDifferent(e9Differential[sig1CausingBits[i][j]])) {
					count++;
					diffType = e9Differential[sig1CausingBits[i][j]];
				}
				else {
					relationVariables.push_back(sig1CausingBits[i][j]);
				}
			}
			if (count==1) {
				if (diffType == pureSigDifferential[i]) {
					conditions.setEqual(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
				else {
					conditions.setDifferent(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
			}
			if (count==3) {
				if (e9Differential[sig1CausingBits[i][0]]=='u') {
					countU++;
				}
				if (e9Differential[sig1CausingBits[i][1]]=='u') {
					countU++;
				}
				if (e9Differential[sig1CausingBits[i][2]]=='u') {
					countU++;
				}
				if (countU==1 || countU==3) {
					if (pureSigDifferential[i]!='u') {
						return false;
					}
				}
				else {
					if (pureSigDifferential[i]!='n') {
						return false;
					}
				}
			}
		}
	}

	if (!conditions.areSatisfiable()) {
		return false;
	}

	if (!checking) {
		setValidAssignment(e9Differential, conditions);
	}

	return true;
}

bool setE10appropriatelyFor23(vector<char> &e10Differential, bool checking) {
	vector<char> sigE10Differential = calculateFromCross(CAP_SIGMA_1, e10Differential);
	vector<char> e8Differential = getOppositeDifferential(e10Differential);
	vector<vector<int> > lList = getLListForCut(sigE10Differential, e8Differential);
	vector<char> pureSigDifferential(32, '-');
	for (int i = 0; i < lList.size(); ++i) {
		char current;
		if (e8Differential[lList[i][0]]=='n') {
			current = 'u';
		}
		else {
			current = 'n';
		}
		for (int j = lList[i][0]; j < lList[i][1]; ++j) {
			if (isDifferent(sigE10Differential[j])) {
				pureSigDifferential[j] = e8Differential[lList[i][0]];
			}
			else {
				if (e8Differential[j]!=e8Differential[lList[i][0]]) {
					return false;
				}
			}
		}
		if (isDifferent(sigE10Differential[lList[i][1]])) {
			pureSigDifferential[lList[i][1]] = current;
		}
		else {
			if (e8Differential[lList[i][1]]!=current) {
				return false;
			}
		}
	}
	
	// get A8 differential which gets pureSigDifferential
	vector<vector<int> > sig1CausingBits = getCausingBits("CAP_SIGMA_1");

	Conditions conditions;
	for (int i = 0; i < 32; ++i) {
		if (!isDifferent(e10Differential[i])) {
			conditions.addParticularVariable(to_string(i), i, i);
		}
	}

	// --- start here

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(pureSigDifferential[i])) {
			int count = 0;
			int countU = 0;
			int countV = 0;
			char diffType;
			vector<int> relationVariables;
			for (int j = 0; j < sig1CausingBits[i].size(); ++j) {
				if (isDifferent(e10Differential[sig1CausingBits[i][j]])) {
					count++;
					diffType = e10Differential[sig1CausingBits[i][j]];
				}
				else {
					relationVariables.push_back(sig1CausingBits[i][j]);
				}
			}
			if (count==1) {
				if (diffType == pureSigDifferential[i]) {
					conditions.setEqual(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
				else {
					conditions.setDifferent(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
			}
			if (count==3) {
				if (e10Differential[sig1CausingBits[i][0]]=='u') {
					countU++;
				}
				if (e10Differential[sig1CausingBits[i][1]]=='u') {
					countU++;
				}
				if (e10Differential[sig1CausingBits[i][2]]=='u') {
					countU++;
				}
				if (countU==1 || countU==3) {
					if (pureSigDifferential[i]!='u') {
						return false;
					}
				}
				else {
					if (pureSigDifferential[i]!='n') {
						return false;
					}
				}
			}
		}
	}

	if (!conditions.areSatisfiable()) {
		return false;
	}

	if (!checking) {
		setValidAssignment(e10Differential, conditions);
	}

	return true;
}

bool setE8appropriatelyFor23(vector<char> &e8Differential, bool checking) {
	vector<char> sigE8Differential = calculateFromCross(CAP_SIGMA_1, e8Differential);
	vector<char> cancelWith = getOppositeDifferential(e8Differential);
	vector<vector<int> > lList = getLListForCut(sigE8Differential, cancelWith);
	vector<char> pureSigDifferential(32, '-');
	for (int i = 0; i < lList.size(); ++i) {
		char current;
		if (cancelWith[lList[i][0]]=='n') {
			current = 'u';
		}
		else {
			current = 'n';
		}
		for (int j = lList[i][0]; j < lList[i][1]; ++j) {
			if (isDifferent(sigE8Differential[j])) {
				pureSigDifferential[j] = cancelWith[lList[i][0]];
			}
			else {
				if (cancelWith[j]!=cancelWith[lList[i][0]]) {
					return false;
				}
			}
		}
		if (isDifferent(sigE8Differential[lList[i][1]])) {
			pureSigDifferential[lList[i][1]] = current;
		}
		else {
			if (cancelWith[lList[i][1]]!=current) {
				return false;
			}
		}
	}
	
	// get A8 differential which gets pureSigDifferential
	vector<vector<int> > sig1CausingBits = getCausingBits("CAP_SIGMA_1");

	Conditions conditions;
	for (int i = 0; i < 32; ++i) {
		if (!isDifferent(e8Differential[i])) {
			conditions.addParticularVariable(to_string(i), i, i);
		}
	}

	// --- start here

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(pureSigDifferential[i])) {
			int count = 0;
			int countU = 0;
			int countV = 0;
			char diffType;
			vector<int> relationVariables;
			for (int j = 0; j < sig1CausingBits[i].size(); ++j) {
				if (isDifferent(e8Differential[sig1CausingBits[i][j]])) {
					count++;
					diffType = e8Differential[sig1CausingBits[i][j]];
				}
				else {
					relationVariables.push_back(sig1CausingBits[i][j]);
				}
			}
			if (count==1) {
				if (diffType == pureSigDifferential[i]) {
					conditions.setEqual(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
				else {
					conditions.setDifferent(to_string(relationVariables[0]), relationVariables[0], relationVariables[0], to_string(relationVariables[1]), relationVariables[1], relationVariables[1]);
				}
			}
			if (count==3) {
				if (e8Differential[sig1CausingBits[i][0]]=='u') {
					countU++;
				}
				if (e8Differential[sig1CausingBits[i][1]]=='u') {
					countU++;
				}
				if (e8Differential[sig1CausingBits[i][2]]=='u') {
					countU++;
				}
				if (countU==1 || countU==3) {
					if (pureSigDifferential[i]!='u') {
						return false;
					}
				}
				else {
					if (pureSigDifferential[i]!='n') {
						return false;
					}
				}
			}
		}
	}

	if (!conditions.areSatisfiable()) {
		return false;
	}

	if (!checking) {
		setValidAssignment(e8Differential, conditions);
	}

	return true;
}

bool constructCompletely(vector<char> &w16VecCanceller, vector<char> w16Vec) {
	// vector<char> sigE8Differential = calculateFromCross(CAP_SIGMA_1, e8Differential);
	vector<char> cancelWith = getOppositeDifferential(w16Vec);
	vector<vector<int> > lList = getLListForCut(w16VecCanceller, cancelWith);
	vector<char> pureSigDifferential(32, '-');
	for (int i = 0; i < lList.size(); ++i) {
		char current;
		if (cancelWith[lList[i][0]]=='n') {
			current = 'u';
		}
		else {
			current = 'n';
		}
		for (int j = lList[i][0]; j < lList[i][1]; ++j) {
			if (isDifferent(w16VecCanceller[j])) {
				pureSigDifferential[j] = cancelWith[lList[i][0]];
			}
			else {
				if (cancelWith[j]!=cancelWith[lList[i][0]]) {
					return false;
				}
			}
		}
		if (isDifferent(w16VecCanceller[lList[i][1]])) {
			pureSigDifferential[lList[i][1]] = current;
		}
		else {
			if (cancelWith[lList[i][1]]!=current) {
				return false;
			}
		}
	}
	w16VecCanceller = pureSigDifferential;
	return canCancel(w16VecCanceller, w16Vec);
}

bool constructCompletelyW11(vector<char> w11Vec, vector<char> w16Vec, vector<char> cancelWith) {
	// get canceller for w11
	// cjecl conditions are satisfiable or not
	// get a satisfying argumernt
	return false;
}

bool canFindW11(vector<vector<char> > aDifferential, vector<vector<char> > eDifferential, vector<vector<char> > &wDifferential) {
	int state = -1;
	bool found = false;
	bool checkAll = false;
	// cout << "before e7" << endl;
	vector<char> w16Vec = wDifferential[16];
	// cout << "before goodV" << endl;
	vector<char> goodV = getGoodDiff(w16Vec);
	// cout << "before triasl" << endl;
	// if (32-hammingWeight(goodV)<15) {
	// 	cout << "Checking all here :";
	// 	printSingleLine(goodV);
	// 	checkAll=true;
	// }

	vector<char> w11Canceller = getOppositeDifferential(eDifferential[8]); // we can get many

	int trials = 0;
	while(!found && state!=-2) {
		trials++;
		// cout << trials << endl;
		if (trials>10000) {
			break;
		}
		// cout << "before " << endl;
		vector<char> w16VecCurrent = getRandomCanceler(eDifferential[8], checkAll, state, goodV);
		vector<char> w16CurrentSmallSig1 = calculateFromCross(SMALL_SIGMA_1, w16VecCurrent);
		// cout << "w15Vec : "; printSingleLine(w15Vec);
		if (canCancel(w16CurrentSmallSig1, w11Canceller)!=-1) {
			vector<char> w11Vec = w16CurrentSmallSig1;
			// if (constructCompletely(w16, w16Vec)) {
			// 	if (constructCompletelyW11(w11Vec, w16VecCanceller, w11Canceller)) {
			// 		wDifferential[16] = w16VecCanceller;
			// 		wDifferential[11] = w11Vec;
			// 		found = true;
			// 	}
			// }
		}
	}
	return found;
}

void setAllElse(vector<vector<char> > &aDifferential, vector<vector<char> > &eDifferential, vector<vector<char> > &wDifferential) {
	
	wDifferential[8] = getOppositeDifferential(wDifferential[9]);

	// for A first
	vector<char> a8Sig = calculateFromCross(CAP_SIGMA_0, aDifferential[8]);
	vector<vector<int> > lList = getLList(a8Sig, eDifferential[9]);
	vector<int> maj8DiffNot;
	for (int i = 0; i < lList.size(); ++i) {
		for (int j = lList[i][0]; j < lList[i][1]+1; ++j) {
			if (isDifferent(aDifferential[8][j])) {
				maj8DiffNot.push_back(j);
			}
		}
	}
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(aDifferential[8][i])) {
			if (find(maj8DiffNot.begin(), maj8DiffNot.end(), i)==maj8DiffNot.end()) {
				aDifferential[6][i]='1';
				aDifferential[7][i]='0';
			}
			else {
				aDifferential[6][i]='0';
				aDifferential[7][i]='0';
			}
		}
	}

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(aDifferential[8][i])) {
			aDifferential[7][i]='0';
			aDifferential[9][i]='1';
			aDifferential[10][i]='1';
		}
	}

	// now E
	for (int i = 0; i < 32; ++i) {
		if (isDifferent(eDifferential[12][i])) {
			eDifferential[14][i]='1';
		}
	}

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(eDifferential[12][i])) {
			eDifferential[13][i]='1';
		}
	}

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(eDifferential[12][i])) {
			eDifferential[11][i]='1';
		}
	}

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(eDifferential[12][i])) {
			eDifferential[7][i]='0';
		}
	}

	for (int i = 0; i < 32; ++i) {
		if (isDifferent(eDifferential[12][i])) {
			eDifferential[6][i]='1';
		}
	}

	return;
}

vector<vector<vector<vector<char> > > > getCompleteDifferentials(vector<vector<vector<char> > > partialDifferentials) {
	vector<vector<vector<vector<char> > > > res;
	vector<vector<char> > aDifferential = partialDifferentials[0];
	vector<vector<char> > eDifferential = partialDifferentials[1];
	vector<vector<char> > wDifferential = partialDifferentials[2];

	vector<vector<char> > aDifferentialCopy = aDifferential;
	vector<vector<char> > eDifferentialCopy = eDifferential;
	vector<vector<char> > wDifferentialCopy = wDifferential;

	// getCompleteDifferentialsHelper(partialDifferentials, res);
	
	int totalPossibilities = 0;
	for (int i = 0; i < 32; ++i) {
		if (eDifferential[8][i]=='x') {
			totalPossibilities++;
		}
	}
	totalPossibilities = pow(2, totalPossibilities);

	vector<int> possibleStates;
	for (int i = 0; i < totalPossibilities+1; ++i) {
		aDifferential = aDifferentialCopy;
		eDifferential = eDifferentialCopy;
		wDifferential = wDifferentialCopy;

		int currentState = i;
		vector<char> currentDifferenceVec = getE7WithState(currentState, eDifferential[8]);
		setAllDifferentials(aDifferential, eDifferential, wDifferential, currentDifferenceVec);
		bool checking = true;
		// cout << "here" << endl;
		if (!setA8appropriatelyFor23(aDifferential[8], checking)) {
			// continue;
		}
		else if (!setE12appropriatelyFor23(eDifferential[12], checking)) {
			// continue;
		}
		else if (!setE9appropriatelyFor23(eDifferential[9], checking)) {
			// cout << "E9" << endl;
			// continue;
		}
		else if (!setE10appropriatelyFor23(eDifferential[10], checking)) {
			// cout << "E10" << endl;
			// continue;
		}
		else if (!setE8appropriatelyFor23(eDifferential[8], checking)) {
			// cout << "E8" << endl;
			// continue;
		}
		// else if (!setW15appropriately(wDifferential[15], checking, wDifferential[10])) {
		// 	// continue;
		// }
		else {
			// cout << "Found 1" << " ";
			possibleStates.push_back(currentState);
			// break;
		}
		// printDifferentials(true, true, true, wDifferential, eDifferential, aDifferential);
		// break;
	}
	cout << possibleStates.size() << endl;
	// for (int i = 0; i < possibleStates.size(); ++i) {
	// 	cout << possibleStates[i] << " ";
	// }
	// cout << endl;
	for (int i = 0; i < possibleStates.size(); ++i) {
		aDifferential = aDifferentialCopy;
		eDifferential = eDifferentialCopy;
		wDifferential = wDifferentialCopy;

		int currentState = possibleStates[i];
		vector<char> currentDifferenceVec = getE7WithState(currentState, eDifferential[8]);
		setAllDifferentials(aDifferential, eDifferential, wDifferential, currentDifferenceVec);
		bool checking = false;
		// cout << "currentState is " << currentState << endl;
		if (!setA8appropriatelyFor23(aDifferential[8], checking)) {
			// continue;
		}
		else if (!setE12appropriatelyFor23(eDifferential[12], checking)) {
			// continue;
		}
		else if (!setE9appropriatelyFor23(eDifferential[9], checking)) {
			// continue;
		}
		else if (!setE10appropriatelyFor23(eDifferential[10], checking)) {
			// continue;
		}
		else if (!setE8appropriatelyFor23(eDifferential[8], checking)) {
			// continue;
		}
		else if (!canFind(wDifferential[11], wDifferential[16], eDifferential[8])) {
			continue;
		}
		// cout << "Found 23" << endl;
		// if (!canFindW8(wDifferential[8], wDifferential[16])) {
		// 	continue;
		// }
		setAllElse(aDifferential, eDifferential, wDifferential);
		printDifferentials(true, true, true, wDifferential, eDifferential, aDifferential);
		vector<vector<vector<char> > > diffs = {aDifferential, eDifferential, wDifferential};
		res.push_back(diffs);
		// break;
		
		// cout << "Strating w15... " << endl;
		// if (canFind(wDifferential[10], wDifferential[15], eDifferential[7])) {
		// 	printDifferentials(true, true, true, wDifferential, eDifferential, aDifferential);
		// 	vector<vector<vector<char> > > diffs = {aDifferential, eDifferential, wDifferential};
		// 	res.push_back(diffs);
		// 	break;
		// }
		// cout << "done." << endl;
	}
	return res;
}

void fillVarsUsingDifferentialWithStateSemi(unsigned int &eNum, unsigned int &eNum2, vector<char> differential, int &state, bool checkAll) {
	vector<int> stateInBinary = convertIntToVector(state);
	int posOfState = 0;
	vector<int> numVec(32, 0);
	vector<int> num2Vec(32, 0);
	for (int i = 0; i < 32; ++i) {
		if (differential[i]=='u') {
			numVec[i]=1;
			num2Vec[i]=0;
		}
		else if (differential[i]=='n') {
			numVec[i]=0;
			num2Vec[i]=1;
		}
		else if (differential[i]=='0') {
			num2Vec[i]=0;
			numVec[i]=0;
		}
		else if (differential[i]=='1') {
			numVec[i]=1;
			num2Vec[i]=1;
		}
		else {
			numVec[i] = stateInBinary[posOfState];
			posOfState++;
			num2Vec[i]=numVec[i];
		}
	}
	eNum2 = convertVectorToInt(num2Vec);
	eNum = convertVectorToInt(numVec);
	if (checkAll) {
		for (int i = posOfState; i < 32; ++i) {
			if (stateInBinary[i]==1) {
				state = -2;
			}
		}
	}
	return;
}

bool variableFollowsDifferential(unsigned int a, unsigned int a2, vector<char> differential) {
	vector<int> aDiff = convertIntToVector(a);
	vector<int> a2Diff = convertIntToVector(a2);
	vector<char> aDifferential = differential;
	bool check1 = true;

	// cout << step << " " << aVar.getIndex(step) << " " << aVar2.getIndex(step) << endl;
	// cout << step << endl;
	for (int i = 0; i < 32; ++i) {
		if (aDifferential[i]=='-') {
			if (aDiff[i]!=a2Diff[i]) {
				// cout << "here";
				check1 = false;
			}
		}
		if (aDifferential[i]=='x') {
			if (aDiff[i]==a2Diff[i]) {
				// cout << "here2";
				check1 = false;
			}
		}
		if (aDifferential[i]=='0') {
			if (aDiff[i]!=0 || a2Diff[i]!=0) {
				// cout << "here3";
				check1 = false;
			}
		}
		if (aDifferential[i]=='1') {
			if (aDiff[i]!=1 || a2Diff[i]!=1) {
				// cout << "here4";
				check1 = false;
			}
		}
		if (aDifferential[i]=='n') {
			if (aDiff[i]!=0 || a2Diff[i]!=1) {
				// cout << "here4";
				check1 = false;
			}
		}
		if (aDifferential[i]=='u') {
			if (aDiff[i]!=1 || a2Diff[i]!=0) {
				// cout << "here4";
				check1 = false;
			}
		}
	}
	return check1;
}

bool conditionsStatisfied(AVarSHA2 aVar, EVarSHA2 eVar, vector<unsigned int> w, AVarSHA2 aVar2, EVarSHA2 eVar2, vector<unsigned int> w2, vector<vector<vector<char> > > &differentials, int step) {
	vector<int> aDiff = convertIntToVector(aVar.getIndex(step));
	vector<int> a2Diff = convertIntToVector(aVar2.getIndex(step));
	vector<char> aDifferential = differentials[0][step];
	bool check1 = true;
	int pos1 =0;

	// cout << step << " " << aVar.getIndex(step) << " " << aVar2.getIndex(step) << endl;
	// cout << step << endl;
	for (int i = 0; i < 32; ++i) {
		if (aDifferential[i]=='-') {
			if (aDiff[i]!=a2Diff[i]) {
				// cout << "here";
				check1 = false;
			}
		}
		if (aDifferential[i]=='x') {
			if (aDiff[i]==a2Diff[i]) {
				// cout << "here2";
				check1 = false;
			}
		}
		if (aDifferential[i]=='0') {
			if (aDiff[i]!=0 || a2Diff[i]!=0) {
				// cout << "here3";
				pos1=i;
				check1 = false;
				break;
			}
		}
		if (aDifferential[i]=='1') {
			if (aDiff[i]!=1 || a2Diff[i]!=1) {
				// cout << "here4";
				check1 = false;
				pos1=i;
				break;
			}
		}
		if (aDifferential[i]=='n') {
			if (aDiff[i]!=0 || a2Diff[i]!=1) {
				// cout << "here4";
				check1 = false;
			}
		}
		if (aDifferential[i]=='u') {
			if (aDiff[i]!=1 || a2Diff[i]!=0) {
				// cout << "here4";
				check1 = false;
			}
		}
	}

	vector<int> eDiff = convertIntToVector(eVar.getIndex(step));
	vector<int> e2Diff = convertIntToVector(eVar2.getIndex(step));
	vector<char> eDifferential = differentials[1][step];
	bool check2 = true;
	int pos2=0;
	for (int i = 0; i < 32; ++i) {
		if (eDifferential[i]=='-') {
			if (eDiff[i]!=e2Diff[i]) {
				check2 = false;
			}
		}
		if (eDifferential[i]=='x') {
			if (eDiff[i]==e2Diff[i]) {
				check2 = false;
			}
		}
		if (eDifferential[i]=='0') {
			if (eDiff[i]!=0 || e2Diff[i]!=0) {
				check2 = false;
				pos2=i;
				break;
			}
		}
		if (eDifferential[i]=='1') {
			if (eDiff[i]!=1 || e2Diff[i]!=1) {
				check2 = false;
				pos2=i;
				break;
			}
		}
		if (eDifferential[i]=='n') {
			if (eDiff[i]!=0 || e2Diff[i]!=1) {
				// cout << "here4";
				check2 = false;
			}
		}
		if (eDifferential[i]=='u') {
			if (eDiff[i]!=1 || e2Diff[i]!=0) {
				// cout << "here4";
				check2 = false;
			}
		}
	}

	vector<int> wDiff = convertIntToVector(w[step]);
	vector<int> w2Diff = convertIntToVector(w2[step]);
	vector<char> wDifferential = differentials[2][step];
	bool check3 = true;
	for (int i = 0; i < 32; ++i) {
		if (wDifferential[i]=='?') {
			continue;
		}
		if (wDifferential[i]=='-') {
			if (wDiff[i]!=w2Diff[i]) {
				check3 = false;
			}
		}
		if (wDifferential[i]=='x') {
			if (wDiff[i]==w2Diff[i]) {
				check3 = false;
			}
		}
		if (wDifferential[i]=='0') {
			if (wDiff[i]!=0 || w2Diff[i]!=0) {
				check3 = false;
			}
		}
		if (wDifferential[i]=='1') {
			if (wDiff[i]!=1 || w2Diff[i]!=1) {
				check3 = false;
			}
		}
		if (wDifferential[i]=='n') {
			if (wDiff[i]!=0 || w2Diff[i]!=1) {
				// cout << "here4";
				check3 = false;
			}
		}
		if (wDifferential[i]=='u') {
			if (wDiff[i]!=1 || w2Diff[i]!=0) {
				// cout << "here4";
				check3 = false;
			}
		}
	}

	// if ((check1 || check2) && step>6) {
	// 	cout << step << check1 << check2 << endl;
	// }
	// if (!check3) {
	// 	cout << "check3 failed" << endl;
	// }
	if (step==9 || step==8 ) {
		// if ((check1 || check2)) {
		// 	cout << step << " " << check1 << " " << pos1 << "   " << check2 << " " << pos2 << endl;
		// }
		return check1 & check2;
	}
	// if ((check1 || check2)) {
	// 	cout << step << " " << check1 << " " << pos1 << "   " << check2 << " " << pos2 << endl;
	// }
	return check1 & check2 & check3;
}

bool conditionsStatisfiedForSemi(AVarSHA2 aVar, EVarSHA2 eVar, vector<unsigned int> w, AVarSHA2 aVar2, EVarSHA2 eVar2, vector<unsigned int> w2, vector<vector<vector<char> > > &differentials, int step) {
	vector<int> aDiff = convertIntToVector(aVar.getIndex(step-4));
	vector<int> a2Diff = convertIntToVector(aVar2.getIndex(step-4));
	vector<char> aDifferential = differentials[0][step-4];
	bool check1 = true;
	int pos1 =0;

	// cout << step << " " << aVar.getIndex(step) << " " << aVar2.getIndex(step) << endl;
	// cout << step << endl;
	for (int i = 0; i < 32; ++i) {
		if (aDifferential[i]=='-') {
			if (aDiff[i]!=a2Diff[i]) {
				// cout << "here";
				check1 = false;
				pos1 = i;
				break;
			}
		}
		if (aDifferential[i]=='x') {
			if (aDiff[i]==a2Diff[i]) {
				// cout << "here2";
				check1 = false;
				pos1 = i;
				break;
			}
		}
		if (aDifferential[i]=='0') {
			if (aDiff[i]!=0 || a2Diff[i]!=0) {
				// cout << "here3";
				check1 = false;
				pos1 = i;
				break;
			}
		}
		if (aDifferential[i]=='1') {
			if (aDiff[i]!=1 || a2Diff[i]!=1) {
				// cout << "here4";
				check1 = false;
				pos1 = i;
				break;
			}
		}
		if (aDifferential[i]=='n') {
			if (aDiff[i]!=0 || a2Diff[i]!=1) {
				// cout << "here4";
				check1 = false;
			}
		}
		if (aDifferential[i]=='u') {
			if (aDiff[i]!=1 || a2Diff[i]!=0) {
				// cout << "here4";
				check1 = false;
			}
		}
	}

	vector<int> eDiff = convertIntToVector(eVar.getIndex(step-4));
	vector<int> e2Diff = convertIntToVector(eVar2.getIndex(step-4));
	vector<char> eDifferential = differentials[1][step-4];
	bool check2 = true;
	int pos2 = 0;
	for (int i = 0; i < 32; ++i) {
		if (eDifferential[i]=='-') {
			if (eDiff[i]!=e2Diff[i]) {
				check2 = false;
			}
		}
		if (eDifferential[i]=='x') {
			if (eDiff[i]==e2Diff[i]) {
				check2 = false;
			}
		}
		if (eDifferential[i]=='0') {
			if (eDiff[i]!=0 || e2Diff[i]!=0) {
				check2 = false;
				pos2 = i;
				break;
			}
		}
		if (eDifferential[i]=='1') {
			if (eDiff[i]!=1 || e2Diff[i]!=1) {
				check2 = false;
				pos2 = i;
				break;
			}
		}
		if (eDifferential[i]=='n') {
			if (eDiff[i]!=0 || e2Diff[i]!=1) {
				// cout << "here4";
				check2 = false;
			}
		}
		if (eDifferential[i]=='u') {
			if (eDiff[i]!=1 || e2Diff[i]!=0) {
				// cout << "here4";
				check2 = false;
			}
		}
	}

	vector<int> wDiff = convertIntToVector(w[step]);
	vector<int> w2Diff = convertIntToVector(w2[step]);
	vector<char> wDifferential = differentials[2][step];
	bool check3 = true;
	for (int i = 0; i < 32; ++i) {
		if (wDifferential[i]=='?') {
			continue;
		}
		if (wDifferential[i]=='-') {
			if (wDiff[i]!=w2Diff[i]) {
				check3 = false;
			}
		}
		if (wDifferential[i]=='x') {
			if (wDiff[i]==w2Diff[i]) {
				check3 = false;
			}
		}
		if (wDifferential[i]=='0') {
			if (wDiff[i]!=0 || w2Diff[i]!=0) {
				check3 = false;
			}
		}
		if (wDifferential[i]=='1') {
			if (wDiff[i]!=1 || w2Diff[i]!=1) {
				check3 = false;
			}
		}
		if (wDifferential[i]=='n') {
			if (wDiff[i]!=0 || w2Diff[i]!=1) {
				// cout << "here4";
				check3 = false;
			}
		}
		if (wDifferential[i]=='u') {
			if (wDiff[i]!=1 || w2Diff[i]!=0) {
				// cout << "here4";
				check3 = false;
			}
		}
	}

	// if ((check1 || check2)) {
	// 	cout << step << " " << check1 << " " << pos1 << "   " << check2 << " " << pos2 << endl;
	// }
	// if (!check3) {
	// 	cout << "check3 failed" << endl;
	// }
	return check1 & check2 & check3;
}

void fillWVarsForSemi(AVarSHA2& aVar, EVarSHA2& eVar, vector<unsigned int> &w, AVarSHA2& aVar2, EVarSHA2& eVar2, vector<unsigned int> &w2, int step) {
	unsigned int res = EP0(aVar.getIndex(step-1));
	res = res + MAJ(aVar.getIndex(step-1), aVar.getIndex(step-2), aVar.getIndex(step-3));
	res = res + EP1(eVar.getIndex(step-1));
	res = res + CH(eVar.getIndex(step-1), eVar.getIndex(step-2), eVar.getIndex(step-3));
	res = res + eVar.getIndex(step-4);
	res = res + K[step];
	res = aVar.getIndex(step) - res;
	w[step] = res;

	unsigned int res2 = EP0(aVar2.getIndex(step-1));
	res2 = res2 + MAJ(aVar2.getIndex(step-1), aVar2.getIndex(step-2), aVar2.getIndex(step-3));
	res2 = res2 + EP1(eVar2.getIndex(step-1));
	res2 = res2 + CH(eVar2.getIndex(step-1), eVar2.getIndex(step-2), eVar2.getIndex(step-3));
	res2 = res2 + eVar2.getIndex(step-4);
	res2 = res2 + K[step];
	res2 = aVar2.getIndex(step) - res2;
	w2[step] = res2;
	return;
}

void fillAVarsForSemi(AVarSHA2& aVar, EVarSHA2& eVar, vector<unsigned int> w, AVarSHA2& aVar2, EVarSHA2& eVar2, vector<unsigned int> w2, int step) {
	// unsigned int res = aVar.getIndex(step-4);
	unsigned int res = EP1(eVar.getIndex(step-1));
	res = res + CH(eVar.getIndex(step-1), eVar.getIndex(step-2), eVar.getIndex(step-3));
	res = res + eVar.getIndex(step-4);
	res = res + K[step] + w[step];
	// eVar.setIndex(step, res);
	res = eVar.getIndex(step) - res;
	aVar.setIndex(step-4, res);

	// unsigned int res2 = aVar2.getIndex(step-4);
	unsigned int res2 = EP1(eVar2.getIndex(step-1));
	res2 = res2 + CH(eVar2.getIndex(step-1), eVar2.getIndex(step-2), eVar2.getIndex(step-3));
	res2 = res2 + eVar2.getIndex(step-4);
	res2 = res2 + K[step] + w2[step];
	// eVar2.setIndex(step, res2);
	res2 = eVar2.getIndex(step) - res2;
	aVar2.setIndex(step-4, res2);
	return;
}

void fillEVarsForSemi(AVarSHA2& aVar, EVarSHA2& eVar, vector<unsigned int> w, AVarSHA2& aVar2, EVarSHA2& eVar2, vector<unsigned int> w2, int step) {
	unsigned int res = EP0(aVar.getIndex(step-1));
	res = res + MAJ(aVar.getIndex(step-1), aVar.getIndex(step-2), aVar.getIndex(step-3));
	res = res + EP1(eVar.getIndex(step-1));
	res = res + CH(eVar.getIndex(step-1), eVar.getIndex(step-2), eVar.getIndex(step-3));
	// res = res + eVar.getIndex(step-4);
	res = res + K[step] + w[step];
	res = aVar.getIndex(step) - res;
	eVar.setIndex(step-4, res);

	unsigned int res2 = EP0(aVar2.getIndex(step-1));
	res2 = res2 + MAJ(aVar2.getIndex(step-1), aVar2.getIndex(step-2), aVar2.getIndex(step-3));
	res2 = res2 + EP1(eVar2.getIndex(step-1));
	res2 = res2 + CH(eVar2.getIndex(step-1), eVar2.getIndex(step-2), eVar2.getIndex(step-3));
	// res2 = res2 + eVar2.getIndex(step-4);
	res2 = res2 + K[step] + w2[step];
	res2 = aVar2.getIndex(step) - res2;
	eVar2.setIndex(step-4, res2);
	return;
}
bool black = false;

void fillPrev(AVarSHA2 &aVar, EVarSHA2 &eVar, vector<unsigned int> &w, AVarSHA2 &aVar2, EVarSHA2 &eVar2, vector<unsigned int> &w2, vector<vector<vector<char> > > &differentials, int step, bool checkBelow4) {
	vector<vector<char> > aDifferential = differentials[0];
	vector<vector<char> > eDifferential = differentials[1];
	vector<vector<char> > wDifferential = differentials[2];

	// cout << "step is " << step << endl;

	// if (step==7) {
	// 	printMessages(w, w2);
	// 	string x;
	// 	cin >> x;
	// }

	// if (step<10) {
	// 	cout << "step is " << step << endl;
	// 	// printMessages(w, w2);
	// 	// black = true;
	// 	// return;
	// }

	if (!checkBelow4) {
		if (step==3) {
			black = true;
			return;
		}
	}

	int state = -1;
	bool checkAll = false;
	if (step==-1) {
		black = true;
		return;
	}

	if (step==11 || step<10) {
		checkAll = true;
		while(state!=-2) {
			unsigned int wVarStepVal;
			unsigned int wVarStepVal2;
			state = state +1;
			fillVarsUsingDifferentialWithStateSemi(wVarStepVal, wVarStepVal2, wDifferential[step], state, checkAll);
			w[step] = wVarStepVal;
			w2[step] = wVarStepVal2;

			if (step==0) {
				int i = step;
				w[i] = w[i+16] - SSIG1(w[i+14]) - w[i+9] - SSIG0(w[i+1]);
				w2[i] = w2[i+16] - SSIG1(w2[i+14]) - w2[i+9] - SSIG0(w2[i+1]);
			}

			fillEVarsForSemi(aVar, eVar, w, aVar2, eVar2, w2, step);
			fillAVarsForSemi(aVar, eVar, w, aVar2, eVar2, w2, step);
			if (step<4 || conditionsStatisfiedForSemi(aVar, eVar, w, aVar2, eVar2, w2, differentials, step)) {
				fillPrev(aVar, eVar, w, aVar2, eVar2, w2, differentials, step-1, checkBelow4);
				if (black==true) {
					return;
				}
			}
			// else {
			// 	if (step==9) {
			// 		int i=step;
			// 		cout << "ds0 "; printDifferentialForVars(EP0(aVar.getIndex(i-1)), EP0(aVar2.getIndex(i-1)));
			// 		cout << "dma "; printDifferentialForVars(MAJ(aVar.getIndex(i-1), aVar.getIndex(i-2), aVar.getIndex(i-3)), MAJ(aVar2.getIndex(i-1), aVar2.getIndex(i-2), aVar2.getIndex(i-3)));
			// 		cout << "w9: "; printDifferentialForVars(w[i], w2[i]);
			// 		cout << "e9: "; printDifferentialForVars(eVar.getIndex(i), eVar2.getIndex(i));
			// 		cout << "ds1 "; printDifferentialForVars(EP1(eVar.getIndex(i-1)), EP1(eVar2.getIndex(i-1)));
			// 		cout << "di: "; printDifferentialForVars(CH(eVar.getIndex(i-1), eVar.getIndex(i-2), eVar.getIndex(i-3)), CH(eVar2.getIndex(i-1), eVar2.getIndex(i-2), eVar2.getIndex(i-3)));	
					
			// 	}
			// 	Conditions conditions;
			// 	printTableDetails(aVar, eVar, w, aVar2, eVar2, w2, differentials, conditions, step);
			// 	string x;
			// 	cin >> x;	
			// }
		}
	}
	else if (step==10) {
		int trials = 0;
		int THRES = 1;
		while(trials<THRES) {
			trials++;
			state = getRandomUnsignedInt();
			unsigned int eVarStepVal;
			unsigned int eVarStepVal2;
			fillVarsUsingDifferentialWithStateSemi(eVarStepVal, eVarStepVal2, eDifferential[step-4], state, checkAll);
			eVar.setIndex(step-4, eVarStepVal);
			eVar2.setIndex(step-4, eVarStepVal2);

			fillWVarsForSemi(aVar, eVar, w, aVar2, eVar2, w2, step);
			fillAVarsForSemi(aVar, eVar, w, aVar2, eVar2, w2, step);
			if (variableFollowsDifferential(aVar.getIndex(step-4), aVar2.getIndex(step-4), aDifferential[step-4])) {
				THRES = 100000;
			}
			if (conditionsStatisfiedForSemi(aVar, eVar, w, aVar2, eVar2, w2, differentials, step)) {
				fillPrev(aVar, eVar, w, aVar2, eVar2, w2, differentials, step-1, checkBelow4);
				if (black==true) {
					return;
				}
			}
			// else {
			// 	Conditions conditions;
			// 	printTableDetails(aVar, eVar, w, aVar2, eVar2, w2, differentials, conditions, step);
			// 	string x;
			// 	cin >> x;	
			// }
		}
	}
}


void fill4RandomlyWithStartStep(AVarSHA2 &aVar, EVarSHA2 &eVar, AVarSHA2 &aVar2, EVarSHA2 &eVar2, vector<vector<vector<char> > > &differentials, int startStep) {
	vector<vector<char> > aDifferential = differentials[0];
	vector<vector<char> > eDifferential = differentials[1];
	
	// setting a var first
	for (int i = startStep; i < startStep+4; ++i) {
		unsigned int aVarStepVal;
		unsigned int aVarStepVal2;
		int state = getRandomUnsignedInt();
		fillVarsUsingDifferentialWithStateSemi(aVarStepVal, aVarStepVal2, aDifferential[i], state, false);
		aVar.setIndex(i, aVarStepVal);
		aVar2.setIndex(i, aVarStepVal2);
	}

	// setting e var next
	for (int i = startStep; i < startStep+4; ++i) {
		unsigned int eVarStepVal;
		unsigned int eVarStepVal2;
		int state = getRandomUnsignedInt();
		fillVarsUsingDifferentialWithStateSemi(eVarStepVal, eVarStepVal2, eDifferential[i], state, false);
		eVar.setIndex(i, eVarStepVal);
		eVar2.setIndex(i, eVarStepVal2);
	}

	return;
}

bool filledOnce = false;

void findFreeStartCollisionByFillingStep(AVarSHA2 &aVar, EVarSHA2 &eVar, vector<unsigned int> &w, AVarSHA2 &aVar2, EVarSHA2 &eVar2, vector<unsigned int> &w2, vector<vector<vector<char> > > &differentials, int startStep) {
	int trials = 0;
	bool checkBelow4 = false;
	while(black!=true) {
		trials++;
		if (trials%100000000==0) {
			cout << "sranding...";
			srand(time(0));
		}
		fill4RandomlyWithStartStep(aVar, eVar, aVar2, eVar2, differentials, startStep);
		fillPrev(aVar, eVar, w, aVar2, eVar2, w2, differentials, startStep+3, checkBelow4);
	}
	black = false;
	filledOnce = true;
	// printTableDetails(aVar, eVar, w, aVar2, eVar2, w2, differentials, dummyVar, startStep+3);
	return;
}