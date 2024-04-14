#pragma once

#include "modules/network/services/authserver.hpp"

#include "nlohmann/json.hpp"

class CAuthServer : public IAuthServer
{
public:
	CAuthServer(const std::string& endpoint);
	~CAuthServer() override;

	virtual AuthServerLoginState GetLoginState();
	virtual AuthServerAuthState GetAuthState();

	virtual AuthServerResult Login(const std::string& nickname);
	virtual AuthServerResult Login(const std::string& nickname, const std::string& password);

	virtual AuthServerResult GetConnectToken(std::string& token);
	virtual AuthServerResult VerifyConnectToken(const std::string& token, VerifyConnectTokenResponse& response);

	virtual AuthServerResult GetUserInfo(AuthServerUserInfo& info);

	virtual std::string& GetAPIEndpoint();

private:
	nlohmann::json PrepareRequestForConnectToken();
	AuthServerResult ParseResponseFromConnectToken(nlohmann::json& json, VerifyConnectTokenResponse& response);

	AuthServerLoginState login_state;
	AuthServerAuthState auth_state;

	std::string endpoint;

	AuthServerUserInfo info;

	std::string user_token;
};