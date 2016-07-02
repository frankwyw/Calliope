#include "Configuration.h"
#include <sstream>

Configuration::Configuration(){}
Configuration::~Configuration(){}

void Configuration::init()
{
    std::stringstream ss
    (R"{
        "base":{
            "worker_thread"   :   1,
            "error_log"   :   "/var/log/honoka/error.log",
            "worker_rlimit_nofile"    :   204800,
            "client_buffer"   :   4048
            },
        "epoll_option":{
            "worker_connection"   :   1024,
            "keepalive_timeout"   :   60
        },
        "listen":
        [{
            "socket":
            [{
                "domain"    :   "AF_INET",
                "type"    :     "socket_stream",
                "nonblocking":  true
            },
            {
                "ip"  :   "127.0.0.1"
            },
            {
                "port"    :   "all"
            }]
        }]
    }");

    ss>>*this;
}