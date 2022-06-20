#pragma once
#include "api.h"

class CSession
{
public:
	CSession(CApi* pApi);

	bool
		federated,
		loaded;

	std::string
		game_name,
		game_tag,
		name,
		pid,
		puuid, // useful
		region,
		resource,
		state;
};

class CPlayer
{
public:
	CPlayer(CApi* pApi);

	std::string
		country,
		sub,
		preferred_username,
		player_locale,
		jti,
		username;
	
	bool
		email_verified,
		phone_number_verified;

	long long country_at;

	struct pw_t 
	{
		long long cng_at;
		bool reset, must_reset;
	};
	pw_t pw;

	// will be added in later updates
	struct ban_t
	{
	};
	ban_t ban;

	struct acct_t 
	{
		std::int32_t type;
		std::string state;
		bool adm;
		std::string game_name, tag_line;
		long long created_at;
	};
	acct_t acct;
};


class CLoadout 
{
public:
	CLoadout(CApi* pApi);

	struct Gun_t 
	{
		std::string 
			ID,
			SkinID,
			SkinLevelID,
			ChromaID;
	};

	std::vector<Gun_t> Guns;
private:

};