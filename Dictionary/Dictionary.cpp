#include "PCH.h"
#include "Dictionary.h"
#include <fstream>
#include <locale>
#include <codecvt>

Dictionary::Dictionary()
{
	fstream ReaderFile(PathForeignWords, ios::in | ios::app);
	wfstream ReaderFile2(PathTranslatedWords, ios::in | ios::app);

	ReaderFile2.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));


	if (!ReaderFile.is_open() || !ReaderFile2.is_open())
	{
		return;
	}

	string ForeignWord;
	wstring TranslatedWord;

	while (getline(ReaderFile, ForeignWord) && getline(ReaderFile2, TranslatedWord))
	{
		Words.emplace_back(ForeignWord, TranslatedWord);
	}
}

Dictionary::~Dictionary()
{
	fstream FileForeignWords(PathForeignWords, ios::out | ios::trunc);
	wfstream FileTranslatedWords(PathTranslatedWords, ios::out | ios::trunc);

	FileTranslatedWords.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));

	if (!FileForeignWords.is_open() || !FileTranslatedWords.is_open())
	{
		cout << "Couldn't open files\n";
		return;
	}

	for (size_t i = 0; i < GetWordsCount(); i++)
	{
		FileForeignWords << Words[i].first << '\n';
		FileTranslatedWords << Words[i].second << '\n';
	}
}

bool Dictionary::IsUniqueWord(const string& Word)
{
	for (auto Pair : Words)
	{
		if (Pair.first == Word)
		{
			cout << "You're trying to enter a word that already has a translation\n";
			system("pause");
			system("cls");
			return false;
		}
	}
	return true;
}

void Dictionary::AddNewWord()
{
	string UserMessage("Write a foreign word: ");

	string ForeignWord;

	do
	{
		AskUserWord(ForeignWord, cin, UserMessage);
	} while (!IsUniqueWord(ForeignWord));


	UserMessage = "Write a translation of \"" + ForeignWord + "\": ";

	wstring TranslatedWord;
	AskUserWord(TranslatedWord, wcin, UserMessage);

	Words.emplace_back(pair<string, wstring>(ForeignWord, TranslatedWord));
}

void Dictionary::OutPutAllWords() const
{
	size_t wordCount = GetWordsCount();

	if (wordCount == 0)
	{
		cout << "You don't have any words in the dictinary.";
		system("pause");
		system("cls");
		return;
	}

	cout << "All the words: \n";

	for (size_t i = 0; i < wordCount; i++)
	{
		cout << i + 1 << '.' << Words[i].first << " -> ";
		wcout << Words[i].second << endl;
	}

	cout << "\nYou have " << wordCount << " translated words in your dictionary.\n";
	system("pause");
	system("cls");
}

size_t Dictionary::GetWordsCount() const
{
	return Words.size();
}
