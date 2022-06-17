#include "player.h"

CSession::CSession(CApi* pApi)
{
	if (!pApi->isSuccessful)
		return;

	cpr::Response session = cpr::Get(
		cpr::Url
		{
			fmt::format("https://127.0.0.1:{0}/chat/v1/session", pApi->lockfilePort)
		},
		cpr::Header
		{
			{
				"Authorization", pApi->lockfilePassword
			}
		},
		cpr::VerifySsl(false));

	nlohmann::json sessionParsed = nlohmann::json::parse(session.text);
	federated = sessionParsed["federated"].get<bool>();
	loaded    = sessionParsed["loaded"].get<bool>();
	game_name = sessionParsed["game_name"].get<std::string>();
	game_tag  = sessionParsed["game_tag"].get<std::string>();
	name      = sessionParsed["name"].get<std::string>();
	pid       = sessionParsed["pid"].get<std::string>();
	puuid     = sessionParsed["puuid"].get<std::string>();
	region    = sessionParsed["region"].get<std::string>();
	resource  = sessionParsed["resource"].get<std::string>();
	state     = sessionParsed["state"].get<std::string>();
}