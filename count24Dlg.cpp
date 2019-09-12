
// count24Dlg.cpp : implementation file
//

#include "stdafx.h"

#include "lasy/fraction.h"

#include "count.h"

#include "count24.h"
#include "count24Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Ccount24Dlg dialog

static CListBox *g_lstSolution = NULL;

static CString __convert_to_string(const struct number *num)
{
	CString str, strLeft, strRight;
	if (num->left != NULL) {
		strLeft = __convert_to_string(num->left);
		if (is_mul_div(num->op) && is_add_sub(num->left->op)) {
			strLeft = CString(_T("( "))+strLeft+CString(_T(" )"));
		}
	}
	if (num->right != NULL) {
		strRight = __convert_to_string(num->right);
		if (is_mul_div(num->op) && is_add_sub(num->right->op) ||
			num->op == SUB && is_add_sub(num->right->op) ||
			num->op == DIV && is_mul_div(num->right->op))
		{
			strRight = CString(_T("( "))+strRight+CString(_T(" )"));
		}
	}
	switch (num->op) {
	case NOP:
		str.Format(_T("%d"), fraction_to_int(&num->value));
		break;
	case ADD:
		str = strLeft+CString(_T(" + "))+strRight;
		break;
	case SUB:
		str = strLeft+CString(_T(" - "))+strRight;
		break;
	case MUL:
		str = strLeft+CString(_T(" * "))+strRight;
		break;
	case DIV:
		str = strLeft+CString(_T(" / "))+strRight;
		break;
	default:
		break;
	}
	return str;
}

extern "C" void solution_output(const struct number *num)
{
	CString str = __convert_to_string(num);
	if (g_lstSolution->FindStringExact(-1, str) == LB_ERR) {
		g_lstSolution->AddString(__convert_to_string(num));
	}
}

Ccount24Dlg::Ccount24Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Ccount24Dlg::IDD, pParent)
	, num1(MIN_NUMBER)
	, num2(MIN_NUMBER)
	, num3(MIN_NUMBER)
	, num4(MIN_NUMBER)
	, result(TARGET_NUMBER)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIconNums[ 0] = AfxGetApp()->LoadIcon(IDI_ICON01);
	m_hIconNums[ 1] = AfxGetApp()->LoadIcon(IDI_ICON02);
	m_hIconNums[ 2] = AfxGetApp()->LoadIcon(IDI_ICON03);
	m_hIconNums[ 3] = AfxGetApp()->LoadIcon(IDI_ICON04);
	m_hIconNums[ 4] = AfxGetApp()->LoadIcon(IDI_ICON05);
	m_hIconNums[ 5] = AfxGetApp()->LoadIcon(IDI_ICON06);
	m_hIconNums[ 6] = AfxGetApp()->LoadIcon(IDI_ICON07);
	m_hIconNums[ 7] = AfxGetApp()->LoadIcon(IDI_ICON08);
	m_hIconNums[ 8] = AfxGetApp()->LoadIcon(IDI_ICON09);
	m_hIconNums[ 9] = AfxGetApp()->LoadIcon(IDI_ICON10);
	m_hIconNums[10] = AfxGetApp()->LoadIcon(IDI_ICON11);
	m_hIconNums[11] = AfxGetApp()->LoadIcon(IDI_ICON12);
	m_hIconNums[12] = AfxGetApp()->LoadIcon(IDI_ICON13);
	g_lstSolution = &lstSolution;
	set_solution_output_func(solution_output);
	set_target_number(result);
}

void Ccount24Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUM1, num1);
	DDX_Text(pDX, IDC_EDIT_NUM2, num2);
	DDX_Text(pDX, IDC_EDIT_NUM3, num3);
	DDX_Text(pDX, IDC_EDIT_NUM4, num4);
	DDX_Text(pDX, IDC_EDIT_RESULT, result);
	DDX_Control(pDX, IDC_BTN_CARD1, btnCard1);
	DDX_Control(pDX, IDC_BTN_CARD2, btnCard2);
	DDX_Control(pDX, IDC_BTN_CARD3, btnCard3);
	DDX_Control(pDX, IDC_BTN_CARD4, btnCard4);
	DDX_Control(pDX, IDC_LIST_SOLUTION, lstSolution);
}

BEGIN_MESSAGE_MAP(Ccount24Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &Ccount24Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_CARD1, &Ccount24Dlg::OnBnClickedBtnCard1)
	ON_BN_CLICKED(IDC_BTN_CARD2, &Ccount24Dlg::OnBnClickedBtnCard2)
	ON_BN_CLICKED(IDC_BTN_CARD3, &Ccount24Dlg::OnBnClickedBtnCard3)
	ON_BN_CLICKED(IDC_BTN_CARD4, &Ccount24Dlg::OnBnClickedBtnCard4)
END_MESSAGE_MAP()


// Ccount24Dlg message handlers

BOOL Ccount24Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	btnCard1.SetIcon(m_hIconNums[num1-MIN_NUMBER]);
	btnCard2.SetIcon(m_hIconNums[num2-MIN_NUMBER]);
	btnCard3.SetIcon(m_hIconNums[num3-MIN_NUMBER]);
	btnCard4.SetIcon(m_hIconNums[num4-MIN_NUMBER]);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Ccount24Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Ccount24Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Ccount24Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Ccount24Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	struct number num[NUM_NUMBER];
	init_number(&num[0], num1);
	init_number(&num[1], num2);
	init_number(&num[2], num3);
	init_number(&num[3], num4);
	lstSolution.ResetContent();
	if (!count(NUM_NUMBER, num)) lstSolution.AddString(_T("No solution."));
	// OnOK();
}

void Ccount24Dlg::OnBnClickedBtnCard1()
{
	// TODO: Add your control notification handler code here
	num1++;
	if (num1 > MAX_NUMBER) num1 = MIN_NUMBER;
	btnCard1.SetIcon(m_hIconNums[num1-MIN_NUMBER]);
	UpdateData(FALSE);
}

void Ccount24Dlg::OnBnClickedBtnCard2()
{
	// TODO: Add your control notification handler code here
	num2++;
	if (num2 > MAX_NUMBER) num2 = MIN_NUMBER;
	btnCard2.SetIcon(m_hIconNums[num2-MIN_NUMBER]);
	UpdateData(FALSE);
}

void Ccount24Dlg::OnBnClickedBtnCard3()
{
	// TODO: Add your control notification handler code here
	num3++;
	if (num3 > MAX_NUMBER) num3 = MIN_NUMBER;
	btnCard3.SetIcon(m_hIconNums[num3-MIN_NUMBER]);
	UpdateData(FALSE);
}

void Ccount24Dlg::OnBnClickedBtnCard4()
{
	// TODO: Add your control notification handler code here
	num4++;
	if (num4 > MAX_NUMBER) num4 = MIN_NUMBER;
	btnCard4.SetIcon(m_hIconNums[num4-MIN_NUMBER]);
	UpdateData(FALSE);
}
