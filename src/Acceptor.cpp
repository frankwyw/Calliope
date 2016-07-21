
#define MAXBACKLOG 1024

#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <arpa/inet.h>

#include <glog/logging.h>

#include "tool_function.h"
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
        DLOG(INFO)<<"Acceptor::init()";
        if(config_ == nullptr)
        {
            //输出到日志
            LOG(FATAL)<<"Accepto::init() config == NULL";
//            exit(1);
        }

        config_sockets_num_ = config_->get_socket_num();

        int i;
        for(i = 0; i < config_sockets_num_; ++i)
        {
            std::string ip = config_->get_ip(i);
            int port = config_->get_port(i);
            int domai = AF_INET;
            int type = SOCK_STREAM;
            init(ip, port, domai, type);
        }
    }


    void Acceptor::init(const std::string& ip, int port, int domai, int type)
    {
        add_socket(ip, port, domai, type);
    }

    void Acceptor::add_socket(const std::string& address, int port, int domain, int type)
    {
        DLOG(INFO)<<"Acceptor::add_socket()";
        auto tmp_socket = create_socket_and_bind(address, port, domain, type);
        listenning_socket(tmp_socket);
        add_socket_listen(tmp_socket);
    }

    std::shared_ptr<Socket> Acceptor::create_socket_and_bind(const std::string& address, int port, int domain, int type)
    {
        DLOG(INFO)<<"Acceptor::create_socket_and_bind()";
        int listen_fd;
        if((listen_fd = ::socket(domain, type, 0)) == -1)
        {
            LOG(ERROR)<<"Accepto::create_socket_and_bind() socket() fail";
        }
        setnonblocking(listen_fd);

        auto servaddr = std::make_shared< struct sockaddr_in>();
        bzero(servaddr.get(), sizeof(struct sockaddr_in));
        servaddr->sin_family = domain;
        servaddr->sin_addr.s_addr = inet_addr(address.c_str());
        servaddr->sin_port = htons(port);
        socklen_t len = sizeof(struct sockaddr);

        auto tmp_socket = std::make_shared<Socket>(listen_fd, servaddr, len);

        sockaddr_in* p = static_cast<sockaddr_in* >(tmp_socket->get_servaddr_ptr());

        if(bind(tmp_socket->get_fd(),(struct ::sockaddr*)(p), sizeof(struct sockaddr)) == -1)
        {
            LOG(ERROR)<<"Accepto::create_socket_and_bind() bind() fail";
        }

	return tmp_socket;

    }

    void Acceptor::listenning_socket(std::shared_ptr<Socket> tmp_socket)
    {
        DLOG(INFO)<<"Acceptor::listenning_socket()";
        if(listen(tmp_socket->get_fd(), MAXBACKLOG) == -1)
        {
            LOG(ERROR)<<"Accepto::listenning_socket() listen() fail";
        }
    }

    void Acceptor::add_socket_listen(std::shared_ptr<Socket> tmp_socket)
    {
        DLOG(INFO)<<"Acceptor::add_socket_listen()";
        if(reactor_ != nullptr)
        {
            reactor_->add_listen(tmp_socket);
        }
        else
        {
            LOG(ERROR)<<"Acceptor::add_socket_listen() reacotr == NULL";
        }

        struct ::sockaddr_in* servaddr = static_cast<sockaddr_in* >(tmp_socket->get_servaddr_ptr());
        char* addr = inet_ntoa(servaddr->sin_addr);
        LOG(INFO)<<"new listen, ip:"<<addr;
    }
}
