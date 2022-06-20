#pragma once

// Libraries
#include <cpr/cpr.h>
#include <json/json.hpp>
#include <base64/base64.h>
#include <fmt/format.h>

// external includes
#include <fstream>
#include <string>
#include <filesystem>

class CApi
{
public:
	CApi() = default;

	void Connect();
	std::string
		riotToken,
		entitlementToken,
		lockfilePassword,
		lockfilePath,
		lockfilePort,
		playerRegion = "eu";

	bool isSuccessful = false;

	cpr::Response localRequest(const char* path) 
	{
		cpr::Response req =
			cpr::Get(cpr::Url
				{
					fmt::format("https://127.0.0.1:{0}/{1}", lockfilePort, path)
				},
				cpr::Header
				{
					{
						"Authorization", lockfilePassword
					}
				},
				cpr::VerifySsl(false));

		return req;
	}
private:
	bool GetLockfilePath()
	{
		char* pLocalAppData;
		size_t len;
		_dupenv_s(&pLocalAppData, &len, "localappdata");

		if (pLocalAppData == nullptr)
			return false;

		// We want it in std::string form as it makes it easier and we can free the memory
		std::string szLocalAppdata = pLocalAppData;
		free(pLocalAppData);

		// Path to lockfile
		std::string LockFile(szLocalAppdata + "\\Riot Games\\Riot Client\\Config\\lockfile");
		if (!std::filesystem::exists(LockFile))
			return false;

		lockfilePath = LockFile;
		return true;
	}
};
