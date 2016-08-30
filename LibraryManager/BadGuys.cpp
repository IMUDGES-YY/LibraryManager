// BadGuys.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LibraryManager.h"
#include "BadGuys.h"
#include "afxdialogex.h"


// BadGuys �Ի���

IMPLEMENT_DYNAMIC(BadGuys, CDialogEx)

BadGuys::BadGuys(CWnd* pParent /*=NULL*/)
	: CDialogEx(BadGuys::IDD, pParent)
{

}

BadGuys::~BadGuys()
{
}

void BadGuys::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(BadGuys, CDialogEx)
END_MESSAGE_MAP()


// BadGuys ��Ϣ�������


BOOL BadGuys::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	MYSQL local_mysql;
	mysql_init(&local_mysql);
	if(!mysql_real_connect(&local_mysql,"127.0.0.1","root","","librarymanager",3306,NULL,0))
	{
		MessageBox(_T("error"));
		AfxMessageBox(_T("connect to databases failed!"));
	}
	else
	{
		mysql_query(&local_mysql,"set names'gb2312'");
	}
	CString sql_query;
	sql_query.Format(_T("select * from user where username=\'%s\';"),username);
	string temp=transformPlus.toString(sql_query);
	const char * sql=temp.c_str();
	MYSQL_RES	*res;
	MYSQL_ROW row;
	if(mysql_query(&local_mysql,sql)==0)
	{
		res=mysql_store_result(&local_mysql);
		row=mysql_fetch_row(res);
		if(row)
		{
			GetDlgItem(IDC_USERNAME)->SetWindowText(transformPlus.toCString(row[1]));
			GetDlgItem(IDC_USERNAME)->ShowWindow(TRUE);
			GetDlgItem(IDC_NAME)->SetWindowText(transformPlus.toCString(row[3]));
			GetDlgItem(IDC_NAME)->ShowWindow(TRUE);
			GetDlgItem(IDC_CLASS)->SetWindowText(transformPlus.toCString(row[4]));
			GetDlgItem(IDC_CLASS)->ShowWindow(TRUE);
			CString TYPE;
			if(transformPlus.toString(row[5])=="1")
			{
				TYPE="������";
			}
			else if(transformPlus.toString(row[5])=="2")
			{
				TYPE="�о���";
			}
			else if(transformPlus.toString(row[5])=="3")
			{
				TYPE="��ʿ��";
			}
			else if(transformPlus.toString(row[5])=="4")
			{
				TYPE="��ʦ";
			}
			GetDlgItem(IDC_TYPE)->SetWindowText(TYPE);
			GetDlgItem(IDC_TYPE)->ShowWindow(TRUE);
			CString haveBooks;
			haveBooks.Format(_T("%s%s"),transformPlus.toCString(row[6]),transformPlus.toCString("��"));
			GetDlgItem(IDC_HAVEBOOKS)->SetWindowText(haveBooks);
			GetDlgItem(IDC_HAVEBOOKS)->ShowWindow(TRUE);
			CString wrongTimes;
			wrongTimes.Format(_T("%s%s"),transformPlus.toCString(row[7]),transformPlus.toCString("��"));
			GetDlgItem(IDC_WRONGTIMES)->SetWindowText(wrongTimes);
			GetDlgItem(IDC_WRONGTIMES)->ShowWindow(TRUE);

		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
