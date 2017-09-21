#include "Utils.h"
#include <algorithm>
#include <stdarg.h>
using namespace ylq;

std::mutex Utils::s_logMutex;

uint16_t Utils::str2num(const std::string &t_str)throw(const char *)
{
	uint16_t result = 0;
	std::for_each(t_str.begin(), t_str.end(), [&](const char c)
	{
		if (!(c >= '0'&&c <= '9'))
			throw "t_str just allow 0-9 number";
		result = result * 10 + c - '0';
	});
	return result;
}
uint32_t Utils::ipStr2num(const std::string &t_ipStr)throw(const char *)
{
	uint32_t result = 0;
	uint32_t mask = 0;
	std::for_each(t_ipStr.begin(), t_ipStr.end(), [&](const char c)
	{
		if (c == '.')
		{
			result |= mask;
			result <<= 8;
			mask = 0;
			return;
		}
		if (!(c >= '0'&&c <= '9'))
			throw "t_ipStr just allow 0-9 & .";
		mask = mask * 10 + c - '0';
	});
	result |= mask;
	return result;
}


#include <stdio.h>
int Utils::log(const char *const t_format,...)
{
    std::unique_lock<std::mutex> lock(s_logMutex);
	int result;
	va_list _ArgList;
    va_start(_ArgList, t_format);
    result = vprintf(t_format, _ArgList);
    va_end(_ArgList);
	return result;
}


Trace::Trace(std::string &t_f) :s(t_f) 
{
	Utils::log((s + " begin\n").c_str());
}
Trace::~Trace()
{
	Utils::log((s + " end\n").c_str());
}
