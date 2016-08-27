
// LibraryManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LibraryManager.h"
#include "LibraryManagerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLibraryManagerDlg �Ի���




CLibraryManagerDlg::CLibraryManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLibraryManagerDlg::IDD, pParent)
	, edit_username(_T(""))
	, edit_password(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLibraryManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, edit_username);
	DDX_Text(pDX, IDC_EDIT2, edit_password);
	DDX_Control(pDX, IDC_EDIT1, control_edit_username);
	DDX_Control(pDX, IDC_EDIT2, control_edit_password);
}

BEGIN_MESSAGE_MAP(CLibraryManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CLibraryManagerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLibraryManagerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CLibraryManagerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CLibraryManagerDlg ��Ϣ�������

BOOL CLibraryManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CLibraryManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLibraryManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLibraryManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLibraryManagerDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(edit_username==""||edit_password=="")
	{
		AfxMessageBox(_T("�û��������벻��Ϊ�գ�"));
		return ;
	}
	MYSQL local_mysql;
	mysql_init(&local_mysql);
	if(!mysql_real_connect(&local_mysql,"127.0.0.1","root","","librarymanager",3306,NULL,0))
	{
		MessageBox(_T("error"));
		AfxMessageBox(_T("connect to databases failed!"));
		return ;
	}
	else
	{
		//AfxMessageBox(_T("connect to database success!"));
		mysql_query(&local_mysql,"set names'gb2312'");
	}
	CString sql_query;
	//����Ǻ��֣���Ҫ���''		
	sql_query.Format(_T("select * from user where username = \'%s\';"),  edit_username);
	string sql_Query=transformPlus.toString(sql_query);
	const char  * sql=sql_Query.c_str();
	MYSQL_RES * result;
	MYSQL_ROW row;
	//����0���ɹ���
	int res;
	res=mysql_query(&local_mysql,sql);
	//mysql_query����ֵ���⣡������������������
	if(res==0)
	{
		//�����������result
		result=mysql_store_result(&local_mysql);
		row=mysql_fetch_row(result);
		//�����ݿ���password ��row[2]������password
		if(!row)
		{
			AfxMessageBox(_T("�˺��������"));
			return ;
		}
		string password=row[2];
		string Password=transformPlus.toString(edit_password);
		string type=row[5];
		if(password!=Password)
		{
			AfxMessageBox(_T("�����������"));
			return ;
		}
		else
		{
			if(type=="0")
			{
				MessageBox(_T("admin"));
				return ;
			}
			else
			{
				//MessageBox(_T("user"));
				InterfaceForUser interfaceForUser;
				interfaceForUser.name=row[3];
				interfaceForUser.username=row[1];
				interfaceForUser.loginUser=edit_username;
				interfaceForUser.loginUserType=transformPlus.toCString(type);
				if(type=="1")
				{
					interfaceForUser.TYPE="������";
				}
				else if(type=="2")
				{
					interfaceForUser.TYPE="�о���";
				}
				else if(type=="3")
				{
					interfaceForUser.TYPE="��ʿ��";
				}
				else if(type=="4")
				{
					interfaceForUser.TYPE="��ʦ";
				}
				this->ShowWindow(SW_HIDE);
				interfaceForUser.DoModal();
				//this->ShowWindow(SW_SHOW);
				//control_edit_username.SetWindowText(_T(""));
				//control_edit_password.SetWindowText(_T(""));
				CLibraryManagerDlg main;
				main.DoModal();

				CDialogEx::OnOK();
				//OnBnClickedOk();
			}
		}
	}
	else
	{
		AfxMessageBox(_T("���޴��ˣ�"));
		return ;
	}
	CDialogEx::OnOK();
}


void CLibraryManagerDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CLibraryManagerDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->ShowWindow(SW_HIDE);
	SignIn signIn;
	signIn.DoModal();
	this->ShowWindow(SW_SHOW);
	//MessageBox(_T("��ת��ע�����"));
}

MYSQL CLibraryManagerDlg::connectMySQL()
{
	MYSQL local_mysql;
	mysql_init(&local_mysql);
	if(!mysql_real_connect(&local_mysql,"127.0.0.1","root","","librarymanager",3306,NULL,0))
	{
		MessageBox(_T("error"));
		AfxMessageBox(_T("connect to databases failed!"));
		AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
	}
	else
	{
		//AfxMessageBox(_T("connect to database success!"));
		mysql_query(&local_mysql,"set names'gb2312'");
		return local_mysql;
	}
}