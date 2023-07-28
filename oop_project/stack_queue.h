#pragma once
#define LEN 900
#include <cstring>

class Data
{
private:
	int Isvisit;
	char side;
	int row, col;
	Data* par;
public:
	Data();

	int Isvisited();
	void setVisit(int n);
	void setRow(int n);
	int getRow();
	void setCol(int n);
	int getCol();
	void setPar(Data* data);
	Data* getPar();
	char getSide();
	void deleteLeft();
	void deleteRight();
	void deleteUp();
	void deleteDown();
};

class MyStack
{
private:
	Data* arr[LEN];
	int idx;
public:
	MyStack();
	
	void Push(Data* data);
	Data* Pop();
	bool Isempty();
};

class MyQueue
{
private:
	Data* arr[LEN + 1];
	int front, rear;

public:
	MyQueue();

	void Push(Data* data);
	Data* Pop();
	bool Isempty();
};