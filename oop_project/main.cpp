#include <iostream>
#include "map.h"

using namespace std;

int main()
{
	Map a;

	a.setBasicMap(10, 10);
	a.setMapByDfs();
	a.setVisitFalse();
	cout << "Map" << endl;
	a.printMap();
	cout << endl << endl;
	a.findPathByDfs(0, 0, 9, 9);
	cout << "DFS: " << endl;
	a.printMap();
	cout << endl << endl;
	a.setVisitFalse();
	a.findPathByBfs(0, 0, 9, 9);
	cout << "BFS: " << endl;
	a.printMap();
	return 0;
}