//
//  Relay.hpp
//  dnsmitm
//
//  Created by yangliqun on 2017/4/27.
//  Copyright © 2017年 yangliqun. All rights reserved.
//

#ifndef Relay_hpp
#define Relay_hpp

#include "ThreadPool.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <string>
#include <algorithm>

namespace ylq {
    class Relay : public Runnable
    {
    public:
        Relay(int t_socketFd,const char *const t_buff,int t_buff_len,const sockaddr_in &t_client,const socklen_t &t_socklen);
        void operator()();
        
        static std::string reallyDNSServerIp;
    private:
        int m_socketFd;
        sockaddr_in m_client;
        socklen_t m_socklen;
        std::vector<char> m_buff;
    };
}


#endif /* Relay_hpp */
