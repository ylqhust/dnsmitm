//
//  Relay.cpp
//  dnsmitm
//
//  Created by yangliqun on 2017/4/27.
//  Copyright © 2017年 yangliqun. All rights reserved.
//

#include "Relay.hpp"
#include "Utils.h"
#include <time.h>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <chrono>
#include "DnsPacket.hpp"

using namespace ylq;

std::string Relay::reallyDNSServerIp("115.159.157.26");//shanghai tencent
Relay::Relay(int t_socketFd,const char *const t_buff,int t_buff_len,const sockaddr_in &t_client,const socklen_t &t_socklen):
m_socketFd(t_socketFd),
m_buff(t_buff_len,0),
m_client(t_client),
m_socklen(t_socklen)
{
    std::copy(t_buff, t_buff+t_buff_len, m_buff.begin());
}

void Relay::operator()()
{
    
    int asClientSocketFd = socket(AF_INET, SOCK_DGRAM, 0);
#ifdef DEBUG
    Utils::log("query %d %s\n",asClientSocketFd,ylq::DnsPacket::domain(&m_buff[0]).c_str());
#endif
    if(asClientSocketFd==-1)
    {
        Utils::log("asClientSocketFd create failed\n");
        return;
    }
    sockaddr_in server,peer;
    socklen_t socklen;
    
    server.sin_family=AF_INET;
    server.sin_port=htons(53);
    server.sin_addr.s_addr=htonl(Utils::ipStr2num(reallyDNSServerIp.c_str()));
    
    //just send client data to google dns server
    sendto(asClientSocketFd, (const void *)&m_buff[0], m_buff.size(), 0, (sockaddr *)&server, sizeof(server));
    
    
    //recv data from google server
    char buff[4096];
    int recvlen;
    std::thread closeSocket([](int ascsfd)
    {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        close(ascsfd);
    },asClientSocketFd);
    closeSocket.detach();
    
    
    recvlen = recvfrom(asClientSocketFd, buff, sizeof(buff), 0, (sockaddr *)&peer, &socklen);
    
    if(recvlen>0)
    {
#ifdef DEBUG
        Utils::log("response %d %s\n",asClientSocketFd,ylq::DnsPacket::domain(&m_buff[0]).c_str());
#endif
        //send dns data to cient
        sendto(asClientSocketFd, buff, recvlen, 0, (sockaddr *)&m_client, m_socklen);
    }
}
