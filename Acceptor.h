#include "Configuration"
#include "Reactor.h"

class Accpetor
{
public:

    Accpetor(Reactor* reactor, Configuration* config)
    ~Accpetor(){}

    //读取endian,创建套接字，绑定，监听，以回调注册到epoller。
    void init();
    void destroy();

    //设置连接回调
    void add_wait_event();

private:


    int listen_fd_;
    Configuration*  config_;
    Reactor*        reactor_;
};

//初始化
