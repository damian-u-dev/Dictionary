#include "PCH.h"
#include "Dictionary.h"
#include "Utils.h"
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

bool Dictionary::IsUniqueWord(const string& word)
{
	for (auto Pair : Words)
	{
		if (Pair.first == word)
		{
			cout << "You're trying to enter a word that already has a translation\n";
			PauseAndClearConsole();
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
		string UserMessage("Write a foreign word: ");
		string ForeignWord;
		
		if (AskUserWord(ForeignWord, cin, UserMessage, "0"))
		{
			return;
		}
		if (!IsUniqueWord(ForeignWord))
		{
			continue;
		}
		
		UserMessage = "Write a translation of \"" + ForeignWord + "\": ";
		wstring TranslatedWord;
		
		if (AskUserWord(TranslatedWord, wcin, UserMessage,L"0"))
		{
			return;
		}
		Words.emplace_back(pair<string, wstring>(ForeignWord, TranslatedWord));
		ClearConsole();
	}
}

void Dictionary::PrintAllWords() const
{
	size_t wordCount = GetWordsCount();

	if (wordCount == 0)
	{
		cout << "You don't have any words in the dictinary.";
		PauseAndClearConsole();
		return;
	}

	cout << "All the words: \n";

	for (size_t i = 0; i < wordCount; i++)
	{
		cout << i + 1 << '.' << Words[i].first << " -> ";
		wcout << Words[i].second << endl;
	}

	cout << "\nYou have " << wordCount << " translated words in your dictionary.\n";
	MakePause();
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


	const char* filePath = "FileWithWords.txt";

	fstream outputFile(filePath, ios::out);
	wfstream outputFile2(filePath, ios::out | ios::app);

	outputFile2.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));

	if (!outputFile.is_open() || !outputFile2.is_open())
	{
		cout << "Couldn't create file.";
		return;
	}

	for (const auto& wordPair : Words)
	{
		outputFile << wordPair.first << " -> ";
		outputFile.flush();

		outputFile2 << wordPair.second << '\n';
		outputFile2.flush();

		outputFile.seekp(0, ios::end);
	}
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
		ClearConsole();
		cout << "You didn't guess try again!\n";
		PauseAndClearConsole();
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
		PauseAndClearConsole();
		break;
	}
	case Dictionary::StateOfGame::AnotherWord:
	{
		ClearConsole();
		cout << "This word was: " << guessedPair.first << " -> ";
		wcout << guessedPair.second << endl;
		PauseAndClearConsole();
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
