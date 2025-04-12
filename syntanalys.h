#pragma once
#include "scanner.h"
#include <stack>
class sintAnalays
{
	Table <Token> stream_of_token; // класс с таблицей токенов
	vector <Ident> table_of_identifier; // иде-ы
	stack<obrPolsk> polsk; //обрпольск
	string tokenName; // имя токена
	int tokNumber; // номер токена в таблице
	int indNumber; // номер и-ра в таблице ид-ов
	int tokenType; // тип токена
	int lineNumber; // номер строки в программе
public:
	vector<obrPolsk> exit; // вектор выход обрпольской
	void newToken(); // взять новый токен из таблицы
	void getExit(); // вывести вектор выхода обпольской
	int priorityChek(string sign); // проверка приоретов для обрпольской
	int forChek(); // проверка фора
	int BodyChek(); // проверка тела функции
	int keywordChek(); // проверка ключевого ствола
	int writeChek(); // проверка врите
	int equaChek(); // проверка уравнений
	int varDecCheck(); // проверка обьявл переменных
	int ero(int er); // ошибки
	int analys(); // гл. функция синт анализа
	int programBody(); // проверка тела программы
	int checkIdentTable(string tokenName); // проверка есть ли введенный инд-р в таблице инд - ов
	int funChek(); // чек функции
	sintAnalays(Table <Token> stream_of_token, vector <Ident> table_of_identifier); //конструктор
};
