#pragma once
#include "api.h"

class CWallet 
{
public:
	CWallet(CApi* pApi);
	std::int32_t radianite, points;
};