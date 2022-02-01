#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <class obj>
class StackNode
{
public:
	StackNode() : next(NULL) {}
	StackNode(const obj &_object, StackNode<obj> *_next) : object(_object), next(_next) {}
	obj object;
	StackNode<obj> *next;
};

template <class obj>
class Stack
{
public:
	Stack() : size(0), top(NULL) {}
	Stack(const Stack<obj> &rhs)
	{
		top = NULL;
		*this = rhs;
	}
	~Stack()
	{
		makeEmpty();
	}
	bool isEmpty() const
	{
		return top == NULL;
	}
	int getSize() const
	{
		return size;
	}
	void makeEmpty()
	{
		while (!isEmpty())
		{
			pop();
		}
		size = 0;
	}
	const obj &getTop() const
	{
		return top->object;
	}
	void push(const obj &_object)
	{
		top = new StackNode<obj>(_object, top);
		size++;
	}
	const obj &pop()
	{
		StackNode<obj> *prevTop = top;
		top = top->next;
		obj object = prevTop->object;
		delete prevTop;
		size--;
		return object;
	}
	const Stack<obj> &operator=(const Stack<obj> &rhs)
	{
		if (this != &rhs)
		{
			makeEmpty();
			if (rhs.isEmpty())
			{
				return *this;
			}
			Stack<obj> dummyStack;
			for (StackNode<obj> *rptr = rhs.top; rptr != NULL; rptr = rptr->next)
			{
				dummyStack.push(rptr->object);
			}
			while (!dummyStack.isEmpty())
			{
				obj object = dummyStack.pop();
				push(object);
			}
			size = rhs.size;
		}
		return *this;
	}

private:
	int size;
	StackNode<obj> *top;
};

void LongestSimpleCircuit(int start, int last, vector<int> isVisited, const vector<vector<int>> &adjacencyMatrix, vector<Stack<int>> &paths, Stack<int> currentStack)
{
	int current = currentStack.getTop();
	if (current == start && isVisited[current] == 1)
	{
		return;
	}
	isVisited[current] = 1;
	for (int index = 0; index < (int)adjacencyMatrix[current].size(); index++)
	{
		if (adjacencyMatrix[current][index] == 1 && (isVisited[index] == 0 || index == start) && last != index)
		{
			currentStack.push(index);
			LongestSimpleCircuit(start, current, isVisited, adjacencyMatrix, paths, currentStack);
			if (currentStack.getTop() == start)
			{
				paths.push_back(currentStack);
			}
			currentStack.pop();
		}
	}
}

vector<vector<int>> RandomGraphGenerator(int vertices, int edges)
{
	vector<vector<int>> adjacencyMatrix;
	for (int i = 0; i < vertices; i++)
	{
		vector<int> eachVector;
		for (int j = 0; j < vertices; j++)
		{
			eachVector.push_back(0);
		}
		adjacencyMatrix.push_back(eachVector);
	}
	while (edges > 0)
	{
		int randVertex1 = rand() % vertices;
		int randVertex2 = rand() % vertices;
		if (randVertex1 != randVertex2 && adjacencyMatrix[randVertex1][randVertex2] == 0)
		{
			adjacencyMatrix[randVertex1][randVertex2] = 1;
			adjacencyMatrix[randVertex2][randVertex1] = 1;
			edges--;
		}
	}
	return adjacencyMatrix;
}

int main()
{
	vector<int> vertices = { 4, 6 };
	vector<int> edges = { 5, 8 };
	for (int turn = 1; turn <= 2; turn++)
	{
		vector<vector<int>> adjacencyMatrix = RandomGraphGenerator(vertices[turn - 1], edges[turn - 1]);
		vector<Stack<int>> paths;
		Stack<int> currentStack;
		int mostDegree = 0;
		int mostTotal = 0;
		for (int i = 0; i < (int)adjacencyMatrix.size(); i++)
		{
			int currentTotal = 0;
			for (int j = 0; j < (int)adjacencyMatrix[0].size(); j++)
			{
				currentTotal += adjacencyMatrix[i][j];
			}
			if (currentTotal > mostTotal)
			{
				mostDegree = i;
				mostTotal = currentTotal;
			}
		}
		int last = -1;
		vector<int> isVisited;
		for (int i = 0; i < vertices[turn - 1]; i++)
		{
			isVisited.push_back(0);
		}
		currentStack.makeEmpty();
		currentStack.push(mostDegree);
		LongestSimpleCircuit(mostDegree, last, isVisited, adjacencyMatrix, paths, currentStack);
		int longestPathLength = 0;
		Stack<int> longestPath;
		for (int i = 0; i < (int)paths.size(); i++)
		{
			if (longestPathLength < paths[i].getSize())
			{
				longestPathLength = paths[i].getSize();
				longestPath = paths[i];
			}
		}
		cout << endl;
		cout << "Sample Case #" << turn << endl;
		cout << "Adjacency Matrix:   ";
		for (int i = 0; i < (int)adjacencyMatrix.size(); i++)
		{
			if (i != 0)
			{
				cout << "                    ";
			}
			for (int j = 0; j < (int)adjacencyMatrix.size(); j++)
			{
				cout << " " << adjacencyMatrix[i][j];
			}
			cout << endl;
		}
		cout << endl;
		cout << "Longest Path:       ";
		while (!longestPath.isEmpty())
		{
			int temp = longestPath.pop();
			cout << " " << temp;
		}
		cout << endl;
	}
	return 0;
}