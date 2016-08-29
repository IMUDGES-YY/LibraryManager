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
	, edit_text(_T(""))
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
	DDX_Control(pDX, IDC_BUTTON2, control_button2);
	DDX_Control(pDX, IDC_BUTTON3, control_button3);
	DDX_Text(pDX, IDC_EDIT1, edit_text);
	DDX_Control(pDX, IDC_RADIO1, control_search_type);
}


BEGIN_MESSAGE_MAP(Manager, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &Manager::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &Manager::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO5, &Manager::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO3, &Manager::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &Manager::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO6, &Manager::OnBnClickedRadio6)
	ON_BN_CLICKED(IDOK, &Manager::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO7, &Manager::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_BUTTON3, &Manager::OnBnClickedButton3)
	ON_LBN_SELCHANGE(IDC_LIST1, &Manager::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON2, &Manager::OnBnClickedButton2)
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
	control_button2.SetWindowText(L"׼�����");
	control_button2.ShowWindow(SW_SHOW);
	control_button3.SetWindowText(L"��ֹ����");
	control_button3.ShowWindow(SW_SHOW);
	search_type=1;

	//insertAppointmentMessageToListBox();
}
void Manager::insertAppointmentMessageToListBox()
{
	appThisNode=NULL;
	control_list_box.ResetContent();
	appointmentList.clearList();
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
		if(appointmentList.head==NULL)
		{
			MessageBox(L"��ʱû��ԤԼ��Ϣ��");
			return ;
		}
	}
	else
	{
		AfxMessageBox(_T("error1"));
		return ;
	}
}
int Manager::addMistake(CString username)
{
	CString sql_select;
	sql_select.Format(_T("select * from user where username = \'%s\';"),username);
	string temp=transformPlus.toString(sql_select);
	const char * sql=temp.c_str();
	MYSQL_RES *res;
	MYSQL_ROW row ;
	if(mysql_query(&local_mysql,sql)==0)
	{
		res=mysql_store_result(&local_mysql);
		row=mysql_fetch_row(res);
		if(row)
		{
			int number =transformPlus.toInt(row[7]);
			number ++;
			CString sql_update;
			sql_update.Format(_T("update user set mistake = \'%s\' where username = \'%s\' ;"),transformPlus.toCString(number),username);
			temp=transformPlus.toString(sql_update);
			sql=temp.c_str();
			if(mysql_query(&local_mysql,sql)==0)
			{
				return 1;
			}
			else
			{
				AfxMessageBox(_T("error!"));
				return 0;
			}
		}
		else
		{
			AfxMessageBox(_T("error!"));
			return 0;
		}
	}
	else
	{
		AfxMessageBox(_T("error;"));
		return 0;
	}
}
int Manager::delAppointmentMessage(CString username ,CString bookISBN)
{
	CString sql_query;
	sql_query.Format(_T("delete from appointment_book where username=\'%s\' and bookISBN=\'%s\';"),username,bookISBN);
	temp=transformPlus.toString(sql_query);
	sql=temp.c_str();
	if(mysql_query(&local_mysql,sql)==0)
	{
		return 1;
	}
	else
	{

		return 0;
	}
}
int Manager::sendMessageToUser(CString username,CString message)
{
	CString sql_update;
	sql_update.Format(_T("update user set message=\'%s\' where username=\'%s\' ;"),message,username);
	temp=transformPlus.toString(sql_update);
	sql=temp.c_str();
	if(mysql_query(&local_mysql,sql)==0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void Manager::acceptBorrowBook()
{
	if(appThisNode==NULL)
	{
		MessageBox(L"����δѡ����Ϣ");
		return ;
	}
	else
	{
		//�жϴ�ʱ�Ƿ���ԤԼʱ��֮��
		time_t nowTime=time(0);
		CString cstrNowTime;
		cstrNowTime.Format(_T("%d"),nowTime);
		if(cstrNowTime>appThisNode->reTime())
		{
			//����
			MessageBox(L"ԤԼʱ���ѹ���");
			int judge=delAppointmentMessage(appThisNode->reUsername(),appThisNode->reISBN());
			if(judge==1)
			{
				MessageBox(L"��ѧ��ԤԼ��Ϣ��ɾ��");
				return ;
			}
			else
			{
				MessageBox(L"��ѧ��ԤԼ��Ϣɾ��ʧ��");
				return ;
			}
		}
		else
		{
			//׼��
			time_t nowTime=time(0);
			CString cstrNowTime;
			cstrNowTime.Format(_T("%d"),nowTime);
			long time_long=transformPlus.toLong(cstrNowTime);
			time_long+=2592000;
			CString reTime;
			reTime=transformPlus.toCString(time_long);

			CString sql_query;
			sql_query.Format(_T("insert into user_book values (\'\',\'%s\',\'%s\',\'%s\');"),appThisNode->reUsername(),appThisNode->reISBN(),reTime);
			temp=transformPlus.toString(sql_query);
			sql=temp.c_str();
			if(mysql_query(&local_mysql,sql)==0)
			{
				MessageBox(L"��׼�ɹ���");
				int judge=delAppointmentMessage(appThisNode->reUsername(),appThisNode->reISBN());
				if(judge==1)
				{
					MessageBox(L"��ѧ��ԤԼ��Ϣ��ɾ��");
					return ;
				}
				else
				{
					MessageBox(L"��ѧ��ԤԼ��Ϣɾ��ʧ��");
					return ;
				}
			}
			else
			{
				MessageBox(L"��׼ʧ�ܣ�");
				return ;
			}
		}
	}
}


//�������
void Manager::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	control_edit.EnableWindow(TRUE);
	control_button2.SetWindowText(L"��������");
	control_button2.ShowWindow(SW_SHOW);
	control_button3.SetWindowText(L"�˳�");
	control_button3.ShowWindow(SW_SHOW);


	MessageBox(L"��������鼮ISBN");
	search_type=2;
}
void Manager::insertUserHaveTheBookToListBox()
{
	mUThisNode=NULL;
	managerUserList.clearList();
	appointmentList.clearList();
	connectMysql();
	CString sql_query;
	sql_query.Format(_T("select * from user_book where bookISBN = \'%s\';"),edit_text);
	temp=transformPlus.toString(sql_query);
	sql=temp.c_str();
	if(mysql_query(&local_mysql,sql)==0)
	{
		res=mysql_store_result(&local_mysql);
		while(row=mysql_fetch_row(res))
		{
			int judge=control_list_box.InsertString(-1,transformPlus.toCString(row[1]));
			managerUser * newNode=new managerUser(transformPlus.toCString(row[1]),transformPlus.toCString(judge));
			managerUserList.add(newNode);
			managerUserList.p->next=NULL;
		}
		if(managerUserList.head==NULL)
		{
			MessageBox(L"���鼮��δ�������");
			return ;
		}
	}
	else
	{
		AfxMessageBox(_T("error!"));
		return ;
	}
}

//���˲���
void Manager::OnBnClickedRadio5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	control_edit.EnableWindow(TRUE);
	control_button2.SetWindowText(L"��������");
	control_button2.ShowWindow(SW_SHOW);
	control_button3.SetWindowText(L"�˳�");
	control_button3.ShowWindow(SW_SHOW);


	MessageBox(L"��������Ҫ��ѯѧ����ѧ��");
	search_type=5;
} 
void Manager::insertUserBookListToListBox()
{
	UpdateData(TRUE);
	userBookList.clearList();
	connectMysql();
	CString sql_query;
	sql_query.Format(_T("select * from user_book where username = \'%s\';"),edit_text);
	temp=transformPlus.toString(sql_query);
	sql=temp.c_str();
	if(mysql_query(&local_mysql,sql)==0)
	{
		res=mysql_store_result(&local_mysql);
		while(row=mysql_fetch_row(res))
		{
			int judge=control_list_box.InsertString(-1,transformPlus.toCString(row[2]));

			Book * newNode=new Book(transformPlus.toString(row[2]),judge);
			userBookList.add(newNode);
			userBookList.p->next=NULL;
		}
		if(userBookList.head==NULL)
		{
			MessageBox(L"��ͬѧû�н�����Ϣ��");
			return ;
		}
	}
	else
	{
		AfxMessageBox(_T("error!"));
		return ;
	}
}


//�鿴ΥԼ���
void Manager::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	control_edit.EnableWindow(FALSE);
	control_button2.SetWindowText(L"�û�����");
	control_button2.ShowWindow(SW_SHOW);
	control_button3.SetWindowText(L"�˳�");
	control_button3.ShowWindow(SW_SHOW);
	search_type=3;
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
		if(BadGuyList.head==NULL)
		{
			MessageBox(L"��ʱû��ԤԼ��Ϣ��");
			return ;
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
	control_button2.SetWindowText(L"�鼮����");
	control_button2.ShowWindow(SW_SHOW);
	control_button3.SetWindowText(L"ɾ���鼮");
	control_button3.ShowWindow(SW_SHOW);
	search_type=4;
	//insertAllBookToListBox();
}
void Manager::insertAllBookToListBox()
{
	managerBookList.clearList();
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
		if(managerBookList.head==NULL)
		{
			AfxMessageBox(_T("δ�鵽�鼮"));
			return ;
		}
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
	control_button2.SetWindowText(L"�û�����");
	control_button2.ShowWindow(SW_SHOW);
	control_button3.SetWindowText(L"�˳�");
	control_button3.ShowWindow(SW_SHOW);
	search_type=6;;
	//insertAllUserToListBox();
}
void Manager::insertAllUserToListBox()
{
	managerUserList.clearList();
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
		if(managerUserList.head==NULL)
		{
			AfxMessageBox(_T("δ�鵽����"));
			return ;
		}
	}
	else
	{
		AfxMessageBox(_T("error!"));
		return ;
	}
}

//�����û� ͨ��username
void Manager::OnBnClickedRadio7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBox(L"�������û���");
	control_edit.EnableWindow(TRUE);
	control_button2.SetWindowText(L"�û�����");
	control_button2.ShowWindow(SW_SHOW);
	control_button3.SetWindowText(L"ɾ���û�");
	control_button3.ShowWindow(SW_SHOW);
	search_type=7;
}
void Manager::insertUserToListBox()
{
	connectMysql();
	managerUserList.clearList();
	CString sql_query;
	sql_query.Format(_T("select * from user where username = \'%s\' ;"),edit_text);
	temp=transformPlus.toString(sql_query);
	sql=temp.c_str();
	if (mysql_query(&local_mysql,sql)==0)
	{
		res=mysql_store_result(&local_mysql);
		row=mysql_fetch_row(res);
		if(row)
		{
			int judge=control_list_box.InsertString(-1,transformPlus.toCString(row[1]));

			managerUser * newNode=new managerUser(transformPlus.toCString(row[1]),transformPlus.toCString(row[3]),transformPlus.toCString(judge));
			managerUserList.add(newNode);
			managerUserList.p->next=NULL;
			if(managerUserList.head==NULL)
			{
				AfxMessageBox(_T("δ�鵽���û�"));
				return ;
			}
			return ;
		}
		else
		{
			if(edit_text=="")
			{
				MessageBox(L"�������û�����");
				return ;
			}
			AfxMessageBox(_T("���޴��ˣ�"));
			return ;
		}
	}
	else
	{
		AfxMessageBox(_T("error"));
		return ;
	}
}


void Manager::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	control_list_box.ResetContent();
	if (search_type==1)
	{
		//�鿴ԤԼ
		insertAppointmentMessageToListBox();
	} 
	else if(search_type==2)
	{
		//�������
		UpdateData(TRUE);
		if(edit_text=="")
		{
			AfxMessageBox(_T("��������������"));
			return ;
		}
		insertUserHaveTheBookToListBox();
	}
	else if(search_type==5)
	{
		//���˲���
		UpdateData(TRUE);
		if(edit_text=="")
		{
			AfxMessageBox(_T("��������������"));
			return ;
		}
		insertUserBookListToListBox();
	}
	else if(search_type==3)
	{
		//�鿴ΥԼ���
		insertBadGuyToListBox();
	}
	else if(search_type==4)
	{
		//�鼮ͳ��
		insertAllBookToListBox();
	}
	else if(search_type==6)
	{
		//�û�ͳ��
		insertAllUserToListBox();
	}
	else if(search_type==7)
	{
		UpdateData(TRUE);
		if(edit_text=="")
		{
			AfxMessageBox(_T("��������������"));
			return ;
		}
		insertUserToListBox();
	}
	else
	{
		AfxMessageBox(_T("error!@"));
		return ;
	}

	//CDialogEx::OnOK();
}

BOOL Manager::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	search_type=-1;
	control_button2.SetWindowText(L"����û�");
	control_button2.ShowWindow(SW_SHOW);
	control_button3.SetWindowText(L"�˳�");
	control_button3.ShowWindow(SW_SHOW);
	userBookList.clearList();
	managerUserList.clearList();
	managerBookList.clearList();
	appointmentList.clearList();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	appThisNode=NULL;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



void Manager::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(search_type==-1)
	{
		this->ShowWindow(SW_HIDE);
		SignIn signIn;
		signIn.DoModal();
		this->ShowWindow(SW_SHOW);
	}
	else if (search_type==1)
	{
		//�鿴ԤԼ
		acceptBorrowBook();
		insertAppointmentMessageToListBox();
		return ;
	} 
	else if(search_type==2)
	{
		//�������
		//��������
	}
	else if(search_type==5)
	{
		//���˲���
	}
	else if(search_type==3)
	{
		//�鿴ΥԼ���

	}
	else if(search_type==4)
	{
		//�鼮ͳ��

	}
	else if(search_type==6)
	{
		//�û�ͳ��

	}
	else if(search_type==7)
	{
		//�û�����
	}
	else
	{
		AfxMessageBox(_T("error!@"));
		return ;
	}
}


void Manager::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(search_type==-1)
	{
		CDialogEx::OnCancel();
	}
	else if (search_type==1)
	{
		//�鿴ԤԼ
		CString message;
		message=L"����ԤԼ��Ϣ�ѱ����أ������½��Ļ�ԤԼ";
		sendMessageToUser(appThisNode->reUsername(),message);
		int judge=delAppointmentMessage(appThisNode->reUsername(),appThisNode->reISBN());
		if(judge==1)
		{
			MessageBox(L"ɾ���ɹ�");
			insertAppointmentMessageToListBox();
			return ;
		}
		else
		{
			MessageBox(L"�����˲���Ԥ֪�Ĵ���");
			return ;
		}
	} 
	else if(search_type==2)
	{
		//�������
		CDialogEx::OnCancel();
	}
	else if(search_type==5)
	{
		//���˲���
	}
	else if(search_type==3)
	{
		//�鿴ΥԼ���

	}
	else if(search_type==4)
	{
		//�鼮ͳ��

	}
	else if(search_type==6)
	{
		//�û�ͳ��

	}
	else if(search_type==7)
	{
		//�û�����
	}
	else
	{
		AfxMessageBox(_T("error!@"));
		return ;
	}
}

//���list box 
void Manager::OnLbnSelchangeList1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cstrText;
	int selectedTextNumber;
	selectedTextNumber=control_list_box.GetCurSel();//��ȡ��ǰ������λ��

	if (search_type==1)
	{
		//�鿴ԤԼ
		appointmentUser *head=appointmentList.head;
		while(head)
		{
			if(head->reListPosition()==transformPlus.toCString(selectedTextNumber))
			{
				appThisNode=head;
				break;
			}
			head=head->next;
		}
	} 
	else if(search_type==2)
	{
		//�������
		managerUser *head=managerUserList.head;
		while(head)
		{
			if(head->rePosition()==transformPlus.toCString(selectedTextNumber))
			{
				mUThisNode=head;
				break;
			}
			head=head->next;
		}
	}
	else if(search_type==5)
	{
		//���˲���
	}
	else if(search_type==3)
	{
		//�鿴ΥԼ���

	}
	else if(search_type==4)
	{
		//�鼮ͳ��

	}
	else if(search_type==6)
	{
		//�û�ͳ��

	}
	else if(search_type==7)
	{
		//�û�����
	}
	else
	{
		AfxMessageBox(_T("error!@"));
		return ;
	}
}


