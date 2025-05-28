#pragma once
#include <string>
#include <windows.h>

namespace fls
{
    std::string toString(LPWSTR string);
    std::string toString(TCHAR string);

   
    std::string calculateSHA256(const std::string& filePath);
}