#include "Accpetor.h"

Accpetor::Accpetor(Reactor* reactor, Configuration* config):reactor_(reactor), config_(config){}

Accpetor::Accpetor(Reactor* reactor):reactor_(reactor), config_(){}

Acceptor::~Acceptor(){}

void Accpetor::init()
{
    if(config_.get() == nullptr)
    {
        //输出到日志
        exit(1);
    }
    int i;
    for(i = 0; i < config_sockets_num_; ++i)
    {
        int address;
        int port;
        int domai;
        int type;
        init(address, port, domai, type);
    }
}

void Acceptor::init(int address, int port, int domai, int type)
{
    add_socket(address, port, domai, type);
}

void Acceptor::add_socket(int address, int port, int domai, int type)
{
    int listen_fd = create_socket(domai, type);
    bind_socket_address(listen_fd, address, port);
    listenning_socket(listen_fd);
    add_wait(listen_fd);
}

int Accpetor::create_socket(int domai, int type)
{
    int listen_fd;
    if((listen_fd = socket(domain, type, 0)) == -1)
    {
        perror_and_exit("socket()");
    }
    return listen_fd;
}

void Acceptor::bind_socket_address(int listen_fd, int addr, int port)
{
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(struct sockaddr_in));
    servaddr.sin_family = domain;
    servaddr.sin_addr.s_addr = htonl(addr);
    servaddr.sin_port = htons(port);

    if(bind(listen_fd,(struct ::sockaddr*)(&servaddr), sizeof(struct sockaddr)) == -1)
    {
        perror_and_exit("bind()");
    }
}

void Accpetor::listenning_socket(int listen_fd)
{
    if(listen(listen_fd, MAXBACKLOG) == -1)
    {
        perror_and_exit("listen()");
    }
}

void add_socket(int listen_fd)
{
    if(reactor_.add_listen(listen_fd) == -1)
    {
        perror_and_exit("Accpetor::init");//后面输出到日志,关闭套接字!!
    }

    auto listen_socket = std::make_shared<Sokcet>(listen_fd);
    listen_sockets_.push_back(listen_socket);
}

void del_socket(Socket* socket)
{
    auto ite = std::find(listen_sockets_.begin(), listen_sockets_.end(), [=socket](auto vec_ite){if(vec_ite->get() == socket) return true; return false;});
    if(ite != vec.end())
        listen_sockets_.erase(ite);
}
