#ifndef __ACCEPTOR__H__
#define __ACCEPTOR__H__

#include <memory>
#include <vector>

namespace honoka
{
    class Configuration;
    class Reactor;
    class Socket;

    class Acceptor
    {
    public:

        Acceptor(Reactor* reactor, Configuration* config);
        Acceptor(Reactor* reactor);
        ~Acceptor();

        //读取endian,创建套接字，绑定，监听，以回调注册到epoller。
        void init();
        void init(const std::string& address, int port, int domai, int type);
        void add_socket(const std::string& address, int port, int domai, int type);
    //    void destroy();

    private:
        std::shared_ptr<Socket> create_socket_and_bind(const std::string& address, int port, int domain, int type);
        void listenning_socket(std::shared_ptr<Socket> tmp_socket);
        void add_socket_listen(std::shared_ptr<Socket> tmp_socket);

//      void add_wait(int config_endian_i);

        Configuration*  config_;
        Reactor*        reactor_;
        int config_sockets_num_;
    };
}


//初始化

#endif
