#pragma once
#include "TransformPlus.h"
#include "badGuyList.h"
#include <string>
#include <mysql.h>


// BadGuys �Ի���

class BadGuys : public CDialogEx
{
	DECLARE_DYNAMIC(BadGuys)

public:
	BadGuys(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~BadGuys();
	CString username;
	TransformPlus transformPlus;
// �Ի�������
	enum { IDD = IDD_BADGUYS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
