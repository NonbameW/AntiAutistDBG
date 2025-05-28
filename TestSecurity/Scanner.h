#pragma once
#include "Process.h"
#include <Windows.h>
#include <String>

std::vector<IProcess> checkProcess(std::vector<std::string> pNms);

void detect(DWORD pid);