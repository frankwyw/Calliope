#include "Event.h"
#include "Reactor.h"
#include <set>

class Epoller
{
public:
    Epoller(Reactor* reactor);

    //创建epoll_fd
    void init();


    void add_listen(int fd);
    void del_listen(int fd);


    //epoll_ctl
    void add_wait(int fd);
    void del_wait(int fd);

    //阻塞在此。
    //如果是有新链接，注册新链接事件，转发给reactor给线程池执行。调用conn_manage处理事件类型：创建新conn,加入新wait_fd。
    //如果套接字可读，创建新读事件，如上。。。
    void run_at(int delay_time);

private:
    
    int epoll_fd;
    int cur_fds_num;

    std::set<int> listenning_fds_;

    Reactor* reactor_;
}
