#include "scanner.h"
//ofstream fout("lexems.txt", ios_base::out);
Scaner::Scaner()
{
	keeper_last_char = '\0';
	in_main.open("main.txt", ios::in);
	if (!in_main.is_open())
	{
		cout << "Error opening file main.txt" << endl;
		std::system("pause");
		std::system("cls");
	}
	//fout.open("lexems.txt", ios_base::out);
}
Scaner::~Scaner()
{
	in_main.close();
	//fout.close();
}
string Scaner::get_lex()
{
	ofstream fout("lexems.txt", ios_base::out | ios_base::app);
	int state = BEGIN_STATE;
	string bufer, t_str;
	char c1, c2;
	//bool flag = true;
	c1 = keeper_last_char;
	static short strok = 1;
	//Пропуск пробелов табуляций и переносов строки преред лексемой
	do {
		if ((c1 == ' ') || (c1 == '\t') || (c1 == '\n'))
		{
			if (c1 == '\n') { strok++; }
			//flag = false;
			if (!in_main.eof())
				c1 = in_main.get();//Получение символа char
		}
		else
		{
			break;
		}
	} while (!in_main.eof());
	if (keeper_last_char == '\0')
	{
		if (!in_main.eof())
			c1 = in_main.get();//Получение символа char
	}
	if (!in_main.eof())
		c2 = in_main.get();//Получение символа char
	if ((c1 >= '0') && (c1 <= '9'))
		state = CONSTANT;
	int flag = 0;
	if ((c1 == '\'') && (c1 <= '"'))
	{
		state = CONSTANT;
		flag = 1;
	}
	while ((!in_main.eof()) || (c1 > 0))
	{
		//Начало коментария, если встречен с1 =/ с2 =/ Или с1 =/ с2 = *
		if (delete_coment(c1, c2))//Пропуск коментариев
		{
			//flag = true;
			if (!in_main.eof())
				c1 = in_main.get();
			//Пропуск пробелов табуляций и переносов строки преред лексемой после пропуска коментариев
				while (!in_main.eof())
				{
					if ((c1 == ' ') || (c1 == '\t') || (c1 == '\n'))
					{
						//flag = false;
						if (!in_main.eof())
							c1 = in_main.get();//Получение символа char
					}
					else
					{
						break;
					}
				}
			if (!in_main.eof())
				c2 = in_main.get();//Получение символа char2
			if ((c1 >= '0') && (c1 <= '9'))
				state = CONSTANT;
		}
		//получение лексемы
		t_str = c1;
		if (state == CONSTANT)
		{
			if (flag == 1)
			{
			}
			else if ((c1 < '0') && (c1 > '9'))
			{
				//error
				t_str = "";
				Token A(t_str, ERROR);
				stream_of_token += A;
				return t_str;
			}
		}
		else
		{
			if (!(((c1 >= 'A') && (c1 <= 'Z')) || ((c1 >= 'a') && (c1 <= 'z')) ||
				(table_of_dividers == t_str) || ((c1 >= '0') && (c1 <= '9'))))
			{
				//error
				t_str = "";
				Token A(t_str, ERROR);
				stream_of_token += A;
				return t_str;
			}
		}
		bufer += c1;
		t_str = c2;
		//Если с1 разделитель или с2 разделитель или пробел или табуляция или перенос строки получить лексему
		if (table_of_dividers == bufer)//
		{
			keeper_last_char = c2;
			//Разделитель
			Token A(bufer, DIVIDER);
			fout << "{3,";
			ifstream file("dividers.txt", ios::in);
			vector <string> temp(21);
			short i = 0;
			while (!file.eof())
			{
				file >> temp[i];
				if (temp[i] == bufer) fout << i + 1 << "," << strok << "} ";
				i++;
			}
			file.close();
			stream_of_token += A;
			return bufer;
		}
		if ((table_of_dividers == t_str) || (c2 == ' ') || (c2 == '\t') || (c2 =='\n'))
		{
			keeper_last_char = c2;
			//Константа или ID Или Ключевое слово
			if (state == CONSTANT)//Константа
			{
				Token A(bufer, CONSTANT);
				stream_of_token += A;
				if (!(table_of_constant == bufer))
				{
					Constant B(bufer);
					fout << "{5,";
					static short q = 0;
					q++;
					fout << q << "," << strok << "} ";
					table_of_constant += B;
				}
			}
			else
			{
				if (table_of_keyWords == bufer)//Ключевое слово
				{
					Token A(bufer, KEYWORD);
					fout << "{1,";
					ifstream file("keywords.txt", ios::in);
					vector <string> temp(17);
					short i = 0;
					while (!file.eof())
					{
						file >> temp[i];
						if (temp[i] == bufer) fout << i + 1 << "," << strok << "} "; i++;
					}
					file.close();
					stream_of_token += A;
				}
				else//Идентификатор
				{
					Token A(bufer, IDENTIFIER);
					fout << "{4,";
					ifstream file("id.txt", ios::in);
					vector <string> temp;
					string t;
					short i = 0;
					while (!file.eof())
					{
						file >> t;
						temp.push_back(t);
						if (temp[i] == bufer) fout << i + 1 << "," <<
							strok << "} ";
						i++;
					}
					file.close();
					if (checkIdentTab(bufer) == -1)
					{
						Ident B;
						if ((stream_of_token.table.at(stream_of_token.count - 1).name == "string") | (stream_of_token.table.at(stream_of_token.count - 1).name == "integer") | (stream_of_token.table.at(stream_of_token.count - 1).name == "FUNC"))
						{
							B.name = bufer;
							B.type =
							stream_of_token.table.at(stream_of_token.count - 1).name;
						}
						table_of_identifier.push_back(B);
					}
					stream_of_token += A;
				}
			}
			return bufer;
		}
		c1 = c2;
		if (!in_main.eof())
			c2 = in_main.get();
	}
	bufer = "";//При возвращении пустой строки - конечное состояние
	Token A(bufer, END_OF_PROGRAMM);
	stream_of_token += A;
	fout.close();
	return bufer;
}
int Scaner::checkIdentTab(string tokenName)
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
bool Scaner::delete_coment(char c1, char c2)
{
	if (c1 == '/')
	{
		if (c2 == '/')//Однострочный коментарий
		{
			char c1;
			do {
				if (!in_main.eof())
					c1 = in_main.get();
			} while (c1 != '\n');
			return true;
		}
		else
		{
			if (c2 == '*')//многострочный коментарий
			{
				while (!in_main.eof())
				{
					if (!in_main.eof())
						c1 = in_main.get();
					if (c1 == '*')
					{
						if (in_main.eof())
							return false;
						if (!in_main.eof())
							c2 = in_main.get();
						if (c2 == '/')
							return true;
					}
				}
			}
		}
	}
	return false;
}
void Scaner::scan()
{
	while (get_lex() != "") {}
}
void Scaner::print_stream_of_lex()
{
	string type;
	cout << setw(10) << "№" << setw(20) << "Name of token" << setw(20) << " Type" <<
		endl;
	for (int i = 0; i < stream_of_token.get_count() - 1; i++)
	{
		switch (stream_of_token[i].get_type())
		{
		case KEYWORD:
			type = "KEYWORD";
			break;
		case DIVIDER:
			type = "DIVIDER";
			break;
		case IDENTIFIER:
			type = "IDENTIFIER";
			break;
		case CONSTANT:
			type = "CONSTANT";
			break;
		case ERROR:
			//type = ERROR;
			cout << "ОШИБКА, невозможно обрабоать лексему " << endl;
			std::system("pause");
			std::exit(0);
			break;
		}
		cout << setw(10) << i << setw(20) << stream_of_token[i].get_name() <<
			setw(20) << type << endl;
	}
}
void Scaner::print_another_table()
{
	int i = 0;
	cout << endl << endl;
	cout << setw(10) << "№" << setw(20) << "Name of KeyWord" << endl;
	for (auto a : table_of_keyWords.get_vect_key_word())
	{
		cout << setw(10) << i << setw(20) << a << endl;
		i++;
	}
	cout << endl << endl;
	cout << setw(10) << "№" << setw(20) << "Name of Divider" << endl;
	for (auto a : table_of_dividers.get_vect_key_word())
	{
		cout << setw(10) << i << setw(20) << a << endl;
		i++;
	}
	cout << endl << endl;
	cout << setw(10) << "№" << setw(20) << "Value of constant" << endl;
	for (i = 0; i < table_of_constant.get_count(); i++)
		cout << setw(10) << i << setw(20) << table_of_constant[i].get_name() <<
		endl;
	cout << endl << endl;
	cout << setw(10) << "№" << setw(20) << "Name of Identifier" << endl;
	for (i = 0; i < table_of_identifier.size(); i++)
		cout << setw(10) << i << setw(20) << table_of_identifier.at(i).get_name()
		<< endl;
	cout << endl << endl;
}
