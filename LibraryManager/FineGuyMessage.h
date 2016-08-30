#pragma once
#include "TransformPlus.h"
#include "FineGuyList.h"
#include <mysql.h>


// FineGuyMessage �Ի���
// ���ͷ��˵� DLG
class FineGuyMessage : public CDialogEx
{
	DECLARE_DYNAMIC(FineGuyMessage)

public:
	FineGuyMessage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~FineGuyMessage();
	FineGuyList *theGuy;
	TransformPlus transformPlus;
// �Ի�������
	enum { IDD = IDD_FINEGUYMESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
