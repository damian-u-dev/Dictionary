#pragma once

#include <vector>
#include <string>
#include <istream>

using namespace std;

class Dictionary
{
private:
	vector<pair<string, wstring>> Words;

	string PathForeignWords = R"(D:\ForeignWords.txt)";
	string PathTranslatedWords = R"(D:\TranslatedWords.txt)";

public:
	Dictionary();
	~Dictionary();

	template<typename T, typename T2>
	void AskUserWord(T& UserWord, basic_istream<T2>& input, const string& messageForUser);
	bool IsUniqueWord(const string& Word);
	void AddNewWord();

	void OutPutAllWords() const;
	size_t GetWordsCount() const;
};

template<typename T, typename T2>
inline void Dictionary::AskUserWord(T& UserWord, basic_istream<T2>& input, const string& messageForUser)
{
	bool isWrongWord = true;

	while (isWrongWord)
	{
		cout << messageForUser << endl;

		getline(input, UserWord);
		if (!input.good())
		{
			input.clear();
			input.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "You wrote an incorrect word.";
		}
		else if (UserWord.size() > 1)
		{
			isWrongWord = false;
		}
		else
		{
			cout << "Your word is too short.\n";
			system("pause");
			system("cls");
		}
	}
}
