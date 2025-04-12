#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;
enum {
	KEYWORD, DIVIDER, IDENTIFIER, CONSTANT, END_OF_PROGRAMM, ERROR, BEGIN_STATE
};
class obrPolsk
{
public:
	obrPolsk();
	~obrPolsk();
	string name;
	string type;
	obrPolsk& operator= (const obrPolsk& copy);
	obrPolsk(const obrPolsk& obg);
private:
};
class Token
{
public:
	string name;
	int type;
	Token(string& NAME, int TYPE);
	Token(const Token& obg);
	void set_name(string& N) { name = N; };
	string& get_name() { return name; };
	void set_type(int N) { type = N; };
	int get_type() { return type; };
};
class Identifier
{
	string name;
public:
	Identifier(string& NAME) { name = NAME; };
	Identifier(const Identifier& obg) { name = obg.name; };
	void set_name(string& N) { name = N; };
	string& get_name() { return name; };
};
class Ident
{
public:
	string name;
	string type;
	bool defined;
	Ident()
	{
		name = "";
		type = "";
		defined = 0;
	}
	Ident(Ident* copy)
	{
		name = copy->name;
		type = copy->type;
	}
	string& get_name() { return name; };
};
class Constant
{
	string name;
public:
	Constant(string& NAME) { name = NAME; };
	Constant(const Constant& obg) { name = obg.name; };
	void set_name(string& N) { name = N; };
	string& get_name() { return name; };
};
class KeyWords//Все ключевые слова из входного файла
{
	vector <string> keyWords;
public:
	KeyWords();
	vector <string>& get_vect_key_word() { return keyWords; };
	bool operator ==(string& obg);//Есть ли среди всех ключевых слов слово obg?
};
class Dividers
{
	vector <string> dividers;
	public:
	Dividers();
	vector<string>& get_vect_key_word() { return dividers; };
	bool operator == (string& obg);
};
template <typename T>
class Table {
	public:
	vector<T> table;
	int count;
	Table& operator= (const Table & Table);
	Table();
    Table(Table* copy);
	int get_count() { return count; };
	Table<T>& operator +=(T& obg);
	T& operator [](int num);
	bool operator ==(string& obg);
};
template <typename T>
inline Table<T>::Table(Table<T>* copy)
{
	count = copy
		->count;
	table = copy
		->table;
}
template <typename T>
Table<T>& Table<T>::operator=(const Table<T>& copy)
{
	count = copy.count;
	table = copy.table;
	return *this;
}
template <typename T>
inline Table<T>::Table()
{
	count = 0;
}
template <typename T>
inline Table<T>& Table<T>::operator+=(T& obg)
{
	table.push_back(obg);
	count++;
	return *this;
}
template <typename T>
inline bool Table<T>::operator== (string& obg)
{
	for (auto var : table)
	{
		if(obg == var.get_name())
			return true;
	}
	return false;
}
template<typename T>
inline T& Table <T>::operator[](int num)
{
	return table[num];
}
template <typename T >
struct indif {
	string name; T elem;
};