#include <iostream>
#include "SimpleDirectedGraph.h"

//The program runs ford falkerson algorithm on a graph in two different methods:
//one uses BFS tho find the improving path each time, and the other one uses a greedy method to find it.


using namespace std;

//prints an error message and exits the program.
void errorMessage();

//checks whether a number x is in range of 1 to n.
bool inRange(int x, int n);

int main()
{
	int n, m, s, t, i, j, c;

	cin >> n >> m >> s >> t;

	if (n < 0 || m < 0 || !inRange(s, n) || !inRange(t, n) || s == t)
		errorMessage();


	SimpleDirectedGraph g(n, s, t);

	for (int k = 0; k < m; k++)
	{
		cin >> i >> j >> c;

		if (c < 0 || !inRange(i, n) || !inRange(j, n))
			errorMessage();
		else
			g.AddEdge(i, j, c);
		g.AddEdge(j, i, 0); // for Skew Symmetry
	}
	SimpleDirectedGraph g2 = g;

	g.fordfalkersonBFS();
	g2.fordfalkersonGreedy();

	return 0;
}

void errorMessage()
{
	cout << "invalid input";
	exit(1);
}

bool inRange(int x, int n)
{
	if (x > 0 && x <= n)
		return true;
	else
		return false;
}