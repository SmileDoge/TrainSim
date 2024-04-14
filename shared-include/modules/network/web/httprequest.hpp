#pragma once

#include <string>

#include "nlohmann/json.hpp"

class IHTTPRequest;

typedef void (*PROC_HTTPRequestCallback)(IHTTPRequest* request);

enum HTTPRequestExecuteMode
{
	EXECUTE_MODE_SYNC,
	EXECUTE_MODE_ASYNC,
};

class IHTTPRequest
{
public:
	virtual ~IHTTPRequest() = default;

	virtual void Execute(HTTPRequestExecuteMode mode = EXECUTE_MODE_ASYNC) = 0;

	virtual void SetTimeout(int timeout) = 0;

	virtual void SetHeader(const std::string& header) = 0;
	virtual void SetAuthHeader(const std::string& auth) = 0;
	virtual void SetAuthBearer(const std::string& auth) = 0;

	virtual void SetCallback(PROC_HTTPRequestCallback callback) = 0;

	virtual void SetURL(const std::string& url) = 0;
	virtual void SetMethod(const std::string& method) = 0;

	virtual void SetRequestBody(const char* body, size_t length = 0) = 0;
	virtual void SetRequestJson(nlohmann::json& json) = 0;

	virtual std::string& GetURL() = 0;
	virtual std::string& GetMethod() = 0;

	virtual int GetStatus() = 0;
	virtual long GetHTTPCode() = 0;

	virtual std::string& GetBody() = 0;
	virtual nlohmann::json GetJson() = 0;
};