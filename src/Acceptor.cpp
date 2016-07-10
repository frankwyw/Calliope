
#define MAXBACKLOG 1024

#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <arpa/inet.h>

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
            exit(1);
        }

        config_sockets_num_ = config_->get_socket_num(i);

        int i;
        for(i = 0; i < config_sockets_num_; ++i)
        {
            auto tmp = config_->get_socket_config();
            int ip = inet_addr(tmp["ip"]);
            int port = tmp["port"];
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
        int listen_fd = create_socket(domain, type);
        bind_socket_address(listen_fd, address, port, domain);
        listenning_socket(listen_fd);
        add_socket_listen(listen_fd);

        int listen_fd;
        if((listen_fd = ::socket(domain, type, 0)) == -1)
        {
            perror_and_exit("socket()");
        }

        struct sockaddr_in servaddr;
        bzero(&servaddr, sizeof(struct sockaddr_in));
        servaddr.sin_family = domain;
        servaddr.sin_addr.s_addr = htonl(addr);
        servaddr.sin_port = htons(port);

        auto tmp_socket = std::make_shared<Socket>(listen_fd, servaddr, sizeof(struct sockaddr));

        if(bind(tmp->get_fd(),(struct ::sockaddr*)(tmp_socket->get_servaddr_ptr()), sizeof(struct sockaddr)) == -1)
        {
            perror_and_exit("bind()");
        }

    }

    void Acceptor::listenning_socket(std::shared_ptr<Socket> tmp_socket)
    {
        if(listen(tmp->get_fd(), MAXBACKLOG) == -1)
        {
            perror_and_exit("listen()");
        }
    }

    void Acceptor::add_socket_listen(std::shared_ptr<Socket> tmp_socket)
    {
        if(reactor_ != nullptr)
        {
            reactor_->add_listen(tmp_socket);
        }
    }
}
