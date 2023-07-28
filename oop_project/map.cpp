#include "map.h"

Map::Map()
{
	map = 0;
	rows = 0;
	cols = 0;
}

Map::~Map()
{
	delete[] map[0];
	delete[] map;
}

void Map::setBasicMap(int row, int col)
{
	Data* ptr;

	rows = row;
	cols = col;
	ptr = new Data[row * col];
	map = new Data * [row];
	map[0] = ptr;
	for (int i = 1; i < row; i++)
		map[i] = map[i - 1] + col;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			(map[i] + j)->setRow(i);
			(map[i] + j)->setCol(j);
		}
	}
}

void Map::setVisitFalse()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
			map[i][j].setVisit(0);
	}
}

bool Map::findNeighbor(Data* ptr)
{
	if (ptr->getCol() != 0 && map[ptr->getRow()][ptr->getCol() - 1].Isvisited() == 0)
		return true;
	if (ptr->getCol() != cols - 1 && map[ptr->getRow()][ptr->getCol() + 1].Isvisited() == 0)
		return true;
	if (ptr->getRow() != 0 && map[ptr->getRow() - 1][ptr->getCol()].Isvisited() == 0)
		return true;
	if (ptr->getRow() != rows - 1 && map[ptr->getRow() + 1][ptr->getCol()].Isvisited() == 0)
		return true;
	return false;
}

void Map::setMapByDfs()
{
	int row, col, ranNum;
	MyStack stack;
	Data* cur, * cho;

	srand((unsigned int)time(NULL));
	row = rand() % rows;
	col = rand() % cols;
	map[row][col].setVisit(2);
	stack.Push(&map[row][col]);
	while (!stack.Isempty())
	{
		cur = stack.Pop();
		// 미방문한 셀이 있으면
		if (findNeighbor(cur))
		{
			stack.Push(cur);
			ranNum = rand() % 4;
			while (1)
			{
				ranNum = rand() % 4;
				if (ranNum == 0)
				{
					if (cur->getCol() == 0 || map[cur->getRow()][cur->getCol() - 1].Isvisited() == 2)
						continue;
					cho = &map[cur->getRow()][cur->getCol() - 1];
					cho->setVisit(2);
					cur->deleteLeft();
					cho->deleteRight();
					break;
				}
				if (ranNum == 1)
				{
					if (cur->getCol() == cols - 1 || map[cur->getRow()][cur->getCol() + 1].Isvisited() == 2)
						continue;
					cho = &map[cur->getRow()][cur->getCol() + 1];
					cho->setVisit(2);
					cur->deleteRight();
					cho->deleteLeft();
					break;
				}
				if (ranNum == 2)
				{
					if (cur->getRow() == 0 || map[cur->getRow() - 1][cur->getCol()].Isvisited() == 2)
						continue;
					cho = &map[cur->getRow() - 1][cur->getCol()];
					cho->setVisit(2);
					cur->deleteUp();
					cho->deleteDown();
					break;
				}
				if (ranNum == 3)
				{
					if (cur->getRow() == rows - 1 || map[cur->getRow() + 1][cur->getCol()].Isvisited() == 2)
						continue;
					cho = &map[cur->getRow() + 1][cur->getCol()];
					cho->setVisit(2);
					cur->deleteDown();
					cho->deleteUp();
					break;
				}
			}
			stack.Push(cho);
		}
	}
}

bool Map::findRoot(Data* ptr)
{
	if ((ptr->getSide() & 1) == 0 && map[ptr->getRow()][ptr->getCol() - 1].Isvisited() == 0)
		return true;
	if ((ptr->getSide() & 2) == 0 && map[ptr->getRow()][ptr->getCol() + 1].Isvisited() == 0)
		return true;
	if ((ptr->getSide() & 4) == 0 && map[ptr->getRow() - 1][ptr->getCol()].Isvisited() == 0)
		return true;
	if ((ptr->getSide() & 8) == 0 && map[ptr->getRow() + 1][ptr->getCol()].Isvisited() == 0)
		return true;
	return false;
}

void Map::findPathByDfs(int s1, int s2, int d1, int d2)
{
	Data* cur, * des, * next;
	MyStack stack;

	cur = &map[s1][s2];
	des = &map[d1][d2];
	cur->setVisit(2);
	stack.Push(cur);
	while (cur != des)
	{
		cur = stack.Pop();
		cur->setVisit(1);
		if (findRoot(cur))
		{
			cur->setVisit(2);
			stack.Push(cur);
			if ((cur->getSide() & 1) == 0 && map[cur->getRow()][cur->getCol() - 1].Isvisited() == 0)
			{
				next = &map[cur->getRow()][cur->getCol() - 1];
				next->setVisit(2);
				if (next == des)
					break;
			}
			else if ((cur->getSide() & 2) == 0 && map[cur->getRow()][cur->getCol() + 1].Isvisited() == 0)
			{
				next = &map[cur->getRow()][cur->getCol() + 1];
				next->setVisit(2);
				if (next == des)
					break;
			}
			else if ((cur->getSide() & 4) == 0 && map[cur->getRow() - 1][cur->getCol()].Isvisited() == 0)
			{
				next = &map[cur->getRow() - 1][cur->getCol()];
				next->setVisit(2);
				if (next == des)
					break;
			}
			else
			{
				next = &map[cur->getRow() + 1][cur->getCol()];
				next->setVisit(2);
				if (next == des)
					break;
			}
			stack.Push(next);
		}
	}
}

void Map::findPathByBfs(int s1, int s2, int d1, int d2)
{
	Data* cur, * des, * next;
	MyQueue queue;

	next = 0;
	cur = &map[s1][s2];
	des = &map[d1][d2];
	cur->setVisit(1);
	queue.Push(cur);
	while (cur != des)
	{
		cur = queue.Pop();
		if (findRoot(cur))
		{
			cur->setVisit(1);
			if ((cur->getSide() & 1) == 0 && map[cur->getRow()][cur->getCol() - 1].Isvisited() == 0)
			{
				next = &map[cur->getRow()][cur->getCol() - 1];
				next->setVisit(1);
				next->setPar(cur);
				queue.Push(next);
				if (next == des)
					break;
			}
			if ((cur->getSide() & 2) == 0 && map[cur->getRow()][cur->getCol() + 1].Isvisited() == 0)
			{
				next = &map[cur->getRow()][cur->getCol() + 1];
				next->setVisit(1);
				next->setPar(cur);
				queue.Push(next);
				if (next == des)
					break;
			}
			if ((cur->getSide() & 4) == 0 && map[cur->getRow() - 1][cur->getCol()].Isvisited() == 0)
			{
				next = &map[cur->getRow() - 1][cur->getCol()];
				next->setVisit(1);
				next->setPar(cur);
				queue.Push(next);
				if (next == des)
					break;
			}
			if ((cur->getSide() & 8) == 0 && map[cur->getRow() + 1][cur->getCol()].Isvisited() == 0)
			{
				next = &map[cur->getRow() + 1][cur->getCol()];
				next->setVisit(1);
				next->setPar(cur);
				queue.Push(next);
				if (next == des)
					break;
			}
		}
	}
	if (cur == des)
		cur->setVisit(2);
	else
	{
		while (next)
		{
			next->setVisit(2);
			next = next->getPar();
		}
	}
}

void Map::printMap()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j].Isvisited() == 2)
				std::cout << "2(";
			else if (map[i][j].Isvisited() == 1)
				std::cout << "1(";
			else
				std::cout << "0(";
			if ((map[i][j].getSide() & 1) == 1)
				std::cout << "L";
			else
				std::cout << " ";
			if ((map[i][j].getSide() & 2) == 2)
				std::cout << "R";
			else
				std::cout << " ";
			if ((map[i][j].getSide() & 4) == 4)
				std::cout << "U";
			else
				std::cout << " ";
			if ((map[i][j].getSide() & 8) == 8)
				std::cout << "D";
			else
				std::cout << " ";
			std::cout << ") ";
		}
		std::cout << std::endl;
	}
}