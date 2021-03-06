// Manager.cpp : 实现文件
//

#include "stdafx.h"
#include "LibraryManager.h"
#include "Manager.h"
#include "afxdialogex.h"


// Manager 对话框

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
	bGThisNode=NULL;
	appThisNode=NULL;
	mBThisNode=NULL;
	mUThisNode=NULL;
	uBThisNode=NULL;
	fGThisNode=NULL;
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
	DDX_Control(pDX, IDC_LIST3, control_fine_list_box);
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
	ON_LBN_SELCHANGE(IDC_LIST3, &Manager::OnLbnSelchangeList3)
	ON_BN_CLICKED(IDC_BUTTON4, &Manager::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &Manager::OnBnClickedButton1)
	ON_LBN_DBLCLK(IDC_LIST1, &Manager::OnLbnDblclkList1)
END_MESSAGE_MAP()

//通用函数



// Manager 消息处理程序
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


//查看预约
void Manager::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	control_edit.EnableWindow(FALSE);
	control_button2.SetWindowText(L"准许借阅");
	control_button2.ShowWindow(SW_SHOW);
	control_button3.SetWindowText(L"禁止借阅");
	control_button3.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT1)->SetWindowText((transformPlus.toCString("")));
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
			MessageBox(L"暂时没有预约信息！");
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

int Manager::judgeUserBookNumber(CString username)
{
	connectMysql();
	CString sql_query;
	sql_query.Format(_T("select * from user where username = \'%s\';"),username);
	temp=transformPlus.toString(sql_query);
	sql=temp.c_str();
	if(mysql_query(&local_mysql,sql)==0)
	{
		res=mysql_store_result(&local_mysql);
		row=mysql_fetch_row(res);
		if(row)
		{
			int staticNumber=0;
			if (transformPlus.toString(row[5])=="1")
			{
				staticNumber=4;
			} 
			else if(transformPlus.toString(row[5])=="2")
			{
				staticNumber=6;
			}
			else if(transformPlus.toString(row[5])=="3")
			{
				staticNumber=10;
			}
			else if(transformPlus.toString(row[5])=="4")
			{
				staticNumber=999999;
			}
			int nowNumber=transformPlus.toInt(row[6]);
			nowNumber++;
			MessageBox(transformPlus.toCString(nowNumber));
			MessageBox(transformPlus.toCString(staticNumber));
			if(nowNumber>staticNumber)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}

}
int Manager::reduceUserBookNumber(CString username)
{
	connectMysql();
	CString sql_select;
	sql_select.Format(_T("select * from user where username=\'%s\';"),username);
	temp=transformPlus.toString(sql_select);
	sql=temp.c_str();
	if(mysql_query(&local_mysql,sql)==0)
	{
		res=mysql_store_result(&local_mysql);
		row=mysql_fetch_row(res);
		if(row)
		{
			int number =transformPlus.toInt(row[6]);
			number--;
			CString sql_update;

			sql_update.Format(_T("update user set bookNumber=\'%s\' where username= \'%s\';"),transformPlus.toCString(number),username);
			temp=transformPlus.toString(sql_update);
			sql=temp.c_str();
			if(mysql_query(&local_mysql,sql)==0)
			{
				return 1;
			}
			else
			{
				AfxMessageBox(_T("errorrrrrrr!"));
				return 0;
			}
		}
		else
		{
			AfxMessageBox(_T("error!!!!!!!!!!!!!!!"));
			return 0;
		}
	}
	else
	{
		AfxMessageBox(_T("er!"));
		return 0;
	}
}

void Manager::acceptBorrowBook()
{
	if(appThisNode==NULL)
	{
		MessageBox(L"您还未选择信息");
		return ;
	}
	else
	{
		//判断此时是否在预约时间之内
		time_t nowTime=time(0);
		CString cstrNowTime;
		cstrNowTime.Format(_T("%d"),nowTime);
		if(cstrNowTime>appThisNode->reTime())
		{
			//过期
			MessageBox(L"预约时间已过！");
			int judge=delAppointmentMessage(appThisNode->reUsername(),appThisNode->reISBN());
			int judge2=reduceUserBookNumber(appThisNode->reUsername());
			if(judge==judge2)
			{
				MessageBox(L"该学生预约信息已删除");
				return ;
			}
			else
			{
				MessageBox(L"该学生预约信息删除失败");
				return ;
			}
		}
		else
		{
			//准许
			int judgeBookNumber=judgeUserBookNumber(appThisNode->reUsername());
			if(judgeBookNumber==0)
			{
				AfxMessageBox(_T("该同学可借书籍已达上限！"));
				delAppointmentMessage(appThisNode->reUsername(),appThisNode->reISBN());
				reduceUserBookNumber(appThisNode->reUsername());
				return ;
			}
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
				MessageBox(L"批准成功！");
				int judge=delAppointmentMessage(appThisNode->reUsername(),appThisNode->reISBN());
				if(judge==1)
				{
					MessageBox(L"该学生预约信息已删除");
					return ;
				}
				else
				{
					MessageBox(L"该学生预约信息删除失败");
					return ;
				}
			}
			else
			{
				MessageBox(L"批准失败！");
				return ;
			}
		}
	}
}


//以书查人
void Manager::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	control_edit.EnableWindow(TRUE);
	if(edit_text=="")
	{
		MessageBox(L"请输入该书籍ISBN");
	}
	search_type=2;
	control_button2.SetWindowText(L"借阅详情");
	control_button2.ShowWindow(SW_HIDE);
	control_button3.SetWindowText(L"退出");
	control_button3.ShowWindow(SW_SHOW);
	control_list_box.ResetContent();

	
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
			MessageBox(L"该书籍并未被借出！");
			return ;
		}
	}
	else
	{
		AfxMessageBox(_T("error!"));
		return ;
	}
}

//以人查书
void Manager::OnBnClickedRadio5()
{
	// TODO: 在此添加控件通知处理程序代码
	control_edit.EnableWindow(TRUE);
	if(edit_text=="")
	{
		MessageBox(L"请输入想要查询学生的学号");
	}
	control_button2.SetWindowText(L"借阅详情");
	control_button2.ShowWindow(SW_SHOW);
	control_button3.SetWindowText(L"退出");
	control_button3.ShowWindow(SW_SHOW);
	control_list_box.ResetContent();

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
			MessageBox(L"该同学没有借书信息！");
			return ;
		}
	}
	else
	{
		AfxMessageBox(_T("error!"));
		return ;
	}
}


//查看违约情况
void Manager::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	control_edit.EnableWindow(FALSE);
	control_button2.SetWindowText(L"用户详情");
	control_button2.ShowWindow(SW_SHOW);
	control_button3.SetWindowText(L"退出");
	control_button3.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT1)->SetWindowText((LPCTSTR)"");
	control_list_box.ResetContent();
	search_type=3;
	//insertBadGuyToListBox();
}
void Manager::insertBadGuyToListBox()
{
	connectMysql();
	BadGuyList.clearList();
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
			if(search_type==1)
			{
				MessageBox(L"暂时没有预约信息！");
				return ;
			}
			else if(search_type==3)
			{
				MessageBox(L"暂时没有违约信息！");
				return ;
			}
		}
	}
	else
	{
		AfxMessageBox(_T("error!"));
		return ;
	}
}

//书籍统计
void Manager::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	control_edit.EnableWindow(FALSE);
	control_button2.SetWindowText(L"书籍详情");
	control_button2.ShowWindow(SW_SHOW);
	control_button3.SetWindowText(L"删除书籍");
	control_button3.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT1)->SetWindowText(transformPlus.toCString(""));
	control_list_box.ResetContent();
	search_type=4;
}
void Manager::insertAllBookToListBox()
{
	managerBookList.clearList();
	allBooksNumber=0;
	control_list_box.ResetContent();
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
			AfxMessageBox(_T("未查到书籍"));
			return ;
		}
	}
	else
	{
		AfxMessageBox(_T("error!"));
		return ;
	}
}

//用户统计
void Manager::OnBnClickedRadio6()
{
	// TODO: 在此添加控件通知处理程序代码
	control_edit.EnableWindow(FALSE);
	control_button2.SetWindowText(L"用户详情");
	control_button2.ShowWindow(SW_SHOW);
	control_button3.SetWindowText(L"退出");
	control_button3.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT1)->SetWindowText(transformPlus.toCString(""));
	control_list_box.ResetContent();
	search_type=6;;
	//insertAllUserToListBox();
}
void Manager::insertAllUserToListBox()
{
	BadGuyList.clearList();
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
			badGuyList * newNode=new badGuyList(transformPlus.toCString(row[1]),transformPlus.toCString(judge),transformPlus.toCString(row[3]));
			BadGuyList.add(newNode);
			BadGuyList.p->next=NULL;
		}
		if(BadGuyList.head==NULL)
		{
			AfxMessageBox(_T("未查到读者"));
			return ;
		}
	}
	else
	{
		AfxMessageBox(_T("error!"));
		return ;
	}
}

//查找用户 通过username
void Manager::OnBnClickedRadio7()
{
	// TODO: 在此添加控件通知处理程序代码
	if(edit_text=="")
	{
		MessageBox(L"请输入用户名");
	}
	control_edit.EnableWindow(TRUE);
	control_button2.SetWindowText(L"用户详情");
	control_button2.ShowWindow(SW_SHOW);
	control_button3.SetWindowText(L"删除用户");
	control_button3.ShowWindow(SW_SHOW);
	control_list_box.ResetContent();
	search_type=7;
}
void Manager::insertUserToListBox()
{
	connectMysql();
	BadGuyList.clearList();
	control_list_box.ResetContent();
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
			badGuyList * newNode=new badGuyList(transformPlus.toCString(row[1]),transformPlus.toCString(judge),transformPlus.toCString(row[3]));
			BadGuyList.add(newNode);
			BadGuyList.p->next=NULL;
			if(BadGuyList.head==NULL)
			{
				AfxMessageBox(_T("未查到该用户"));
				return ;
			}
			return ;
		}
		else
		{
			if(edit_text=="")
			{
				MessageBox(L"请输入用户名！");
				return ;
			}
			AfxMessageBox(_T("查无此人！"));
			return ;
		}
	}
	else
	{
		AfxMessageBox(_T("error"));
		return ;
	}
}

//查看被禁用户
void Manager::insertFineGuiesListToListBox()
{
	connectMysql();
	FineGuiesList.clearList();
	CString sql_query;
	sql_query.Format(_T("select * from user_finetime"));
	temp=transformPlus.toString(sql_query);
	sql=temp.c_str();
	if(mysql_query(&local_mysql,sql)==0)
	{
		res=mysql_store_result(&local_mysql);
		while(row=mysql_fetch_row(res))
		{
			int judge=control_fine_list_box.InsertString(-1,transformPlus.toCString(row[1]));
			FineGuyList * newNode=new FineGuyList(transformPlus.toCString(row[1]),transformPlus.toCString(judge),transformPlus.toCString(row[2]));
			FineGuiesList.add(newNode);
			FineGuiesList.p->next=NULL;
		}
	}
	else
	{
		return ;
	}
}


void Manager::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	control_list_box.ResetContent();
	if (search_type==1)
	{
		//查看预约
		insertAppointmentMessageToListBox();
	} 
	else if(search_type==2)
	{
		//以书查人
		UpdateData(TRUE);
		if(edit_text=="")
		{
			AfxMessageBox(_T("请输入搜索内容"));
			return ;
		}
		insertUserHaveTheBookToListBox();
	}
	else if(search_type==5)
	{
		//以人查书
		UpdateData(TRUE);
		if(edit_text=="")
		{
			AfxMessageBox(_T("请输入搜索内容"));
			return ;
		}
		insertUserBookListToListBox();
	}
	else if(search_type==3)
	{
		//查看违约情况
		insertBadGuyToListBox();
	}
	else if(search_type==4)
	{
		//书籍统计
		insertAllBookToListBox();
	}
	else if(search_type==6)
	{
		//用户统计
		insertAllUserToListBox();
	}
	else if(search_type==7)
	{
		UpdateData(TRUE);
		//MessageBox(edit_text);
		if(edit_text=="")
		{
			AfxMessageBox(_T("请输入搜索内容"));
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
	insertFineGuiesListToListBox();
	search_type=-1;
	control_button2.SetWindowText(L"添加用户");
	control_button2.ShowWindow(SW_SHOW);
	control_button3.SetWindowText(L"退出");
	control_button3.ShowWindow(SW_SHOW);
	userBookList.clearList();
	managerUserList.clearList();
	managerBookList.clearList();
	appointmentList.clearList();
	// TODO:  在此添加额外的初始化
	appThisNode=NULL;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

int Manager::delBook(CString book_ISBN)
{
	CString sql_del;
	sql_del.Format(_T("delete from book where ISBN=\'%s\';"),book_ISBN);
	temp=transformPlus.toString(sql_del);
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
void Manager::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if(search_type==-1)
	{
		CDialogEx::OnCancel();
	}
	else if (search_type==1)
	{
		//查看预约
		if(appThisNode==NULL)
		{
			AfxMessageBox(_T("您还未选择信息！"));
			return ;
		}
		CString message;
		message=L"您的预约信息已被驳回，请重新借阅或预约";
		sendMessageToUser(appThisNode->reUsername(),message);
		int judge=delAppointmentMessage(appThisNode->reUsername(),appThisNode->reISBN());
		if(judge==1)
		{
			MessageBox(L"删除成功");
			insertAppointmentMessageToListBox();
			return ;
		}
		else
		{
			MessageBox(L"发生了不可预知的错误");
			return ;
		}
	} 
	else if(search_type==2)
	{
		//以书查人
		CDialogEx::OnCancel();
	}
	else if(search_type==5)
	{
		//以人查书
		CDialogEx::OnCancel();
	}
	else if(search_type==3)
	{
		//查看违约情况
		CDialogEx::OnCancel();

	}
	else if(search_type==4)
	{
		//书籍统计
		if(uBThisNode==NULL)
		{
			AfxMessageBox(_T("您还为选取任何书籍"));
			return ;
		}
		int i=delBook(transformPlus.toCString(uBThisNode->reISBN()));
		if(i==1)
		{
			MessageBox(L"删除成功！");
			insertAllBookToListBox();
			return ;
		}
	}
	else if(search_type==6)
	{
		//用户统计
		CDialogEx::OnCancel();
	}
	else if(search_type==7)
	{
		//用户搜索
		if(bGThisNode==NULL)
		{
			AfxMessageBox(_T("您还为选取任何书籍"));
			return ;
		}
		int i=delUser(bGThisNode->reUsername());
		if(i==1)
		{
			MessageBox(L"删除成功！");
			insertUserToListBox();
			return ;
		}
	}
	else
	{
		AfxMessageBox(_T("error!@"));
		return ;
	}
}

//点击list box 
void Manager::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
	bGThisNode=NULL;
	appThisNode=NULL;
	mBThisNode=NULL;
	mUThisNode=NULL;
	uBThisNode=NULL;
	fGThisNode=NULL;
	CString cstrText;
	int selectedTextNumber;
	selectedTextNumber=control_list_box.GetCurSel();//获取当前点击项的位置

	if (search_type==1)
	{
		//查看预约
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
		//以书查人
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
		//以人查书
		Book *head=userBookList.head;
		while(head)
		{
			if(head->reListPosition()==transformPlus.toInt(selectedTextNumber))
			{
				uBThisNode=head;
				break;
			}
			head=head->next;
		}
	}
	else if(search_type==3)
	{
		//查看违约情况
		badGuyList *head=BadGuyList.head;
		while(head)
		{
			if(head->rePosition()==transformPlus.toCString(selectedTextNumber))
			{
				bGThisNode=head;
				break;
			}
			head=head->next;
		}

	}
	else if(search_type==4)
	{
		//书籍统计
		managerBook *head=managerBookList.head;
		//MessageBox(transformPlus.toCString(userBookList.head->reListPosition()));
		while(head)
		{
			if(head->reListBookPosition()==transformPlus.toCString(selectedTextNumber))
			{
				mBThisNode=head;
				break;
			}
			head=head->next;
			
		}
	}
	else if(search_type==6)
	{
		//用户统计 因为数据类型与badguy 相同，所以用 查看违约的代码
		badGuyList *head=BadGuyList.head;
		while(head)
		{
			if(head->rePosition()==transformPlus.toCString(selectedTextNumber))
			{
				bGThisNode=head;
				break;
			}
			head=head->next;
		}
	}
	else if(search_type==7)
	{
		//用户搜索
		bGThisNode=NULL;
		badGuyList *head=BadGuyList.head;
		while(head)
		{
			if(head->rePosition()==transformPlus.toCString(selectedTextNumber))
			{
				bGThisNode=head;
				break;
			}
			head=head->next;
		}
	}
	else
	{
		AfxMessageBox(_T("error!@"));
		return ;
	}
}

//点击finlistbox 
void Manager::OnLbnSelchangeList3()
{
	// TODO: 在此添加控件通知处理程序代码
	fGThisNode=NULL;
	CString cstrText;
	int selectedTextNumber;
	selectedTextNumber=control_fine_list_box.GetCurSel();//获取当前点击项的位置
	FineGuyList * head=FineGuiesList.head;
	while(head)
	{
		if(head->reListPosition()==transformPlus.toCString(selectedTextNumber))
		{
			fGThisNode=head;
			break;
		}
		head=head->next;
	}
	return ;
}
int Manager::delUser(CString username)
{
	CString sql_del;
	sql_del.Format(_T("delete from user where username=\'%s\';"),username);
	temp=transformPlus.toString(sql_del);
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

void Manager::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(search_type==-1)
	{
		this->ShowWindow(SW_HIDE);
		SignIn signIn;
		signIn.DoModal();
		this->ShowWindow(SW_SHOW);
	}
	else if (search_type==1)
	{
		//查看预约
		acceptBorrowBook();
		insertAppointmentMessageToListBox();
		return ;
	} 
	else if(search_type==2)
	{
		//以书查人
		//借阅详情已经隐藏
	}
	else if(search_type==5)
	{
		//以人查书
		//借阅详情
		if(uBThisNode==NULL)
		{
			AfxMessageBox(_T("还未选取学生"));
			return ;
		}
		AboutUserBook aboutUserBook;
		aboutUserBook.ISBN=transformPlus.toCString(uBThisNode->reISBN());
		aboutUserBook.DoModal();
		return ;
	}
	else if(search_type==3)
	{
		//查看违约情况
		if(bGThisNode==NULL)
		{
			AfxMessageBox(_T("还未选取学生"));
			return ;
		}
		BadGuys badguys;
		badguys.username=bGThisNode->reUsername();
		badguys.DoModal();
		return ;
	}
	else if(search_type==4)
	{
		//书籍统计
		if(mBThisNode==NULL)
		{
			AfxMessageBox(_T("Adadafawftqwtwqet"));
			return ;
		}
		AboutTheBookMessage aboutTheBookMessage;
		aboutTheBookMessage.theBookNode=mBThisNode;
		aboutTheBookMessage.DoModal();
		return ;
	}
	else if(search_type==6)
	{
		//用户统计
		if(bGThisNode==NULL)
		{
			AfxMessageBox(_T("还未选取学生"));
			return ;
		}
		BadGuys badguys;
		badguys.username=bGThisNode->reUsername();
		badguys.DoModal();
		return ;
	}
	else if(search_type==7)
	{
		//用户搜索
		if(bGThisNode==NULL)
		{
			AfxMessageBox(_T("请输入搜索项"));
			return ;
		}
		BadGuys badguys;
		badguys.username=bGThisNode->reUsername();
		badguys.DoModal();
		return ;
	}
	else
	{
		AfxMessageBox(_T("error!@"));
		return ;
	}
}



//被禁同学详情
void Manager::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	if(fGThisNode==NULL)
	{
		AfxMessageBox(_T("您还未选择学生！"));
		return ;
	}
	FineGuyMessage fineGuyMessage;
	fineGuyMessage.theGuy=fGThisNode;
	fineGuyMessage.DoModal();
	return ;
}

//解禁
void Manager::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(fGThisNode==NULL)
	{
		AfxMessageBox(_T("您还未选择学生！"));
		return ;
	}
	CString sql_query;
	sql_query.Format(_T("delete from user_finetime where username=\'%s\' and recoverTime=\'%s\';"),fGThisNode->reUsername(),fGThisNode->reFineTime());
	temp=transformPlus.toString(sql_query);
	sql=temp.c_str();
	if(mysql_query(&local_mysql,sql)==0)
	{
		MessageBox(L"解除封印成功！");
	}
	else
	{
		MessageBox(L"解除封印失败！");
	}
	control_fine_list_box.ResetContent();
	insertFineGuiesListToListBox();
}


void Manager::OnLbnDblclkList1()
{
	// TODO: 在此添加控件通知处理程序代码
	OnLbnSelchangeList1();
	OnBnClickedButton2();
}
