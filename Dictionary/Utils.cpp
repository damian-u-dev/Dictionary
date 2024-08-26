#include "PCH.h"
#include "Utils.h"

void PauseAndClearConsole()
{
	MakePause();
	ClearConsole();
}

void ClearConsoleAndPause()
{
	ClearConsole();
	MakePause();
}

void ClearConsole()
{
	system("cls");
}

void MakePause()
{
	system("pause");
}