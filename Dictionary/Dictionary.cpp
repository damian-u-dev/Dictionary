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
	wstring TranslationWord;
		
	while (getline(ReaderFile, ForeignWord) && getline(ReaderFile2, TranslationWord))
	{
		Words.emplace_back(ForeignWord, TranslationWord);
	}
}