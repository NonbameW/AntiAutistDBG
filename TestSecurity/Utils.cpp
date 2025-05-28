#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <locale>
#include <codecvt>

#include <Windows.h>
#include <Wincrypt.h>

namespace fls
{
    std::string wstringToString(const std::wstring& wstr)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr);
    }
    std::string toString(LPWSTR string)
    {
        if (string == nullptr) {
            return ""; 
        }
        return wstringToString(string);
    }

    std::string toString(TCHAR character)
    {
#ifdef UNICODE
        std::wstring wstr(1, character); 
        return wstringToString(wstr);
#else
        
        return std::string(1, character);
#endif
    }

    std::string calculateSHA256(const std::wstring& filePath)
    {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            return "";
        }

        HCRYPTPROV hProv = 0;
        HCRYPTHASH hHash = 0;
        BYTE rgbHash[32];
        DWORD cbHash = 32;
        std::stringstream ss;

        if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
            return ""; 
        }

        if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
            CryptReleaseContext(hProv, 0);
            return "";
        }

        char buffer[4096];
        while (file.read(buffer, sizeof(buffer))) {
            if (!CryptHashData(hHash, (BYTE*)buffer, file.gcount(), 0)) {
                CryptDestroyHash(hHash);
                CryptReleaseContext(hProv, 0);
                return "";
            }
        }
        if (file.gcount() > 0) {
            if (!CryptHashData(hHash, (BYTE*)buffer, file.gcount(), 0)) {
                CryptDestroyHash(hHash);
                CryptReleaseContext(hProv, 0);
                return "";
            }
        }

        if (!CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
            CryptDestroyHash(hHash);
            CryptReleaseContext(hProv, 0);
            return "";
        }

        for (int i = 0; i < 32; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)rgbHash[i];
        }

        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);

        return ss.str();
    }

    
    std::string calculateSHA256(const std::string& filePath) 
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::wstring wfilePath = converter.from_bytes(filePath);
        return calculateSHA256(wfilePath);
    }

    
}


