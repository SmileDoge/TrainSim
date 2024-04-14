#include "cauthserver.hpp"

#include "modules/cnetworkmanager.hpp"
#include "modules/clogmodule.hpp"

CAuthServer::CAuthServer(const std::string& endpoint) : login_state(AUTH_SERVER_LOGIN_STATE_DISABLED), auth_state(AUTH_SERVER_AUTH_STATE_ONLY_NICKNAME)
{
	this->endpoint = endpoint;
}

CAuthServer::~CAuthServer()
{
}

AuthServerLoginState CAuthServer::GetLoginState()
{
	return login_state;
}

AuthServerAuthState CAuthServer::GetAuthState()
{
	return auth_state;
}

AuthServerResult CAuthServer::Login(const std::string& nickname)
{
	info.nickname = nickname;

	user_token = "";

	g_Log->LogInfo("[AUTH] Loggined as %s", nickname.c_str());

	return AUTH_RESULT_OK;
}

AuthServerResult CAuthServer::Login(const std::string& nickname, const std::string& password)
{
	return AUTH_RESULT_ERROR;
}

#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define JSON_EXCEPTION_HANDLE_BEING try {
#define JSON_EXCEPTION_HANDLE_END } catch (const nlohmann::json::parse_error& e) { g_Log->LogError("JSON PARSE ERROR [Line: %d, File: %s]: %s", __LINE__, __FILENAME__, e.what()); result = AUTH_RESULT_ERROR; }

AuthServerResult CAuthServer::GetConnectToken(std::string& token)
{
	auto request = g_NetworkManager->CreateHTTPRequest();

	request->SetTimeout(2);
	request->SetRequestJson(PrepareRequestForConnectToken());
	request->SetURL(endpoint + "get-connect-token");
	request->Execute(EXECUTE_MODE_SYNC);

	if (request->GetStatus() != CURLE_OK)
	{
		g_Log->LogError("GetConnectToken CURL ERROR: %d", request->GetStatus());

		delete request;

		return AUTH_RESULT_TIMED_OUT;
	}

	auto result = AUTH_RESULT_OK;

	JSON_EXCEPTION_HANDLE_BEING

		auto response_json = nlohmann::json::parse(request->GetBody());

		token = response_json["token"].get<std::string>();

	JSON_EXCEPTION_HANDLE_END

	delete request;

	return result;
}

AuthServerResult CAuthServer::VerifyConnectToken(const std::string& token, VerifyConnectTokenResponse& response)
{
	auto request = g_NetworkManager->CreateHTTPRequest();

	request->SetTimeout(2);
	request->SetURL(endpoint + "verify-connect-token");
	request->SetAuthBearer(token);
	request->Execute(EXECUTE_MODE_SYNC);

	if (request->GetStatus() != CURLE_OK)
	{
		g_Log->LogError("VerifyConnectToken CURL ERROR: %d", request->GetStatus());

		delete request;

		return AUTH_RESULT_TIMED_OUT;
	}

	auto result = AUTH_RESULT_OK;

	JSON_EXCEPTION_HANDLE_BEING

		auto response_json = nlohmann::json::parse(request->GetBody());

		result = ParseResponseFromConnectToken(response_json, response);

	JSON_EXCEPTION_HANDLE_END

	delete request;

	return result;
}

AuthServerResult CAuthServer::GetUserInfo(AuthServerUserInfo& info)
{
	info = this->info;

	return AUTH_RESULT_OK;
}

std::string& CAuthServer::GetAPIEndpoint()
{
	return endpoint;
}

nlohmann::json CAuthServer::PrepareRequestForConnectToken()
{
	auto json = nlohmann::json::object();

	json["nickname"] = info.nickname;

	return json;
}

AuthServerResult CAuthServer::ParseResponseFromConnectToken(nlohmann::json& json, VerifyConnectTokenResponse& response)
{
	auto is_valid = json["is_valid"].get<bool>();

	if (is_valid)
	{
		auto data = json["data"].get<nlohmann::json>();

		response.nickname = data["nickname"].get<std::string>();

		return AUTH_RESULT_OK;
	}
	else
	{
		return AUTH_RESULT_INVALID_TOKEN;
	}
}