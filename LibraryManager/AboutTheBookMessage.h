#pragma once
#include "TransformPlus.h"
#include <string>
#include <mysql.h>
#include "managerBook.h"


// AboutTheBookMessage �Ի���
// ����Ա�鿴�鼮��ϢDLG

class AboutTheBookMessage : public CDialogEx
{
	DECLARE_DYNAMIC(AboutTheBookMessage)

public:
	AboutTheBookMessage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AboutTheBookMessage();
	managerBook * theBookNode;
	TransformPlus transformPlus;
// �Ի�������
	enum { IDD = IDD_ABOUTTHEBOOKMESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
