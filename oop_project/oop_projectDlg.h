
// oop_projectDlg.h: 헤더 파일
//

#pragma once

#include "CStaticCanvasControl.h"

// CoopprojectDlg 대화 상자
class CoopprojectDlg : public CDialogEx
{
// 생성입니다.
public:
	CoopprojectDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OOP_PROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSolvingRadioGroupClicked(UINT uiID);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonExit();
private:
	CStaticCanvasControl m_static_canvas_control;
public:
	afx_msg void OnBnClickedButtonGenerate();
	afx_msg void OnBnClickedButtonSolve();
private:
	int m_edit_row;
	int m_edit_column;
	BOOL m_solving_method;
	BOOL m_check_animate_generation;
private:
	BOOL m_check_animate_solve;
};
