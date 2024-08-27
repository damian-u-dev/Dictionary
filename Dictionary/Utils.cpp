#include "PCH.h"
#include "Utils.h"

void Utils::PauseAndClearConsole()
{
	MakePause();
	ClearConsole();
}

void Utils::ClearConsoleAndPause()
{
	ClearConsole();
	MakePause();
}

void Utils::ClearConsole()
{
	system("cls");
}

void Utils::MakePause()
{
	system("pause");
}