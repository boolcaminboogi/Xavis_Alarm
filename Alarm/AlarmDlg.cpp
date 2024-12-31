
// AlarmDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Alarm.h"
#include "AlarmDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <afx.h> // MFC의 CTime 및 CTimeSpan을 포함



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

public:
	afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult);

};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX){}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_STARTDATE, &CAboutDlg::OnDtnDatetimechangeDatetimepicker1)
END_MESSAGE_MAP()
// CAlarmDlg 대화 상자
CAlarmDlg::CAlarmDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ALARM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAlarmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAlarmDlg, CDialogEx)
	/*ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()*/
	//ON_BN_CLICKED(IDC_BUTTON1, &CAlarmDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_ADD_BUTTON, &CAlarmDlg::OnBnClickedAddButton)
	ON_BN_CLICKED(IDC_DEL_BUTTON, &CAlarmDlg::OnBnClickedDelButton)
	ON_BN_CLICKED(IDC_SAVE_BUTTON, &CAlarmDlg::OnBnClickedSaveButton)
	ON_BN_CLICKED(IDC_LOAD_BUTTON, &CAlarmDlg::OnBnClickedLoadButton)
	ON_BN_CLICKED(IDC_CURRENT_BUTTON, &CAlarmDlg::OnBnClickedCurrentButton)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LISTCONTROL, &CAlarmDlg::OnNMCustomdrawList)
END_MESSAGE_MAP()


// CAlarmDlg 메시지 처리기

BOOL CAlarmDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 현재 시간으로 초기화
	CTime currentTime = CTime::GetCurrentTime();

	// Date-Time Picker 초기화
	if (m_StartDate)
	{
		m_StartDate.SetTime(&currentTime);
	}


	// Edit Control 초기화 (HH:mm 형식)
	CString timeText;
	timeText.Format(_T("%02d:%02d"), currentTime.GetHour(), currentTime.GetMinute());
	GetDlgItem(IDC_TIME_EDIT)->SetWindowText(timeText); // Edit Control에 초기값 설정

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	ASSERT(GetDlgItem(IDC_TIME_EDIT) != nullptr); // Edit Control 확인
	ASSERT(GetDlgItem(IDC_CURRENT_BUTTON) != nullptr); // 현재 버튼 확인
	ASSERT(GetDlgItem(IDC_LISTCONTROL) != nullptr); // 리스트 컨트롤 초기화 확인



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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	
	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// 컨트롤 연결
	//m_ListBox.Attach(GetDlgItem(IDC_LISTBOX)->m_hWnd);
	m_ComboBox.Attach(GetDlgItem(IDC_COMBOBOX)->m_hWnd);
	m_StartDate.Attach(GetDlgItem(IDC_STARTDATE)->m_hWnd);
	//m_EndDate.Attach(GetDlgItem(IDC_ENDDATE)->m_hWnd);
	// 리스트 컨트롤 초기화
	m_ListCtrl.Attach(GetDlgItem(IDC_LISTCONTROL)->m_hWnd); // 리스트 컨트롤 연결
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); // 스타일 설정

	// 콤보박스 초기값 설정 (예: 품목 추가)
	m_ComboBox.AddString(_T("Filament 교체"));
	m_ComboBox.AddString(_T("진공(Oring) 체크"));
	m_ComboBox.AddString(_T("고압 케이블 grease 도포"));
	m_ComboBox.AddString(_T("진공펌프 오일 교체	"));
	m_ComboBox.AddString(_T("진공펌프 오일 주입	"));
	m_ComboBox.AddString(_T("진공센서 교체"));
	m_ComboBox.AddString(_T("진공펌프 필터 교체/청소"));
	// 컬럼 추가
	m_ListCtrl.InsertColumn(0, _T("항목"), LVCFMT_LEFT, 150);
	m_ListCtrl.InsertColumn(1, _T("시작 날짜/시간"), LVCFMT_LEFT, 150);
	m_ListCtrl.InsertColumn(2, _T("종료 날짜/시간"), LVCFMT_LEFT, 150);

	// Bold Font 초기화
	LOGFONT lf = { 0 };
	GetFont()->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	m_BoldFont.CreateFontIndirect(&lf);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CAlarmDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAlarmDlg::OnPaint()
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
HCURSOR CAlarmDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAboutDlg::OnDtnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

/*
void CAlarmDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}*/

//추가 버튼
void CAlarmDlg::OnBnClickedAddButton()
{
	CString selectedItem;
	int index = m_ComboBox.GetCurSel();

	// 콤보박스에서 선택된 항목 가져오기
	if (index != CB_ERR)
	{
		m_ComboBox.GetLBText(index, selectedItem);
	}
	else
	{
		AfxMessageBox(_T("품목을 선택하세요."));
		return;
	}

	// Date-Time Picker에서 날짜 가져오기
	CTime startDate;
	m_StartDate.GetTime(startDate);

	// Edit Control에서 시간 가져오기
	CString timeText;
	GetDlgItem(IDC_TIME_EDIT)->GetWindowText(timeText);

	int hour = 0, minute = 0;
	if (_stscanf_s(timeText, _T("%d:%d"), &hour, &minute) != 2)
	{
		AfxMessageBox(_T("시간 형식이 올바르지 않습니다. (예: HH:mm)"));
		return;
	}

	// 시작 시간 생성
	CTime startTime(startDate.GetYear(), startDate.GetMonth(), startDate.GetDay(), hour, minute, 0);

	// 종료 시간 계산
	CTime endTime = CalculateEndTime(startTime, selectedItem);

	// 데이터 저장용 구조체 생성 및 초기화
	ListCtrlData data;
	data.itemText = selectedItem;
	data.startTime = startTime;
	data.endTime = endTime;

	// 리스트컨트롤에 추가
	int rowIndex = m_ListCtrl.InsertItem(0, data.itemText); // 첫 번째 컬럼 (항목)
	m_ListCtrl.SetItemText(rowIndex, 1, data.GetStartTimeStr()); // 두 번째 컬럼 (시작 날짜/시간)
	m_ListCtrl.SetItemText(rowIndex, 2, data.GetEndTimeStr());   // 세 번째 컬럼 (종료 날짜/시간)

	// 데이터 리스트에 저장
	m_DataList.push_back(data);
}


//제거 버튼
void CAlarmDlg::OnBnClickedDelButton()
{
	int selIndex = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	//int selIndex = m_ListBox.GetCurSel();
	if (selIndex != LB_ERR)
	{
		m_ListCtrl.DeleteItem(selIndex);
	}
	else
	{
		AfxMessageBox(_T("삭제할 항목을 선택하세요."));
	}
}

//현재시각 버튼
void CAlarmDlg::OnBnClickedCurrentButton()
{
	// 현재 시간 가져오기
	CTime currentTime = CTime::GetCurrentTime();

	// Date-Time Picker에 현재 날짜 설정
	m_StartDate.SetTime(&currentTime);

	// Edit Control에 현재 시간 설정 (HH:mm 형식)
	CString timeText;
	timeText.Format(_T("%02d:%02d"), currentTime.GetHour(), currentTime.GetMinute());
	GetDlgItem(IDC_TIME_EDIT)->SetWindowText(timeText); // Edit Control에 설정
}


//파일 저장하기
void CAlarmDlg::OnBnClickedSaveButton()
{
	CFile file;
	if (file.Open(_T("ALARM_TEST_DATA.bin"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		for (const auto& data : m_DataList)
		{
			file.Write(&data, sizeof(data)); // 데이터를 바이너리 파일에 저장
		}

		file.Close();
		AfxMessageBox(_T("데이터를 성공적으로 저장했습니다."));
	}
	else
	{
		AfxMessageBox(_T("파일을 열 수 없습니다."));
	}
}

//파일 불러오기
void CAlarmDlg::OnBnClickedLoadButton()
{
	CFile file;
	if (file.Open(_T("ALARM_TEST_DATA.bin"), CFile::modeRead | CFile::typeBinary))
	{
		m_ListCtrl.DeleteAllItems();  // 기존 리스트 컨트롤 항목 삭제
		m_DataList.clear();           // 기존 데이터 리스트 초기화

		ListCtrlData data;
		size_t dataSize = sizeof(data);

		// 파일 크기 확인
		ULONGLONG fileSize = file.GetLength();
		if (fileSize % dataSize != 0)
		{
			AfxMessageBox(_T("파일 데이터 형식이 잘못되었습니다."));
			file.Close();
			return;
		}

		while (file.Read(&data, dataSize) == dataSize)
		{
			// 리스트 컨트롤에 항목 추가
			int rowIndex = m_ListCtrl.InsertItem(0, data.itemText);	//첫번째 칼럼
			m_ListCtrl.SetItemText(rowIndex, 1, data.GetStartTimeStr());	//두번째 칼럼
			m_ListCtrl.SetItemText(rowIndex, 2, data.GetEndTimeStr());	//세번째 칼럼

			// 데이터 리스트에 추가
			m_DataList.push_back(data);
		}

		file.Close();
		AfxMessageBox(_T("파일 데이터를 성공적으로 불러왔습니다."));
	}
	else
	{
		AfxMessageBox(_T("파일을 열 수 없습니다."));
	}
}



//각 항목의 주기 설정
	CTime CAlarmDlg::CalculateEndTime(const CTime & startTime, const CString & selectedItem)
	{
		int durationHours = 0;

		if (selectedItem == _T("Filament 교체"))
		{
			durationHours = 500; // 500시간
		}
		else if (selectedItem == _T("진공(Oring) 체크"))
		{
			durationHours = 7 * 24; // 7일 = 168시간
		}
		else if (selectedItem == _T("고압 케이블 grease 도포"))
		{
			durationHours = 3 * 30 * 24; // 3개월 = 2160시간
		}
		else if (selectedItem == _T("진공펌프 오일 교체") || selectedItem == _T("진공센서 교체"))
		{
			durationHours = 6 * 30 * 24; // 6개월 = 4320시간
		}
		else if (selectedItem == _T("진공펌프 필터 교체/청소"))
		{
			durationHours = 12 * 30 * 24; // 1년 = 8760시간
		}

		// 종료 시간 계산
		CTimeSpan duration(0, durationHours, 0, 0); // 시간 단위로 설정
		return startTime + duration;
	}


//타이머
void CAlarmDlg::OnTimer(UINT_PTR nIDEvent)
{
	CTime currentTime = CTime::GetCurrentTime();

	for (auto it = m_DataList.begin(); it != m_DataList.end(); )
	{
		if (it->endTime <= currentTime)
		{
			CString message;
			message.Format(_T("알람: %s가 종료되었습니다!"), it->itemText);
			AfxMessageBox(message);

			// 알람이 울린 후 리스트에서 제거
			for (int i = 0; i < m_ListCtrl.GetItemCount(); i++)
			{
				if (m_ListCtrl.GetItemText(i, 0) == CString(it->itemText))
				{
					m_ListCtrl.DeleteItem(i);
					break;
				}
			}

			it = m_DataList.erase(it);
		}
		else
		{
			++it;
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

//리스트 컬럼 설정
void CAlarmDlg::OnNMCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW pLVCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW; // 각 항목에 대해 추가 처리를 요청
		return;
	}

	if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW; // 서브 아이템에 대한 처리를 요청
		return;
	}

	if (pLVCD->nmcd.dwDrawStage == CDDS_SUBITEM | CDDS_ITEMPREPAINT)
	{
		int columnIndex = pLVCD->iSubItem;

		// 세 번째 컬럼에 강조 색 적용
		if (columnIndex == 2)
		{
			pLVCD->clrText = RGB(255, 0, 0); // 텍스트 색 빨간색
			pLVCD->clrTextBk = RGB(255, 255, 0); // 배경색 노란색
		}

		// 첫 번째 컬럼에 볼드체 적용
		if (columnIndex == 0) // 첫 번째 컬럼
		{
			SelectObject(pLVCD->nmcd.hdc, m_BoldFont);
		}
		/*if (columnIndex == 0)
		{
			CFont boldFont;
			LOGFONT lf = { 0 };
			GetFont()->GetLogFont(&lf);
			lf.lfWeight = FW_BOLD;
			boldFont.CreateFontIndirect(&lf);

			CFont* pOldFont = pLVCD->nmcd.hdc->SelectObject(&boldFont);
			pLVCD->nmcd.hdc->SelectObject(pOldFont);
		}*/

		*pResult = CDRF_DODEFAULT;
		return;
	}
}

