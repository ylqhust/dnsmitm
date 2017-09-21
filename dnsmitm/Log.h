#pragma once
#include <string>
#include <sstream>
#include <mutex>

namespace ylq
{
	struct Test
	{
		int i;
#define alisaI i;
	};
	class Log
	{
	public:
		static std::shared_ptr<Log> &getInstance();
		std::string getLogString()const;
		Log &append(const std::string &);
		Log &operator<<(const std::string &);
		operator std::string();
		void clear();
	private:
		Log(){}


	private:
		static std::shared_ptr<Log> instance;
		static std::mutex m;

		std::ostringstream os;
	};
	
}
