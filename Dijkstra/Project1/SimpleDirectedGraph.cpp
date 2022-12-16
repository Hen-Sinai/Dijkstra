#include "SimpleDirectedGraph.h"

vector<int> SimpleDirectedGraph::BFS()
{
	int curr;
	queue<int> q;
	vector<int> d(numOfVertices);
	vector<int> p(numOfVertices);
	list<Edge> currAdjList;

	for (int i = 0; i < numOfVertices; i++)
	{
		d[i] = -1;
		p[i] = -1;
	}
	q.push(s);
	d[s - 1] = 0;

	while (q.size() > 0)
	{
		curr = q.front();
		q.pop();
		currAdjList = GetAdjList(curr);
		for (Edge ed : currAdjList)
		{
			if (ed.capacity != 0 && d[ed.dest - 1] == -1)
			{
				d[ed.dest - 1] = d[curr - 1] + 1;
				p[ed.dest - 1] = curr;
				q.push(ed.dest);
			}

		}

	}

	return p;
}

vector<int> SimpleDirectedGraph::Greedy()
{
	int curr;
	priority_queue <pair<int, int> > q;
	vector<int> d(numOfVertices);
	vector<int> p(numOfVertices);
	vector<bool> visited(numOfVertices);
	list<Edge> currAdjList;

	for (int i = 0; i < numOfVertices; i++)
	{
		d[i] = 0;
		p[i] = -1;
	}

	d[s - 1] = numeric_limits<int>::max();

	for (int i = 0; i < numOfVertices; i++)
		q.push(make_pair(d[i], i + 1)); //build the queue

	visited[t - 1] = true;

	while (q.size() > 0)
	{
		if (visited[q.top().second - 1] == true || q.top().first == 0)
			q.pop();

		else
		{
			curr = q.top().second;
			q.pop();
			visited[curr - 1] = true;

			currAdjList = GetAdjList(curr);
			for (Edge ed : currAdjList)
			{
				if ((ed.dest == t || visited[ed.dest - 1] == false) && (d[ed.dest - 1] < ed.capacity))
				{
					d[ed.dest - 1] = min(d[curr - 1], ed.capacity);
					p[ed.dest - 1] = curr;
					q.push(make_pair(d[ed.dest - 1], ed.dest));
				}

			}
		}
	}

	return p;
}

void SimpleDirectedGraph::setGfEdgePointers(SimpleDirectedGraph& gf)
{

	for (int i = 0; i < this->adjList.size(); i++)
	{
		list<Edge>::iterator itG = this->adjList[i].begin();
		list<Edge>::iterator itGf = gf.adjList[i].begin();


		while (itG != this->adjList[i].end() && itGf != gf.adjList[i].end()) {

			itG->GfPointer = &(*itGf);
			itG++;
			itGf++;
		}

	}

}

int SimpleDirectedGraph::findMinPathCapacity(vector<int>& parents)
{
	int curr = t, minPathCapacity = -1;
	while (curr != s) //while the path still going
	{
		for (Edge edge : this->adjList[parents[curr - 1] - 1])
		{
			if (edge.dest == curr)
			{
				if (minPathCapacity == -1)
					minPathCapacity = edge.capacity;
				else
					minPathCapacity = min(minPathCapacity, edge.capacity);
				break;
			}

		}
		curr = parents[curr - 1];
	}

	return minPathCapacity;
}

void SimpleDirectedGraph::updateFlow(vector<int>& parents, int minPathCapacity)
{
	int curr = t;
	while (curr != s) //while the path still going
	{
		for (Edge& edge : this->adjList[parents[curr - 1] - 1])
		{
			if (edge.dest == curr)
			{
				edge.flow += minPathCapacity;
				edge.GfPointer->capacity = edge.capacity - edge.flow;


				for (Edge& revEdge : this->adjList[edge.dest - 1]) //Skew Symmatry
					if (revEdge.dest == parents[curr - 1])
					{
						revEdge.flow = -1 * edge.flow;
						revEdge.GfPointer->capacity = revEdge.capacity - revEdge.flow;
						break;
					}

				break;
			}

		}
		curr = parents[curr - 1];
	}

}

int SimpleDirectedGraph::findTotalFlow()
{
	int totalFlow = 0;
	for (Edge& edge : this->adjList[s - 1])
		totalFlow += edge.flow;

	return totalFlow;
}

void SimpleDirectedGraph::findMinCut(vector<int>& parents, vector<int>& sVertices, vector<int>& tVertices)
{
	for (int i = 0; i < parents.size(); i++)
	{

		if (i == s - 1 || parents[i] != -1)
			sVertices.push_back(i + 1);
		else
			tVertices.push_back(i + 1);

	}
}

void SimpleDirectedGraph::printMaxFlowMinCut(vector<int>& sVertices, vector<int>& tVertices, int totalFlow)
{
	cout << "Max flow = " << totalFlow << endl;
	cout << "Min cut: S = ";
	for (int i = 0; i < sVertices.size(); i++)
	{
		if (i < sVertices.size() - 1)
			cout << sVertices[i] << ", ";
		else
			cout << sVertices[i] << ". ";

	}
	cout << "T = ";
	for (int i = 0; i < tVertices.size(); i++)
	{
		if (i < tVertices.size() - 1)
			cout << tVertices[i] << ", ";
		else
			cout << tVertices[i] << endl;

	}
}

void SimpleDirectedGraph::AddEdge(int u, int v, int c)
{
	for (Edge& edge : adjList[u - 1])
	{
		if (edge.dest == v)
		{
			edge.capacity += c;
			return;
		}
	}

	Edge newEdge(v, c);
	adjList[u - 1].push_back(newEdge);

}

void SimpleDirectedGraph::fordfalkersonBFS()
{
	int minPathCapacity, totalFlow;
	SimpleDirectedGraph gf = *this;
	vector<int> parents, sVertices, tVertices;

	this->setGfEdgePointers(gf);

	parents = gf.BFS();


	while (parents[t - 1] != -1)  //while theres an improving path
	{
		//find the minPathCapactiy

		minPathCapacity = gf.findMinPathCapacity(parents);

		//improve the flow in G and capacity in Gf on the P path according to min capacity

		this->updateFlow(parents, minPathCapacity);

		parents = gf.BFS();

	}

	//find total flow and S , T:
	totalFlow = this->findTotalFlow();
	this->findMinCut(parents, sVertices, tVertices);


	//Prints:
	cout << "BFS Method:" << endl;
	this->printMaxFlowMinCut(sVertices, tVertices, totalFlow);

}

void SimpleDirectedGraph::fordfalkersonGreedy()
{
	int minPathCapacity, totalFlow;
	SimpleDirectedGraph gf = *this;
	vector<int> parents, sVertices, tVertices;

	this->setGfEdgePointers(gf);

	parents = gf.Greedy();


	while (parents[t - 1] != -1)  //while theres an improving path
	{
		//find the minPathCapactiy

		minPathCapacity = gf.findMinPathCapacity(parents);

		//improve the flow in G and capacity in Gf on the P path according to min capacity

		this->updateFlow(parents, minPathCapacity);

		parents = gf.Greedy();

	}

	//find total flow and S , T:
	totalFlow = this->findTotalFlow();
	this->findMinCut(parents, sVertices, tVertices);


	//Prints:
	cout << "Greedy Method:" << endl;
	this->printMaxFlowMinCut(sVertices, tVertices, totalFlow);

}

