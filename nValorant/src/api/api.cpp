#include "api.h"

void CApi::Connect() 
{
	if (!GetLockfilePath())
	{
		isSuccessful = false;
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

	std::string lockFileData = readFile(lockfilePath).substr(5);

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
		name = Parse(lockFileData, 1),
		pid = Parse(lockFileData, 2),
		port = Parse(lockFileData, 3),
		password = Parse(lockFileData, 4),
		protocol = Parse(lockFileData, 5);

	lockfilePort = port;
	lockfilePassword = fmt::format("Basic {}", macaron::Base64().Encode(fmt::format("riot:{}", password)));
	cpr::Response tokens = localRequest("entitlements/v1/token");

	if (tokens.status_code != cpr::status::HTTP_OK)
	{
		isSuccessful = false;
		return;
	}

	// Parse these tokens and save them
	nlohmann::json tokensParsed = nlohmann::json::parse(tokens.text);
	riotToken = tokensParsed["accessToken"].get<std::string>();
	entitlementToken = tokensParsed["token"].get<std::string>();
	isSuccessful = true;
}