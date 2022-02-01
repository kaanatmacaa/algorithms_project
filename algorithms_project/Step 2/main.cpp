#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;

template <class obj> class StackNode;
template <class obj> class Stack;
enum class Color;
struct Vertex;
void LSC_BRUTEFORCE(int, int, vector<int>, const vector<vector<int>> &, vector<Stack<int>> &, Stack<int>);
void LSC(const int &, vector<Vertex> &, const vector<vector<int>> &);
void LSC_TRAV(const int &, const int &, vector<Vertex> &, const vector<vector<int>> &, int);

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

enum class Color { White, Pink, Red };

struct Vertex
{
	Color color;
	int discover;
	int finish;
	int pred;
	int vertex_code;
	int adj_counter;
	Vertex() : color(Color::White), discover(-1), finish(-1), pred(-1), vertex_code(-1), adj_counter(-1) {}
	Vertex(int _vertex_code, int _adj_counter) : color(Color::White), discover(-1), finish(-1), pred(-1), vertex_code(_vertex_code), adj_counter(_adj_counter) {}
};

void LSC_BRUTEFORCE(int start, int last, vector<int> isVisited, const vector<vector<int>> &adjacencyMatrix, vector<Stack<int>> &paths, Stack<int> currentStack)
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
			LSC_BRUTEFORCE(start, current, isVisited, adjacencyMatrix, paths, currentStack);
			if (currentStack.getTop() == start)
			{
				paths.push_back(currentStack);
			}
			currentStack.pop();
		}
	}
}

void LSC(const int &root, vector<Vertex> &vertices, const vector<vector<int>> &adj_matrix)
{
	for (int i = 0; i < (int)vertices.size(); i++)
	{
		if (vertices[i].vertex_code != root)
		{
			vertices[i].color = Color::White;
		}
		else
		{
			vertices[i].color = Color::Pink;
		}
		vertices[i].pred = -1;
	}
	int count = 0;
	for (int i = 0; i < (int)vertices.size(); i++)
	{
		if (adj_matrix[i][root] == 1 && vertices[i].color == Color::White)
		{
			vertices[i].pred = root;
			LSC_TRAV(i, root, vertices, adj_matrix, count);
		}
	}
}

void LSC_TRAV(const int &current, const int &root, vector<Vertex> &vertices, const vector<vector<int>> &adj_matrix, int count)
{
	vertices[current].color = Color::Pink;
	count++;
	vertices[current].discover = count;
	for (int i = 0; i < (int)vertices.size(); i++)
	{
		if (adj_matrix[i][current] == 1)
		{
			if (i == root)
			{
				vertices[i].finish = count + 1;
			}
			else if (vertices[i].adj_counter == 1)
			{
				vertices[i].finish = count;
			}
			else if (vertices[i].color == Color::White)
			{
				vertices[i].pred = current;
				LSC_TRAV(i, root, vertices, adj_matrix, count);
			}
		}
	}
	vertices[current].color = Color::Red;
	count++;
	vertices[current].finish = count;
}

void PRINT_CYCLE(const int &current, const int &root, vector<Vertex> &vertices)
{
	int finish_max_index = 0;
	for (int i = 1; i < (int)vertices.size(); i++)
	{
		if (vertices[i].finish > vertices[finish_max_index].finish)
		{
			finish_max_index = i;
		}
	}
	int itr = finish_max_index;

	cout << root << " -> ";
	while (vertices[itr].pred != -1)
	{
		cout << itr << " -> ";
		itr = vertices[itr].pred;
		if (vertices[itr].pred == -1)
		{
			cout << itr << endl;
		}
	}
}

float STD_DEV_CALCULATE(vector<float> &data)
{
	float total = 0.0;
	float mean;
	float stdDev = 0.0;
	for (int i = 0; i < (int)data.size(); i++)
	{
		total += data[i];
	}
	mean = total / (int)data.size();
	for (int i = 0; i < (int)data.size(); i++)
	{
		stdDev += pow(data[i] - mean, 2);
	}
	stdDev = sqrt(stdDev / (int)data.size());
	return stdDev;
}

float STD_ERR_CALCULATE(float stdDev, int size)
{
	return stdDev / sqrt(size);
}

void STATS_DISPLAY(vector<float> &rt)
{
	float total = 0.0;
	for (int i = 0; i < (int)rt.size(); i++)
	{
		total += rt[i];
	}

	float mean = total / (int)rt.size();
	float stdDev = STD_DEV_CALCULATE(rt);
	float stdErr = STD_ERR_CALCULATE(stdDev, (int)rt.size());

	const float tval90 = 1.645;
	float upperMean90 = mean + (tval90 * stdErr);
	float lowerMean90 = mean - (tval90 * stdErr);

	const float tval95 = 1.96;
	float upperMean95 = mean + (tval95 * stdErr);
	float lowerMean95 = mean - (tval95 * stdErr);

	cout << fixed << setprecision(6) << mean << " "
		<< fixed << setprecision(6) << stdDev << " "
		<< fixed << setprecision(6) << stdErr << " "
		<< fixed << setprecision(6) << upperMean90 << "-"
		<< fixed << setprecision(6) << lowerMean90 << " "
		<< fixed << setprecision(6) << upperMean95 << "-"
		<< fixed << setprecision(6) << lowerMean95 << endl;
}

vector<vector<int>> RANDOM_GRAPH(int vertices, int edges)
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

void FIXED_VERTEX_ANALYZE()
{
	vector<int> iterCounts = { 250, 500, 1000, 2500 };
	for (int iterIndex = 0; iterIndex < (int)iterCounts.size(); iterIndex++)
	{
		cout << "Statistics for ITER_COUNT=" << iterCounts[iterIndex] << endl;
		cout << "Mean     StdDev   StdErr   CL%90             CL%95" << endl;
		int vertex = 250;
		vector<int> edges = { 250, 500, 1000, 2500, 5000 };
		for (int edgeIndex = 0; edgeIndex < (int)edges.size(); edgeIndex++)
		{
			vector<float> runningTimes;
			for (int i = 0; i < iterCounts[iterIndex]; i++)
			{
				vector<Vertex> vertices;
				vector<vector<int>> graph = RANDOM_GRAPH(vertex, edges[edgeIndex]);
				int maxIndex = 0;
				int maxValue = 0;
				for (int j = 0; j < (int)graph.size(); j++)
				{
					int adj_counter = 0;
					for (int k = 0; k < (int)graph[j].size(); k++)
					{
						adj_counter += graph[j][k];
					}
					if (adj_counter > maxValue)
					{
						maxIndex = j;
						maxValue = adj_counter;
					}
					vertices.push_back(Vertex(j, adj_counter));
				}
				int root = maxIndex;

				auto startTimer = chrono::high_resolution_clock::now();
				LSC(root, vertices, graph);
				auto endTimer = chrono::high_resolution_clock::now();
				float runningTime = (endTimer - startTimer).count() / (float)1000000;
				runningTimes.push_back(runningTime);
			}
			//cout << "V=" << vertex << ", E=" << edges[edgeIndex] << endl;
			STATS_DISPLAY(runningTimes);
		}
		cout << endl;
	}
}

void FIXED_EDGE_ANALYZE()
{
	vector<int> iterCounts = { 250, 500, 1000, 2500 };
	for (int iterIndex = 0; iterIndex < (int)iterCounts.size(); iterIndex++)
	{
		cout << "Statistics for ITER_COUNT=" << iterCounts[iterIndex] << endl;
		cout << "Mean     StdDev   StdErr   CL%90             CL%95" << endl;
		vector<int> verticesCount = { 250, 500, 1000, 2500, 5000 };
		int edge = 250;
		for (int vertexIndex = 0; vertexIndex < (int)verticesCount.size(); vertexIndex++)
		{
			vector<float> runningTimes;
			for (int i = 0; i < iterCounts[iterIndex]; i++)
			{
				vector<Vertex> vertices;
				vector<vector<int>> graph = RANDOM_GRAPH(verticesCount[vertexIndex], edge);
				int maxIndex = 0;
				int maxValue = 0;
				for (int j = 0; j < (int)graph.size(); j++)
				{
					int adj_counter = 0;
					for (int k = 0; k < (int)graph[j].size(); k++)
					{
						adj_counter += graph[j][k];
					}
					if (adj_counter > maxValue)
					{
						maxIndex = j;
						maxValue = adj_counter;
					}
					vertices.push_back(Vertex(j, adj_counter));
				}
				int root = maxIndex;

				auto startTimer = chrono::high_resolution_clock::now();
				LSC(root, vertices, graph);
				auto endTimer = chrono::high_resolution_clock::now();
				float runningTime = (endTimer - startTimer).count() / (float)1000000;
				runningTimes.push_back(runningTime);
			}
			STATS_DISPLAY(runningTimes);
		}
		cout << endl;
	}
}

int main()
{
	cout << "FIXED VERTEX => 250" << endl;
	FIXED_VERTEX_ANALYZE();
	cout << endl << "FIXED EDGE   => 250" << endl;
	FIXED_EDGE_ANALYZE();
	return 0;
}























/*

	int root = 2;
	vector<Vertex> vertices = { Vertex(0, 2), Vertex(1, 3), Vertex(2, 4), Vertex(3, 2), Vertex(4, 3) };
	vector<vector<int>> adj_matrix = { { 0, 1, 1, 0, 0 }, { 1, 0, 1, 0, 1 }, { 1, 1, 0, 1, 1 }, { 0, 0, 1, 0, 1 }, { 0, 1, 1, 1, 0 } };
	LSC(root, vertices, adj_matrix);
	PRINT_CYCLE(0, root, vertices);

*/