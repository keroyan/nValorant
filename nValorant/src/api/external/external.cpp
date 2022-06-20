/*
	Full credits goes to valorant-api.com for their great api!
	The wrapper is made by me (sorry#0977)
*/

#include "cards.h"
#include "agents.h"

cpr::Response CCards::Get() const
{
	return cpr::Get(cpr::Url{ "https://valorant-api.com/v1/playercards" });
}

cpr::Response CCards::GetSingle(const char* cardID) const
{
	return cpr::Get(cpr::Url{ fmt::format("https://valorant-api.com/v1/playercards/{0}", cardID)});
}

void CCards::Download(const char* cardID) const
{
	// Get response
	cpr::Response card = GetSingle(cardID);

	// Parse response
	nlohmann::json cardParsed = nlohmann::json::parse(card.text);
	std::string displayIconUrl = cardParsed["data"]["displayIcon"].get<std::string>();

	// Get image
	std::ofstream of(fmt::format("C:\\Users\\cow\\source\\repos\\nValorant\\x64\\Release\\resources\\{0}.png", cardID), std::ios::binary);
	cpr::Download(of, cpr::Url{ displayIconUrl });
}

cpr::Response CAgents::Get() const
{
	return cpr::Get(cpr::Url{ "https://valorant-api.com/v1/agents?isPlayableCharacter=true" });
}

cpr::Response CAgents::GetSingle(const char* agentID) const
{
	return cpr::Get(cpr::Url{ fmt::format("https://valorant-api.com/v1/agents/{}", agentID) });
}

bool CAgents::GetParsed() 
{
	cpr::Response agents = Get();
	nlohmann::json agentsParsed = nlohmann::json::parse(agents.text);

	if (agents.status_code != cpr::status::HTTP_OK)
		return false;

	for (auto& agent : agentsParsed["data"])
	{
		Agents.push_back({
			agent["uuid"],
			});
	}

	return true;
}

void CAgents::Download(const char* agentID) const
{
	// Get response
	cpr::Response card = GetSingle(agentID);

	// Parse response
	nlohmann::json cardParsed = nlohmann::json::parse(card.text);
	std::string displayIconUrl = cardParsed["data"]["displayIcon"].get<std::string>();

	// Get image
	std::ofstream of(fmt::format("C:\\Users\\cow\\source\\repos\\nValorant\\x64\\Release\\resources\\{0}.png", agentID), std::ios::binary);
	cpr::Download(of, cpr::Url{ displayIconUrl });
}