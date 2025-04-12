#pragma once
#include "syntanalys.h"
class translator
{
	vector<obrPolsk> input;// обрпольская на входе
	vector<obrPolsk> data;// выходной вектор блока данных
	vector<obrPolsk> code;// выходной вектор блока кода
	stack<obrPolsk> STACK;// стек для разворота обрпольской
	stack<int> endForLink;// стек для корректной работы ссылки для форов
	int forLink;// номер фора
	int numbWrit;
	int numberOperation;// номер операции из обрпольской
	obrPolsk newElem;// новый элемент
public:
	translator();// конструткор тр-а
	translator(vector<obrPolsk> exit); // конструктор с параметром интерпритатора
	int translation(); // основная ф-я тр-а
	int doOperation();// перевод операций
	int doLink();// построение ссылки
	int doFunc(); // работа с функцией
	int doWrite(); // построение вывода на консоль
	int MOV(); // вывод мув
	int ADD(); // построение ад
	int SUB(); // построение вычитания
	int JL(); // построения <3
	int JG(); // postroenie >
	int DD(); // postroenie opisaniya peremennoy
	int DB();// postroenie opisaniya peremennoy tip string
	int putInFile(); // pomestit` rez v fail
	void getNewElem();// vz9t` new elem obrpolsk
};
