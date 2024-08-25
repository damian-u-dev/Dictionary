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
