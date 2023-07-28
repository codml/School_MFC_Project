#pragma once
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "stack_queue.h"

class Map
{
private:
	Data** map;
	int rows;
	int cols;


public:
	Map();
	~Map();

	void setBasicMap(int row, int col);
	void setVisitFalse();
	bool findNeighbor(Data* ptr);
	void setMapByDfs();
	bool findRoot(Data* ptr);
	void findPathByDfs(int s1, int s2, int d1, int d2);
	void findPathByBfs(int s1, int s2, int d1, int d2);
	void printMap();
};