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
		puuid,
		region,
		resource,
		state;
private:
};