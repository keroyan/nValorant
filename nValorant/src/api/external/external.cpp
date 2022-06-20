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
	return cpr::Get(cpr::Url{ fmt::format("https://valorant-api.com/v1/playercards/{}", cardID)});
}

void CCards::Download(const char* cardID) const
{
	std::ofstream of(fmt::format("{0}\\resources\\{1}.png", Helper::GetExecutablePath(), cardID), std::ios::binary);
	cpr::Download(of, cpr::Url{ fmt::format("https://valorant-api.com/v1/playercards/{}", cardID) });
}