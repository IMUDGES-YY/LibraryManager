
// LibraryManagerDlg.h : ͷ�ļ�
//

#pragma once
#include <iostream>
#include <string>
#include <mysql.h>
#include "TransformPlus.h"
#include "binary_log_types.h"
#include "SignIn.h"
#include "InterfaceForUser.h"
#include "afxwin.h"
#include "Manager.h"

// CLibraryManagerDlg �Ի���
class CLibraryManagerDlg : public CDialogEx
{
// ����
public:
	CLibraryManagerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LIBRARYMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	TransformPlus transformPlus;
	afx_msg void OnBnClickedOk();
	CString edit_username;
	CString edit_password;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	MYSQL connectMySQL();
	CEdit control_edit_username;
	CEdit control_edit_password;
};
