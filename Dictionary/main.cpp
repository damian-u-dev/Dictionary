#include "PCH.h"
#include "Dictionary.h"
#include "Windows.h"
#include "limits"


void PrintMenu();
void HandleInputError();
void MainLoop();

int main()
{
	setlocale(LC_ALL, "ru");

	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	MainLoop();

	return 0;
}

void PrintMenu()
{
	cout << "Select what to do:\n";
	cout << "1. Add new word\n";
	cout << "2. Print all words\n";
	cout << "3. Save all words in the file\n";
	cout << "0. Exit\n";
}

void HandleInputError()
{
	cin.clear();
	cin.ignore(LLONG_MAX, '\n');

	cout << "You entered an incorrect number\n";
	system("pause");
	system("cls");
}

void MainLoop()
{
	Dictionary dictionary;

	bool isRunning = true;
	int userChoice{};

	while (isRunning)
	{
		PrintMenu();

		cin >> userChoice;

		if (!cin.good())
		{
			HandleInputError();
			continue;
		}

		cin.clear();
		cin.ignore(LLONG_MAX, '\n');

		switch (userChoice)
		{
		case 1:
		{
			dictionary.AddNewWord();
			break;
		}
		case 2:
		{
			dictionary.PrintAllWords();
			break;
		}
		case 3:
		{
			dictionary.SaveWordsInFile();
			break;
		}
		case 0:
		{
			isRunning = false;
			break;
		}
		default:
			cout << "Invalid choice.Try another\n";
			break;
		}

	}
}
