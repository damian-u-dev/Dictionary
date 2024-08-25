#include "PCH.h"
#include "Dictionary.h"
#include "Windows.h"

int main()
{
	setlocale(LC_ALL, "ru");
	
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);


	Dictionary dictonary;
	dictonary.AddNewWord();

	return 0;
}