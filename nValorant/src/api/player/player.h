#pragma once
#include "api.h"

struct Gun_t
{
	std::string
		ID,
		SkinID,
		SkinLevelID,
		ChromaID;
};

struct Identity_t
{
	std::string
		PlayerCardID,
		PlayerTitleID,
		PreferredLevelBorderID;
	std::int32_t AccountLevel;
	bool HideAccountLevel;
};

struct pw_t
{
	long long cng_at;
	bool reset, must_reset;
};


struct acct_t
{
	std::int32_t type;
	std::string state;
	bool adm;
	std::string game_name, tag_line;
	long long created_at;
};

struct ban_t
{
};

struct Progress_t 
{
	std::int32_t 
		Level, 
		XP;
};

namespace Session 
{
	inline bool
		federated,
		loaded;

	inline std::string
		game_name,
		game_tag,
		name,
		pid,
		puuid, // useful
		region,
		resource,
		state;

	inline bool GetData(CApi* pApi)
	{
		if (!pApi->isSuccessful)
			return false;

		cpr::Response session = pApi->localRequest("chat/v1/session");
		if (session.status_code != cpr::status::HTTP_OK)
		{
			pApi->isSuccessful = false;
			return false;
		}

		nlohmann::json sessionParsed = nlohmann::json::parse(session.text);
		federated = sessionParsed["federated"].get<bool>();
		loaded = sessionParsed["loaded"].get<bool>();
		game_name = sessionParsed["game_name"].get<std::string>();
		game_tag = sessionParsed["game_tag"].get<std::string>();
		name = sessionParsed["name"].get<std::string>();
		pid = sessionParsed["pid"].get<std::string>();
		puuid = sessionParsed["puuid"].get<std::string>();
		region = sessionParsed["region"].get<std::string>();
		resource = sessionParsed["resource"].get<std::string>();
		state = sessionParsed["state"].get<std::string>();

		return true;
	}
}

namespace Player 
{
	inline std::string
		country,
		sub,
		preferred_username,
		player_locale,
		jti,
		username;

	inline bool
		email_verified,
		phone_number_verified;

	inline long long country_at;
	inline pw_t pw;
	inline ban_t ban;
	inline acct_t acct;

	inline bool GetData(CApi* pApi)
	{
		if (!pApi->isSuccessful)
			return false;

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
			return false;
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

		return true;
	}
}

namespace Loadout 
{
	inline Identity_t Identity;
	inline std::vector<Gun_t> Guns;

	inline bool GetData(CApi* pApi)
	{
		if (!pApi->isSuccessful)
			return false;

		Session::GetData(pApi);
		cpr::Response loadout =
			cpr::Get(cpr::Url
				{
					fmt::format("https://pd.{0}.a.pvp.net/personalization/v2/players/{1}/playerloadout", pApi->playerRegion, Session::puuid)
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
			return false;
		}

		nlohmann::json loadoutParsed = nlohmann::json::parse(loadout.text);

		for (auto& gun : loadoutParsed["Guns"])
		{
			Guns.push_back({
				gun["ID"],
				gun["SkinID"],
				gun["SkinLevelID"],
				gun["ChromaID"],
				});
		}

		Identity.PlayerCardID = loadoutParsed["Identity"]["PlayerCardID"].get<std::string>();
		Identity.PlayerTitleID = loadoutParsed["Identity"]["PlayerTitleID"].get<std::string>();
		Identity.AccountLevel = loadoutParsed["Identity"]["AccountLevel"].get<std::int32_t>();
		Identity.PreferredLevelBorderID = loadoutParsed["Identity"]["PreferredLevelBorderID"].get<std::string>();
		Identity.HideAccountLevel = loadoutParsed["Identity"]["HideAccountLevel"].get<bool>();

		return true;
	}
}

namespace Progression 
{
	inline Progress_t progress;

	inline bool GetData(CApi* pApi)
	{
		if (!pApi->isSuccessful)
			return false;

		// Using the remote url because the local one is really bad formatted
		cpr::Response progression =
			cpr::Get(cpr::Url
				{
					fmt::format("https://pd.{0}.a.pvp.net/account-xp/v1/players/{1}", pApi->playerRegion, Session::puuid)
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


		nlohmann::json progressionParsed = nlohmann::json::parse(progression.text);
		progress.Level = progressionParsed["Progress"]["Level"].get<std::int32_t>();
		progress.XP = progressionParsed["Progress"]["XP"].get<std::int32_t>();

		return true;
	}
}