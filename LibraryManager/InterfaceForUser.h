#pragma once
#include "afxwin.h"
#include <string>
#include "TransformPlus.h"
#include <mysql.h>
#include "Book.h"
#include "userBook.h"
#include "List.h"
#include <ctime>
#include "AboutYourBookMessage.h"

// InterfaceForUser 对话框
//用户的DLG

class InterfaceForUser : public CDialogEx
{
	DECLARE_DYNAMIC(InterfaceForUser)

public:
	InterfaceForUser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~InterfaceForUser();

// 对话框数据
	enum { IDD = IDD_INTERFACEFORUSER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString loginUser;//登陆者的账号
	CString loginUserType;//登陆者的种类
	CString edit_search;
	CString loginUserProfessioanl;	//登陆者专业
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio1();

	TransformPlus transformPlus;
	int select_type;//搜索方式
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	int all_book_number;//书籍总数
	CListBox select_list_box;
	CString book_number;
	CButton control_search_button;
	afx_msg void OnEnChangeEdit1();
	CStatic control_book_number;
	afx_msg void OnLbnSelchangeList1();
	//value 变量
	CString book_name;
	CString book_author;
	CString book_press;
	CString book_date;
	CString book_type;
	CString book_about;
	//control变量
	CStatic control_book_name;
	CStatic control_book_author;
	CStatic control_book_press;
	CStatic control_book_date;
	CStatic control_book_type;
	CStatic control_book_about;
	List<Book> list;//book 类的list对象，搜索出来的内容
	List<userBook> userBookList;//用户的借阅信息
	Book *thisNode;//选中的书籍
	userBook *thisUserBookNode;//选中的用户的书籍
	string TYPE;//显示的用户类型
	string name;//显示的用户姓名
	string username;//显示的用户名
	CString book_ISBN;
	CStatic control_book_ISBN;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void borrowBook(string ,string ,MYSQL );
	//stitic text 变量
	CStatic control_name;
	CStatic control_type;
	CStatic control_username;
	CString name_text;
	CString type_text;
	CString username_text;
	//DLG初始化时 的函数
	virtual BOOL OnInitDialog();
	CEdit control_edit_text;
	CListBox user_book_list;
	void setUserBookMessage();
	afx_msg void OnLbnSelchangeList3();
	MYSQL connectMySQL();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	int reduceBookNumber(string bookISBN,MYSQL local_mysql);//1 成功 ！0 失败
	int reduceUserBookNumber(string username,MYSQL local_mysql);//1 成功 ！0 失败
	int addBookNumber(string bookISBN,MYSQL local_mysql);//1 成功 ！0 失败
	int judgeTime(long reTime);//1 成功 ！0 失败
	int delBookMessage(string bookISBN,MYSQL local_mysql);//1 成功 ！0 失败
	int addMistake(string username,MYSQL locai_mysql);//1 成功 ！0 失败
	int addFineUser(CString username,MYSQL local_mysql);
	afx_msg void OnBnClickedButton6();
	int judgeBookNumber(CString bookISBN,MYSQL local_mysql);
	int addBookToAppointmentBookTable(CString username ,CString bookISBN,MYSQL local_mysql);
	int judgeTheBookIsAppointment(CString username,CString bookISBN,MYSQL local_mysql);
	int addUserBookNumber(CString username,MYSQL local_mysql);
	int checkUserBookNumber(CString,MYSQL loca_mysql);
	int judgeMessage();
	int delBlockMessage(CString );
	int judgeUserInFineListOrNot();
	CString fineTime;
	int judgeUserWrongTime();
	int addUserToFineUserTable();
	afx_msg void OnLbnDblclkList1();
	afx_msg void OnLbnDblclkList3();
};
