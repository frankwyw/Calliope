
#define MAXBACKLOG 1024

#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <arpa/inet.h>

#include <glog/logging.h>

#include "tool_function.hpp"
#include "Acceptor.h"
#include "Configuration.h"
#include "Reactor.h"
#include "Socket.h"


namespace honoka
{
    Acceptor::Acceptor(Reactor* reactor, Configuration* config):reactor_(reactor), config_(config){}

    Acceptor::Acceptor(Reactor* reactor):reactor_(reactor), config_(){}

    Acceptor::~Acceptor(){}

    void Acceptor::init()
    {
        if(config_ == nullptr)
        {
            //输出到日志
            LOG(ERROR)<<"Accepto::init() config == NULL";
            exit(1);
        }

        config_sockets_num_ = config_->get_socket_num();

        int i;
        for(i = 0; i < config_sockets_num_; ++i)
        {
            int ip = inet_addr(config_->get_ip(i).c_str());
            int port = config_->get_port(i);
            int domai = AF_INET;
            int type = SOCK_STREAM;
            init(ip, port, domai, type);
        }
    }


    void Acceptor::init(int ip, int port, int domai, int type)
    {
        add_socket(ip, port, domai, type);
    }

    void Acceptor::add_socket(int address, int port, int domain, int type)
    {
        auto tmp_socket = create_socket_and_bind(address, port, domain, type);
        listenning_socket(tmp_socket);
        add_socket_listen(tmp_socket);
    }

    std::shared_ptr<Socket> Acceptor::create_socket_and_bind(int address, int port, int domain, int type)
    {

        int listen_fd;
        if((listen_fd = ::socket(domain, type, 0)) == -1)
        {
            LOG(ERROR)<<"Accepto::create_socket_and_bind() socket() fail";
        }

        auto servaddr = std::make_shared< struct sockaddr_in>();
        bzero(&servaddr, sizeof(struct sockaddr_in));
        servaddr->sin_family = domain;
        servaddr->sin_addr.s_addr = htonl(address);
        servaddr->sin_port = htons(port);

        auto tmp_socket = std::make_shared<Socket>(listen_fd, servaddr, sizeof(struct sockaddr));

	sockaddr_in* p = static_cast<sockaddr_in* >(tmp_socket->get_servaddr_ptr());

        if(bind(tmp_socket->get_fd(),(struct ::sockaddr*)(p), sizeof(struct sockaddr)) == -1)
        {
            LOG(ERROR)<<"Accepto::create_socket_and_bind() bind() fail";
        }



    }

    void Acceptor::listenning_socket(std::shared_ptr<Socket> tmp_socket)
    {
        if(listen(tmp_socket->get_fd(), MAXBACKLOG) == -1)
        {
            LOG(ERROR)<<"Accepto::listenning_socket() listen() fail";
        }
    }

    void Acceptor::add_socket_listen(std::shared_ptr<Socket> tmp_socket)
    {
        if(reactor_ != nullptr)
        {
            reactor_->add_listen(tmp_socket);
        }
        else
        {
            LOG(ERROR)<<"Acceptor::add_socket_listen() reacotr == NULL";
        }

        struct sockaddr_in* servaddr = static_cast<sockaddr_in* >(tmp_socket->get_servaddr_ptr());
        LOG(INFO)<<"new listen, ip:"<<servaddr->sin_addr.s_addr<<" port:"<<servaddr->sin_port;
    }
}
