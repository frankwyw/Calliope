#include "Configuration"
#include "Reactor.h"
#include "Socket.h"
#include <memory>
#include <vector>

class Accpetor
{
public:

    Accpetor(Reactor* reactor, Configuration* config);
    Acceptor(Reactor* reactor);
    ~Accpetor();

    //读取endian,创建套接字，绑定，监听，以回调注册到epoller。
    void init();
    void init(int address, int port, int domai, int type);
    void add_socket(int address, int port, int domai, int type);
//    void destroy();


    void del_socket(Socket* );

private:
    int create_socket(int address, int port, int domai, int type);
    void bind_socket_address(int listen_fd, int addr, int port)；
    void listenning_socket();
    void add_wait(int config_endian_i);
    std::vector<std::shared<Socket>> listen_sockets_;
    Configuration*  config_;
    Reactor*        reactor_;
    int config_sockets_num_;
};

//初始化
