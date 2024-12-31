
// AlarmDlg.h: 헤더 파일
//

#pragma once
#include <vector>

/*
// 리스트박스 데이터를 저장하기 위한 구조체
struct ListBoxData
{
	TCHAR itemText[256];  // 리스트박스 항목 텍스트 (최대 256자)
	CTime startTime;       // 시작 시간
	CTime endTime;         // 종료 시간

	// 생성자 추가
	ListBoxData()
	{
		ZeroMemory(itemText, sizeof(itemText)); // itemText 초기화
		startTime = CTime(0);                  // 시작 시간 초기화
		endTime = CTime(0);                    // 종료 시간 초기화
	}
};*/
struct ListCtrlData
{
	CString itemText;     // 항목
	CTime startTime;      // 시작 날짜/시간
	CTime endTime;        // 종료 날짜/시간

	CString GetStartTimeStr() const
	{
		return startTime.Format(_T("%Y-%m-%d %H:%M"));
	}

	CString GetEndTimeStr() const
	{
		return endTime.Format(_T("%Y-%m-%d %H:%M"));
	}

	// 생성자
	ListCtrlData()
	{
		itemText = _T("");   // CString 초기화
		startTime = CTime(0);
		endTime = CTime(0);
	}
};





// CAlarmDlg 대화 상자
class CAlarmDlg : public CDialogEx
{
// 생성입니다.
public:
	CAlarmDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedAddButton();  // 추가 버튼 클릭 핸들러
	afx_msg void OnBnClickedDelButton();  // 제거 버튼 클릭 핸들러
	afx_msg void OnBnClickedSaveButton(); // 저장 버튼 클릭 핸들러
	afx_msg void OnBnClickedLoadButton(); // 불러오기 버튼 클릭 핸들러
	afx_msg void OnBnClickedCurrentButton(); // 현재 버튼 클릭 핸들러
	afx_msg void OnNMCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLbnSelchangeListbox();
	virtual ~CAlarmDlg() = default; // 기본 소멸자 사용

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ALARM_DIALOG };
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

	DECLARE_MESSAGE_MAP()


// nana
private:
	CComboBox m_ComboBox;           // 콤보박스
	CDateTimeCtrl m_StartDate;      // 시작 날짜
	CDateTimeCtrl m_EndDate;        // 종료 날짜
	//CListBox m_ListBox;             // 리스트박스
	CListCtrl m_ListCtrl; // 리스트 컨트롤
	std::vector<ListCtrlData> m_DataList; // 알람 데이터를 저장
	CTime CalculateEndTime(const CTime& startTime, const CString& selectedItem);
	CFont m_BoldFont; // 볼드체 폰트 멤버 변수
	
};
