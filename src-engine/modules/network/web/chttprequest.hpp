#pragma once

#include "modules/network/web/httprequest.hpp"

#include "curl/curl.h"

#include <thread>

class CHTTPRequest : public IHTTPRequest
{
public:
	CHTTPRequest();
	~CHTTPRequest() override;

	virtual void Execute(HTTPRequestExecuteMode mode = EXECUTE_MODE_ASYNC);

	virtual void SetTimeout(int timeout);

	virtual void SetHeader(const std::string& header);
	virtual void SetAuthHeader(const std::string& auth);
	virtual void SetAuthBearer(const std::string& auth);

	virtual void SetCallback(PROC_HTTPRequestCallback callback);

	virtual void SetURL(const std::string& url);
	virtual void SetMethod(const std::string& method);

	virtual void SetRequestBody(const char* body, size_t length = 0);
	virtual void SetRequestJson(nlohmann::json& json);

	virtual std::string& GetURL();
	virtual std::string& GetMethod();

	virtual int GetStatus();
	virtual long GetHTTPCode();

	virtual std::string& GetBody();
	virtual nlohmann::json GetJson();

	void PerformAsync();
	void CallCallback();

private:
	void Perform();

	void ExecuteSync();
	void ExecuteAsync();

	static size_t WriteCallback(char* contents, size_t size, size_t nmemb, CHTTPRequest* request);

	bool is_finished;

	PROC_HTTPRequestCallback callback;

	std::string url, method, body, json_body;

	HTTPRequestExecuteMode mode;

	CURL* curl;
	curl_slist* slist;
	CURLcode status;
};