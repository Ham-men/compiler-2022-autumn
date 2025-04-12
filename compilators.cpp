#include "translator.h"
#include <locale>
int main()
{
	setlocale(LC_ALL, "rus");
	Scaner A;
	A.scan();
	A.print_stream_of_lex();
	sintAnalays B(A.stream_of_token, A.table_of_identifier);
	if (B.analys() == 0)
	{
		cout << "Входная программа написана некорректно";
		return 0;
	};
	B.getExit();
	translator C(B.exit);
	C.translation();
	C.putInFile();
	system("pause");
	return 0;
}
