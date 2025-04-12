#include "translator.h"
translator::translator(vector<obrPolsk> exit)	//конструктор. инициализация переменных передаем обратнопольскую запись транслятору
{
	input = exit;
	forLink = 0;
	newElem.name = "fmt";
	newElem.type = "IDENTIFIER";
	data.push_back(newElem);
	newElem.name = "db";
	newElem.type = "OPERATION";
	data.push_back(newElem);
	newElem.name = "\"%d\",10,0";
	newElem.type = "CONSTANT";
	data.push_back(newElem);
}
translator::translator()
{
	forLink = 0;
}
void translator::getNewElem()	//берем новый элемент
{
	newElem = input.at(numberOperation);
	numberOperation++;
}
int translator::translation()	//в зависимости какой у нас тип элемента, то вызываем такую функцию
{
	while (numberOperation < input.size())
	{
		getNewElem();
		if ((newElem.type == "CONSTANT") || (newElem.type == "IDENTIFIER"))
		{
			STACK.push(newElem);
		}
		else if (newElem.type == "OPERATION")
		{
			doOperation();
		}
		else if ((newElem.type == "MOVELINK") || (newElem.type == "LINK"))
		{
			doLink();
		}
		else if (newElem.type == "FUNC")
		{
			doFunc();
		}
	}
	return 1;
}
int translator::doOperation() {	//берет обратно польскую запись.
	if (newElem.name == "=")	//переписать оператор присванивания
	{
		obrPolsk Right, Left;
		Right
			= STACK.top();
		STACK.pop();
		if (Right.type == "IDENTIFIRE")
		{
			MOV();
			newElem.name
				= "eax"
				;
			newElem.type
				= "REGISTER"
				;
			code.push_back(newElem);
			code.push_back(Right);
			Right
				= newElem;
		}
		Left
			= STACK.top();
		STACK.pop();
		MOV();
		code.push_back(Left);
		code.push_back(Right);
		return 1;
	}
	if (newElem.name == "+")//меняем операции на язык асм
	{
		ADD();
		return 1;
	}
	if (newElem.name == "-")
	{
		SUB();
		return 1;
	}
	if (newElem.name == "<")
	{
		JL();
		return 1;
	}
	if (newElem.name == ">")
	{
		JG();
		return 1;
	}
	if (newElem.name == "dd")	//резервируем память для инт
	{
		DD();
		return 1;
	}
	if (newElem.name == "db")//резервируем память для строки
	{
		DB();
		return 1;
	}
	if ((newElem.name == "write") | (newElem.name == "begwrite")) {//операция вывода на экран
		if (newElem.name == "begwrite")
		{
			numbWrit = numberOperation;	
		}
		else
		{
			doWrite();
		}
		return 1;
	}
	return 0;
}
int translator::MOV()//добавляем операциию в стек
{
	obrPolsk newElem1;
	newElem1.type = "OPERATION";
	newElem1.name = "mov";
	code.push_back(newElem1);
	return 1;
}
int translator::ADD()
{
	obrPolsk Right, Left, copyElem;
	int flag = 0;
	if ((STACK.top().type == "IDENTIFIER") | (STACK.top().type == "REGISTER"))
	{
		MOV();
		newElem.type = "REGISTER";
		newElem.name = "ebx";
		code.push_back(newElem);
		code.push_back(STACK.top());
		Right.name = "ebx";
		Right.type = "REGISTER";
	}
	else if (STACK.top().type == "CONSTANT")
	{
		flag = 1;
		Right = STACK.top();
	}
	STACK.pop();
	if (STACK.top().type == "IDENTIFIER")
	{
		MOV();
		newElem.type = "REGISTER";
		newElem.name = "eax";
		code.push_back(newElem);
		code.push_back(STACK.top());
		Left = newElem;
		copyElem = STACK.top();
	}
	else if (STACK.top().type == "CONSTANT")
	{
		if (flag == 1)
		{
			const char* c_inp = STACK.top().name.c_str();
			const char* c_inp2 = Right.name.c_str();
			newElem.name = to_string(atoi(c_inp) + atoi(c_inp2));
			newElem.type = "CONSTANT";
			STACK.pop();
			STACK.push(newElem);
			return 1;
		}
		else
		{
			MOV();
			newElem.type = "REGISTER";
			newElem.name = "eax";
			code.push_back(newElem);
			code.push_back(STACK.top());
			copyElem = STACK.top();
			Left = newElem;
		}
	}
	STACK.pop();
	newElem.type = "OPERATION";
	newElem.name = "add";
	code.push_back(newElem);
	code.push_back(Left);
	code.push_back(Right);
	if ((copyElem.type == "CONSTANT") | (copyElem.type == "REGISTER"))
	{
		newElem.type = "REGISTER";
		newElem.name = "eax";
		STACK.push(newElem);
	}
	else
	{
		MOV();
		newElem.type = "REGISTER";
		newElem.name = "eax";
		code.push_back(copyElem);
		code.push_back(newElem);
		STACK.push(copyElem);
	}
	return 1;
}
int translator::SUB()
{
	obrPolsk Right, Left, copyElem;
	int flag = 0;
	if ((STACK.top().type == "IDENTIFIER") | (STACK.top().type == "REGISTER"))
	{
		MOV();
		newElem.type = "REGISTER";
		newElem.name = "ebx";
		code.push_back(newElem);
		code.push_back(STACK.top());
		Right.name = "ebx";
		Right.type = "REGISTER";
	}
	else if (STACK.top().type == "CONSTANT")
	{
		flag = 1;
		Right = STACK.top();
	}
	STACK.pop();
	if (STACK.top().type == "IDENTIFIER")
	{
		MOV();
		newElem.type = "REGISTER";
		newElem.name = "eax";
		code.push_back(newElem);
		code.push_back(STACK.top());
		Left = newElem;
		copyElem = STACK.top();
	}
	else if (STACK.top().type == "CONSTANT")
	{
		if (flag == 1)
		{
			newElem.name = to_string(stoi(STACK.top().name) + stoi(Right.name));
			newElem.type = "CONSTANT";
			STACK.pop();
			STACK.push(newElem);
			return 1;
		}
		else
		{
			MOV();
			newElem.type = "REGISTER";
			newElem.name = "eax";
			code.push_back(newElem);
			code.push_back(STACK.top());
			copyElem = STACK.top();
			Left = newElem;
		}
	}
	STACK.pop();
	newElem.type = "OPERATION";
	newElem.name = "sub";
	code.push_back(newElem);
	code.push_back(Left);
	code.push_back(Right);
	if ((copyElem.type == "CONSTANT") | (copyElem.type == "REGISTER"))
	{
		newElem.type = "REGISTER";
		newElem.name = "eax";
		STACK.push(newElem);
	}
	else
	{
		MOV();
		newElem.type = "REGISTER";
		newElem.name = "eax";
		code.push_back(copyElem);
		code.push_back(newElem);
		STACK.push(copyElem);
	}
	return 1;
}
int translator::JL()
{
	obrPolsk Right, Left;
	int flag = 0;
	if ((STACK.top().type == "IDENTIFIER") | (STACK.top().type == "REGISTER"))
	{
		MOV();
		newElem.type = "REGISTER";
		newElem.name = "ebx";
		Right = newElem;
		code.push_back(newElem);
		newElem = STACK.top();
		code.push_back(newElem);
	}
	else
	{
		Right = STACK.top();
	}
	STACK.pop();
	if (STACK.top().type == "IDENTIFIER")
	{
		MOV();
		newElem.type = "REGISTER";
		newElem.name = "eax";
		code.push_back(newElem);
		code.push_back(STACK.top());
		Left = newElem;
	}
	else
	{
		Left = STACK.top();
	}
	STACK.pop();
	newElem.type = "OPERATION";
	newElem.name = "cmp";
	code.push_back(newElem);
	code.push_back(Left);
	code.push_back(Right);
	newElem.type = "OPERATION";
	newElem.name = "jl";
	return 1;
}
int translator::JG()
{
	obrPolsk Right, Left;
	int flag = 0;
	if ((STACK.top().type == "IDENTIFIER") | (STACK.top().type == "REGISTER"))
	{
		MOV();
		newElem.type = "REGISTER";
		newElem.name = "ebx";
		Right = newElem;
		code.push_back(newElem);
		newElem = STACK.top();
		code.push_back(newElem);
	}
	else
	{
		Right = STACK.top();
	}
	STACK.pop();
	if (STACK.top().type == "IDENTIFIER")
	{
		MOV();
		newElem.type = "REGISTER";
		newElem.name = "eax";
		code.push_back(newElem);
		code.push_back(STACK.top());
		Left = newElem;
	}
	else
	{
		Left = STACK.top();
	}
	STACK.pop();
	newElem.type = "OPERATION";
	newElem.name = "cmp";
	code.push_back(newElem);
	code.push_back(Left);
	code.push_back(Right);
	newElem.type = "OPERATION";
	newElem.name = "jg";
	code.push_back(newElem);
	return 1;
}
int translator::DD()
{
	data.push_back(STACK.top());
	STACK.pop();
	data.push_back(newElem);
	newElem.type = "OPERATION";
	newElem.name = "?";
	data.push_back(newElem);
	return 1;
}
int translator::DB()
{
	data.push_back(STACK.top());
	STACK.pop();
	data.push_back(newElem);
	newElem.type = "OPERATION";
	newElem.name = "?";
	data.push_back(newElem);
	return 1;
}
int translator::doLink()// for на асм. переносим в асм
{
	if (newElem.type == "LINK")
	{
		if (newElem.name == "$FOR")
		{
			forLink++;
			endForLink.push(forLink);
			newElem.name = "for" + to_string(forLink) + ":";
			code.push_back(newElem);
		}
		else if (newElem.name == "$ENDFOR")
		{
			newElem.name = "endfor" + to_string(endForLink.top()) + ":";
			code.push_back(newElem);
			endForLink.pop();
		}
		else
		{
			while (newElem.name != "$BACK")
			{
				getNewElem();
			}
		}
	}
	else if (newElem.type == "MOVELINK")
	{
		if (newElem.name == "$FOR")
		{
			newElem.type = "OPERATION";
			newElem.name = "jmp";
			code.push_back(newElem);
			newElem.type = "MOVELINK";
			newElem.name = "for" + to_string(endForLink.top());
			code.push_back(newElem);
		}
		if (newElem.name == "$INENDFOR")
		{
			newElem.name = "endfor" + to_string(forLink);
			code.push_back(newElem);
		}
	}
	return 1;
}
int translator::doFunc()
{
	obrPolsk funcElem, funcName = newElem;
	int Numb;
	funcElem = input.at(0);
	Numb = numberOperation;
	numberOperation = 0;
	while (funcElem.name != "$" + funcName.name)
	{
		funcElem = input.at(numberOperation);
		numberOperation++;
	}
	while (newElem.name != "$BACK")
	{
		getNewElem();
		if ((newElem.type == "CONSTANT") || (newElem.type == "IDENTIFIER"))
		{
			STACK.push(newElem);
		}
		else if (newElem.type == "OPERATION")
		{
			doOperation();
		}
		else if ((newElem.type == "MOVELINK") | (newElem.type == "LINK"))
		{
			doLink();
		}
		else if (newElem.type == "FUNC")
		{
			doFunc();
		}
	}
	STACK.push(input.at(numberOperation - 3));
	numberOperation = Numb;
	return 1;
}
int translator::doWrite()	
{
	obrPolsk Right;
	int colvo = numberOperation - numbWrit - 1;
	for (int i = 0; i < colvo; i++)
	{
		Right = STACK.top();
		STACK.pop();
		MOV();
		newElem.name = "eax";
		newElem.type = "REGISTER";
		code.push_back(newElem);		//в массив code записываем код на асм вместе с типами
		code.push_back(Right);
		newElem.name = "push";
		newElem.type = "OPERATION";
		code.push_back(newElem);
		newElem.name = "eax";
		newElem.type = "REGISTER";
		code.push_back(newElem);
		newElem.name = "push";
		newElem.type = "OPERATION";
		code.push_back(newElem);
		newElem.name = "offset fmt";
		newElem.type = "IDENTIFIER";
		code.push_back(newElem);
		newElem.name = "call";
		newElem.type = "OPERATION";
		code.push_back(newElem);
		newElem.name = "crt_printf";
		newElem.type = "IDENTIFIER";
		code.push_back(newElem);
		newElem.name = "sub";
		newElem.type = "OPERATION";
		code.push_back(newElem);
		newElem.name = "esp";
		newElem.type = "REGISTER";
		code.push_back(newElem);
		newElem.name = "8";
		newElem.type = "CONSTANT";
		code.push_back(newElem);
	}
	return 1;
}
int translator::putInFile()	//обрпольскую запись переписывает в код на асм. записывает в файл.
{
	ofstream fout;
	fout.open("Assembler.txt");
	fout << ".386\n.model flat, stdcall\ninclude c : \\masm32\\include\\msvcrt.inc\ninclude c : \\masm32\\include\\kernel32.inc\\nincludelib c: \\masm32\\lib\\msvcrt.lib\nincludelib c : \\masm32\\lib\\kernel32.lib"<<endl;
	fout << ".DATA" << endl;
	int i = 0;
	while (i < data.size())
	{
		newElem = data.at(i);
		fout << data.at(i).name;
		fout << " ";
		i++;
		fout << data.at(i).name;
		fout << " ";
		i++;
		fout << data.at(i).name << "\n";
		i++;
	}
	fout << ".CODE" << endl;
	i = 0;
	while (i < code.size())	
	{
		newElem = code.at(i);
		if ((newElem.name == "jmp") | (newElem.name == "jg") | (newElem.name == "jl") | (newElem.name == "push") | (newElem.name == "call"))
		{
			fout << code.at(i).name << " ";
			i++;
			fout << code.at(i).name << "\n";
			i++;
		}
		else if (newElem.type == "LINK")
		{
			fout << code.at(i).name << "\n";
			i++;
		}
		else
		{
			fout << code.at(i).name;
			fout << " ";
			i++;
			fout << code.at(i).name;
			fout << ", ";
			i++;
			fout << code.at(i).name << "\n";
			i++;
		}
	}
	fout.close();
	return 0;
}
