#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <iterator>
#include "tableUtil.h"

#define DIFFERENTIAL_HEIGHT 27

using namespace std;

// for global usage
vector<vector<int> > smallSigma0;
vector<vector<int> > smallSigma1;

class GraphNode {
public:
	GraphNode(string varName) {
		this->varName = varName;
	}
	string getVarName() {
		return varName;
	}
	string varName;
};

class Graph {
public:
	Graph() {
		vertices = vector<GraphNode> ();
		varToIndex = map<string, int> ();
		indexCounter = 0;
	}
	bool addVertex(GraphNode graphNode) {
		if (varToIndex.find(graphNode.getVarName())!=varToIndex.end()) {
			//already present
			return false;
		}
		//insert node
		varToIndex.insert(pair<string, int> (graphNode.getVarName(), indexCounter));
		indexCounter++;
		this->vertices.push_back(graphNode);
		this->adjacencyList = vector<vector<int> > (this->vertices.size(), vector<int> ());
		return true;
	}
	bool addEdge(GraphNode a, GraphNode b) {
		if (varToIndex.find(a.getVarName())==varToIndex.end() || varToIndex.find(b.getVarName())==varToIndex.end()) {
			// one or more not intiated
			return false;
		}
		this->adjacencyList[varToIndex.find(a.getVarName())->second].push_back(varToIndex.find(b.getVarName())->second);
		return true;
	}
	vector<vector<int> > getEdges() {
		return adjacencyList;
	}

	vector<GraphNode> getVertices() {
		return vertices;
	}

	map<string, int> getVarToIndexMap() {
		return varToIndex;
	}
private:
	vector<GraphNode> vertices;
	int indexCounter;
	map<string, int> varToIndex;
	vector<vector<int> > adjacencyList;
};

string getHeightIndString(int i) {
	string res = "";
	if (i<10) {
		res = res + "0";
	}
	res = res + to_string(i);
	return res;
}

string getPosIndString(int i) {
	string res = to_string(i);
	return res;
}

vector<string> getVarName(string varSection, int height, int pos) {
	vector<string> res(2);
	res[0] = varSection + getHeightIndString(height) + "[" + getPosIndString(pos) + "]";
	res[1] = varSection + "'" + getHeightIndString(height) + "[" + getPosIndString(pos) + "]";
	return res;
}

// every E[i][j], !E[i][j] , E'[i][j], !E'[i][j] is a different vertex
Graph getGraphSetUp() {
	Graph conditions = Graph();
	GraphNode *gNode;
	//W[i][j], !W[i][j], W'[i][j], !W'[i][j]
	for (int i = 0; i < DIFFERENTIAL_HEIGHT; ++i) {
		for (int j = 0; j < 32; ++j) {
			vector<string> varName = getVarName("W", i, j);
			gNode = new GraphNode(varName[0]);
			conditions.addVertex(*gNode);
			gNode = new GraphNode("!" + varName[0]);
			conditions.addVertex(*gNode);
			gNode = new GraphNode(varName[1]);
			conditions.addVertex(*gNode);
			gNode = new GraphNode("!" + varName[1]);
			conditions.addVertex(*gNode);
		}
	}
	return conditions;
}

void addDifferentialHas_Condition(string varSection, int height, int pos, Graph &conditions) {
	vector<string> varName = getVarName(varSection, height, pos);
	conditions.addEdge(varName[0], varName[1]);
	conditions.addEdge(varName[1], varName[0]);
	return;
}

void addDifferentialHasXCondition(string varSection, int height, int pos, Graph &conditions) {
	vector<string> varName = getVarName(varSection, height, pos);
	conditions.addEdge(varName[0], "!"+varName[1]);
	conditions.addEdge("!"+varName[1], varName[0]);
	return;
}

vector<int> getDirectDifferenceBits(vector<vector<int> > &prevSigma, vector<char> deltaXCross) {
	vector<int> deltaX = convertNumCrossToNumVal(deltaXCross);
	vector<int> deltaSig = matMul(deltaX, prevSigma);
	return deltaSig;
}

bool addConditionsToCancelDifference(vector<char> diff0, vector<char> diff1, Graph conditions) {
	int i=0;
	bool possibleDiff=true;
	while (i<32) {
		if (diff0[i]=='-' && diff1[i]=='-') {
			i++;
		}
		else if (diff0[i]=='x' && diff1[i]=='x') {
			if (i==31) {
				setDifferentCondition();
				i++;
			}
			else {
				if (diff0[i+1]=='x' && diff1[i+1]=='x') {
					possibleDiff = false;
					break;
				}
				else if (diff0[i+1]=='x') {
					int j = i+1;
					while(j<32) {
						if (diff0[j]=='x' && diff1[j]=='-') {
							if (j==31 || diff0[j+1]=='-' || (diff0[j+1]=='x' && diff1[j+1]=='x')) {
								// this is last
								setConditionDifferent();
							}
							else {
								setConditionSame();
							}
							j++;
						}
					}
					i = j;
				}
				else if (diff1[i+1]=='x') {
					int j = i+1;
					while(j<32) {
						if (diff1[j]=='x' && diff0[j]=='-') {
							if (j==31 || diff1[j+1]=='-' || (diff1[j+1]=='x' && diff0[j+1]=='x')) {
								// this is last
								setConditionDifferent();
							}
							else {
								setConditionSame();
							}
							j++;
						}
					}
					i = j;
				}
				else {
					setConditionDifferent();
					i++;
				}
			}
		}
		else {
			possibleDiff=false;
			break;
		}
	}
	if (possibleDiff & conditions.satisfiable()) {
		return true;
	}
	return false;
}

void findValidWDifferentialsHelper(vector<vector<char> > wDifferential, Graph conditions, int step) {
	if (step==8) {
		// wDifferential[8][26] = 'x';
		// wDifferential[8][23] = 'x';
		// wDifferential[8][19] = 'x';
		// wDifferential[8][8] = 'x';
		// wDifferential[8][9] = 'x';

		vector<int> crossPositionsInW8 = {2,3,4,6,7,8,10,12,13,16,17,18,20,21,23,24,27,28,29,30,31};
		for (int i = 0; i < 32; ++i) {
			if (find(crossPositionsInW8.begin(), crossPositionsInW8.end(), i) != crossPositionsInW8.end()) {
				wDifferential[8][i] = 'x';
			}
			else {
				wDifferential[8][i] = '-';
			}
		}
		for (int i = 0; i < 32; ++i) {
			if (wDifferential[8][i]=='x') {
				addDifferentialHasXCondition("W", 8, i, conditions);
			}
			else{
				wDifferential[8][i] = '-';
				addDifferentialHas_Condition("W", 8, i, conditions);
			}
		}
		findValidWDifferentialsHelper(wDifferential, conditions, 7);
	}
	if (step==7) {
		int previousStep = 8;
		vector<vector<int> > prevSigma = smallSigma0;
		vector<int> directSig = getDirectDifferenceBits(prevSigma, wDifferential[previousStep]);

		cout << "This is directSig:  ";
		for (int i = 0; i < 32; ++i) {
			cout << directSig[i];
		}
		cout << endl;

		// select random possibilities and stop when we repeat for the first time
		unordered_set <long long> guesses;
		long long currentGuess = convertNumValToNum(directSig);
		vector<char> currentGuessVector = convertNumValToNumCross(directSig);
		while (guesses.find(currentGuess)==guesses.end()) {
			for (int i = 0; i < 32; ++i) {
				wDifferential[step][i] = currentGuessVector[i];
				if (wDifferential[step][i]=='-') {
					addDifferentialHas_Condition("W", step, i, conditions);
				}
				if (wDifferential[step][i]=='x') {
					addDifferentialHasXCondition("W", step, i, conditions);
				}
			}
			findValidWDifferentialsHelper(wDifferential, conditions, 15);
			guesses.insert(currentGuess);
		}
	}

	if (step==15) {
		int previousStep = 7;
		vector<vector<int> > prevSigma = smallSigma0;
		vector<int> directSig = getDirectDifferenceBits(prevSigma, wDifferential[previousStep]);

		// select random possibilities and stop when we repeat for the first time
		unordered_set <long long> guesses;
		long long currentGuess = convertNumValToNum(directSig);
		vector<char> currentGuessVector = convertNumValToNumCross(directSig);
		while (guesses.find(currentGuess)==guesses.end()) {
			for (int i = 0; i < 32; ++i) {
				wDifferential[step][i] = currentGuessVector[i];
				if (wDifferential[step][i]=='-') {
					addDifferentialHas_Condition("W", step, i, conditions);
				}
				if (wDifferential[step][i]=='x') {
					addDifferentialHasXCondition("W", step, i, conditions);
				}
			}
			findValidWDifferentialsHelper(wDifferential, conditions, 17);
			guesses.insert(currentGuess);
		}
	}

	if (step==17) {
		int previousStep = 15;
		vector<vector<int> > prevSigma = smallSigma1;
		vector<int> directSig = getDirectDifferenceBits(prevSigma, wDifferential[previousStep]);

		// select random possibilities and stop when we repeat for the first time
		unordered_set <long long> guesses;
		long long currentGuess = convertNumValToNum(directSig);
		vector<char> currentGuessVector = convertNumValToNumCross(directSig);
		while (guesses.find(currentGuess)==guesses.end()) {
			for (int i = 0; i < 32; ++i) {
				wDifferential[step][i] = currentGuessVector[i];
				if (wDifferential[step][i]=='-') {
					addDifferentialHas_Condition("W", step, i, conditions);
				}
				if (wDifferential[step][i]=='x') {
					addDifferentialHasXCondition("W", step, i, conditions);
				}
			}
			// findValidWDifferentialsHelper(wDifferential, conditions, 17);
			bool satisfied = addConditionsToCancelDifference(wDifferential[17], wDifferential[8], conditions);
			if (satisfied) {
				cout << "FOUND THis" << endl;
				printWDifferential(wDifferential);
			}
			guesses.insert(currentGuess);
		}
	}
}

void findValidWDifferentials(vector<vector<char> > wDifferential, Graph conditions) {
	findValidWDifferentialsHelper(wDifferential, conditions, 8);
	return;
}

int main(int argc, char const *argv[]) {
	// initializing sigma functions
	smallSigma0 = getSmallSig(7, 18, 3);
	smallSigma1 = getSmallSig(17, 19, 10);

	// intializing conditions graph
	Graph conditions = getGraphSetUp();

	// initializing differntial for W:
	vector<vector<char> > wDifferential = vector<vector<char> > (DIFFERENTIAL_HEIGHT, vector<char>(32, '-'));
	for (int i = 0; i < DIFFERENTIAL_HEIGHT; ++i) {
		if (i==7 || i==8 || i==12 || i==15 || i==17) {
			for (int j = 0; j < 32; ++j) {
				wDifferential[i][j] = '?';
			}
		}
	}

	//adding conditions
	for (int i = 0; i < DIFFERENTIAL_HEIGHT; ++i) {
		for (int j = 0; j < 32; ++j) {
			if (wDifferential[i][j]=='-') {
				addDifferentialHas_Condition("W", i, j, conditions);
			}
		}
	}

	cout << "Initialization Done!" << endl;

	findValidWDifferentials(wDifferential, conditions);

	return 0;
}