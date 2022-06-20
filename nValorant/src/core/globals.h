#pragma once
#include <api.h>

// external stuff like agent icons!
#include <external/agents.h>
#include <external/cards.h>

// player
#include <player/player.h>

// Store
#include <store/wallet.h>
#include <store/store.h>
#include <store/offers.h>

namespace Globals 
{
	// Authentication and primary stuff
	inline CApi Api;

	// external where no authentication is required
	inline CAgents Agents;
	inline CCards Cards;
}