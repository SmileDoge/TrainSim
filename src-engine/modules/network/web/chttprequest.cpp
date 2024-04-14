#include "chttprequest.hpp"

#include "modules/cnetworkmanager.hpp"
#include "modules/clogmodule.hpp"
#include "modules/filesystem.hpp"

CHTTPRequest::CHTTPRequest() : callback(NULL), is_finished(false), mode(EXECUTE_MODE_ASYNC), status(CURLE_OK), slist(NULL)
{
	curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
}

CHTTPRequest::~CHTTPRequest()
{
	curl_slist_free_all(slist);

	curl_easy_cleanup(curl);
}

void CHTTPRequest::Execute(HTTPRequestExecuteMode mode)
{
	this->mode = mode;

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

	if (mode == EXECUTE_MODE_SYNC)
		ExecuteSync();
	else
		ExecuteAsync();
}

void CHTTPRequest::SetTimeout(int timeout)
{
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)timeout);
}

void CHTTPRequest::SetHeader(const std::string& header)
{
	slist = curl_slist_append(slist, header.c_str());
}

void CHTTPRequest::SetAuthHeader(const std::string& auth)
{
	std::string header = "Authorization: ";

	SetHeader(header + auth);
}

void CHTTPRequest::SetAuthBearer(const std::string& auth)
{
	std::string bearer = "Bearer ";

	SetAuthHeader(bearer + auth);
}

void CHTTPRequest::SetCallback(PROC_HTTPRequestCallback callback)
{
	this->callback = callback;
}

void CHTTPRequest::SetURL(const std::string& url)
{
	this->url = url;

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
}

void CHTTPRequest::SetMethod(const std::string& method)
{
	this->method = method;

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
}

void CHTTPRequest::SetRequestBody(const char* body, size_t length)
{
	if (length == 0)
		length = strlen(body);

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, length);
}

void CHTTPRequest::SetRequestJson(nlohmann::json& json)
{
	json_body = json.dump();

	SetHeader("Content-Type: application/json");

	SetRequestBody(json_body.c_str());
}

std::string& CHTTPRequest::GetURL()
{
	return url;
}

std::string& CHTTPRequest::GetMethod()
{
	return method;
}

int CHTTPRequest::GetStatus()
{
	return status;
}

long CHTTPRequest::GetHTTPCode()
{
	long code = 0;

	if (is_finished)
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);

	return code;
}

static std::string empty_string = "";

std::string& CHTTPRequest::GetBody()
{
	if (is_finished)
		return body;

	return empty_string;
}

nlohmann::json CHTTPRequest::GetJson()
{
	if (is_finished)
		return nlohmann::json::parse(body);

	return nlohmann::json();
}

void CHTTPRequest::PerformAsync()
{
	Perform();
}

void CHTTPRequest::CallCallback()
{
	if (callback != NULL)
		callback(this);
}

void CHTTPRequest::Perform()
{
	status = curl_easy_perform(curl);

	is_finished = true;
}

void CHTTPRequest::ExecuteSync()
{
	Perform();

	CallCallback();
}

void CHTTPRequest::ExecuteAsync()
{
	g_NetworkManager->AddHTTPRequestToQueue(this);
}

size_t CHTTPRequest::WriteCallback(char* contents, size_t size, size_t nmemb, CHTTPRequest* request)
{
	request->body.append(contents, size * nmemb);

	return size * nmemb;
}
