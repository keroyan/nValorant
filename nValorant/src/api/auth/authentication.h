#pragma once
#include <cpr/cpr.h>
#include <string>

class Auth
{
public:
	Auth(const std::string& username, const std::string& password)
	{
		this->username = username;
		this->password = password;
	}

	void Perform();
private:
	std::string username, password;
};