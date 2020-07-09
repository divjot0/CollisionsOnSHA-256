#include <bits/stdc++.h> 

using namespace std;

// interfaces provided :::
	// conditions (which are implemented as graph here)

// functions provided :::
	// getCapSig(<<<x, <<<y, <<<z);
	// getSmallSig(<<<x, <<<y, <<z);

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
		if (v[v.size()-1]=='n') {
			u[u.size()-1]='u';
		}
		else {
			u[u.size()-1]='n';
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
			conditionsGraph.addVertex(variableNameString + "=u");
			conditionsGraph.addVertex(variableNameString + "=n");
		}
		return;
	}

	void addParticularVariable(string W, int lineNumber, int bitPosition) {
		string variableNameString = getVariableNameString(W, lineNumber, bitPosition);
		conditionsGraph.addVertex(variableNameString + "=u");
		conditionsGraph.addVertex(variableNameString + "=n");
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
		conditionsGraph.addEdge(v1 + "=u", v2 + "=u");
		conditionsGraph.addEdge(v2 + "=u", v1 + "=u");
		conditionsGraph.addEdge(v1 + "=n", v2 + "=n");
		conditionsGraph.addEdge(v2 + "=n", v1 + "=n");
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
		conditionsGraph.addEdge(v1 + "=u", v2 + "=n");
		conditionsGraph.addEdge(v2 + "=n", v1 + "=u");
		conditionsGraph.addEdge(v1 + "=n", v2 + "=u");
		conditionsGraph.addEdge(v2 + "=u", v1 + "=n");
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
		newConditionsGraph.addEdge(v1 + "=u", v2 + "=u");
		newConditionsGraph.addEdge(v2 + "=u", v1 + "=u");
		newConditionsGraph.addEdge(v1 + "=n", v2 + "=n");
		newConditionsGraph.addEdge(v2 + "=n", v1 + "=n");
		if (newConditionsGraph.isSatisfiable()) {
			return true;
		}
		return false;
	}

	bool canSetDifferent(string W1, int lineNumber1, int bitPosition1, string W2, int lineNumber2, int bitPosition2) {
		Graph newConditionsGraph = conditionsGraph;
		string v1 = getVariableNameString(W1, lineNumber1, bitPosition1);
		string v2 = getVariableNameString(W2, lineNumber2, bitPosition2);
		if (conditionsStringSet.find(v1 + "!=" + v2)!=conditionsStringSet.end()) {
			// already present
			return this->areSatisfiable();
		}
		newConditionsGraph.addEdge(v1 + "=u", v2 + "=n");
		newConditionsGraph.addEdge(v2 + "=n", v1 + "=u");
		newConditionsGraph.addEdge(v1 + "=n", v2 + "=u");
		newConditionsGraph.addEdge(v2 + "=u", v1 + "=n");
		if (newConditionsGraph.isSatisfiable()) {
			return true;
		}
		return false;
	}

private:
	Graph conditionsGraph;
	unordered_set<string> conditionsStringSet;
};




// next are the functions for SHA256

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

void printSingleLine(vector<char> v) {
	for (int i = 0; i < v.size(); ++i) {
		cout << v[i];
	}
	cout << endl;
}

vector<char> calculateFromFixed(vector<vector<int> > &sigma, vector<char> &numCross) {
	vector<char> numCrossU(32, '-');
	vector<char> numCrossN(32, '-');
	for (int i = 0; i < 32; ++i) {
		if (numCross[i]=='u') {
			numCrossU[i] = 'u';
		}
		if (numCross[i]=='n') {
			numCrossN[i] = 'n';
		}
	}
	vector<int> numValU = convertNumCrossToNumVal(numCrossU);
	vector<int> resValU = matMul(numValU, sigma);
	vector<char> resCrossU = convertNumValToNumCross(resValU);
	vector<int> numValN = convertNumCrossToNumVal(numCrossN);
	vector<int> resValN = matMul(numValN, sigma);
	vector<char> resCrossN = convertNumValToNumCross(resValN);
	vector<int> numVal = convertNumCrossToNumVal(numCross);
	vector<int> resVal = matMulMod2(numVal, sigma);
	vector<char> resCross = convertNumValToNumCross(resVal);

	// printSingleLine(numCrossU);
	// printSingleLine(numCrossN);
	// printSingleLine(resCrossU);
	// printSingleLine(resCrossN);
	// for (int i = 0; i < 12; ++i) {
	// 	cout << " ";
	// }
	// for (int i = 0; i < 32; ++i) {
	// 	cout << resValN[i];
	// }
	// cout << endl;

	// for (int i = 0; i < 12; ++i) {
	// 	cout << " ";
	// }
	// for (int i = 0; i < 32; ++i) {
	// 	cout << resValU[i];
	// }
	// cout << endl;

	for (int i = 0; i < 32; ++i) {
		cout << resValU[i]+resValN[i];
	}
	cout << endl;

	for (int i = 0; i < 32; ++i) {
		if (resVal[i]==1) {
			// if ((resValN[i] + resValU[i])==3) {
			// 	// cout << "here";
			// 	if (resValU[i]>resValN[i]) {
			// 		resCross[i]='u';
			// 	}
			// 	else {
			// 		resCross[i]='n';
			// 	}
			// }
			// else if ((resValN[i] + resValU[i])==1) {
			// 	if (resValU[i]>resValN[i]) {
			// 		resCross[i]='u';
			// 	}
			// 	else {
			// 		resCross[i]='n';
			// 	}
			// }
			// else {
			// 	resCross[i] = 'x';
			// }
			resCross[i] = 'x';
		}
	}
	return resCross;
}

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