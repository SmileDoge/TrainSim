#pragma once

#include <string>

enum AuthServerResult
{
	AUTH_RESULT_OK,

	AUTH_RESULT_ERROR,

	AUTH_RESULT_TIMED_OUT,
	AUTH_RESULT_INVALID_TOKEN,
	AUTH_RESULT_INVALID_CREDENTIALS,
	AUTH_RESULT_DISABLED,
};

enum AuthServerLoginState
{
	AUTH_SERVER_LOGIN_STATE_DISABLED,

	AUTH_SERVER_LOGIN_STATE_LOGGINED_AS_CLIENT,
	AUTH_SERVER_LOGIN_STATE_LOGGINED_AS_SERVER,
};

enum AuthServerAuthState
{
	AUTH_SERVER_AUTH_STATE_ONLY_NICKNAME,
	AUTH_SERVER_AUTH_STATE_NICKNAME_PASSWORD,
};

struct AuthServerUserInfo
{
	std::string nickname;
};

struct GetConnectTokenResponse
{
	std::string token;
};

struct VerifyConnectTokenResponse
{
	std::string nickname;
};

class IAuthServer
{
public:
	virtual ~IAuthServer() = default;

	virtual AuthServerLoginState GetLoginState() = 0;
	virtual AuthServerAuthState GetAuthState() = 0;

	virtual AuthServerResult Login(const std::string& nickname) = 0;
	virtual AuthServerResult Login(const std::string& nickname, const std::string& password) = 0;

	virtual AuthServerResult GetConnectToken(std::string& token) = 0;
	virtual AuthServerResult VerifyConnectToken(const std::string& token, VerifyConnectTokenResponse& response) = 0;

	virtual AuthServerResult GetUserInfo(AuthServerUserInfo& info) = 0;

	virtual std::string& GetAPIEndpoint() = 0;
};