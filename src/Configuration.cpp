#include "Configuration.h"
#include <sstream>
#include <fstream>

namespace honoka
{
    Configuration::Configuration(){}
    Configuration::~Configuration(){}

    void Configuration::init()
    {
        std::string text;
        std::stringstream ss(text);
        ss>>*this;
    }

    void Configuration::init(std::string filename){}
    void Configuration::init(char* filename){}

    auto&& Configuration::get_socket_config(int i)
    {
        if(i >= config_["listen"].size())
        {
            //logstream
            exit(1)
        }

        return config_["listen"][0];
    }

    int Configuration::get_socket_num()
    {
        return config_sockets_num_ = config_["listen"].size();
    }
}
