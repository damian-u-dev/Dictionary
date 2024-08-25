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

void Dictionary::AddNewWord()
{
	string UserMessage("Write a foreign word: ");
	
	string ForeignWord;
	AskUserWord(ForeignWord,cin,UserMessage);
	

	UserMessage = "Write a translation of \"" + ForeignWord + "\": ";
	
	wstring TranslatedWord;
	AskUserWord(TranslatedWord,wcin,UserMessage);

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
}

int Dictionary::GetWordsCount() const
{
	return Words.size();
}
