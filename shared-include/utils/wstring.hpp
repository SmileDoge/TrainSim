#pragma once

#include <codecvt>
#include <string>

namespace TSUtils
{
	std::wstring ConvertToWSTRFromUTF8(std::string utf8)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.from_bytes(utf8);
	}

	std::string ConvertToUTF8FromWSTR(std::wstring wstr)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.to_bytes(wstr);
	}
}