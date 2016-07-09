#ifndef __CONFIGURATION__H__
#define __CONFIGURATION__H__

#include <string>
#include "Singleton.hpp"
#include "3rd party/json.hpp"


namespace honoka
{
    class Configuration : public nlohmann::json, public Singleton<Configuration>
    {
    public:
        Configuration();
        ~Configuration();
    //应当将其初始化为某个默认json
        void init();
	    void init(std::string filename);
	    void init(char* filename);

        int get_socket_num();

        auto&& get_socket_config(int i);
    };
}

#endif
