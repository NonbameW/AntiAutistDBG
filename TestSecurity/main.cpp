#include "Process.h"
#include "debuggers.h"
#include "Windows.h"
#include "Scanner.h"
#include <memory>
#include <iostream>
#include <fstream>

#pragma warning(disable:4996)

int main(int argc, char* argv[])
{
	if (argc != 2) { return 1337; }


	while (not GetAsyncKeyState(VK_END))
	{
		for (IProcess p : checkProcess(getDbgsList()))
		{
			detect((DWORD)std::atoi(argv[1]));
		}
		Sleep(100);
	}
}