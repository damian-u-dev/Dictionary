#include "PCH.h"
#include "Dictionary.h"
#include "Utils.h"
#include <fstream>
#include <locale>
#include <codecvt>

Dictionary::Dictionary()
{
	fstream fileForeignWords(PathForeignWords, ios::in | ios::app);
	wfstream fileTranslatedWords(PathTranslatedWords, ios::in | ios::app);

	fileTranslatedWords.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));


	if (!fileForeignWords.is_open() || !fileTranslatedWords.is_open())
	{
		return;
	}

	string foreignWord;
	wstring translatedWord;

	while (getline(fileForeignWords, foreignWord) && getline(fileTranslatedWords, translatedWord))
	{
		Words.emplace_back(foreignWord, translatedWord);
	}
}

Dictionary::~Dictionary()
{
	fstream fileForeignWords(PathForeignWords, ios::out | ios::trunc);
	wfstream fileTranslatedWords(PathTranslatedWords, ios::out | ios::trunc);

	fileTranslatedWords.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));

	if (!fileForeignWords.is_open() || !fileTranslatedWords.is_open())
	{
		cout << "Couldn't open files\n";
		return;
	}

	for (size_t i = 0; i < GetWordsCount(); i++)
	{
		fileForeignWords << Words[i].first << '\n';
		fileTranslatedWords << Words[i].second << '\n';
	}
}

bool Dictionary::IsUniqueWord(const string& word)
{
	for (auto pairWords : Words)
	{
		if (pairWords.first == word)
		{
			cout << "You're trying to enter a word that already has a translation\n";
			Utils::PauseAndClearConsole();
			return false;
		}
	}
	return true;
}

void Dictionary::AddNewWord()
{
	cout << "Tip: If you want to exit write: \"0\"" << endl;
	
	while (true)
	{
		string userMessage("Write a foreign word: ");
		string foreignWord;
		
		if (AskUserWord(foreignWord, cin, userMessage, "0"))
		{
			return;
		}
		if (!IsUniqueWord(foreignWord))
		{
			continue;
		}
		
		userMessage = "Write a translation of \"" + foreignWord + "\": ";
		wstring translatedWord;
		
		if (AskUserWord(translatedWord, wcin, userMessage,L"0"))
		{
			return;
		}
		Words.emplace_back(pair<string, wstring>(foreignWord, translatedWord));
		Utils::ClearConsole();
	}
}

void Dictionary::PrintAllWords() const
{
	size_t wordCount = GetWordsCount();

	if (wordCount == 0)
	{
		cout << "You don't have any words in the dictinary.";
		Utils::PauseAndClearConsole();
		return;
	}

	cout << "All the words: \n";

	for (size_t i = 0; i < wordCount; i++)
	{
		cout << i + 1 << '.' << Words[i].first << " -> ";
		wcout << Words[i].second << endl;
	}

	cout << "\nYou have " << wordCount << " translated words in your dictionary.\n";
	Utils::MakePause();
}

size_t Dictionary::GetWordsCount() const
{
	return Words.size();
}

void Dictionary::SaveWordsInFile() const
{
	if (GetWordsCount() == 0)
	{
		cout << "You don't have any words to save them.\n";
		return;
	}

	fstream writingForeignWord(FileWords, ios::out);
	wfstream writingTranslatedWord(FileWords, ios::out | ios::app);

	writingTranslatedWord.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));

	if (!writingForeignWord.is_open() || !writingTranslatedWord.is_open())
	{
		cout << "Couldn't create file.";
		return;
	}

	for (const auto& wordPair : Words)
	{
		writingForeignWord << wordPair.first << " -> ";
		writingForeignWord.flush();

		writingTranslatedWord << wordPair.second << '\n';
		writingTranslatedWord.flush();

		writingForeignWord.seekp(0, ios::end);
	}
	cout << "File was created successful.\n";
	Utils::MakePause();
}

int Dictionary::GenerateRandomIndex(int oldGuessedIndex) const
{
	int newGuessedIndex{};
	srand(static_cast<unsigned int>(time(nullptr)));

	do
	{
		newGuessedIndex = rand() % GetWordsCount();
	} while (newGuessedIndex == oldGuessedIndex);

	return newGuessedIndex;
}

Dictionary::StateOfGame Dictionary::ProcessUserInput(const pair<string, wstring>& newGuessedPair) const
{
	cout << "Write a translation of this word: \"" << newGuessedPair.first << "\"" << endl;
	cout << "0. To stop guess words\n";
	cout << "1. Guess another word\n";

	wstring userWord;
	getline(wcin, userWord);

	if (userWord == newGuessedPair.second)
	{
		return StateOfGame::Guessed;
	}
	else if (userWord == L"0")
	{
		return StateOfGame::StopGame;
	}
	else if (userWord == L"1")
	{
		return StateOfGame::AnotherWord;
	}
	else
	{
		Utils::ClearConsole();
		cout << "You didn't guess try again!\n";
		Utils::PauseAndClearConsole();
		return StateOfGame::Working;
	}
}

void Dictionary::ProcessGameState(StateOfGame gameState, const pair<string, wstring>& guessedPair, bool& isGameActive) const
{
	switch (gameState)
	{
	case Dictionary::StateOfGame::Guessed:
	{
		cout << "\nYou guessed!\n";
		Utils::PauseAndClearConsole();
		break;
	}
	case Dictionary::StateOfGame::AnotherWord:
	{
		Utils::ClearConsole();
		cout << "This word was: " << guessedPair.first << " -> ";
		wcout << guessedPair.second << endl;
		Utils::PauseAndClearConsole();
		break;
	}
	case Dictionary::StateOfGame::StopGame:
	{
		isGameActive = false;
		break;
	}
	}
}

void Dictionary::GuessTranslatedWord() const
{
	int oldGuessedIndex = -1;

	bool isGameActive = true;
	do
	{
		int newGuessedIndex = GenerateRandomIndex(oldGuessedIndex);
		oldGuessedIndex = newGuessedIndex;
		const auto& newGuessedPair = Words[newGuessedIndex];

		StateOfGame gameState = StateOfGame::Working;
		while (gameState == StateOfGame::Working)
		{
			gameState = ProcessUserInput(newGuessedPair);
			ProcessGameState(gameState, newGuessedPair, isGameActive);
		}
	} while (isGameActive);
}
