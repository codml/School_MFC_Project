// CStaticCanvasControl.cpp: 구현 파일
//

#include "pch.h"
#include "oop_project.h"
#include "CStaticCanvasControl.h"

Data::Data()
{
	Isvisit = false;
	side = 15;
	row = 0;
	col = 0;
	par = 0;
}

bool Data::Isvisited()
{
	return Isvisit;
}

void Data::setVisit(bool n)
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

void Data::setSide()
{
	side = 15;
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
	memset(arr, 0, sizeof(Data*) * LEN);
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

Map::Map()
{
	rows = 0;
	cols = 0;
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			(map[i] + j)->setRow(i);
			(map[i] + j)->setCol(j);
		}
	}
}

void Map::setSize(int row, int col)
{
	rows = row;
	cols = col;
}

void* Map::getMap()
{
	return map;
}

int Map::getRows()
{
	return rows;
}

int Map::getCols()
{
	return cols;
}

bool Map::findNeighbor(Data* ptr)
{
	if (ptr->getCol() != 0 && map[ptr->getRow()][ptr->getCol() - 1].Isvisited() == false)
		return true;
	if (ptr->getCol() != cols - 1 && map[ptr->getRow()][ptr->getCol() + 1].Isvisited() == false)
		return true;
	if (ptr->getRow() != 0 && map[ptr->getRow() - 1][ptr->getCol()].Isvisited() == false)
		return true;
	if (ptr->getRow() != rows - 1 && map[ptr->getRow() + 1][ptr->getCol()].Isvisited() == false)
		return true;
	return false;
}

bool Map::findRoot(Data* ptr)
{
	if ((ptr->getSide() & 1) == 0 && map[ptr->getRow()][ptr->getCol() - 1].Isvisited() == false)
		return true;
	if ((ptr->getSide() & 2) == 0 && map[ptr->getRow()][ptr->getCol() + 1].Isvisited() == false)
		return true;
	if ((ptr->getSide() & 4) == 0 && map[ptr->getRow() - 1][ptr->getCol()].Isvisited() == false)
		return true;
	if ((ptr->getSide() & 8) == 0 && map[ptr->getRow() + 1][ptr->getCol()].Isvisited() == false)
		return true;
	return false;
}

void Map::setVisitFalse()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
			map[i][j].setVisit(false);
	}
}

void Map::setSide()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
			map[i][j].setSide();
	}
}

void Map::setPar()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
			map[i][j].setPar(0);
	}
}

// CStaticCanvasControl

IMPLEMENT_DYNAMIC(CStaticCanvasControl, CStatic)

CStaticCanvasControl::CStaticCanvasControl()
	: m_black_pen(PS_SOLID, 2, RGB(0, 0, 0))
	, m_white_brush(RGB(255, 255, 255))
	, startX(0)
	, startY(0)
	, maze_height(0)
	, maze_width(0)
	, rect_size(0)
	, current_mouse_point(-1, -1)
	, red_y(-1)
	, red_x(-1)
	, blue_y(-1)
	, blue_x(-1)
{
}

CStaticCanvasControl::~CStaticCanvasControl()
{
}


BEGIN_MESSAGE_MAP(CStaticCanvasControl, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// CStaticCanvasControl 메시지 처리기

void CStaticCanvasControl::BuildMapByDfs(BOOL animate)
{
	Invalidate(FALSE);
	int row, col, ranNum;
	MyStack stack;
	Data* cur, * cho;
	Data(*map)[30];
	CPaintDC dc(this);
	CPen path_pen(PS_SOLID, 4, RGB(155, 155, 255));
	CBrush visit(RGB(155, 155, 255));
	CRect r(0, 0, rect_size - 2, rect_size - 2);

	dc.SelectObject(&path_pen);
	srand((unsigned int)time(NULL));
	row = rand() % miro.getRows();
	col = rand() % miro.getCols();
	map = (Data(*)[30])miro.getMap();
	stack.Push(&map[row][col]);
	map[row][col].setVisit(true);
	if (animate)
	{
		r.MoveToXY(startX + col * rect_size + 1, startY + row * rect_size + 1);
		dc.FillRect(&r, &visit);
	}
	while (!stack.Isempty())
	{
		cur = stack.Pop();
		if (miro.findNeighbor(cur))
		{
			stack.Push(cur);
			ranNum = rand() % 4;
			while (1)
			{
				ranNum = rand() % 4;
				if (ranNum == 0)
				{
					if (cur->getCol() == 0
						|| map[cur->getRow()][cur->getCol() - 1].Isvisited() == true)
						continue;
					cho = &map[cur->getRow()][cur->getCol() - 1];
					cho->setVisit(true);
					cur->deleteLeft();
					cho->deleteRight();
					if (animate)
					{
						r.MoveToXY(startX + cho->getCol() * rect_size + 1, startY + cho->getRow() * rect_size + 1);
						dc.FillRect(&r, &visit);
						dc.MoveTo(this->startX + cur->getCol() * rect_size,
							this->startY + cur->getRow() * rect_size);
						dc.LineTo(this->startX + cur->getCol() * rect_size,
							this->startY + (cur->getRow() + 1) * rect_size);
					}
					break;
				}
				if (ranNum == 1)
				{
					if (cur->getCol() == miro.getCols() - 1
						|| map[cur->getRow()][cur->getCol() + 1].Isvisited() == true)
						continue;
					cho = &map[cur->getRow()][cur->getCol() + 1];
					cho->setVisit(true);
					cur->deleteRight();
					cho->deleteLeft();
					if (animate)
					{
						r.MoveToXY(startX + cho->getCol() * rect_size + 1, startY + cho->getRow() * rect_size + 1);
						dc.FillRect(&r, &visit);
						dc.MoveTo(this->startX + (cur->getCol() + 1) * rect_size,
							this->startY + cur->getRow() * rect_size);
						dc.LineTo(this->startX + (cur->getCol() + 1) * rect_size,
							this->startY + (cur->getRow() + 1) * rect_size);
					}
					break;
				}
				if (ranNum == 2)
				{
					if (cur->getRow() == 0
						|| map[cur->getRow() - 1][cur->getCol()].Isvisited() == true)
						continue;
					cho = &map[cur->getRow() - 1][cur->getCol()];
					cho->setVisit(true);
					cur->deleteUp();
					cho->deleteDown();
					if (animate)
					{
						r.MoveToXY(startX + cho->getCol() * rect_size + 1, startY + cho->getRow() * rect_size + 1);
						dc.FillRect(&r, &visit);
						dc.MoveTo(this->startX + cur->getCol() * rect_size,
							this->startY + cur->getRow() * rect_size);
						dc.LineTo(this->startX + (cur->getCol() + 1) * rect_size,
							this->startY + cur->getRow() * rect_size);
					}
					break;
				}
				if (ranNum == 3)
				{
					if (cur->getRow() == miro.getRows() - 1
						|| map[cur->getRow() + 1][cur->getCol()].Isvisited() == true)
						continue;
					cho = &map[cur->getRow() + 1][cur->getCol()];
					cho->setVisit(true);
					cur->deleteDown();
					cho->deleteUp();
					if (animate)
					{
						r.MoveToXY(startX + cho->getCol() * rect_size + 1, startY + cho->getRow() * rect_size + 1);
						dc.FillRect(&r, &visit);
						dc.MoveTo(this->startX + cur->getCol() * rect_size,
							this->startY + (cur->getRow() + 1) * rect_size);
						dc.LineTo(this->startX + (cur->getCol() + 1) * rect_size,
							this->startY + (cur->getRow() + 1) * rect_size);
					}
					break;
				}
			}
			if (animate)
				Sleep(10);
			stack.Push(cho);
		}
	}
}

void CStaticCanvasControl::findPathByDfs(BOOL animate)
{
	Invalidate(FALSE);
	Data* cur, * des, * next, * end;
	MyStack stack;
	Data(*map)[30];
	CPaintDC dc(this);
	CPen red_pen(PS_SOLID, 5, RGB(255, 0, 0));
	CPen path_pen(PS_SOLID, 4, RGB(155, 155, 255));
	CPen* old_pen;
	CRect r(0, 0, rect_size - 2, rect_size - 2);
	CBrush visit(RGB(155, 155, 255));
	CBrush red(RGB(255, 0, 0));
	CBrush blue(RGB(0, 0, 255));

	map = (Data(*)[30])miro.getMap();
	cur = &map[red_y][red_x];
	next = cur;
	des = &map[blue_y][blue_x];
	r.MoveToXY(startX + cur->getCol() * rect_size + 1, startY + cur->getRow() * rect_size + 1);
	dc.FillRect(&r, &visit);
	dc.SelectObject(&red);
	dc.Ellipse(startX + cur->getCol() * rect_size, startY + cur->getRow() * rect_size,
		startX + (cur->getCol() + 1) * rect_size, startY + (cur->getRow() + 1) * rect_size);
	cur->setVisit(true);
	stack.Push(cur);
	old_pen = dc.SelectObject(&path_pen);
	while (1)
	{
		cur = stack.Pop();
		if (cur == des)
			break;
		if (miro.findRoot(cur))
		{
			cur->setVisit(true);
			stack.Push(cur);
			if ((cur->getSide() & 1) == 0 && map[cur->getRow()][cur->getCol() - 1].Isvisited() == false)
			{
				next = &map[cur->getRow()][cur->getCol() - 1];
				r.MoveToXY(startX + next->getCol() * rect_size + 1, startY + next->getRow() * rect_size + 1);
				dc.FillRect(&r, &visit);
				dc.MoveTo(this->startX + cur->getCol() * rect_size,
					this->startY + cur->getRow() * rect_size);
				dc.LineTo(this->startX + cur->getCol() * rect_size,
					this->startY + (cur->getRow() + 1) * rect_size);
			}
			else if ((cur->getSide() & 2) == 0 && map[cur->getRow()][cur->getCol() + 1].Isvisited() == false)
			{
				next = &map[cur->getRow()][cur->getCol() + 1];
				r.MoveToXY(startX + next->getCol() * rect_size + 1, startY + next->getRow() * rect_size + 1);
				dc.FillRect(&r, &visit);
				dc.MoveTo(this->startX + (cur->getCol() + 1) * rect_size,
					this->startY + cur->getRow() * rect_size);
				dc.LineTo(this->startX + (cur->getCol() + 1) * rect_size,
					this->startY + (cur->getRow() + 1) * rect_size);
			}
			else if ((cur->getSide() & 4) == 0 && map[cur->getRow() - 1][cur->getCol()].Isvisited() == false)
			{
				next = &map[cur->getRow() - 1][cur->getCol()];
				r.MoveToXY(startX + next->getCol() * rect_size + 1, startY + next->getRow() * rect_size + 1);
				dc.FillRect(&r, &visit);
				dc.MoveTo(this->startX + cur->getCol() * rect_size,
					this->startY + cur->getRow() * rect_size);
				dc.LineTo(this->startX + (cur->getCol() + 1) * rect_size,
					this->startY + cur->getRow() * rect_size);
			}
			else
			{
				next = &map[cur->getRow() + 1][cur->getCol()];
				r.MoveToXY(startX + next->getCol() * rect_size + 1, startY + next->getRow() * rect_size + 1);
				dc.FillRect(&r, &visit);
				dc.MoveTo(this->startX + cur->getCol() * rect_size,
					this->startY + (cur->getRow() + 1) * rect_size);
				dc.LineTo(this->startX + (cur->getCol() + 1) * rect_size,
					this->startY + (cur->getRow() + 1) * rect_size);
			}
			if (animate)
				Sleep(10);
			next->setVisit(true);
			next->setPar(cur);
			stack.Push(next);
		}
	}
	end = cur;
	dc.SelectObject(old_pen);
	old_pen = dc.SelectObject(&red_pen);
	dc.MoveTo(startX + cur->getCol() * rect_size + rect_size / 2,
		startY + cur->getRow() * rect_size + rect_size / 2);
	cur = cur->getPar();
	while (cur)
	{
		dc.LineTo(startX + cur->getCol() * rect_size + rect_size / 2,
			startY + cur->getRow() * rect_size + rect_size / 2);
		cur = cur->getPar();
	}
	dc.SelectObject(old_pen);
	dc.SelectObject(&blue);
	dc.Ellipse(startX + end->getCol() * rect_size, startY + end->getRow() * rect_size,
		startX + (end->getCol() + 1) * rect_size, startY + (end->getRow() + 1) * rect_size);
	UpdateWindow();
}

void CStaticCanvasControl::findPathByBfs(BOOL animate)
{
	Invalidate(FALSE);
	Data* cur, * des, * next, * end;
	MyQueue queue;
	Data(*map)[30];
	CPaintDC dc(this);
	CPen red_pen(PS_SOLID, 5, RGB(255, 0, 0));
	CPen path_pen(PS_SOLID, 4, RGB(155, 155, 255));
	CPen* old_pen;
	CRect r(0, 0, rect_size - 2, rect_size - 2);
	CBrush visit(RGB(155, 155, 255));
	CBrush red(RGB(255, 0, 0));
	CBrush blue(RGB(0, 0, 255));

	map = (Data(*)[30])miro.getMap();
	cur = &map[red_y][red_x];
	next = cur;
	des = &map[blue_y][blue_x];
	r.MoveToXY(startX + cur->getCol() * rect_size + 1, startY + cur->getRow() * rect_size + 1);
	dc.FillRect(&r, &visit);
	dc.SelectObject(&red);
	dc.Ellipse(startX + cur->getCol() * rect_size, startY + cur->getRow() * rect_size,
		startX + (cur->getCol() + 1) * rect_size, startY + (cur->getRow() + 1) * rect_size);
	cur->setVisit(true);
	queue.Push(cur);
	old_pen = dc.SelectObject(&path_pen);
	while (1)
	{
		cur = queue.Pop();
		if (cur == des)
			break;
		if (miro.findRoot(cur))
		{
			cur->setVisit(true);
			if ((cur->getSide() & 1) == 0 && map[cur->getRow()][cur->getCol() - 1].Isvisited() == false)
			{
				next = &map[cur->getRow()][cur->getCol() - 1];
				r.MoveToXY(startX + next->getCol() * rect_size + 1, startY + next->getRow() * rect_size + 1);
				dc.FillRect(&r, &visit);
				dc.MoveTo(this->startX + cur->getCol() * rect_size,
					this->startY + cur->getRow() * rect_size);
				dc.LineTo(this->startX + cur->getCol() * rect_size,
					this->startY + (cur->getRow() + 1) * rect_size);
				if (animate)
					Sleep(10);
				next->setVisit(true);
				next->setPar(cur);
				queue.Push(next);
			}
			if ((cur->getSide() & 2) == 0 && map[cur->getRow()][cur->getCol() + 1].Isvisited() == false)
			{
				next = &map[cur->getRow()][cur->getCol() + 1];
				r.MoveToXY(startX + next->getCol() * rect_size + 1, startY + next->getRow() * rect_size + 1);
				dc.FillRect(&r, &visit);
				dc.MoveTo(this->startX + (cur->getCol() + 1) * rect_size,
					this->startY + cur->getRow() * rect_size);
				dc.LineTo(this->startX + (cur->getCol() + 1) * rect_size,
					this->startY + (cur->getRow() + 1) * rect_size);
				if (animate)
					Sleep(10);
				next->setVisit(true);
				next->setPar(cur);
				queue.Push(next);
			}
			if ((cur->getSide() & 4) == 0 && map[cur->getRow() - 1][cur->getCol()].Isvisited() == false)
			{
				next = &map[cur->getRow() - 1][cur->getCol()];
				r.MoveToXY(startX + next->getCol() * rect_size + 1, startY + next->getRow() * rect_size + 1);
				dc.FillRect(&r, &visit);
				dc.MoveTo(this->startX + cur->getCol() * rect_size,
					this->startY + cur->getRow() * rect_size);
				dc.LineTo(this->startX + (cur->getCol() + 1) * rect_size,
					this->startY + cur->getRow() * rect_size);
				if (animate)
					Sleep(10);
				next->setVisit(true);
				next->setPar(cur);
				queue.Push(next);
			}
			if ((cur->getSide() & 8) == 0 && map[cur->getRow() + 1][cur->getCol()].Isvisited() == false)
			{
				next = &map[cur->getRow() + 1][cur->getCol()];
				r.MoveToXY(startX + next->getCol() * rect_size + 1, startY + next->getRow() * rect_size + 1);
				dc.FillRect(&r, &visit);
				dc.MoveTo(this->startX + cur->getCol() * rect_size,
					this->startY + (cur->getRow() + 1) * rect_size);
				dc.LineTo(this->startX + (cur->getCol() + 1) * rect_size,
					this->startY + (cur->getRow() + 1) * rect_size);
				if (animate)
					Sleep(10);
				next->setVisit(true);
				next->setPar(cur);
				queue.Push(next);
			}
		}
	}
	end = cur;
	dc.SelectObject(old_pen);
	old_pen = dc.SelectObject(&red_pen);
	dc.MoveTo(startX + cur->getCol() * rect_size + rect_size / 2,
		startY + cur->getRow() * rect_size + rect_size / 2);
	cur = cur->getPar();
	while (cur)
	{
		dc.LineTo(startX + cur->getCol() * rect_size + rect_size / 2,
			startY + cur->getRow() * rect_size + rect_size / 2);
		cur = cur->getPar();
	}
	dc.SelectObject(old_pen);
	dc.SelectObject(&blue);
	dc.Ellipse(startX + end->getCol() * rect_size, startY + end->getRow() * rect_size,
		startX + (end->getCol() + 1) * rect_size, startY + (end->getRow() + 1) * rect_size);
	UpdateWindow();
}

void CStaticCanvasControl::DrawMap()
{
	Invalidate();
	CPaintDC dc(this);
	Data(*map)[30];
	CPen white_pen(PS_SOLID, 4, RGB(255, 255, 255));
	CBrush red(RGB(255, 0, 0));
	CBrush blue(RGB(0, 0, 255));
	CBrush white(RGB(255, 255, 255));

	// 더블 버퍼링 코드
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, m_rect_window.Width(), m_rect_window.Height());
	memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, m_rect_window.Width(), m_rect_window.Height(), WHITENESS);

	map = (Data(*)[30])miro.getMap();
	memDC.SelectObject(&m_black_pen);
	for (int y = startY; y < startY + maze_height; y += rect_size)
	{
		for (int x = startX; x < startX + maze_width; x += rect_size)
		{
			CRect r(x, y, x + rect_size, y + rect_size);
			if (x <= current_mouse_point.x && current_mouse_point.x < x + rect_size
				&& y <= current_mouse_point.y && current_mouse_point.y < y + rect_size)
			{
				CBrush yellow(RGB(255, 255, 0));
				memDC.FillRect(&r, &yellow);
			}
			else
				memDC.Rectangle(r);
			if (startX + red_x * rect_size <= x && x < startX + (red_x + 1) * rect_size
				&& startY + red_y * rect_size <= y && y < startY + (red_y + 1) * rect_size)
			{
				memDC.SelectObject(&red);
				memDC.Ellipse(x, y, x + rect_size, y + rect_size);
				memDC.SelectObject(&white);
			}
			else if (startX + blue_x * rect_size <= x && x < startX + (blue_x + 1) * rect_size
				&& startY + blue_y * rect_size <= y && y < startY + (blue_y + 1) * rect_size)
			{
				memDC.SelectObject(&blue);
				memDC.Ellipse(x, y, x + rect_size, y + rect_size);
				memDC.SelectObject(&white);
			}
		}
	}
	memDC.SelectObject(&white_pen);
	for (int y = 0; y < miro.getRows(); y++)
	{
		for (int x = 0; x < miro.getCols(); x++)
		{
			if ((map[y][x].getSide() & 1) == 0)
			{
				memDC.MoveTo(startX + x * rect_size, startY + y * rect_size);
				memDC.LineTo(startX + x * rect_size, startY + (y + 1) * rect_size);
			}
			if ((map[y][x].getSide() & 2) == 0)
			{
				memDC.MoveTo(startX + (x + 1) * rect_size, startY + y * rect_size);
				memDC.LineTo(startX + (x + 1) * rect_size, startY + (y + 1) * rect_size);
			}
			if ((map[y][x].getSide() & 4) == 0)
			{
				memDC.MoveTo(startX + x * rect_size, startY + y * rect_size);
				memDC.LineTo(startX + (x + 1) * rect_size, startY + y * rect_size);
			}
			if ((map[y][x].getSide() & 8) == 0)
			{
				memDC.MoveTo(startX + x * rect_size, startY + (y + 1) * rect_size);
				memDC.LineTo(startX + (x + 1) * rect_size, startY + (y + 1) * rect_size);
			}
		}
	}
	dc.BitBlt(0, 0, m_rect_window.Width(), m_rect_window.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.DeleteDC();
	bitmap.DeleteObject();
	UpdateWindow();
}

void CStaticCanvasControl::OnPaint()
{
	CPaintDC dc(this);// device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CStatic::OnPaint()을(를) 호출하지 마십시오.
	GetClientRect(&(this->m_rect_window));
	dc.FillRect(&(this->m_rect_window), &(this->m_white_brush));
}

void CStaticCanvasControl::DrawMazeOnGeneration(int row, int column, BOOL animate)
{
	Invalidate();
	CPaintDC dc(this);
	CPen m_black_pen(PS_SOLID, 2, RGB(0, 0, 0));

	if ((row < 4 || row > 30) || (column < 4 || column > 30))
		return;
	if (m_rect_window.Height() / row > m_rect_window.Width() / column)
		rect_size = m_rect_window.Width() / column;
	else
		rect_size = m_rect_window.Height() / row;
	maze_height = rect_size * row;
	maze_width = rect_size * column;
	startY = (this->m_rect_window.Height() - maze_height) / 2;
	startX = (this->m_rect_window.Width() - maze_width) / 2;

	miro.setSize(row, column);
	miro.setSide();
	dc.SelectObject(&m_black_pen);
	for (int y = this->startY; y < this->startY + maze_height; y += rect_size)
	{
		for (int x = this->startX; x < this->startX + maze_width; x += rect_size)
			dc.Rectangle(x, y, x + rect_size, y + rect_size);
	}
	UpdateWindow();
	BuildMapByDfs(animate);
	DrawMap();
	miro.setVisitFalse();
}

void CStaticCanvasControl::DrawPath(BOOL method, BOOL animate)
{
	if (red_y < 0 || red_x < 0 || blue_y < 0 || blue_x < 0 || (red_y == blue_y && red_x == blue_x)
		|| red_y >= miro.getRows() || red_x >= miro.getCols()
		|| blue_y >= miro.getRows() || blue_x >= miro.getCols())
		return;
	DrawMap();
	if (method == 1)
		findPathByDfs(animate);
	else if (method == 2)
		findPathByBfs(animate);
	miro.setPar();
	miro.setVisitFalse();
}

void CStaticCanvasControl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	this->current_mouse_point = point;
	this->DrawMap();
	CStatic::OnMouseMove(nFlags, point);
}

void CStaticCanvasControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	Invalidate();
	CPaintDC dc(this);
	CBrush red(RGB(255, 0, 0));

	dc.SelectObject(&red);
	for (int y = 0; y < miro.getRows(); y++)
	{
		for (int x = 0; x < miro.getCols(); x++)
		{
			if (startX + x * rect_size <= point.x && point.x < startX + (x + 1) * rect_size
				&& startY + y * rect_size <= point.y && point.y < startY + (y + 1) * rect_size)
			{
				dc.Ellipse(startX + x * rect_size, startY + y * rect_size,
					startX + (x + 1) * rect_size, startY + (y + 1) * rect_size);
				red_y = y;
				red_x = x;
			}
		}
	}
	CStatic::OnLButtonDown(nFlags, point);
}

void CStaticCanvasControl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	Invalidate();
	CPaintDC dc(this);
	CBrush blue(RGB(0, 0, 255));

	dc.SelectObject(&blue);
	for (int y = 0; y < miro.getRows(); y++)
	{
		for (int x = 0; x < miro.getCols(); x++)
		{
			if (startX + x * rect_size <= point.x && point.x < startX + (x + 1) * rect_size
				&& startY + y * rect_size <= point.y && point.y < startY + (y + 1) * rect_size)
			{
				dc.Ellipse(startX + x * rect_size, startY + y * rect_size,
					startX + (x + 1) * rect_size, startY + (y + 1) * rect_size);
				blue_y = y;
				blue_x = x;
			}
		}
	}
	CStatic::OnRButtonDown(nFlags, point);
}
