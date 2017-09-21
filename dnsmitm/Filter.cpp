//
//  Filter.cpp
//  dnsmitm
//
//  Created by System Administrator on 2017/4/27.
//  Copyright © 2017年 yangliqun. All rights reserved.
//

#include "Filter.hpp"
#include <sstream>
using namespace ylq;

std::string Filter::dnsFilePath("dns.txt");
Filter::Filter(const std::string &t_filePath)throw (const char *)
{
    std::ifstream ifile(dnsFilePath.c_str(),std::ios::in);
    std::string ip,domain;
    if(ifile.is_open()&&ifile.good())
    {
        while(!ifile.eof())
        {
            ifile>>ip>>domain;
            dnsmap.insert({domain,ip});
        }
    }
    else
    {
        throw "cant open dns.txt file";
    }
}

std::shared_ptr<Filter> Filter::getInstance()throw (const char *)
{
    static std::shared_ptr<Filter> instance(new Filter(dnsFilePath));
    return instance;
}

void Filter::filter(const std::string t_domain, std::function<void (std::string)> t_match, std::function<void ()> t_unmatch)const
{
    auto pos=dnsmap.find(t_domain);
    if(pos!=dnsmap.end())
    {
        t_match(pos->second);
    }
    else
    {
        t_unmatch();
    }
}

