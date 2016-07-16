#include "Configuration.h"

#include <sstream>
#include <fstream>

#include <glog/logging.h>

namespace honoka
{
    Configuration::Configuration()
    {
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
                "domain": "AF_INET",
                "type": "socket_stream",
                "nonblocking": true,
                "ip": "127.0.0.1",
                "port": "all"
            }
       ]
}
	)");

	ss >> json_;
    }
    Configuration::~Configuration(){}

    void Configuration::init()
    {
        std::string text;
        std::stringstream ss(text);
        ss>>json_;
    }

    void Configuration::init(std::string filename)
    {
        std::fstream fs(filename);
        fs>>json_;
        fs.close();
    }
    void Configuration::init(char* filename)
    {
        std::fstream fs(filename);
        fs>>json_;
        fs.close();
    }

    std::string Configuration::get_ip(int i)
    {
	if(i >= json_["listen"].size())
        {
            //logstream
            LOG(INFO)<<"Configuration::get_socket_config get i error";
            exit(1);
        }

	return json_["listen"]["ip"];
    }
    int Configuration::get_port(int i)
    {
	if(i >= json_["listen"].size())
        {
            //logstream
            LOG(INFO)<<"Configuration::get_socket_config get i error";
            exit(1);
        }

	return json_["listen"]["port"];
    }

    int Configuration::get_socket_num()
    {
        return json_["listen"].size();
    }
}
