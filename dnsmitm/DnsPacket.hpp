//
//  DnsPacket.hpp
//  dnsmitm
//
//  Created by System Administrator on 2017/4/27.
//  Copyright © 2017年 yangliqun. All rights reserved.
//

#ifndef DnsPacket_hpp
#define DnsPacket_hpp
#include <string>
#include <vector>
#include <sys/types.h>
namespace ylq {
    class DnsPacket
    {
    public:
        static std::vector<u_int8_t> response(const char *t_buff,const std::string &t_ip);
        static std::string domain(const char *t_buff);
    public:
        struct Query
        {
            std::vector<u_int8_t> m_name;
            u_int16_t m_type;
            u_int16_t m_class;
        };
        struct Answer
        {
            u_int16_t m_offset;
            u_int16_t m_type;
            u_int16_t m_class;
            u_int32_t m_timeToLive;
            u_int16_t m_dataLength;
            std::vector<u_int8_t> m_data;
        };
    private:
        u_int16_t m_transactionId;
        u_int16_t m_flags;
        u_int16_t m_questions;
        u_int16_t m_answerRRs;
        u_int16_t m_authorityRRs;
        u_int16_t m_additionalRRs;
        std::vector<Query> m_queries;
        std::vector<Answer> m_answeres;
        std::vector<u_int8_t> m_other;
    };
}

#endif /* DnsPacket_hpp */
