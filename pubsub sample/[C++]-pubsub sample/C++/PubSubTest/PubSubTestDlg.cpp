
// PubSubTestDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "PubSubTest.h"
#include "PubSubTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPubSubTestDlg ��ȭ ����

CPubSubTestDlg *CPubSubTestDlg::instance = NULL;


CPubSubTestDlg::CPubSubTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPubSubTestDlg::IDD, pParent)
	, ipaddr_(0)
	, port_(0)
	, channel_(_T(""))
	, message_(_T(""))
{
	instance = this;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	WSAData wsaData;
	WSAStartup(MAKEWORD(1, 1), &wsaData);
}

void CPubSubTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IPADDRESS, ipaddr_);
	DDX_Text(pDX, IDC_EDIT_PORT, port_);
	DDX_Text(pDX, IDC_EDIT_KEY, channel_);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, message_);
}

BEGIN_MESSAGE_MAP(CPubSubTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CPubSubTestDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_SUBSCRIBE, &CPubSubTestDlg::OnBnClickedBtnSubscribe)
	ON_BN_CLICKED(IDC_BTN_UNSUBSCRIBE, &CPubSubTestDlg::OnBnClickedBtnUnsubscribe)
	ON_BN_CLICKED(IDC_BTN_PUBLISH, &CPubSubTestDlg::OnBnClickedBtnPublish)
END_MESSAGE_MAP()


// CPubSubTestDlg �޽��� ó����

BOOL CPubSubTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	SetTimer(2699, 100, NULL);

	CIPAddressCtrl *ipctrl = (CIPAddressCtrl *)GetDlgItem(IDC_IPADDRESS);
	ipctrl->SetAddress(192, 168, 25, 200);

	SetDlgItemText(IDC_EDIT_PORT, "6379");

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CPubSubTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CPubSubTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPubSubTestDlg::OnBnClickedBtnConnect()
{
	UpdateData(TRUE);

	CString ipaddr;
	ipaddr.Format("%d.%d.%d.%d", (ipaddr_ >> 24) & 0xff, (ipaddr_ >> 16) & 0xff, (ipaddr_ >> 8) & 0xff, ipaddr_ & 0xff);

	PubSubMan_.Connect(ipaddr, port_);
}

void CPubSubTestDlg::OnBnClickedBtnSubscribe()
{
	UpdateData(TRUE);

	PubSubMan_.Subscribe(channel_);
}

void CPubSubTestDlg::OnBnClickedBtnUnsubscribe()
{
	UpdateData(TRUE);

	PubSubMan_.Unsubscribe(channel_);
}

void CPubSubTestDlg::OnBnClickedBtnPublish()
{
	UpdateData(TRUE);

	PubSubMan_.Publish(channel_, message_);
}

void CPubSubTestDlg::OnTimer( UINT_PTR nIDEvent )
{
	PubSubMan_.OnTimer();
}

void CPubSubTestDlg::DebugLog(const char *log)
{
	CEdit *edit = (CEdit *)instance->GetDlgItem(IDC_EDIT_STATUS);

	edit->SetSel(0x7fffffff, 0x7fffffff);
	edit->ReplaceSel(log);
	edit->LineScroll(0xffffffff);
}

void DEBUGLOG(const char *fmt, ...)
{
	char szTempBuf[4096] ;
	va_list vlMarker ;

	va_start(vlMarker,fmt) ;
	_vstprintf(szTempBuf,fmt,vlMarker) ;
	va_end(vlMarker) ;

	strcat_s(szTempBuf, "\r\n");
	CPubSubTestDlg::DebugLog(szTempBuf);
}