#pragma once

#include <vector>
#include <string>

using namespace std;

class Dictionary
{
private:
	vector<pair<string, wstring>> Words;

	string PathForeignWords = R"(D:\ForeignWords.txt)";
	string PathTranslatedWords = R"(D:\TranslatedWords.txt)";

public:
	Dictionary();
};

