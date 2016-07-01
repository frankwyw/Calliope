#include "Singleton.hpp"
#include "3rd party/json.hpp"

class Configuration : public json,  public Singleton<Configuration>
{
public:
    Configuration(){}
    ~Configuration(){}
//应当将其初始化为某个默认json

};


/*
{
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
            "ip"  :   "127.0.0.1"
        },
        {
            "port"    :   "all"
        }]
    }]
}
*/
