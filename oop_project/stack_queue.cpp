#include "stack_queue.h"

Data::Data()
{
	Isvisit = 0;
	side = 15;
	row = 0;
	col = 0;
	par = 0;
}

int Data::Isvisited()
{
	return Isvisit;
}

void Data::setVisit(int n)
{
	Isvisit = n;
}

void Data::setRow(int n)
{
	row = n;
}

int Data::getRow()
{
	return row;
}

void Data::setCol(int n)
{
	col = n;
}

int Data::getCol()
{
	return col;
}

void Data::setPar(Data* data)
{
	par = data;
}

Data* Data::getPar()
{
	return par;
}

char Data::getSide()
{
	return side;
}

void Data::deleteLeft()
{
	if ((side & 1) == 1)
		side--;
}

void Data::deleteRight()
{
	if ((side & 2) == 2)
		side -= 2;
}

void Data::deleteUp()
{
	if ((side & 4) == 4)
		side -= 4;
}

void Data::deleteDown()
{
	if ((side & 8) == 8)
		side -= 8;
}

MyStack::MyStack()
{
	memset(arr, 0, sizeof(Data *) * LEN);
	idx = -1;
}

void MyStack::Push(Data* data)
{
	if (idx == LEN - 1)
		return;
	arr[++idx] = data;
}

Data* MyStack::Pop()
{
	if (Isempty())
		return 0;
	return arr[idx--];
}

bool MyStack::Isempty()
{
	if (idx == -1)
		return true;
	return false;
}

MyQueue::MyQueue()
{
	memset(arr, 0, sizeof(Data*) * (LEN + 1));
	front = 0;
	rear = 0;
}

void MyQueue::Push(Data* data)
{
	if ((rear + 1) % (LEN + 1) == front)
		return;
	rear = (rear + 1) % (LEN + 1);
	arr[rear] = data;
}

Data* MyQueue::Pop()
{
	if (Isempty())
		return 0;
	front = (front + 1) % (LEN + 1);
	return arr[front];
}

bool MyQueue::Isempty()
{
	if (front == rear)
		return true;
	else
		return false;
}