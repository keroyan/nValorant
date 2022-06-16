#include "authentication.h"
#include <json/json.hpp>
#include <base64/base64.h>
#include <iostream>
#include <filesystem>

#include <fmt/format.h>

template<typename T>
void PrintAny(T any)
{
	std::cout << any << std::endl;
}

void Auth::Perform() 
{
	// Stage 1: Get lockfile
	char* pLocalAppData;
	size_t len;
	_dupenv_s(&pLocalAppData, &len, "localappdata");

	if (pLocalAppData == nullptr)
	{
		PrintAny("Was unable to get localappdata which makes it impossible for us to connect!");
		return;
	}

	// We want it in std::string form as it makes it easier and we can free the memory
	std::string szLocalAppdata = pLocalAppData;
	free(pLocalAppData);

	// Path to lockfile
	std::string szLockFile(szLocalAppdata + "\\Riot Games\\Riot Client\\Config\\lockfile");
	if (!std::filesystem::exists(szLockFile))
	{
		PrintAny("Please make sure Valorant is running otherwise we are unable to connect!");
		return;
	}

	const auto readFile = [](std::string_view path)
	{
		constexpr auto read_size = std::size_t(4096);
		auto stream = std::ifstream(path.data());
		stream.exceptions(std::ios_base::badbit);

		auto out = std::string();
		auto buf = std::string(read_size, '\0');
		while (stream.read(&buf[0], read_size)) 
		{
			out.append(buf, 0, stream.gcount());
		}
		out.append(buf, 0, stream.gcount());
		return out;
	};
	std::string lockFileData = readFile(szLockFile).substr(5);

	// Stage 2: Parse

	// https://stackoverflow.com/a/46931770
	const auto Parse = [](std::string data, std::int32_t index)
	{
		std::int32_t i = 0;
		std::string token;
		size_t pos = 0;
		while ((pos = data.find(":")) != std::string::npos)
		{
			i++;
			token = data.substr(0, pos);

			if (index == i)
				return token;

			data.erase(0, pos + std::string(":").length());
		}
		return data;
	};
	
	const std::string&
		name     = Parse(lockFileData, 1),
		pid      = Parse(lockFileData, 2),
		port     = Parse(lockFileData, 3),
		password = Parse(lockFileData, 4),
		protocol = Parse(lockFileData, 5);

	cpr::Get(cpr::Url{
		fmt::format("https://127.0.0.1:{0}/{1}", port, "entitlements/v1/token")
		}, cpr::Header
		{
			{
				"Authorization", fmt::format("Basic {}", macaron::Base64().Encode(fmt::format("riot:{}", password)))
			}
		},
		cpr::VerifySsl(false));
}