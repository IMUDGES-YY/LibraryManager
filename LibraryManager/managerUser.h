#pragma once
//�����û�List
class managerUser
{
public:
	CString username;
	CString name;
	CString listPosition;
	CString reName()
	{
		return name;
	}
	CString reUsername()
	{
		return username;
	}
	CString rePosition()
	{
		return listPosition;
	}
	managerUser(CString,CString ,CString);
	managerUser(CString,CString);
	~managerUser(void);
	managerUser * next;
};

