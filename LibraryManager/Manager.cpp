// Manager.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LibraryManager.h"
#include "Manager.h"
#include "afxdialogex.h"


// Manager �Ի���

IMPLEMENT_DYNAMIC(Manager, CDialogEx)

Manager::Manager(CWnd* pParent /*=NULL*/)
	: CDialogEx(Manager::IDD, pParent)
	, search_type(-1)
	, control_appointment(0)
	, control_book_to_user(0)
	, control_user_to_book(0)
	, control_weiyue(0)
	, control_calculate_book(0)
	, control_calculate_user(0)
{

}

Manager::~Manager()
{
}

void Manager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, control_edit);
	DDX_Control(pDX, IDC_LIST1, control_list_box);
}


BEGIN_MESSAGE_MAP(Manager, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &Manager::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &Manager::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO5, &Manager::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO3, &Manager::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &Manager::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO6, &Manager::OnBnClickedRadio6)
	ON_BN_CLICKED(IDOK, &Manager::OnBnClickedOk)
END_MESSAGE_MAP()

//ͨ�ú���



// Manager ��Ϣ�������
void Manager::connectMysql()
{
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
	}
}


//�鿴ԤԼ
void Manager::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	control_edit.EnableWindow(FALSE);
	search_type=1;
	control_list_box.ResetContent();
	//insertAppointmentMessageToListBox();
}
void Manager::insertAppointmentMessageToListBox()
{
	connectMysql();
	CString sql_query;
	sql_query.Format(_T("select * from appointment_book;"));
	temp=transformPlus.toString(sql_query);
	sql=temp.c_str();
	if(mysql_query(&local_mysql,sql)==0)
	{
		res=mysql_store_result(&local_mysql);
		while(row=mysql_fetch_row(res))
		{
			int judge=control_list_box.InsertString(-1,transformPlus.toCString(row[1]));
			appointmentUser * newNode=new appointmentUser(transformPlus.toCString(row[1]),transformPlus.toCString(row[2]),transformPlus.toCString(row[3]),transformPlus.toCString(judge));
			appointmentList.add(newNode);
			appointmentList.p->next=NULL;
		}
	}
	else
	{
		AfxMessageBox(_T("error1"));
		return ;
	}
}

//�������
void Manager::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	search_type=2;
}

//���˲���
void Manager::OnBnClickedRadio5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//�鿴ΥԼ���
void Manager::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	control_edit.EnableWindow(FALSE);
	search_type=4;
	control_list_box.ResetContent();
	//insertBadGuyToListBox();
}
void Manager::insertBadGuyToListBox()
{
	connectMysql();
	CString sql_query;
	sql_query.Format(_T("select * from user where mistake>0 ;"));
	temp=transformPlus.toString(sql_query);
	sql=temp.c_str();
	if(mysql_query(&local_mysql,sql)==0)
	{
		res=mysql_store_result(&local_mysql);
		while(row=mysql_fetch_row(res))
		{
			int judge=control_list_box.InsertString(-1,transformPlus.toCString(row[1]));
			badGuyList * newNode=new badGuyList(transformPlus.toCString(row[1]),transformPlus.toCString(judge),transformPlus.toCString(row[3]));
			BadGuyList.add(newNode);
			BadGuyList.p->next=NULL;
		}
	}
	else
	{
		AfxMessageBox(_T("error!"));
		return ;
	}
}

//�鼮ͳ��
void Manager::OnBnClickedRadio4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	control_edit.EnableWindow(FALSE);
	search_type=5;
	control_list_box.ResetContent();
	//insertAllBookToListBox();
}
void Manager::insertAllBookToListBox()
{
	allBooksNumber=0;
	connectMysql();
	CString sql_query;
	sql_query.Format(_T("select * from book;"));
	temp=transformPlus.toString(sql_query);
	sql=temp.c_str();
	if(mysql_query(&local_mysql,sql)==0)
	{
		res=mysql_store_result(&local_mysql);
		while(row=mysql_fetch_row(res))
		{
			allBooksNumber++;
			int judge=control_list_box.InsertString(-1,transformPlus.toCString(row[3]));
			managerBook *newNode=new managerBook(transformPlus.toCString(row[3]),transformPlus.toCString(row[7]),transformPlus.toCString(judge));
			managerBookList.add(newNode);
			managerBookList.p->next=NULL;
		}
		///MessageBox(transformPlus.toCString(allBooksNumber));
	}
	else
	{
		AfxMessageBox(_T("error!"));
		return ;
	}
}

//�û�ͳ��
void Manager::OnBnClickedRadio6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	control_edit.EnableWindow(FALSE);
	search_type=6;
	control_list_box.ResetContent();
	//insertAllUserToListBox();
}
void Manager::insertAllUserToListBox()
{
	allUsersNumber=0;
	connectMysql();
	CString sql_query;
	sql_query.Format(_T("select * from user ;"));
	temp=transformPlus.toString(sql_query);
	sql=temp.c_str();
	if(mysql_query(&local_mysql,sql)==0)
	{
		res=mysql_store_result(&local_mysql);
		while(row=mysql_fetch_row(res))
		{
			allUsersNumber++;
			int judge=control_list_box.InsertString(-1,transformPlus.toCString(row[1]));
			managerUser * newNode=new managerUser(transformPlus.toCString(row[1]),transformPlus.toCString(row[3]),transformPlus.toCString(judge));
			managerUserList.add(newNode);
			managerUserList.p->next=NULL;
		}
		//MessageBox(transformPlus.toCString(allUsersNumber));
	}
	else
	{
		AfxMessageBox(_T("error!"));
		return ;
	}
}

void Manager::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (search_type==1)
	{
		//�鿴ԤԼ
		insertAppointmentMessageToListBox();
	} 
	else if(search_type==2)
	{
		//�������
	}
	else if(search_type==3)
	{
		//���˲���
	}
	else if(search_type==4)
	{
		//�鿴ΥԼ���
		insertBadGuyToListBox();
	}
	else if(search_type==5)
	{
		//�鼮ͳ��
		insertAllBookToListBox();
	}
	else if(search_type==6)
	{
		//�û�ͳ��
		insertAllUserToListBox();
	}

	//CDialogEx::OnOK();
}
