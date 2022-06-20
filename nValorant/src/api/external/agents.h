#pragma once

// libs
#include <cpr/cpr.h>
#include <fmt/format.h>
#include <json/json.hpp>

// External
#include <string>
#include <fstream>
#include <vector>

// ...
#include "helper.h"

class CAgents 
{
public:
	CAgents() = default;

	struct Agent_t 
	{
		std::string uuid;
	};

	cpr::Response Get() const;
	cpr::Response GetSingle(const char* cardID) const;
	bool GetParsed();
	void Download(const char* cardID) const;

	std::vector<Agent_t> Agents;
private:

};