#include "Configuration.h"

#include <sstream>
#include <fstream>

#include <glog/logging.h>

namespace honoka
{
    Configuration::Configuration()
    {

    }
    Configuration::~Configuration(){}

    void Configuration::init()
    {
	DLOG(INFO)<<"Configuration::init()";
        std::stringstream ss
	(R"(
	{
    "base": {
        "worker_thread": 1,
        "error_log": "/var/log/honoka/error.log",
        "worker_rlimit_nofile": 204800,
        "client_buffer": 4048
    },
    "epoll_option": {
        "worker_connection": 1024,
        "keepalive_timeout": 60
    },
    "listen": [
           {
                "nonblocking": true,
                "ip": "127.0.0.1",
                "port": 10008
            }
            ]
        }
	)");

	ss >> json_;
    }

    void Configuration::init(std::string filename)
    {
        DLOG(INFO)<<"Configuration::init(std::string)";
        std::fstream fs(filename);
        fs>>json_;
        fs.close();
    }
    void Configuration::init(char* filename)
    {
        DLOG(INFO)<<"Configuration::init(char*)";
        std::fstream fs(filename);
        fs>>json_;
        fs.close();
    }

    std::string Configuration::get_ip(int i)
    {
        DLOG(INFO)<<"Configuration::get_ip(int)";
        if(i >= json_["listen"].size())
        {
            //logstream
            LOG(FATAL)<<"Configuration::get_socket_config get i error";
        }

        return json_["listen"][0]["ip"];
    }
    int Configuration::get_port(int i)
    {
        DLOG(INFO)<<"Configuration::get_port(int)";
        if(i >= json_["listen"].size())
        {
            //logstream
            LOG(FATAL)<<"Configuration::get_socket_config get i error";
        }

        return json_["listen"][0]["port"];
    }

    int Configuration::get_socket_num()
    {
        DLOG(INFO)<<"Configuration::get_socket_num()";
        return json_["listen"].size();
    }
}
