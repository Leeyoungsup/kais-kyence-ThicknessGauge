
// IGlimYSDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "IGlimYS.h"
#include "IGlimYSDlg.h"
#include "afxdialogex.h"
#include "MotionPAS.h"
#include <thread>
#ifdef _DEBUG
#define new DEBUG_NEW

#endif

using std::thread;
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CIGlimYSDlg 대화 상자



CIGlimYSDlg::CIGlimYSDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IGLIMYS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIGlimYSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLUS_TEXT, PlusMoveV);
	DDX_Control(pDX, IDC_PREV_TEXT, PrevMoveV);
	DDX_Control(pDX, IDC_LIMIT_TEXT, m_LimitV);
	DDX_Control(pDX, IDC_EDIT1, m_actualV);
	DDX_Control(pDX, IDC_EDIT2,m_commandV);
	
}

BEGIN_MESSAGE_MAP(CIGlimYSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CONTROL(BN_CLICK_DOWN, IDC_MOVER_BTN, MovePlus)
	ON_CONTROL(BN_CLICK_UP, IDC_MOVER_BTN,Movestop)
	ON_CONTROL(BN_CLICK_DOWN, IDC_MOVEL_BTN, MovePrev)
	ON_CONTROL(BN_CLICK_UP, IDC_MOVEL_BTN, Movestop)
	ON_BN_CLICKED(IDC_BUTTON_LIMIT_PLUS, &CIGlimYSDlg::OnBnClickedButtonLimitPlus)
	ON_BN_CLICKED(IDC_BUTTON_LIMIT_PREV, &CIGlimYSDlg::OnBnClickedButtonLimitPrev)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CIGlimYSDlg 메시지 처리기

BOOL CIGlimYSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	m_mover_btn.SubclassDlgItem(IDC_MOVER_BTN, this);
	m_movel_btn.SubclassDlgItem(IDC_MOVEL_BTN, this);
	CString str; 
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	m_motionKais.InitMotion(this, 0);
	TRACE("This is a debug string of text in MFC");
	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	str = "5000";
	PrevMoveV.SetWindowTextW(str);
	PlusMoveV.SetWindowTextW(str);
	m_LimitV.SetWindowTextW(str);
	SetTimer(1, 200, NULL);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CIGlimYSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CIGlimYSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CIGlimYSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CIGlimYSDlg ::MovePlus() {
	CString str;
	PlusMoveV.GetWindowTextW(str);
	int nTestNum;
	nTestNum = _ttoi(str);
	DWORD Velo = nTestNum;
	m_motionKais.moveV(0, 1, Velo);
}

void CIGlimYSDlg::Movestop() {
	m_motionKais.moveStop(0);
}
void CIGlimYSDlg::MovePrev() {
	CString str;
	PrevMoveV.GetWindowTextW(str);
	int nTestNum;
	nTestNum = _ttoi(str);
	DWORD Velo = nTestNum;
	m_motionKais.moveV(0, 0, Velo);
}


void CIGlimYSDlg::CommandPos()
{
	long value;
	value = m_motionKais.getCPosition(0);
	CString strValue;
	char vOut[12];
	_itoa_s(value, vOut, sizeof(vOut), 10);
	strValue = (LPSTR)vOut;
	m_commandV.SetWindowTextW(strValue);
	
}

void CIGlimYSDlg::ActualPos()
{
	long value;
	value = m_motionKais.getAPosition(0);
	CString strValue;
	char vOut[12];
	_itoa_s(value, vOut, sizeof(vOut), 10);
	strValue = (LPSTR)vOut;
	m_actualV.SetWindowTextW(strValue);
	
	
}


void CIGlimYSDlg::OnBnClickedButtonLimitPlus()
{
	CString str;
	PrevMoveV.GetWindowTextW(str);

	int nTestNum;
	nTestNum = _ttoi(str);
	DWORD Velo = nTestNum;
	m_motionKais.moveV(0, 1, Velo);
}


void CIGlimYSDlg::OnBnClickedButtonLimitPrev()
{
	CString str;
	PrevMoveV.GetWindowTextW(str);
	int nTestNum;
	nTestNum = _ttoi(str);
	DWORD Velo = nTestNum;
	m_motionKais.moveV(0, 0, Velo);
}


void CIGlimYSDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	ActualPos();
	CommandPos();
	CDialogEx::OnTimer(nIDEvent);
}
