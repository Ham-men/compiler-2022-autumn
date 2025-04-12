#include "syntanalys.h"
// синт анализатор получает на вход из лексического ана-ра таблицу лексем
sintAnalays::sintAnalays(Table <Token> stream_of_token, vector <Ident> table_of_identifier)
{
	this->stream_of_token = stream_of_token;
	this->table_of_identifier = table_of_identifier;
	tokNumber = 0;
	lineNumber = 0;
}
void sintAnalays::newToken()
{
	tokenName = stream_of_token.table.at(tokNumber).get_name();
	tokenType = stream_of_token.table.at(tokNumber).get_type();
	tokNumber++;
}
int sintAnalays::analys()
{
	newToken();
	if (tokenName != "PROGRAMM")
	{
		return ero(1);
		cout << "Завершена с ошибкой" << endl;
	}
	newToken();
	if (tokenName != ";")
	{
		return ero(2);
		cout << "Завершена с ошибкой" << endl;
	}
	if (programBody() == 0)
	{
		return 0;
		cout << "Завершена с ошибкой" << endl;
	}
	cout << "Программа выполнена успешно" << endl;
	return 1;
}
int sintAnalays::programBody()
{
	newToken();
	do
	{
		if (tokenName == "END")
		{
			break;
		}
		if ((tokenType == KEYWORD) | (tokenType == IDENTIFIER))
		{
			if (tokenName == "FUNC")
			{
				if (funChek() == 0)
				{
					return 0;
				}
				newToken();
			}
			else if (BodyChek() == 0)
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
		if (stream_of_token.table.size() == tokNumber)
		{
			ero(9);
			return 0;
		}
	} while (tokenName != "END");
	return 1;
}
int sintAnalays::checkIdentTable(string tokenName)
{
	for (int i = 0; i < table_of_identifier.size(); i++)
	{
		if (table_of_identifier.at(i).name == tokenName)
		{
			return i;
		}
	}
	return -1;
}
int sintAnalays::forChek()
{
	if (tokenName == "for")
	{
		obrPolsk newElem;
		newElem.name = tokenName;
		newElem.type = "KEYWORD";
		newToken();
		if (tokenName != "(")
		{
			ero(4);
			return 0;
		}
		else
		{
			newToken();
			newElem.name = tokenName;
			newElem.type = "IDENTIFIER";
			if (tokenType != IDENTIFIER)
			{
				ero(5);
				return 0;
			}
			if (checkIdentTable(tokenName) > -1)
			{
				exit.push_back(newElem);
				polsk.push(newElem);
				newToken();
				if (tokenName != "=")
				{
					ero(6);
					return 0;
				}
				newElem.name = tokenName;
				newElem.type= "OPERATION";
				polsk.push(newElem);
				newToken();
				if (tokenType != CONSTANT)
				{
					ero(8);
					return 0;
				}
				newElem.name = tokenName;
				newElem.type = "CONSTANT";
				exit.push_back(newElem);
				exit.push_back(polsk.top());
				polsk.pop();
				newElem.name = "$FOR";
				newElem.type = "LINK";
				exit.push_back(newElem);
				exit.push_back(polsk.top());
				newToken();
				if (tokenName != ":")
				{
					ero(10);
					return 0;
				}
				newToken();
				if (tokenType != CONSTANT)
				{
					ero(8);
					return 0;
				}
				newElem.name = tokenName;
				newElem.type = "CONSTANT";
				exit.push_back(newElem);
				newElem.name = ">";
				newElem.type = "OPERATION";
				exit.push_back(newElem);
				newElem.name = "$INENDFOR";
				newElem.type = "MOVELINK";
				exit.push_back(newElem);
				newToken();
				if (tokenName != "by")
				{
					ero(11);
					return 0;
				}
				newToken();
				if (tokenType != CONSTANT
					)
				{
					ero(8);
					return 0;
				}
				newElem.name = tokenName;
				newElem.type = "CONSTANT";
				polsk.push(newElem);
				newToken();
				if (tokenName != ")")
				{
					ero(12);
					return 0;
				}
				newToken();
				while (tokenName != "endfor")
				{
					if (BodyChek() == 0) {
						return 0;
					}
				}
				newElem = polsk.top();
				polsk.pop();
				exit.push_back(polsk.top());
				exit.push_back(newElem);
				polsk.pop();
				newElem.name = "+";
				newElem.type = "OPERATION";
				exit.push_back(newElem);
				newElem.name = "$FOR";
				newElem.type = "MOVELINK";
				exit.push_back(newElem);
				newElem.name = "$ENDFOR";
				newElem.type = "LINK";
				exit.push_back(newElem);
				newToken();
				if (tokenName != ";")
				{
					ero(2);
					return 0;
				}
			}
			else {
				ero(7);
				return 0;
			}
		}
		return 1;
	}
	return 0;
}
int sintAnalays::BodyChek() {
	lineNumber++;
	if (tokenType == KEYWORD)
	{
		if (tokenName == "FUNC")
		{
			ero(15);
			return 0;
		}
		if (keywordChek() == 0) {
			return 0;
		}
	}
	else if (tokenType == IDENTIFIER)
	{
		if (equaChek() == 0)
		{
			return 0;
		}
	}
	else
	{
		ero(14);
		return 0;
	}
	if (stream_of_token.table.size() - 1 == tokNumber)
	{
		ero(13);
		return 0;
	}
	newToken();
	return 1;
}
int sintAnalays::equaChek()
{
	int numbInd, numbNext;
	obrPolsk newElem;
	if (tokenType == IDENTIFIER)
	{
		numbInd = checkIdentTable(tokenName);
		if (table_of_identifier.at(numbInd).defined == 0)
		{
			ero(7);
			return 0;
		}
		newElem.name = tokenName;
		newElem.type = "IDENTIFIER";
		exit.push_back(newElem);
		if (table_of_identifier.at(numbInd).type == "string")
		{
			newToken();
			if (tokenName != "=")
			{
				ero(6);
				return 0;
			}
			newElem.name = tokenName;
			newElem.type = "OPERATION";
			polsk.push(newElem);
			newToken();
			if (tokenType == IDENTIFIER)
			{
				if (table_of_identifier.at(checkIdentTable(tokenName)).type != "string")
				{
					ero(16);
					return 0;
				}
				newElem.name = tokenName;
				newElem.type = "IDENTIFIER";
				exit.push_back(newElem);
				newToken();
				if (tokenName != ";")
				{
					ero(15);
					return 0;
				}
				return 0;
			}
			else
			{
				do
				{
					newElem.name = "";
					if (tokenType != DIVIDER)
					{
						ero(15);
						return 0;
					}
					newElem.name += tokenName;
					newToken();
					if (tokenType != IDENTIFIER)
					{
						ero(15);
						return 0;
					}
					newElem.name += tokenName;
					newToken();
					if (tokenType != DIVIDER)
					{
						ero(15);
						return 0;
					}
					newElem.name += tokenName;
					newToken();
				} while (tokenName != ";");
				if (tokenName != ";")
				{
					ero(15);
					return 0;
				}
				newElem.type = "CONSTANT";
				exit.push_back(newElem);
			}
		}
		else
		{
			newToken();
			if (tokenName != "=")
			{
				ero(6);
				return 0;
			}
			newElem.name = tokenName;
			newElem.type = "OPERATION";
			polsk.push(newElem);
			newElem.name = "1";
			newElem.type = "1";
			polsk.push(newElem);
			int flag = 0;
			do
			{
				newToken();
				if ((tokenType != CONSTANT) && (tokenType != IDENTIFIER))
				{
					if (tokenName == "(")
					{
						newElem.name = tokenName;
						newElem.type = "OPERATION";
						polsk.push(newElem);
						newToken();
						if (tokenType == DIVIDER)
						{
							ero(17);
							return 0;
						}
						flag++;
					}
					if (tokenName == "-")
					{
						newToken();
						if ((tokenType == DIVIDER) | tokenName != "(")
						{
							ero(17);
							return 0;
						}
						newElem.name = "-";
						newElem.type = "OPERATION";
						polsk.push(newElem);
						newElem.name = "(";
						newElem.type = "OPERATION";
						polsk.push(newElem);
					}
					else if (tokenName == ")")
					{
						while (polsk.top().type != "(")
						{
							exit.push_back(polsk.top());
							polsk.pop();
						}
						polsk.pop();
						flag--;
					}
					else
					{
						ero(15);
						return 0;
					}
				}
				if (tokenName == ";")
				{
					break;
				}
				if (tokenType == IDENTIFIER)
				{
					numbNext = checkIdentTable(tokenName);
					newElem.name = tokenName;
					newToken();
					if (tokenName == "(")
					{
						newToken();
						if ((tokenType == IDENTIFIER) | (tokenType == CONSTANT))
						{
							obrPolsk newElem2;
							newElem2.name = tokenName;
							newElem2.type = "IDENTIFIER";
							exit.push_back(newElem2);
							newToken();
							if (tokenName != ")")
							{
								ero(12);
								return 0;
							}
							newToken();
						}
						else
						{
							ero(16);
							return 0;
						}
						newElem.type = "FUNC";
					}
					else
					{
						if (numbNext == -1)
						{
							ero(7);
							return 0;
						}
						if (table_of_identifier.at(numbInd).type !=
							table_of_identifier.at(numbNext).type)
						{
							ero(16);
							return 0;
						}
						newElem.type = "IDENTIFIER";
					}
					exit.push_back(newElem);
				}
				else
				{
					newElem.name = tokenName;
					newElem.type = "CONSTANT";
					exit.push_back(newElem);
					newToken();
				}
				if (tokenName == ";")
				{
					break;
				}
				if ((tokenName != "+") && (tokenName != "-") && (tokenName != "*") && (tokenName != "/"))
				{
					ero(3);
					return 0;
				}
				if (priorityChek(polsk.top().name) > priorityChek(tokenName))
				{
					while (polsk.top().name != "1")
					{
						exit.push_back(polsk.top());
						polsk.pop();
					}
					newElem.name = tokenName;
					newElem.type = "OPERATION";
					polsk.push(newElem);
				}
				else
				{
					newElem.name = tokenName;
					newElem.type = "OPERATION";
					polsk.push(newElem);
				}
			} while (tokenName != ";");
			if (flag != 0)
			{
				ero(12);
				return 0;
			}
			while (polsk.top().name != "1")
			{
				exit.push_back(polsk.top());
				polsk.pop();
			}
			polsk.pop();
		}
		exit.push_back(polsk.top());
		polsk.pop();
		return 1;
	}
	return 0;
}
int sintAnalays::varDecCheck()
{
	int tableNum;
	if (tokenName == "integer" | tokenName == "string")
	{
		obrPolsk newElem;
		newElem.type = "OPERATION";
		if (tokenName == "integer")
		{
			newElem.name = "dd";
		}
		else
		{
			newElem.name = "db";
		}
		polsk.push(newElem);
		newToken();
		if (tokenType == IDENTIFIER)
		{
			newElem.type = "IDENTIFIER";
			newElem.name = tokenName;
			tableNum = checkIdentTable(tokenName);
			if (tableNum == -1)
			{
				ero(19);
				return 0;
			}
			newToken();
			if (tokenName != ";")
			{
				ero(2);
				return 0;
			}
			table_of_identifier.at(tableNum).defined = 1;
			exit.push_back(newElem);
			newElem = polsk.top();
			exit.push_back(polsk.top());
			polsk.pop();
		}
		else {
			ero(3);
			return 0;
		}
		return 1;
	}
	return 0;
}
int sintAnalays::keywordChek() {
	if (tokenName == "for"
		)
	{
		if (forChek() == 0) {
			return 0;
		}
	}
	else if (tokenName == "write"
		)
	{
		if (writeChek() == 0) {
			return 0;
		}
	}
	else if (tokenName == "FUNC"
		)
	{
		if (funChek() == 0) {
			return 0;
		}
	}
	else {
		if (varDecCheck() == 0) {
			return 0;
		}
	}
	return 1;
}
int sintAnalays::writeChek() {
	obrPolsk newElem;
	newToken();
	newElem.name = "begwrite";
	newElem.type = "OPERATION";
	exit.push_back(newElem);
	if (tokenName != "(")
	{
		ero(4);
		return 0;
	}
	do {
		newToken();
		if (tokenName == ")")
		{
			break
				;
		}
		if (tokenType == DIVIDER)
		{
			do
			{
				newElem.name = tokenName;
				newToken();
				if (tokenType != IDENTIFIER && tokenType != CONSTANT)
				{
					ero(16);
					return 0;
				}
				newElem.name += tokenName;
				newToken();
				if (tokenType != DIVIDER)
				{
					ero(16);
					return 0;
				}
				if (tokenName == ")")
				{
					ero(16);
					return 0;
				}
				newElem.name += tokenName;
			} while ((tokenType == CONSTANT) | (tokenType == IDENTIFIER) | (tokenName == "=") | (tokenName == "-") | (tokenName == " ") | (tokenName == "+"));
			newElem.type = "CONSTANT";
			exit.push_back(newElem);
		}
		else
		{
			if (tokenType != IDENTIFIER && tokenType != CONSTANT)
			{
				ero(16);
				return 0;
			}
		}
		if (tokenType == IDENTIFIER)
		{
			if (checkIdentTable(tokenName) == -1)
			{
				ero(7);
				return 0;
			}
			newElem.name = tokenName;
			newElem.type = "IDENTIFIER";
			exit.push_back(newElem);
		}
		newToken();
		if (tokenName != ",")
		{
			if (tokenName == ")")
			{
				break;
			}
			else
			{
				ero(12);
				return 0;
			}
		}
		newToken();
		if (stream_of_token.table.size() - 1 == tokNumber)
		{
			ero(12);
			return 0;
		}
	} while (tokenName != ")");
	newToken();
	if (tokenName != ";")
	{
		ero(2);
		return 0;
	}
	newElem.name = "write";
	newElem.type = "OPERATION";
	exit.push_back(newElem);
	return 1;
}
int sintAnalays::funChek()
{
	if (tokenName == "FUNC")
	{
		obrPolsk newElem;
		Ident a;
		a.type = tokenName;
		newToken();
		if (tokenType != IDENTIFIER)
		{
			ero(5);
			return 0;
		}
		newElem.name = "$" + tokenName;
		newElem.type = "LINK";
		exit.push_back(newElem);
		a.name = tokenName;
		newToken();
		if (tokenName != "(")
		{
			ero(4);
			return 0;
		}
		newToken();
		while (tokenName != ")")
		{
			if ((tokenName != "string") && (tokenName != "integer"))
			{
				ero(15);
				return 0;
			}
			newToken();
			if (tokenType != IDENTIFIER)
			{
				ero(15);
				return 0;
			}
			newToken();
			if (tokenName == ")")
			{
				break;
			}
			if (tokenName != ",")
			{
				ero(17);
				return 0;
			}
			if (stream_of_token.table.size() - 1 == tokNumber)
			{
				ero(12);
				return 0;
			}
			newToken();
		}
		newToken();
		if (tokenName != ";")
		{
			ero(15);
			return 0;
		}
		newToken();
		while (tokenName != "return")
		{
			if (BodyChek() == 0) {
				return 0;
			}
		}
		newToken();
		if (tokenType != IDENTIFIER && tokenType != CONSTANT)
		{
			ero(16);
			return 0;
		}
		if (tokenType == IDENTIFIER)
		{
			newElem.name = tokenName;
			newElem.type = "IDENTIFIER";
			exit.push_back(newElem);
		}
		else {
			newElem.name = tokenName;
			newElem.type = "CONSTANT";
			exit.push_back(newElem);
		}
		newElem.name = "=";
		newElem.type = "OPERATION";
		exit.push_back(newElem);
		newToken();
		if (tokenName != ";")
		{
			ero(2);
			return 0;
		}
		newToken();
		newElem.name = "$BACK";
		newElem.type = "MOVLINK";
		exit.push_back(newElem);
		if (tokenName != "ENDF")
		{
			ero(20);
			return 0;
		}
		newToken();
		if (tokenName != ";")
		{
			ero(2);
			return 0;
		}
		return 1;
	}
	return 0;
}
int sintAnalays::ero(int er)
{
	if (er == 1)
	{
		cout << "Нет PROGRAM в начале программы" << endl;
	}
	else if (er == 2)
	{
		cout << "Нет ';'" << endl;
	}
	else if (er == 3)
	{
		cout << "Неправильное объявление переменной" << endl;
	}
	else if (er == 4)
	{
		cout << "Ожидается (" << endl;
	}
	else if (er == 5)
	{
		cout << "Ожидается идендификатор" << endl;
	}
	else if (er == 6)
	{
		cout << "Ожидается =" << endl;
	}
	else if (er == 7)
	{
		cout << "Необъявленый идндификатор" << endl;
	}
	else if (er == 8)
	{
		cout << "Ожидается константа" << endl;
	}
	else if (er == 9)
	{
		cout << "Нет END" << endl;
	}
	else if (er == 10)
	{
		cout << "Ожидается :" << endl;
	}
	else if (er == 11)
	{
		cout << "Ожидается by" << endl;
	}
	else if (er == 12)
	{
		cout << "Ожидается )" << endl;
	}
	else if (er == 13)
	{
		cout << "нет }" << endl;
	}
	else if (er == 14)
	{
		cout << "втретилась константа" << endl;
	}
	else if (er == 15)
	{
		cout << "Неверное выражение" << endl;
	}
	else if (er == 16)
	{
		cout << "Не совпадение типов" << endl;
	}
	else if (er == 17)
	{
		cout << "Не верное построение" << endl;
	}
	else if (er == 18)
	{
		cout << "Ожидается ','" << endl;
	}
	else if (er == 19)
	{
		cout << "Идентификатор уже обЪявлен" << endl;
	}
	else if (er == 20)
	{
		cout << "Нет ENDF" << endl;
	}
	cout << "В строке " << lineNumber << endl;
	return er;
}
int sintAnalays::priorityChek(string sign)
{
	vector<string> priority = { "1", "=","-","+","*","/", "(",")", "FUNC" };
	for (int i = 0; i < priority.size(); i++)
	{
		if (sign == priority.at(i))
		{
			return i;
		}
	}
	return -1;
}
void sintAnalays::getExit()
{
	for (int i = 0; i < exit.size(); i++)
	{
		cout << setw(10) << exit.at(i).name << setw(20) << exit.at(i).type << endl;
	}
}
