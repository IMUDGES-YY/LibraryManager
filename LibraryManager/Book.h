#pragma once
#include <string>
//�鼮List
using namespace std;
class Book
{
public:
	Book();
	Book(string ,int );
	~Book(void);
	string reISBN();
	int reListPosition();
	string ISBN;
	int listPosition;
	Book *next;
};