//
//  Filter.hpp
//  dnsmitm
//
//  Created by System Administrator on 2017/4/27.
//  Copyright © 2017年 yangliqun. All rights reserved.
//

#ifndef Filter_hpp
#define Filter_hpp
#include <fstream>
#include <unordered_map>
#include <functional>

namespace ylq {
    class Filter
    {
        Filter(const std::string &t_filePath)throw (const char *);
    public:
        static std::shared_ptr<Filter> getInstance()throw (const char *);
        void filter(const std::string t_domain,std::function<void(std::string)> t_match,std::function<void()> t_unmatch)const;
    public:
        static std::string dnsFilePath;
    private:
        std::unordered_map<std::string, std::string> dnsmap;
    };
}

#endif /* Filter_hpp */
