#include "Event.h"
#include "Reactor.h"

class Epoller
{
public:
    Epoller(Reactor* reactor_);

    //创建epoll_fd
    void init();


    //epoll_ctl
    void add_wait_event();
    void del_wait_event();

    void add_listen_fd();

    //阻塞在此。
    //如果是有新链接，注册新链接事件，转发给reactor给线程池执行。调用conn_manage处理事件类型：创建新conn,加入新wait_fd。
    //如果套接字可读，创建新读事件，如上。。。
    void run_after();

private:
    int epoll_fd;

    Reactor* reactor_;
}
