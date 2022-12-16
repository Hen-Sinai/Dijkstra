#ifndef SimpleDirectedGraph_H
#define SimpleDirectedGraph_H

#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

struct Edge {
	int dest;
	int capacity;
	int flow = 0;
	Edge* GfPointer = nullptr;

	Edge(int d, int c)
	{
		dest = d;
		capacity = c;
	}
};


class SimpleDirectedGraph
{
	int s, t;
	int numOfVertices;
	vector <list<Edge>> adjList;

	//creates an empty graph.
	void MakeEmptyGraph(int n)
	{
		numOfVertices = n;
		adjList.resize(numOfVertices);
	}

	//return the adjacency list of a vertex u.
	list<Edge>& GetAdjList(int u)
	{
		return adjList[u - 1];
	}

	//removes the edge (u, v) from the graph.
	void RemoveEdge(int u, int v)
	{
		adjList[u - 1].remove_if([v](Edge e) { return e.dest == v; });
	}

	//returns a vector of parents which represnts the shortest paths from s in the graph, by BFS algorithm.
	vector<int> BFS();

	//returns a vector of parents which represnts the most improving path (from s to t) in the graph (Greedy algorithm).
	vector<int> Greedy();

	//sets the Gfpointers of the edges to point to the corresponding edges in the Gf graph.
	void setGfEdgePointers(SimpleDirectedGraph& gf);

	//returns the min path capacity of a path from s to t which represtented by the parents vector.
	int findMinPathCapacity(vector<int>& parents);

	//updates the flow on G and the capacity in Gf with the min path capacity of the path.
	void updateFlow(vector<int>& parents, int minPathCapacity);

	//find the total flow of the graph.
	int findTotalFlow();

	//find the minimal cut, the vector sVertices represents S, tVertices represents T.
	void findMinCut(vector<int>& parents, vector<int>& sVertices, vector<int>& tVertices);

	//prints the maximum flow and the minimal cut.
	void printMaxFlowMinCut(vector<int>& sVertices, vector<int>& tVertices, int totalFlow);

public:

	SimpleDirectedGraph(int n, int sIn, int tIn)
	{
		s = sIn;
		t = tIn;
		MakeEmptyGraph(n);
	}

	//Adds an edge (u, v) with capacity c to the graph.
	void AddEdge(int u, int v, int c);

	//runs ford falkerson with BFS algorithm to find an improving path.
	void fordfalkersonBFS();

	//runs ford falkerson with greedy algorithm to find an improving path.
	void fordfalkersonGreedy();

};


#endif