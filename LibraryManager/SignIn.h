#pragma once
#include <mysql.h>
#include <string>
#include "TransformPlus.h"
#include "afxwin.h"

// SignIn �Ի���

class SignIn : public CDialogEx
{
	DECLARE_DYNAMIC(SignIn)

public:
	SignIn(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SignIn();

// �Ի�������
	enum { IDD = IDD_SIGNIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CString new_username;
	CString new_password;
	CString confirm_password;
	CString new_name;
	CString new_type;
	CString new_unit;
	TransformPlus transformPlus;
	CString textCheck;
	afx_msg void OnEnChangeEdit3();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeEdit1();
	CString checkUsername;
	afx_msg void OnEnChangeEdit4();
	CString confirmPassword;
	CEdit ct_confirm_password;
};
