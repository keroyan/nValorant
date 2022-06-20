#pragma once
#include <Windows.h>
#include <string>

namespace Helper 
{
	inline std::string GetExecutablePath() 
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		std::string::size_type pos = std::string(buffer).find_last_of("\\/");
		std::string dir = std::string(buffer).substr(0, pos);
		free(buffer);

		return dir;
	}
}