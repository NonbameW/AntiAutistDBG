#include "Scanner.h"
#include "Utils.h" 
#include "Windows.h"
#include "WtsApi32.h"
#include "string.h"
#include "vector"
#include "psapi.h"
#include "tchar.h"
#include "tlhelp32.h"
#include "Process.h"
#include <iostream> 
#pragma comment(lib, "WtsApi32.lib")

std::vector<IProcess> checkProcess(std::vector<std::string> pNms)
{
    WTS_PROCESS_INFO* pWPIs = NULL;
    DWORD dwProcCount = 0;
    std::vector<IProcess> pList;

    if (WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE, NULL, 1, &pWPIs, &dwProcCount))
    {


        for (DWORD i = 0; i < dwProcCount; i++)
        {
            HANDLE processHandle = NULL;
            TCHAR filename[MAX_PATH];

            processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pWPIs[i].ProcessId);
            if (processHandle != NULL)
            {
                if (GetModuleFileNameEx(processHandle, NULL, filename, MAX_PATH) != 0)
                {
                    std::string filePath = fls::toString(filename);
                    std::string processHash = fls::calculateSHA256(filePath);
                  
                    for (const std::string& dbgHash : pNms)
                    {
                        if (processHash == dbgHash)
                        {
                            std::string pName = fls::toString(pWPIs[i].pProcessName);
                            pList.push_back(IProcess{ (int)pWPIs[i].ProcessId, pName, filePath })
                                ;
                            
                            std::cout << "FUCKNIGGERS: " << pName << " (PID: " << pWPIs[i].ProcessId << ", Hash: " << processHash << ")" << std::endl;
                            
                            break; 
                        }
                    }
                }
                CloseHandle(processHandle);
            }
        }

        if (pWPIs)
        {
            WTSFreeMemory(pWPIs);
            pWPIs = NULL;
        }


    }
    else {
     
        DWORD error = GetLastError();
        std::cerr << "WTSEnumerateProcesses failed with error: " << error << std::endl;
    }

    return pList;
}


void detect(DWORD pid)
{
    HANDLE hndl = OpenProcess(PROCESS_TERMINATE, false, pid);

    TerminateProcess(hndl, 1);

    CloseHandle(hndl);
}