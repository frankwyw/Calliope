#ifndef __EPOLLER__H__
#define __EPOLLER__H__

#include <set>
#include <map>
#include <memory>

namespace honoka
{
    class Event;
    class Reactor;
    class Thread_pool;
    class Connection;
    

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
        void run(int delay_time, Thread_pool* thread_pool_, std::map<int, std::shared_ptr<Connection>>&  socket_conns);

    private:

        int epoll_fd;
        int cur_fds_num;

        std::set<int> listenning_fds_;

        Reactor* reactor_;
    };
}

#endif
