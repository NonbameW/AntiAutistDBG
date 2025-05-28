#pragma once
#include <vector>
#include <string>

class IProcess
{
	int m_Iid;
	std::string m_IName;
	std::string m_IPath;

public:

	IProcess(int pId = 0, std::string pName = "None", std::string pPath = "None");

	int getId() { return this->m_Iid; }

	std::string getName() { return this->m_IName; }

	std::string getPath() { return this->m_IPath; }
};