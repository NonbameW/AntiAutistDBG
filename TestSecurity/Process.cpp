#include "Process.h"

IProcess::IProcess(int pId, std::string pName, std::string pPath) : m_Iid(pId), m_IName(pName), m_IPath(pPath) {};