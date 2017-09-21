//
//  main.cpp
//  dnsmitm
//
//  Created by yangliqun on 2017/4/27.
//  Copyright © 2017年 yangliqun. All rights reserved.
//

#include <iostream>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "ThreadPool.h"
#include "Relay.hpp"
#include "Filter.hpp"
#include "DnsPacket.hpp"
#include "Utils.h"

void option(int args,char **argv)
{
    for(int i=1;i<args;++i)
    {
        switch (argv[i][1]) {
            case 'f':
                ++i;
                ylq::Filter::dnsFilePath=argv[i];
                break;
            case 'd':
                ++i;
                ylq::Relay::reallyDNSServerIp=argv[i];
                break;
            default:
                break;
        }
    }
}
void helper()
{
    std::cout<<
"Usage:\n\
./dnsmitm -f dns.txt\n\
./dnsmitm -f dns.txt -d 8.8.8.8\n\
\n\
you can select a dns server ip address as the relay \n\
query server,the default dns server ip is 115.159.157.26\n\
you must give a dns.txt file\n\
the dns.txt should like this\n\
\n\
192.168.1.102   www.microsoft.com\n\
ip   domain\n"<<std::endl;
}
int main(int args,char **argv)
{
    ylq::Filter::dnsFilePath="/Users/yangliqun/Desktop/dns.txt";
    try {
        option(args,argv);
    } catch (const char *s) {
        helper();
        return EXIT_FAILURE;
    }
    int    socket_fd;
    struct sockaddr_in     servaddr;
    if( (socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
    servaddr.sin_port = htons(53);//设置的端口为DEFAULT_PORT
    if( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }
    printf("===waitting for dns data===\n");
    
    struct sockaddr_in client;
    socklen_t fromlen=sizeof(client);
    char    buff[4096];
    int     n;
    while(1)try
    {
        n=recvfrom(socket_fd,buff,sizeof(buff),0,(sockaddr *)&client,&fromlen);
        ylq::Filter::getInstance()->filter(ylq::DnsPacket::domain(buff), std::move([&](std::string t_ip)
        {
            ylq::Utils::log("filter find %s <---> %s\n",ylq::DnsPacket::domain(buff).c_str(),t_ip.c_str());
            std::vector<u_int8_t> response=ylq::DnsPacket::response(buff,t_ip);
            sendto(socket_fd, (const void *)&response[0], response.size(), 0, (sockaddr *)&client, fromlen);
        }), std::move([&]()
        {
            ylq::ThreadPool::getInstance()->addTask(std::shared_ptr<ylq::Relay>(new ylq::Relay(socket_fd,buff,n,client,fromlen)));
        }));
    }
    catch(const char *s)
    {
        std::cout<<"Error: "<<s<<std::endl;
        break;
    }
    close(socket_fd);
    return 0;
}  

