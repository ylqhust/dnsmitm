//
//  DnsPacket.cpp
//  dnsmitm
//
//  Created by System Administrator on 2017/4/27.
//  Copyright © 2017年 yangliqun. All rights reserved.
//

#include "DnsPacket.hpp"
#include <sstream>
#define DNS_DOMAIN_BEGIN_INDEX_IN_PACKET 13

using namespace ylq;

std::string DnsPacket::domain(const char *t_buff)
{
    std::ostringstream os;
    const char *p=t_buff+DNS_DOMAIN_BEGIN_INDEX_IN_PACKET;
    while(*p)
    {
        if(*p<' ')//space is the minest printable char
            os<<'.';
        else
            os<<*p;
        ++p;
    }
    return os.str();
}

std::vector<u_int8_t> DnsPacket::response(const char *t_buff, const std::string &t_ip)
{
    std::vector<u_int8_t> rp;
    //transactionId
    rp.push_back(t_buff[0]);
    rp.push_back(t_buff[1]);
    //flags
    rp.push_back(0x81);
    rp.push_back(0x80);//standary query ,no error ,learn from wireshark
    //Questions
    rp.push_back(0x00);
    rp.push_back(0x01);
    //Answers rrs
    rp.push_back(0x00);
    rp.push_back(0x01);
    //Authority rrs
    rp.push_back(0x00);
    rp.push_back(0x00);
    //additional rrs
    rp.push_back(0x00);
    rp.push_back(0x00);
    //queries
    int i=12;
    for(;t_buff[i]!=0;++i)
    {
        rp.push_back(t_buff[i]);
    }
    rp.push_back(t_buff[i++]);
    
    //type
    rp.push_back(t_buff[i++]);
    rp.push_back(t_buff[i++]);
    
    //class
    rp.push_back(t_buff[i++]);
    rp.push_back(t_buff[i++]);
    
    //answer offset
    rp.push_back(0xc0);
    rp.push_back(0x0c);
    //answer type
    rp.push_back(0x00);
    rp.push_back(0x01);//must be type A
    //answer class
    rp.push_back(0x00);
    rp.push_back(0x01);
    //answer time to live,i random a number
    rp.push_back(0x00);
    rp.push_back(0x00);
    rp.push_back(0x01);
    rp.push_back(0x10);
    //answer data length,must be 4
    rp.push_back(0x00);
    rp.push_back(0x04);
    //answer ip address
    u_int8_t num=0;
    for(unsigned char c:t_ip)
    {
        if(c=='.')
        {
            rp.push_back(num);
            num=0;
            continue;
        }
        num*=10;
        num+=c-'0';
    }
    rp.push_back(num);
    return rp;
}
