#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct vertex {
	string color; //white,pink,red
	int discover;
	int finish;
	
	vertex() : discover(0) ,finish(0) {} 
};



int main() {
	//generate a random graph
	vertex a, b, c, d, e;
	vector<vector<int>> adjacencyMatrix;
	for (int i = 0; i < 5; i++) {
		vector<int> row;
		if (i == 0) {
			row = { 0,1,0,0,1 };
		}
		else if (i == 1) {
			row = { 1,0,1,0,1 };
		}
		else if (i == 2) {
			row = { 1,1,0,1,1 };
		}
		else if (i == 3) {
			row = { 0,0,1,0,1 };
		}
		else if (i == 4) {
			row = { 0,1,1,1,0 };
		}
		adjacencyMatrix.push_back(row);
	}
	for (int i = 0; i < adjacencyMatrix.size(); i++) {
		for (int j = 0; j < adjacencyMatrix[0].size();j++) {
			cout << adjacencyMatrix[i][j] << " ";
		}
		cout << endl;
	}

	return 0;
}