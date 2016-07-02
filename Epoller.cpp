#include "epoller.h"


Epoller::Epooler(Reactor* reactor):reactor_(reactor), cur_fds_num(0){}

Epoller::init()
{
    if((epoll_fd = epoll_create(MAXEPOLL)) == -1)
    {
        perror_and_exit("epoll_create()");
    }
}

void Epoller::add_listen(int listen_fd)
{
    listenning_fds_.insert(listen_fd);
    add_wait(listen_fd);
}

void Epoller::del_listen(int listen_fd)
{
    del_wait(listenning_fds_);
    auto ite = listenning_fds_.find(listen_fd);
    listenning_fds_.erase(ite);
}

void Epoller::add_wait(int listen_fd)
{
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listen_fd;

    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev ) < 0 )
    {
        perror_and_exit("epoll_ctl()");
    }

    ++cur_fds_num;
}

void Epoller::del_wait(int listen_fd)
{
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listen_fd;

    if(epoll_ctl(epoll_fd, EPOLL_CTL_MDO, listen_fd, &ev ) < 0 )
    {
        perror_and_exit("epoll_mod()");
    }

    --cur_fds_num;
}

void Epoller::run(int delay_time, Thread_pool* thread_pool_, std::map<int, std::shared_ptr<Connection>>&  socket_conns)
{
    int wait_fds_num;
    if( ( wait_fds_num = epoll_wait( epoll_fd, evs, cur_fds_num, -1 ) ) == -1 )
    {
        perror_and_exit("epoll_wait");
    }

    for (i = 0; i < wait_fds_num; ++i)
    {
        conn_fd = evs[i].data.fd;

        //处理新链接
        if (listenning_fds_.find(conn_fd) != listenning_fds_.end())
        {

            /*
            封装成EVENT，加入线程池  
            while ((conn_fd = accept(listen_fd,(struct sockaddr *) &servaddr,&len)) > 0)
            {
                setnonblocking(conn_fd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd,&ev) == -1)
                {
                    perror("epoll_ctl: add");
                    exit(EXIT_FAILURE);
                }
            }
            if (conn_fd == -1)
            {
                if (errno != EAGAIN && errno != ECONNABORTED&& errno != EPROTO && errno != EINTR)
                    perror("accept");
            }
            continue;
            */
        }
        else
        {

        }
    }


}
