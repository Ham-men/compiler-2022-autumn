#include "Struct.h"
KeyWords::KeyWords() {
	//Чтение ключевых слов
	string str;
	try {
		ifstream in_KW;
		in_KW.open("keywords.txt", ios::in);
		if (!in_KW.is_open())
			throw in_KW;
		while (!in_KW.eof()) {
			in_KW >> str;
			if (!str.empty())
				keyWords.push_back(str);
		}
		in_KW.close();
	}
	catch
		(ifstream&)
	{
		cout << "Error opening file keywords.txt" << endl;
		system("pause");
		system("cls");
	}
}
bool KeyWords::operator== (string& obg)
{
	for(auto var : keyWords)
	{
		if(obg == var)
			return true;
	}
	return false
		;
}
Dividers::Dividers() {
	//Чтение ключевых слов
	string str;
	try {
		ifstream in_KW;
		in_KW.open("dividers.txt", ios::in);
		if (!in_KW.is_open())
			throw in_KW;
		while (!in_KW.eof()) {
			in_KW >> str;
			if (!str.empty())
				dividers.push_back(str);
		}
		in_KW.close();
	}
	catch
		(ifstream&)
	{
		cout << "Error opening filedividers.txt" << endl;
		system("pause");
		system("cls");
	}
}
bool Dividers::operator==(string& obg)
{
	int i = 0;
	for (auto var : dividers)
	{
		if (obg == var)
			return true;
	}
	return false;
}
Token::Token(string& NAME, int TYPE)
{
	name = NAME;
	type = TYPE;
}
Token::Token(const Token& obg)
{
	name = obg.name;
	type = obg.type;
}
obrPolsk::obrPolsk()
{
	name = "";
	type = "";
}
obrPolsk::obrPolsk(const obrPolsk& obg)
{
	name = obg.name;
	type = obg.type;
}
obrPolsk::~obrPolsk()
{
}
obrPolsk& obrPolsk::operator= (const obrPolsk& copy)
{
	name = copy.name;
	type = copy.type;
	return *this;
}