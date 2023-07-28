#pragma once
#include <Windows.h>
#define LEN 900
#include <cstring>
#include <cstdlib>
#include <ctime>
// CStaticCanvasControl

class Data
{
private:
	bool Isvisit;
	char side;
	int row, col;
	Data* par;
public:
	Data();

	bool Isvisited();
	void setVisit(bool n);
	void setRow(int n);
	int getRow();
	void setCol(int n);
	int getCol();
	void setPar(Data* data);
	Data* getPar();
	void setSide();
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

class Map
{
private:
	Data map[30][30];
	int rows;
	int cols;

public:
	Map();

	void setSize(int row, int col);
	int getRows();
	int getCols();
	void* getMap();
	bool findNeighbor(Data* ptr);
	bool findRoot(Data* ptr);
	void setVisitFalse();
	void setSide();
	void setPar();
};

class CStaticCanvasControl : public CStatic
{
	DECLARE_DYNAMIC(CStaticCanvasControl)

public:
	CStaticCanvasControl();
	virtual ~CStaticCanvasControl();

private:
	CPen m_black_pen;
	CRect m_rect_window;
	CBrush m_white_brush;
	int startX;
	int startY;
	int maze_height;
	int maze_width;
	int rect_size;
	int red_y, red_x, blue_y, blue_x;
	Map miro;
	CPoint current_mouse_point;
	CPoint start_point;
	CPoint end_point;
	void BuildMapByDfs(BOOL animate);
	void findPathByDfs(BOOL animate);
	void findPathByBfs(BOOL animate);
	void DrawMap();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	void DrawMazeOnGeneration(int row, int column, BOOL animate);
	void DrawPath(BOOL method, BOOL animate);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
