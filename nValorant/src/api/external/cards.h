#pragma once

// libs
#include <cpr/cpr.h>
#include <fmt/format.h>
#include <json/json.hpp>

// External
#include <string>
#include <fstream>

// ...
#include "helper.h"

class CCards 
{
public:
	CCards() = default;

	cpr::Response Get() const;
	cpr::Response GetSingle(const char* cardID) const;
    void Download(const char* cardID) const;
private:

};