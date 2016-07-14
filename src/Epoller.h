#ifndef __EPOLLER__H__
#define __EPOLLER__H__

#include <set>
#include <map>
#include <memory>
#include <mutex>

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


        void add_listen(std::shared_ptr<Socket> socket);
        void del_listen(std::shared_ptr<Socket> socket);


        //epoll_ctl
        void add_wait(std::shared_ptr<Socket> socket);
        void del_wait(std::shared_ptr<Socket> socket);

        //阻塞在此。
        //如果是有新链接，注册新链接事件，转发给reactor给线程池执行。调用conn_manage处理事件类型：创建新conn,加入新wait_fd。
        //如果套接字可读，创建新读事件，如上。。。
        void run(int delay_time, Thread_pool* thread_pool_, std::map<int, std::shared_ptr<Connection>>&  socket_conns);

        void close_listenning();

    private:
        void set_epoll_ev(struct epoll_event* ev);

        int epoll_fd;
        int cur_fds_num;

        std::set<int> listenning_fds_;

        Reactor* reactor_;

        std::mutex_;
    };
}

#endif
