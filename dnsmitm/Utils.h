#pragma once
#include <string>
#include <mutex>

#ifndef DEBUG
#define DEBUG
#endif
namespace ylq
{
	struct Trace
	{
		Trace(std::string &t_functionName);
		~Trace();
		std::string s;
	};
	struct Utils
	{
		static uint16_t str2num(const std::string &t_str) throw(const char *);
		static uint32_t ipStr2num(const std::string &t_ipStr) throw(const char *);
		static int log(const char *const t_format,...);
		static std::mutex s_logMutex;
	};
};
