#ifndef __CONFIGURATION__H__
#define __CONFIGURATION__H__

#include <string>
#include "Singleton.hpp"
#include "3rd party/json.hpp"


namespace honoka
{
    class Configuration : public Singleton<Configuration>
    {
    public:
        Configuration();
        ~Configuration();
    //应当将其初始化为某个默认json
        void init();
	void init(std::string filename);
	void init(char* filename);

        int get_socket_num();

        std::string get_ip(int i);
	int get_port(int i);

    private:
	nlohmann::json json_;
    };
}

#endif
