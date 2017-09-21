#include "Log.h"
using namespace ylq;

std::shared_ptr<Log> Log::instance(nullptr);
std::mutex Log::m;

std::shared_ptr<Log> &Log::getInstance()
{
	if(instance.get()==nullptr)
	{
		std::lock_guard<std::mutex> t(m);
		if(instance.get()==nullptr)
		{
			instance.reset(new Log);
		}
	}
	return instance;
}

std::string Log::getLogString() const
{
	std::lock_guard<std::mutex> t(m);
	return os.str();
}

Log &Log::append(const std::string &t_str)
{
	std::lock_guard<std::mutex> t(m);
	os << t_str;
	return *this;
}

Log &Log::operator<<(const std::string &t_str)
{
	append(t_str);
	return *this;
}

Log::operator std::string()
{
	return getLogString();
}

void Log::clear()
{
	std::lock_guard<std::mutex> t(m);
	os.clear();
}







