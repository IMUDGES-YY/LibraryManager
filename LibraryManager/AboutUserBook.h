#pragma once
#include "TransformPlus.h"
#include <mysql.h>
#include "Book.h"
#include <cstring>

// AboutUserBook �Ի���

class AboutUserBook : public CDialogEx
{
	DECLARE_DYNAMIC(AboutUserBook)

public:
	AboutUserBook(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AboutUserBook();
	Book * userBookMessage;
	TransformPlus transformPlus;
	CString ISBN;

// �Ի�������
	enum { IDD = IDD_ABOUTUSERBOOK };
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
