
// IGlimYSDlg.h: 헤더 파일
//
#include "MotionPAS.h"
#include "TW_Button.h"
#pragma once

// CIGlimYSDlg 대화 상자
class CIGlimYSDlg : public CDialogEx
{
// 생성입니다.
public:
	CIGlimYSDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IGLIMYS_DIALOG };
#endif
	MotionPAS		m_motionKais;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	TW_Button m_mover_btn, m_movel_btn;
	HICON m_hIcon;
	
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void MovePlus();
	
	void Movestop();
	void MovePrev();
	DECLARE_MESSAGE_MAP()
public:
	
	
	afx_msg void ActualPos();
	afx_msg void CommandPos();

	CEdit PlusMoveV;
	CEdit PrevMoveV;
	CEdit m_LimitV;
	CEdit m_actualV;
	CEdit m_commandV;
	afx_msg void OnBnClickedButtonLimitPlus();
	afx_msg void OnBnClickedButtonLimitPrev();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};




