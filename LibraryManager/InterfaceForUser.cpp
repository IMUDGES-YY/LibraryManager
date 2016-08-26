// InterfaceForUser.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LibraryManager.h"
#include "InterfaceForUser.h"
#include "afxdialogex.h"


// InterfaceForUser �Ի���

IMPLEMENT_DYNAMIC(InterfaceForUser, CDialogEx)

InterfaceForUser::InterfaceForUser(CWnd* pParent /*=NULL*/)
	: CDialogEx(InterfaceForUser::IDD, pParent)
	, edit_search(_T(""))


	, select_type(-1)

	, book_number(_T(""))
	, book_name(_T(""))
	, book_author(_T(""))
	, book_press(_T(""))
	, book_date(_T(""))
	, book_type(_T(""))
	, book_about(_T(""))
	, book_ISBN(_T(""))
	, name_text(_T(""))
	, type_text(_T(""))
	, username_text(_T(""))
{
}

InterfaceForUser::~InterfaceForUser()
{
}

void InterfaceForUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, edit_search);
	DDX_Radio(pDX, IDC_RADIO1, select_type);

	DDX_Control(pDX, IDC_LIST1, select_list_box);
	DDX_Text(pDX, IDC_BOOKNUMBER, book_number);
	DDX_Control(pDX, IDOK, control_search_button);
	DDX_Control(pDX, IDC_BOOKNUMBER, control_book_number);
	DDX_Text(pDX, IDC_BOOKNAMETEXT, book_name);
	DDX_Text(pDX, IDC_BOOKAUTHERTEXT, book_author);
	DDX_Text(pDX, IDC_BOOKPRESSTEXT, book_press);
	DDX_Text(pDX, IDC_BOOKDATETEXT, book_date);
	DDX_Text(pDX, IDC_BOOKTYPETEXT, book_type);
	DDX_Text(pDX, IDC_ABOUTBOOKTEXT, book_about);
	DDX_Control(pDX, IDC_BOOKNAMETEXT, control_book_name);
	DDX_Control(pDX, IDC_BOOKAUTHERTEXT, control_book_author);
	DDX_Control(pDX, IDC_BOOKPRESSTEXT, control_book_press);
	DDX_Control(pDX, IDC_BOOKDATETEXT, control_book_date);
	DDX_Control(pDX, IDC_BOOKTYPETEXT, control_book_type);
	DDX_Control(pDX, IDC_ABOUTBOOKTEXT, control_book_about);
	DDX_Text(pDX, IDC_BOOKISBNTEXT, book_ISBN);
	DDX_Control(pDX, IDC_BOOKISBNTEXT, control_book_ISBN);
	DDX_Control(pDX, IDC_USERNAME, control_name);
	DDX_Control(pDX, IDC_USERTYPE, control_type);
	DDX_Control(pDX, IDC_USERUSERNAME, control_username);
	DDX_Text(pDX, IDC_USERNAME, name_text);
	DDX_Text(pDX, IDC_USERTYPE, type_text);
	DDX_Text(pDX, IDC_USERUSERNAME, username_text);
}


BEGIN_MESSAGE_MAP(InterfaceForUser, CDialogEx)

	ON_BN_CLICKED(IDOK, &InterfaceForUser::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &InterfaceForUser::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &InterfaceForUser::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &InterfaceForUser::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &InterfaceForUser::OnBnClickedRadio4)
	ON_EN_CHANGE(IDC_EDIT1, &InterfaceForUser::OnEnChangeEdit1)
	ON_LBN_SELCHANGE(IDC_LIST1, &InterfaceForUser::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON1, &InterfaceForUser::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &InterfaceForUser::OnBnClickedButton2)
END_MESSAGE_MAP()


// InterfaceForUser ��Ϣ�������




void InterfaceForUser::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	CString sql_select;
	
	//ͼ����Ϣlist,��ѡ�е�Node �ÿգ�
	select_list_box.ResetContent();
	thisNode=NULL;
	if(select_type==-1)
	{
		all_book_number=0;
		CString cstr=transformPlus.toCString(all_book_number);
		book_number=L"0";
		UpdateData(FALSE);
		AfxMessageBox(_T("��ѡ��������ʽ��"));
		return ;
	}
	if(select_type==1)
	{
		//UpdateData(TRUE);
		//ÿ������ʱ��Ҫ�������
		list.clearList();
		sql_select.Format(_T("select * from book where bookName like \'%s\';"),  edit_search);
		string sql_Select=transformPlus.toString(sql_select);
		const char  * sql=sql_Select.c_str();
		int res=mysql_query(&local_mysql,sql);
		MYSQL_RES * result;
		MYSQL_ROW row;
		if(res==0)
		{
			result=mysql_store_result(&local_mysql);
			while(row=mysql_fetch_row(result))
			{
				if(row)
				{
					string bookName=row[3];
					string bookISBN=row[7];
					CString cstr=transformPlus.toCString(bookName);
					int judge=select_list_box.InsertString(-1,cstr);


					//��ͼ�����ݴ���list
					if(judge>=0)
					{
						Book * newNode=new Book(bookISBN,judge);
						list.add(newNode);
						list.p->next=NULL;
					}
					else
					{
						AfxMessageBox(_T("Out of memory!"));
					}
				}
				else
				{
					AfxMessageBox(L"error");
				}
			}
			all_book_number=select_list_box.GetCount();
			CString cstr=transformPlus.toCString(all_book_number);
			if (all_book_number==0)
			{
				book_number=L"";
				AfxMessageBox(_T("û�в鵽����Ҫ����"));
				control_book_number.SetWindowText(L"0");
				//UpdateData(FALSE);
			}
			else if(all_book_number>0)
			{
				book_number.Format(_T("%s"),cstr);
				select_type=-1;
				UpdateData(FALSE);
				return ;
			}
			else
			{
				book_number=L"";
				return ;
			}
		}
		else
		{
			AfxMessageBox(_T("δ��ѯ�������"));
			control_book_number.SetWindowText(L"0");
			return ;
		}
		return ;
	}
	else if(select_type==2)
	{
		UpdateData(TRUE);
		//ÿ������ʱ��Ҫ�������
		list.clearList();
		sql_select.Format(_T("select * from book where ISBN = %s;"),  edit_search);
		string sql_Select=transformPlus.toString(sql_select);
		const char  * sql=sql_Select.c_str();
		int res=mysql_query(&local_mysql,sql);
		MYSQL_RES * result;
		MYSQL_ROW row;
		if(res==0)
		{
			result=mysql_store_result(&local_mysql);
			while(row=mysql_fetch_row(result))
			{
				if(row)
				{
					string bookName=row[3];
					string bookISBN=row[7];
					CString cstr=transformPlus.toCString(bookName);
					int judge=select_list_box.InsertString(-1,cstr);


					//��ͼ�����ݴ���list
					if(judge>=0)
					{
						Book * newNode=new Book(bookISBN,judge);
						list.add(newNode);
						list.p->next=NULL;
					}
					else
					{
						AfxMessageBox(_T("Out of memory!"));
					}
				}
				else
				{
					AfxMessageBox(L"error");
					control_book_number.SetWindowText(L"0");
				}
			}
			all_book_number=select_list_box.GetCount();
			CString cstr=transformPlus.toCString(all_book_number);
			if (all_book_number==0)
			{
				book_number=L"";
				AfxMessageBox(_T("û�в鵽����Ҫ����"));
				control_book_number.SetWindowText(L"0");
				//UpdateData(FALSE);
			}
			else if(all_book_number>0)
			{
				book_number.Format(_T("%s"),cstr);
				select_type=-1;
				UpdateData(FALSE);
				return ;
			}
			else
			{
				book_number=L"";
				return ;
			}
		}
		else
		{
			AfxMessageBox(_T("δ��ѯ�������"));
			control_book_number.SetWindowText(L"0");
			return ;
		}
		return ;

	}
	else if(select_type==3)
	{
		UpdateData(TRUE);
		//ÿ������ʱ��Ҫ�������
		list.clearList();
		sql_select.Format(_T("select * from book where type = \'%s\';"),  edit_search);
		string sql_Select=transformPlus.toString(sql_select);
		const char  * sql=sql_Select.c_str();
		int res=mysql_query(&local_mysql,sql);
		MYSQL_RES * result;
		MYSQL_ROW row;
		if(res==0)
		{
			result=mysql_store_result(&local_mysql);
			while(row=mysql_fetch_row(result))
			{
				if(row)
				{
					string bookName=row[3];
					string bookISBN=row[7];
					CString cstr=transformPlus.toCString(bookName);
					int judge=select_list_box.InsertString(-1,cstr);


					//��ͼ�����ݴ���list
					if(judge>=0)
					{
						Book * newNode=new Book(bookISBN,judge);
						list.add(newNode);
						list.p->next=NULL;
					}
					else
					{
						AfxMessageBox(_T("Out of memory!"));
					}
				}
				else
				{
					AfxMessageBox(L"error");
				}
			}
			all_book_number=select_list_box.GetCount();
			CString cstr=transformPlus.toCString(all_book_number);
			if (all_book_number==0)
			{
				book_number=L"";
				AfxMessageBox(_T("û�в鵽����Ҫ����"));
				control_book_number.SetWindowText(L"0");
				//UpdateData(FALSE);
			}
			else if(all_book_number>0)
			{
				book_number.Format(_T("%s"),cstr);
				select_type=-1;
				UpdateData(FALSE);
				return ;
			}
			else
			{
				book_number=L"";
				return ;
			}
		}
		else
		{
			AfxMessageBox(_T("δ��ѯ�������"));
			control_book_number.SetWindowText(L"0");
			return ;
		}
		return ;
	}
	else if(select_type==4)
	{
		//ÿ������ʱ��Ҫ�������
		list.clearList();
		sql_select=("select * from book where bookNum>1;");
		string sql_Select=transformPlus.toString(sql_select);
		const char  * sql=sql_Select.c_str();
		int res=mysql_query(&local_mysql,sql);
		MYSQL_RES * result;
		MYSQL_ROW row;
		if(res==0)
		{
			result=mysql_store_result(&local_mysql);
			while(row=mysql_fetch_row(result))
			{
				if(row)
				{
					/*
					���ݿ�����������Ϊutf8
					�ڴ˴����ֿ���������ʾ
					AddString(LPCTSTR);Unicode����
					CString Ҳ��Unicode����
					*/
					
					string bookName=row[3];
					string bookISBN=row[7];
					CString cstr=transformPlus.toCString(bookName);
					int judge=select_list_box.InsertString(-1,cstr);


					//��ͼ�����ݴ���list
					if(judge>=0)
					{
						Book * newNode=new Book(bookISBN,judge);
						list.add(newNode);
						list.p->next=NULL;
					}
					else
					{
						AfxMessageBox(_T("Out of memory!"));
					}
				}
				else
				{
					AfxMessageBox(L"error");
				}
			}
			all_book_number=select_list_box.GetCount();
			CString cstr=transformPlus.toCString(all_book_number);
			
			if (all_book_number==0)
			{
				book_number=L"0";
				AfxMessageBox(_T("û�в鵽����Ҫ����"));
				control_book_number.SetWindowText(L"0");
			}
			else if(all_book_number>0)
			{
				book_number.Format(_T("%s"),cstr);
				select_type=-1;
				UpdateData(FALSE);
				return ;
			}
			else
			{
				book_number=L"";
				UpdateData(FALSE);
				return ;
			}
			
		}
		else
		{
			AfxMessageBox(_T("δ��ѯ�������"));
			control_book_number.SetWindowText(L"0");
			return ;
		}
	}
	//CDialogEx::OnOK();
}


void InterfaceForUser::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(edit_search=="")
	{
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		select_type=1;//�鼮��
		control_search_button.EnableWindow(FALSE);
		//UpdateData(FALSE);
		return ;
	}
	else
	{
		control_search_button.EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		select_type=1;//�鼮��0
		//UpdateData(FALSE);
		return ;
	}
}


void InterfaceForUser::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(edit_search=="")
	{
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		select_type=2;//�鼮��
		control_search_button.EnableWindow(FALSE);
	}
	else
	{
		control_search_button.EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		select_type=2;//�鼮��
		return ;
	}
}


void InterfaceForUser::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(edit_search=="")
	{
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		select_type=3;//�鼮��
		control_search_button.EnableWindow(FALSE);
	}
	else
	{
		control_search_button.EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		select_type=3;//�鼮��
		return ;
	}
}


void InterfaceForUser::OnBnClickedRadio4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	select_type=4;//�ɽ��ѯ
	return ;
}





void InterfaceForUser::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	//UpdateData(FALSE);
	if (select_type==-1)
	{
		control_search_button.EnableWindow(FALSE);
		return ;
	}
	else
	{
		control_search_button.EnableWindow(TRUE);
	}
	//UpdateData(TRUE);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

//��ʾͼ����Ϣ
void InterfaceForUser::OnLbnSelchangeList1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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


	CString cstrText;
	int selectedTextNumber;
	selectedTextNumber=select_list_box.GetCurSel();//��ȡ��ǰѡ���б���
	/*
	����selectTextNumber ����list �ж�Ӧ�Ľڵ㣬���Ҷ�Ӧ��isbn ���Ҷ�Ӧ���鼮��������Ϣ
	*/
	Book *head=list.head;
	
	if(head==NULL)
	{
		AfxMessageBox(_T("error!"));
		return ;
	}
	else
	{
		while(head)
		{
			if(head->reListPosition()==selectedTextNumber)
			{
				thisNode=head;
				break;
			}
			head=head->next;
		}
	}
	CString sql_select;
	sql_select.Format(_T("select * from book where ISBN =\'%s\'"),transformPlus.toCString(thisNode->reISBN()));
	string sql_Select=transformPlus.toString(sql_select);
	const char  * sql=sql_Select.c_str();
	int res=mysql_query(&local_mysql,sql);
	MYSQL_RES * result;
	MYSQL_ROW row;
	if(res==0)
	{
		result=mysql_store_result(&local_mysql);
		row=mysql_fetch_row(result);
		book_name=row[3];
		book_author=row[4];
		book_press=row[5];
		book_date=row[6];
		book_ISBN=row[7];
		book_type=row[2];
		book_about=row[10];
		control_book_name.SetWindowText(book_name);
		control_book_author.SetWindowText(book_author);
		control_book_press.SetWindowText(book_press);
		control_book_date.SetWindowText(book_date);
		control_book_ISBN.SetWindowText(book_ISBN);
		control_book_type.SetWindowText(book_type);
		control_book_about.SetWindowText(book_about);
		return ;
	}
	else
	{
		AfxMessageBox(_T("error!"));
		return ;
	}
	/*
	select_list_box.GetText(selectedTextNumber,cstrText);//��ȡ��ǰѡ���ַ��� ��ֵ��cstrText
	book_name=cstrText;
	control_book_name.SetWindowText(book_name);
	*/
}

void InterfaceForUser::borrowBook(string userType,string userBookNumber,MYSQL local_mysql)
{
	string staticNumber;
	if(userType=="1")
	{
		staticNumber="4";
	}
	else if(userType=="2")
	{
		staticNumber="6";
	}
	else if(userType=="3")
	{
		staticNumber="10";
	}
	else if(userType=="4")
	{
		staticNumber="999999";
	}
	if(userBookNumber>=staticNumber)
	{
		MessageBox(L"�������Ѵ����ޣ���黹�����鼮����н��ģ�");
		return ;
	}
	else
	{
		//�ж��鼮����
		MYSQL_ROW Row;
		MYSQL_ROW row;
		MYSQL_RES *result;
		CString sql_query;
		sql_query.Format(_T("select * from book where ISBN=%s ;"),transformPlus.toCString(thisNode->reISBN()));
		string sql_Query=transformPlus.toString(sql_query);
		const char  * SQL=sql_Query.c_str();
		int Res=mysql_query(&local_mysql,SQL);
		if(Res==0)
		{
			result=mysql_store_result(&local_mysql);
			Row=mysql_fetch_row(result);
			if(Row)
			{

				if(transformPlus.toInt(Row[1])>1)
				{	
					CString sql_query;
					sql_query.Format(_T("select * from user_book where username= \'%s\';"),loginUser);
					sql_Query=transformPlus.toString(sql_query);
					SQL=sql_Query.c_str();
					Res=mysql_query(&local_mysql,SQL);
					if(Res==0)
					{
						int judge=0;
						result=mysql_use_result(&local_mysql);
						while(row=mysql_fetch_row(result))
						{
							if(row)
							{
								if(transformPlus.toString(row[2])==thisNode->reISBN())
								{
									judge=1;
									break ;
								}
							}
						}
						if(judge==1)
						{
							MessageBox(L"���ѽ��Ĵ�ͼ�飬�����ظ����ģ�");
							return ;
						}
						else
						{
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
							//Ӧ��ʱ��reTime
							time_t now_time=time(0);
							CString cstrNowTime;
							cstrNowTime.Format(_T("%d"),now_time);
							long time_long=transformPlus.toLong(cstrNowTime);
							time_long+=2592000;
							CString reTime;
							reTime=transformPlus.toCString(time_long);
							//����
							CString sql_insert;
							sql_insert.Format(_T("insert into user_book values (\'\',\'%s\',\'%s\',\'%s\');"),loginUser,transformPlus.toCString(thisNode->reISBN()),reTime);
							string sql_Insert=transformPlus.toString(sql_insert);
							const char  * SQL_insert=sql_Insert.c_str();
							if(mysql_query(&local_mysql,SQL_insert)==0)
							{   
								int number=0;
								MYSQL_RES * Result;
								MYSQL_ROW ROW;
								CString sql_search;
								sql_search.Format(_T("select * from user_book where username = \'%s\';"),loginUser);
								string sql_Search=transformPlus.toString(sql_search);
								const char * search=sql_Search.c_str();
								if(mysql_query(&local_mysql,search)==0)
								{
									Result=mysql_store_result(&local_mysql);
									while(ROW=mysql_fetch_row(Result))
									{
										if(ROW)
										{
											number++;
										}
									}
								}
								else
								{
									AfxMessageBox(_T("error313131312!"));
									return ;
								}
								CString sql_update;
								sql_update.Format(_T("update user set bookNumber =\'%s\'  where username =\'%s\'; "),transformPlus.toCString(number),loginUser);
								string sql_Update=transformPlus.toString(sql_update);
								const char * update=sql_Update.c_str(); 
								if(mysql_query(&local_mysql,update)==0)
								{
									MessageBox(_T("���ĳɹ���"));   
									return ;
								}
								else
								{
									AfxMessageBox(_T("����ʧ�ܣ�"));
									return ;
								}
							}
							else
							{
								AfxMessageBox(_T("����ʧ�ܣ�"));
								return ;
							}
							return ;
						}
					}
					else
					{
						AfxMessageBox(_T("error!!!!!!!!!!!"));
						return ;
					}

					return ;
				}
				else
				{	//�鼮��������
					MessageBox(L"���鼮�ݲ������㣡");
					return ;
				}
			}
			else
			{
				AfxMessageBox(_T("errrrrrror"));
				return ;
			}
		}
		else
		{
			AfxMessageBox(_T("query error!"));
			return ;
		}
	}
}

//�����鼮
/*

0.�жϸ��˿ɽ��鼮�����Ƿ�ɽ�
2.�ж��鼮�����Ƿ����1
3.�����Ӧ��
4.�����鼮������һ


*/
void InterfaceForUser::OnBnClickedButton1()
{
	//ѡ���鼮
	if(thisNode!=NULL)
	{
		//�жϱ����Ƿ����Ȿ��
		
		//�������ݿ�
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
		CString sql_select;

		//��ѯ
		sql_select.Format(_T("select * from user where username = \'%s\'"),loginUser);
		string sql_Select=transformPlus.toString(sql_select);
		const char  * sql=sql_Select.c_str();
		int res=mysql_query(&local_mysql,sql);
		MYSQL_RES * result;
		MYSQL_ROW row;
		if(res==0)
		{
			result=mysql_store_result(&local_mysql);
			row=mysql_fetch_row(result);
			if(row)
			{
				
				string userType=row[5];
				string userBookNumber=row[6];
				//������
				if(userType=="1")
				{
					borrowBook(userType,userBookNumber,local_mysql);
				}
				//�о���
				else if(userType=="2")
				{
					borrowBook(userType,userBookNumber,local_mysql);
				}
				//��ʿ��
				else if(userType=="3")
				{
					borrowBook(userType,userBookNumber,local_mysql);
				}
				//��ʦ
				else if(userType=="4")
				{
					borrowBook(userType,userBookNumber,local_mysql);
				}
				else
				{
					AfxMessageBox(_T("error type!"));
					return ;
				}
			}
			else
			{
				AfxMessageBox(_T("error!"));
				return ;
			}
		}
		else
		{
			AfxMessageBox(_T("error!"));
			return ;
		}
	}
	//δѡ���鼮
	else
	{
		MessageBox(L"��δѡ���鼮��");
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}



void InterfaceForUser::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	select_list_box.ResetContent();
	control_book_name.ShowWindow(SW_HIDE);
	control_book_author.ShowWindow(SW_HIDE);
	control_book_press.ShowWindow(SW_HIDE);
	control_book_date.ShowWindow(SW_HIDE);
	control_book_ISBN.ShowWindow(SW_HIDE);
	control_book_type.ShowWindow(SW_HIDE);
	control_book_about.ShowWindow(SW_HIDE);
	control_book_number.ShowWindow(SW_HIDE);
}


BOOL InterfaceForUser::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	control_type.SetWindowText(transformPlus.toCString(TYPE));
	control_type.ShowWindow(TRUE);
	control_name.SetWindowText(transformPlus.toCString(name));
	control_name.ShowWindow(TRUE);
	control_username.SetWindowText(transformPlus.toCString(username));
	control_username.ShowWindow(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
