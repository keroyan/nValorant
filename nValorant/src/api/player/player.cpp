#include "player.h"

CPlayer::CPlayer(CApi* pApi)
{
	if (!pApi->isSuccessful)
		return;

	// Using the remote url because the local one is really bad formatted
	cpr::Response player =
		cpr::Get(cpr::Url
			{
				"https://auth.riotgames.com/userinfo"
			},
			cpr::Bearer
			{
				pApi->riotToken
			});

	if (player.status_code != cpr::status::HTTP_OK)
	{
		pApi->isSuccessful = false;
		return;
	}

	nlohmann::json playerParsed = nlohmann::json::parse(player.text);
	country = playerParsed["country"].get<std::string>();
	sub = playerParsed["sub"].get<std::string>();
	email_verified = playerParsed["email_verified"].get<bool>();
	country_at = playerParsed["country_at"].get<long long>();
	username = playerParsed["username"].get<std::string>();
	preferred_username = playerParsed["preferred_username"].get<std::string>();
	jti = playerParsed["jti"].get<std::string>();

	// pw
	pw.cng_at = playerParsed["pw"]["cng_at"].get<long long>();
	pw.reset = playerParsed["pw"]["reset"].get<bool>();
	pw.must_reset = playerParsed["pw"]["must_reset"].get<bool>();

	// ban

	// acct
	acct.type = playerParsed["acct"]["type"].get<std::int32_t>();
	acct.state = playerParsed["acct"]["state"].get<std::string>();
	acct.adm = playerParsed["acct"]["adm"].get<bool>();
	acct.game_name = playerParsed["acct"]["game_name"].get<std::string>();
	acct.tag_line = playerParsed["acct"]["tag_line"].get<std::string>();
	acct.created_at = playerParsed["acct"]["created_at"].get<long long>();
}

CSession::CSession(CApi* pApi)
{
	if (!pApi->isSuccessful)
		return;

	cpr::Response session = pApi->localRequest("chat/v1/session");
	if (session.status_code != cpr::status::HTTP_OK)
	{
		pApi->isSuccessful = false;
		return;
	}

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

CLoadout::CLoadout(CApi* pApi)
{
	if (!pApi->isSuccessful)
		return;

	CSession session(pApi);
	cpr::Response loadout =
		cpr::Get(cpr::Url
			{
				fmt::format("https://pd.{0}.a.pvp.net/personalization/v2/players/{1}/playerloadout", pApi->playerRegion, session.puuid)
			},
			cpr::Header
			{
				{
					"X-Riot-Entitlements-JWT", pApi->entitlementToken
				}	
			},
			cpr::Bearer
			{
				pApi->riotToken
			});

	if (loadout.status_code != cpr::status::HTTP_OK)
	{
		pApi->isSuccessful = false;
		return;
	}

	nlohmann::json loadoutParsed = nlohmann::json::parse(loadout.text);
	nlohmann::json gunsParsed = loadoutParsed["Guns"];

	for (auto& gun : gunsParsed)
	{	
		Guns.push_back({
			gun["ID"],
			gun["SkinID"],
			gun["SkinLevelID"],
			gun["ChromaID"],
			});
	}

	// Got a few work!
}